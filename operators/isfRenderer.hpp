// ISFRenderer.hpp
#ifndef ISFRENDERER_HPP
#define ISFRENDERER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

// TODO: possibly split this to a private header?
#include <ISFParameters.hpp>
#include <WidgetOp.hpp>

#pragma region fileWatcher

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>
#include <functional>
#include <vector>

class FileWatcher
{
public:
    FileWatcher(const std::string &filePath,
                std::function<void()> callback) :
        filePath(filePath),
        lastContentHash(getFileContentHash(filePath)),
        shouldExit(false),
        fileChangedCallback(callback)
    {
        watchFile();
    }

    ~FileWatcher()
    {
        shouldExit = true;
        if (watcherThread.joinable())
        {
            watcherThread.join();
        }
    }

private:

    #define checkIntervalMs 200

    const std::string filePath;
    std::size_t lastContentHash;
    bool shouldExit;
    std::thread watcherThread;
    std::function<void()> fileChangedCallback;

    std::size_t getFileContentHash(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
        {
            return 0; // Return a hash of 0 for non-existing files
        }

        // Read the file content into a string
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // Calculate the hash of the file content using a custom hash function
        std::size_t hash = 0;
        for (char c : content)
        {
            hash = hash * 31 + static_cast<unsigned char>(c);
        }

        return hash;
    }

    void watchFile()
    {
        watcherThread = std::thread([this]
        {
            while (!shouldExit) {
                std::this_thread::sleep_for(std::chrono::milliseconds(checkIntervalMs));

                auto currentContentHash = getFileContentHash(filePath);

                if ( currentContentHash != lastContentHash) {
                    // File has changed
                    lastContentHash = currentContentHash;
                    std::cout << "File changed: " << filePath << std::endl;

                    // Call the provided callback function
                    fileChangedCallback();
                }
            }
        });
    }
};

#pragma endregion

class ISFRenderer : public WidgetOp
{
public:
    ISFRenderer(const std::string &_shaderPath);
    ~ISFRenderer();

    // Static method to create an ISFRenderer instance and load a shader
    static ISFRenderer *createRendererAndLoadShader(const std::string &shaderPath);

    void watchNewISF(const std::string &shaderPath);
    void render();

    // Function to get the error message
    std::string getError() const;
    std::string shaderPath;

    FileWatcher isfWatcher;
    std::atomic<bool> shouldReloadShader; // Atomic flag to indicate if shader reload is needed

private:
    std::string error; // Member variable to store error messages

    // Function to set an error message
    void setError(const std::string &errorMsg);

    GLuint shaderProgram;
    float timer;


    void loadAndCompileISF(const std::string &shaderPath);
    std::string readShaderSource(const std::string &filePath);
    void updateISFUniforms();

    ISFParameters * m_isfParameters;

    void renderQuad();

    void doGui() override;
};

#endif // ISFRENDERER_HPP

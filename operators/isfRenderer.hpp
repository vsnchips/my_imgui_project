// ISFRenderer.hpp
#ifndef ISFRENDERER_HPP
#define ISFRENDERER_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ISFRenderer {
public:
    ISFRenderer();
    ~ISFRenderer();

    // Static method to create an ISFRenderer instance and load a shader
    static ISFRenderer* createAndLoadISFShader(const std::string& shaderPath);

    void loadISFShader(const std::string& shaderPath);
    void render();

    // Function to get the error message
    std::string getError() const;

private:

    std::string error; // Member variable to store error messages

    // Function to set an error message
    void setError(const std::string& errorMsg);

    GLuint shaderProgram;
    float timer;

    // Vertex and index buffer objects
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void compileISFShader(const std::string& shaderPath);
    std::string loadShaderCode(const std::string& filePath);
    void updateISFUniforms();
    void renderQuad();
};

#endif // ISFRENDERER_HPP

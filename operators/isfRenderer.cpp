// ISFRenderer.cpp
#include "ISFRenderer.hpp"
#include <iostream>
#include <fstream>
#include <vector>

#include <filesystem>
#include <chrono>
#include <thread>
#include <ctime>

#include <renderable.hpp>

// #include "ISFParameters.hpp" // Include your ISF parameter handling functions
ISFRenderer::ISFRenderer(const std::string &isfPath) : shaderPath(isfPath), shaderProgram(0),
                                                       timer(0.0f), VAO(0), VBO(0), EBO(0),
                                                       isfWatcher(isfPath, [&]()
                                                                  { shouldReloadShader.store(true); })
{
    std::cout << "Created a FileWatcher for ISF Shader " << shaderPath << std::endl;
    shouldReloadShader.store(true);
    registerWidget(this);
}

ISFRenderer::~ISFRenderer()
{
    glDeleteProgram(shaderProgram);

    // Delete quad geometry buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// ISFRenderer.cpp
ISFRenderer *ISFRenderer::createRendererAndLoadShader(const std::string &shaderPath)
{
    std::cout << "Creating a Renderer" << std::endl;
    ISFRenderer *renderer = new ISFRenderer(shaderPath);

    if (renderer->shaderProgram == 0)
    {
        std::cerr << "Failed to load ISF shader: " << shaderPath << std::endl;
    }
    return renderer;
}

void ISFRenderer::render()
{
    // Check if shader reload is needed
    if (shouldReloadShader.load())
    {
        loadAndCompileISF(shaderPath);
        // Reset the flag, regardless of the result of the load attempt.
        shouldReloadShader.store(false);
    }

    // Use the ISF shader program
    if (shaderProgram)
        glUseProgram(shaderProgram);

    // Update ISF shader uniforms here (e.g., time, other parameters)
    updateISFUniforms();

    // Render the quad
    renderQuad();
}

// Utility function to check shader compilation errors
bool checkShaderCompilation(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, errorLog.data());
        std::cerr << "Shader compilation error: " << errorLog.data() << std::endl;
        return false;
    }
    return true;
}

// Utility function to check program linking errors
bool checkProgramLinking(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());
        std::cerr << "Program linking error: " << errorLog.data() << std::endl;
        return false;
    }
    return true;
}

void ISFRenderer::loadAndCompileISF(const std::string &shaderPath)
{

    executeWithExceptionHandling([this, shaderPath]()
    {
        // Load and compile the ISF shader code (you'll need to implement this)
        std::string shaderCode = readShaderSource(shaderPath);
        const char *shaderCodePtr = shaderCode.c_str();

        // Initialize ISF parameters
        m_isfParameters = ISFParameters::parseISFShaderAndDisplayParams(shaderCode);

        /*
          GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

          glShaderSource(vertexShader, 1, &FullScreenQuad::DefaultVertexShaderSource, nullptr);
          glCompileShader(vertexShader);

          // Check for vertex shader compilation errors
          if (!checkShaderCompilation(vertexShader)) {
              setError("Vertex shader compilation failed.");
              return;
          }

          GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
          glShaderSource(fragmentShader, 1, &shaderCodePtr, nullptr);
          glCompileShader(fragmentShader);

          // Check for fragment shader compilation errors
          if (!checkShaderCompilation(fragmentShader)) {
              setError("Fragment shader compilation failed.");
              return;
          }

          GLuint program = glCreateProgram();
          glAttachShader(program, vertexShader);
          glAttachShader(program, fragmentShader);
          glLinkProgram(program);

          // Check for program linking errors
          if (!checkProgramLinking(program)) {
              setError("Shader program linking failed.");
              return;
          }

          glDeleteShader(vertexShader);
          glDeleteShader(fragmentShader);


          shaderProgram = program;
          */

        shaderProgram = compileShaderProgram(FullScreenQuad::DefaultVertexShaderSource, shaderCodePtr);

        std::cout << "Loaded Shader " << shaderPath << std::endl;
    });
}

std::string ISFRenderer::readShaderSource(const std::string &filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile)
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string shaderCode;
    std::string line;
    while (std::getline(shaderFile, line))
    {
        shaderCode += line + "\n";
    }
    shaderFile.close();

    return shaderCode;
}

void ISFRenderer::updateISFUniforms()
{

    // Update ISF shader uniforms here
    // For example, you can set a time uniform to create animations
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), timer);
    glUniform2f(glGetUniformLocation(shaderProgram, "RENDERSIZE"), 1920, 1080); // Replace 'width' and 'height' with your framebuffer dimensions

    /*
    // You can set other ISF parameters here using your ISFParameters namespace
    ISFParameters::FloatParameter param;
    param.label = "param_name";
    param.value = 0.5f;
    ISFParameters::floatSliderParamConstructor(param);

    // Increment the timer for animations
    timer += 0.01f;
    */
}

void ISFRenderer::renderQuad()
{
    // Bind the VAO before rendering
    glBindVertexArray(VAO);

    // Use the ISF shader program
    glUseProgram(shaderProgram);

    // Render the quad using a draw call (e.g., glDrawElements for indexed rendering)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind the VAO after rendering
    glBindVertexArray(0);
}

void ISFRenderer::setError(const std::string &errorMsg)
{
    error = errorMsg;
}

std::string ISFRenderer::getError() const
{
    return error;
}

void ISFRenderer::doGui()
{
    // For each param, call the constructor map
    m_isfParameters->doImGuiWidgets();
}
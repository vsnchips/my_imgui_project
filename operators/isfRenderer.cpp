// ISFRenderer.cpp
#include "ISFRenderer.hpp"
#include <iostream>
#include <fstream>
#include <vector>
// #include "ISFParameters.hpp" // Include your ISF parameter handling functions

ISFRenderer::ISFRenderer() : shaderProgram(0), timer(0.0f), VAO(0), VBO(0), EBO(0)
{
    // Create and initialize the quad geometry

    // Define the indices for rendering a quad
    static const GLuint quadIndices[] = {
        0, 1, 3,
        1, 2, 3};

    // Define the vertices and texture coordinates for a quad
    static const GLfloat quadVertices[] = {
        // Position        // Texture Coordinates
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top-right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // Top-left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

ISFRenderer::~ISFRenderer()
{
    glDeleteProgram(shaderProgram);

    // Delete quad geometry buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void ISFRenderer::loadISFShader(const std::string &shaderPath)
{
    // Load and compile the ISF shader program
    shaderProgram = compileISFShader(shaderPath);
    if (shaderProgram == 0)
    {
        std::cerr << "Failed to load ISF shader: " << shaderPath << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize ISF parameters
    // ISFParameters::parseISFShaderAndDisplayParams(shaderPath);
}

void ISFRenderer::render()
{
    // Use the ISF shader program
    glUseProgram(shaderProgram);

    // Update ISF shader uniforms here (e.g., time, other parameters)
    updateISFUniforms();

    // Render the quad
    renderQuad();
}

GLuint ISFRenderer::compileISFShader(const std::string &shaderPath)
{
    // Load and compile the ISF shader code (you'll need to implement this)
    std::string shaderCode = loadShaderCode(shaderPath);
    const char *shaderCodePtr = shaderCode.c_str();

    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shaderCodePtr, nullptr);
    glCompileShader(shader);

    // Check for shader compilation errors (you should add error checking)
    // ...

    // Link the shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    // Check for program linking errors (you should add error checking)
    // ...

    glDeleteShader(shader);

    return program;
}

std::string ISFRenderer::loadShaderCode(const std::string &filePath)
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
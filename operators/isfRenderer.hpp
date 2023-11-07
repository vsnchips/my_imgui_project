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

    void loadISFShader(const std::string& shaderPath);
    void render();

private:
    GLuint shaderProgram;
    float timer;

    // Vertex and index buffer objects
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint compileISFShader(const std::string& shaderPath);
    std::string loadShaderCode(const std::string& filePath);
    void updateISFUniforms();
    void renderQuad();
};

#endif // ISFRENDERER_HPP

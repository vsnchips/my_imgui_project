
#include <renderable.hpp>

#include <iostream>
#include <vector>

// Static member definitions
GLuint FullScreenQuad::VAO = 0;
GLuint FullScreenQuad::VBO = 0;
GLuint FullScreenQuad::EBO = 0;

bool FullScreenQuad::initialized = false;

// Define a default vertex shader
char *FullScreenQuad::DefaultVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main() {
            gl_Position = vec4(aPos, 1.0);
            TexCoord = aTexCoord;
            //gl_FragCoord = aTexCoord;
        }
    )";

std::mutex FullScreenQuad::initMutex;

void FullScreenQuad::Init()
{
    std::lock_guard<std::mutex> lock(initMutex); // Lock for thread safety

    if (!initialized)
    { // Create and initialize the quad geometry

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

        std::cout << "VAO: " << VAO << std::endl;

        glGenBuffers(1, &VBO);
        std::cout << "VAO: " << VAO << std::endl;
        glGenBuffers(1, &EBO);
        std::cout << "VAO: " << VAO << std::endl;

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

        initialized = true;
    }
}

// Render the full-screen quad
void FullScreenQuad::RenderQuad()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

#pragma region SHADERS
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

GLuint compileShaderProgram(const char * vertSource, const char * fragSource)
{

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertSource, nullptr);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    if (!checkShaderCompilation(vertexShader))
    {
        glDeleteShader(vertexShader);
        throw std::runtime_error("Vertex shader compilation failed.");
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    if (!checkShaderCompilation(fragmentShader))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); // Delete the fragment shader as well
        throw std::runtime_error("Fragment shader compilation failed.");
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for program linking errors
    if (!checkProgramLinking(program))
    {
        // Detach and delete shaders
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader); // Assuming you have a fragment shader too
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); // Delete the fragment shader as well
        throw std::runtime_error("Shader program linking failed.");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

#pragma endregion
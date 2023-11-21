
#include <renderable.hpp>

// Static member definitions
GLuint FullScreenQuad::vao = 0;
GLuint FullScreenQuad::vbo = 0;
bool FullScreenQuad::initialized = false;
std::mutex FullScreenQuad::initMutex;

    static void FullScreenQuad::Init()
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
    static void FullScreenQuad::RenderQuad()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

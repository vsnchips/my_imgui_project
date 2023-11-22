#ifndef _RENDERABLE_HPP
#define _RENDERABLE_HPP

#include <GL/glew.h>

// #if defined(WIN32)
// #include <GL/gl.h>
// #else
// #include <OpenGL/gl.h>
// #endif

#include <GLFW/glfw3.h>

#include <mutex>

class FullScreenQuad
{
private:
    static bool initialized;
    static std::mutex initMutex; // Mutex for thread-safe initialization

    // Vertex and index buffer objects
    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

public:
    // Initializes the VBO and VAO for the full-screen quad
    static void Init(); // Render the full-screen quad
    static void RenderQuad();

    static char* DefaultVertexShaderSource;

};

extern GLuint compileShaderProgram( const char*, const char* );

#endif
#ifndef _NDI_SENDER_HPP
#define _NDI_SENDER_HPP

#include <WidgetOp.hpp>

#include <GL/glew.h>

/*
#if defined(WIN32)
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
*/

#include <GLFW/glfw3.h>

#include "Processing.NDI.Lib.h"

class NDISender : public WidgetOp
{
public:
    void update();
    void doGui() override;

    NDISender();

    ~NDISender() override;

    int sourceWidth, sourceHeight;

    int targetResolution[2] = {1920, 1080};
    int lastTargetResolution[2] = {1920, 1080};

    void captureFramebufferAndAssignToNDI(GLuint framebufferID);

    void InitializeGLObjects();

    void sendToNDI();

private:

    bool enabled = false;

    void captureFrame();

    void InitializeNDI();
    void closeNDI();

    // NDI 
    NDIlib_send_instance_t pNDI_send = nullptr;
    NDIlib_video_frame_v2_t video_frame;

    //GL
    GLint originalFBO;
    GLuint capturedTexture;
    GLuint resizedFBO;
    GLuint colorRenderbuffer; // If you need a color attachment
    GLuint program;

    // Use a simple shader program which just renders a texture.
    const char *fragmentShaderSrc = R"glsl(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;
                uniform sampler2D screenTexture;
                void main()
                {
                    // Flip TexCoord;
                    vec2 FlipTexCoord = TexCoord;
                    FlipTexCoord.y = - FlipTexCoord.y + 1.0;

                   FragColor = texture(screenTexture, FlipTexCoord);
                }
            )glsl";
};

#endif
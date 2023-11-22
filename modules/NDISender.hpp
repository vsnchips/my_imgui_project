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

    int targetResolution[2] = {1920, 1080};
    void captureFramebufferAndAssignToNDI(GLuint framebufferID,
                                          int frameBufferWidth, int frameBufferHeight,
                                          int targetWidth, int targetHeight);

    void sendToNDI();

private:

    bool enabled = false;

    void captureFrame();

    void InitializeNDI();
    void closeNDI();
    // NDI variables
    NDIlib_send_instance_t pNDI_send = nullptr;
    NDIlib_video_frame_v2_t video_frame;
};

#endif
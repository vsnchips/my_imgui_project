#ifndef _NDI_SENDER_HPP
#define _NDI_SENDER_HPP

#include <WidgetOp.hpp>

#include <GL/glew.h>

#if defined(WIN32)
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include "Processing.NDI.Lib.h"

class NDISender : public WidgetOp {
public:
    void update();
    void doGui() override;

    NDISender();

    ~NDISender() override;

    void captureFramebufferAndAssignToNDI(
        GLuint framebufferID,
        int fbWidth,
        int fbHeight,
        int targetWidth,
        int targetHeight);

    void sendToNDI();

private:
    void InitializeNDI();
    void closeNDI();
    // NDI variables
    NDIlib_send_instance_t pNDI_send;
    NDIlib_video_frame_v2_t video_frame;
};

#endif
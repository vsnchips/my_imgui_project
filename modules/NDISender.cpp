#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Processing.NDI.Lib.h"

#include <imgui.h>

#include <NDISender.hpp>

// Assume video_frame is a global variable of type NDIlib_video_frame_v2_t

NDISender::NDISender()
{
    WidgetOp::registerWidget(this);
}

void NDISender::closeNDI()
{
    executeWithExceptionHandling([this]()
                                 {
        // Clean up NDI resources
        NDIlib_send_destroy(pNDI_send);
        NDIlib_destroy(); });
}

NDISender::~NDISender()
{
}

// Function to initialize NDI
void NDISender::InitializeNDI()
{
    // Initialize NDI
    NDIlib_initialize();

    // Create NDI sender
    NDIlib_send_create_t NDI_send_create_desc;
    NDI_send_create_desc.p_ndi_name = "YourNDISenderName";
    pNDI_send = NDIlib_send_create(&NDI_send_create_desc);

    // Initialize video frame
    video_frame.xres = 1920; // Set your video resolution
    video_frame.yres = 1080;
    video_frame.FourCC = NDIlib_FourCC_type_BGRA;
    video_frame.line_stride_in_bytes = video_frame.xres * 4; // Assuming 4 bytes per pixel

    // You may need to allocate memory for video_frame.p_data and set other parameters
    video_frame.p_data = new uint8_t[video_frame.line_stride_in_bytes * video_frame.yres];
}

void NDISender::doGui()
{
    ImGui::Begin("NDI Sender");

    if (ImGui::Button("Setup NDI"))
    {
        InitializeNDI();
    }

    int res[2];
    ImGui::SliderInt2("Resolution", res, 0, 1920);

    if (ImGui::Button("Capture Framebuffer"))
    {
        captureFramebufferAndAssignToNDI(0, 640, 480, res[0], res[2]);
    }

    if (ImGui::Button("Send to NDI"))
    {
        sendToNDI();
    }

    if (ImGui::Button("Close NDI"))
    {
        closeNDI();
    }

    ImGui::End();
}


// Function to capture the current framebuffer and render it to a resized one
void resizeFramebuffer(GLFWwindow* window, int newWidth, int newHeight) {
    // First, capture the current framebuffer content
    GLint originalFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);

    // Create a texture to hold the captured content
    GLuint capturedTexture;
    glGenTextures(1, &capturedTexture);
    glBindTexture(GL_TEXTURE_2D, capturedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Copy the current framebuffer content to the texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, originalFBO);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, newWidth, newHeight, 0);

    // Create a new framebuffer
    GLuint resizedFBO;
    glGenFramebuffers(1, &resizedFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, resizedFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, capturedTexture, 0);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // Handle errors here
    }

    // Set the viewport to the new dimensions
    glViewport(0, 0, newWidth, newHeight);

    // Render the texture to the new framebuffer
    // ... [Set up and use a shader to render the quad with the captured texture]

    // Bind the original framebuffer back
    glBindFramebuffer(GL_FRAMEBUFFER, originalFBO);

    // Clean up
    glDeleteTextures(1, &capturedTexture);
    glDeleteFramebuffers(1, &resizedFBO);
}


void NDISender::captureFramebufferAndAssignToNDI(GLuint framebufferID,
                                                 int frameBufferWidth, int frameBufferHeight,
                                                 int targetWidth, int targetHeight)
{
    executeWithExceptionHandling([this, framebufferID, frameBufferWidth, frameBufferHeight, targetWidth, targetHeight]()
                                 {
        // Bind the specified framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Allocate memory for the framebuffer data
        std::vector<uint8_t> framebufferData(frameBufferWidth * frameBufferHeight * 4); // Assuming 4 bytes per pixel (RGBA)

        // Read the framebuffer data
        glReadPixels(0, 0, frameBufferWidth, frameBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, framebufferData.data());

        // Resample the data to the target resolution if needed
        if (frameBufferWidth != targetWidth || frameBufferHeight != targetHeight)
        {
            // You may use your own resampling algorithm or a library
            // For simplicity, this example uses GLFW's image resampling function
            GLFWimage resampledImage;
            resampledImage.width = targetWidth;
            resampledImage.height = targetHeight;
            resampledImage.pixels = framebufferData.data(); // Assumes RGBA data

            // Resample the image using GLFW's function
            resampledImage = glfwCreateResizedImage(&resampledImage, targetWidth, targetHeight, GLFW_IMAGE_RESIZE_NEAREST);

            // Update the framebuffer data with the resampled image
            framebufferData = std::vector<uint8_t>(resampledImage.pixels, resampledImage.pixels + targetWidth * targetHeight * 4);

            // Free the memory allocated by GLFW for the resampled image
            glfwFreeImage(&resampledImage);
        }

        // Assign the data to the NDI video frame
        video_frame.xres = targetWidth;
        video_frame.yres = targetHeight;
        video_frame.FourCC = NDIlib_FourCC_type_BGRA;
        video_frame.line_stride_in_bytes = targetWidth * 4; // Assuming 4 bytes per pixel

        // Assuming video_frame.p_data is already allocated and has enough space
        std::memcpy(video_frame.p_data, framebufferData.data(), framebufferData.size());

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0); });
}

void NDISender::sendToNDI()
{
    executeWithExceptionHandling([this]()
                                 {
        if (pNDI_send == nullptr) {
            throw std::runtime_error("NDI sender is not initialized.");
        }

        // Assuming video_frame is already populated from the captureFramebufferAndAssignToNDI function
        NDIlib_send_send_video_v2(pNDI_send, &video_frame);

        std::cout << "Sent frame to NDI." << std::endl; });
}

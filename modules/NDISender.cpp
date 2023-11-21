

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Processing.NDI.Lib.h"

#include <imgui.h>

// Assume video_frame is a global variable of type NDIlib_video_frame_v2_t

NDISender::NDISender()
{
    WidgetOp::registerWidget(this);
}

void closeNDI()
{
    executeWithExceptionHandling([]() = > {
        // Clean up NDI resources
        NDIlib_send_destroy(pNDI_send);
        NDIlib_destroy();
    });
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

    if (ImGui::Button("Capture Framebuffer"))
    {
        captureFramebufferAndAssignToNDI(/* Pass framebuffer ID, targetWidth, targetHeight */);
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

void NDISender::captureFramebufferAndAssignToNDI(GLuint framebufferID, int targetWidth, int targetHeight)
{
    executeWithExceptionHandling([]() = > {
        // Bind the specified framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Get the size of the currently bound framebuffer
        // Get the size of the framebuffer
        GLint framebufferWidth, framebufferHeight;
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_WIDTH, &framebufferWidth);
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_HEIGHT, &framebufferHeight);

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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    });
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

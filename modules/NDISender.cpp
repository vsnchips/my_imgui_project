
#include <NDISender.hpp>

#include <iostream>

// #include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_toggle.h>

#include <renderable.hpp>
#include <mainWindow.hpp>

// Assume video_frame is a global variable of type NDIlib_video_frame_v2_t

NDISender::NDISender()
{
    FullScreenQuad::Init();

    WidgetOp::registerWidget(this);

    InitializeNDI();
    InitializeGLObjects();
}

void NDISender::closeNDI()
{
    executeWithExceptionHandling([this]()
                                 {
                                     std::cout << "Closing NDI" << std::endl;

                                     // Clean up NDI resources
                                     if (pNDI_send != nullptr)
                                         NDIlib_send_destroy(pNDI_send);
                                     else
                                         std::cout << "the sender is already closed.";

                                     std::cout << "closing the lib" << std::endl;
                                     NDIlib_destroy();

                                     pNDI_send = nullptr; });
}

NDISender::~NDISender()
{
    closeNDI();
}

// TODO expose the window to modules and ops?
#define WINDOW 0
void NDISender::captureFrame()
{
    std::cout << "Getting window FB size";

    int sw,sh;
    glfwGetFramebufferSize(window1, &sw, &sh);
    if(sw != sourceWidth || sh != sourceHeight){
        InitializeGLObjects();
    }

    captureFramebufferAndAssignToNDI(0, sourceWidth, sourceHeight, targetResolution[0], targetResolution[1]);
    // captureFramebufferAndAssignToNDI(0, 100, 100, targetResolution[0], targetResolution[1]);
}

void InitializeGLObjects(){

    // First, capture the current framebuffer content
    int sourceWidth, sourceHeight;
    glfwGetFramebufferSize(window1, &sourceWidth, &sourceHeight);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);

    glGenTextures(1, &capturedTexture);
    glBindTexture(GL_TEXTURE_2D, capturedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sourceWidth, sourceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate and bind the framebuffer
    glGenFramebuffers(1, &resizedFBO);


    // If you need a color attachment:
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, targetWidth, targetHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    // ... [Set up and use a shader to render the quad with the captured texture]
    // Compile the shaders
    program = compileShaderProgram(
        FullScreenQuad::DefaultVertexShaderSource,
        fragmentShaderSrc);
    glUseProgram(program);
    std::cout << "Shader program compiled and used" << std::endl;



}
// Function to initialize NDI
void NDISender::InitializeNDI()
{
    executeWithExceptionHandling([this]()
                                 {
                                     // Clear old data
                                     closeNDI();

                                     std::cout << "Initialising NDI" << std::endl;

                                     // Initialize NDI
                                     NDIlib_initialize();

                                     // Create NDI sender
                                     NDIlib_send_create_t NDI_send_create_desc;
                                     NDI_send_create_desc.p_ndi_name = "YourNDISenderName";
                                     pNDI_send = NDIlib_send_create(&NDI_send_create_desc);

                                     std::cout << "pNDI_send value: " << pNDI_send << std::endl;

                                     // Initialize video frame
                                     video_frame = NDIlib_video_frame_v2_t();
                                     video_frame.xres = targetResolution[0]; // Set your video resolution
                                     video_frame.yres = targetResolution[1];

                                     video_frame.FourCC = NDIlib_FourCC_type_BGRA;
                                     video_frame.line_stride_in_bytes = video_frame.xres * 4; // Assuming 4 bytes per pixel

                                     // You may need to allocate memory for video_frame.p_data and set other parameters
                                     video_frame.p_data = new uint8_t[video_frame.line_stride_in_bytes * video_frame.yres]; });
}

void NDISender::doGui()
{
    ImGui::Begin("NDI Sender");

    ImGui::Toggle("Enabled", &enabled,
                  ImGuiToggleFlags_Animated |
                      ImGuiToggleFlags_Bordered |
                      ImGuiToggleFlags_Shadowed);

    if (ImGui::Button("Setup NDI"))
    {
        InitializeNDI();
    }

    if(ImGui::SliderInt2("Resolution", targetResolution, 0, 1920)){
        if (targetResolution[0]!=lastTargetResolution[0] || 
            targetResolution[1]!=lastTargetResolution[1] )
            {
                InitializeGLObjects();
            }

    }

    if (ImGui::Button("Capture Framebuffer"))
    {
        captureFrame();
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
void renderToResizedFrameBuffer(int sourceWidth, int sourceHeight, int newWidth, int newHeight)
{
    // First, capture the current framebuffer content
    // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);

    // Create a texture to hold the captured content
    // glGenTextures(1, &capturedTexture);
    // glBindTexture(GL_TEXTURE_2D, capturedTexture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sourceWidth, sourceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Copy the current framebuffer content to the texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, originalFBO);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, sourceWidth, sourceHeight, 0);

    // Bind the new framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, resizedFBO);

    // If you need a color attachment:
    // glGenRenderbuffers(1, &colorRenderbuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, newWidth, newHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    // Set viewport size for the new framebuffer
    glViewport(0, 0, newWidth, newHeight);

    // Render the texture to the new framebuffer
    // Bind the captured texture
    glActiveTexture(GL_TEXTURE0); // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, capturedTexture);
    glUniform1i(glGetUniformLocation(program, "screenTexture"), 0); // Set 'screenTexture' uniform to texture unit 0

    // Render the quad
    FullScreenQuad::RenderQuad();

    // std::cout << "Framebuffer data resized" << std::endl;

    // Bind the original framebuffer back
    // glBindFramebuffer(GL_FRAMEBUFFER, originalFBO);

    // Clean up
    // glDeleteTextures(1, &capturedTexture);
    // glDeleteFramebuffers(1, &resizedFBO);
}

void NDISender::captureFramebufferAndAssignToNDI(GLuint framebufferID)
{
    executeWithExceptionHandling([this, framebufferID]()
                                 {
        int targetWidth = targetResolution[0];
        int targetHeight = targetResolution[1];

        // std::cout << "Starting captureFramebufferAndAssignToNDI" << std::endl;

        // Bind the specified framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
        // std::cout << "Framebuffer bound: " << framebufferID << std::endl;

        // Check if resizing is needed
        if (sourceWidth != targetWidth || sourceHeight != targetHeight)
        {
            std::cout << "Resizing is required" << std::endl;
            renderToResizedFrameBuffer(sourceWidth, sourceHeight, targetWidth, targetHeight);
        }

        // Allocate memory for the framebuffer data
        std::vector<uint8_t> resizedFBData(targetWidth * targetHeight * 4); // Assuming 4 bytes per pixel (RGBA)
        // std::cout << "Framebuffer data allocated: " << targetWidth * targetHeight * 4 << " bytes" << std::endl;

        // Move the framebuffer data
        glReadPixels(0, 0, targetWidth, targetHeight, GL_BGRA, GL_UNSIGNED_BYTE, resizedFBData.data());
        // std::cout << "Framebuffer data read" << std::endl;

        // Assign the data to the NDI video frame
        video_frame.xres = targetWidth;
        video_frame.yres = targetHeight;
        video_frame.FourCC = NDIlib_FourCC_type_BGRA;
        size_t requiredSize = video_frame.xres * video_frame.yres * 4; // 4 bytes per pixel for RGBA
        video_frame.p_data = new uint8_t[requiredSize];
        video_frame.line_stride_in_bytes = targetWidth * 4; // Assuming 4 bytes per pixel

    size_t framebufferSize = resizedFBData.size();
    if (framebufferSize > requiredSize) {

        // Handle error: the framebuffer data is too large for the allocated buffer
        std::cerr << "Error: Framebuffer data size exceeds allocated buffer size." << std::endl;
        // Copy as much a possible anyway.
        std::memcpy(video_frame.p_data, resizedFBData.data(), requiredSize);

    } else {
        std::memcpy(video_frame.p_data, resizedFBData.data(), framebufferSize);
        // std::cout << "Framebuffer data copied to NDI video frame" << std::endl;
    }

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // std::cout << "Framebuffer unbound" << std::endl; });
}

void NDISender::sendToNDI()
{
    executeWithExceptionHandling([this]()
                                 {
                                     if (pNDI_send == nullptr)
                                     {
                                         throw std::runtime_error("NDI sender is not initialized.");
                                     }

                                     // Assuming video_frame is already populated from the captureFramebufferAndAssignToNDI function
                                     NDIlib_send_send_video_v2(pNDI_send, &video_frame);

                                     std::cout << "Sent frame to NDI." << std::endl; });
}

void NDISender::update()
{
    if (enabled)
    {
        captureFrame();
        sendToNDI();
    }
}
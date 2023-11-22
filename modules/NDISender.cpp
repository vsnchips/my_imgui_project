
#include <NDISender.hpp>

#include <iostream>

// #include <GL/glew.h>
 #include <GLFW/glfw3.h>

#include <imgui.h>

#include <renderable.hpp>

// Assume video_frame is a global variable of type NDIlib_video_frame_v2_t

NDISender::NDISender()
{
    FullScreenQuad::Init();

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
    closeNDI();
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
    video_frame = NDIlib_video_frame_v2_t();
    video_frame.xres = res[0]; // Set your video resolution
    video_frame.yres = res[1];

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

    ImGui::SliderInt2("Resolution", res, 0, 1920);

    if (ImGui::Button("Capture Framebuffer"))
    {
        captureFramebufferAndAssignToNDI(0, res[0], res[1], res[0], res[2]);
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
void resizeFramebuffer(GLFWwindow *window, int newWidth, int newHeight)
{
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
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
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
        std::cout << "Starting captureFramebufferAndAssignToNDI" << std::endl;

        // Bind the specified framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
        std::cout << "Framebuffer bound: " << framebufferID << std::endl;

        // Allocate memory for the framebuffer data
        std::vector<uint8_t> framebufferData(frameBufferWidth * frameBufferHeight * 4); // Assuming 4 bytes per pixel (RGBA)
        std::cout << "Framebuffer data allocated: " << frameBufferWidth * frameBufferHeight * 4 << " bytes" << std::endl;

        // Check if resizing is needed
        if (frameBufferWidth != targetWidth || frameBufferHeight != targetHeight)
        {
            std::cout << "Resizing is required" << std::endl;

            // Use a simple shader program which just renders a texture.
            const char *fragmentShaderSrc = R"glsl(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;
                uniform sampler2D screenTexture;
                void main()
                {
                   FragColor = texture(screenTexture, TexCoord);
                }
            )glsl";

            // Compile the shaders
            GLuint program = compileShaderProgram(
                FullScreenQuad::DefaultVertexShaderSource,
                fragmentShaderSrc);
            glUseProgram(program);
            std::cout << "Shader program compiled and used" << std::endl;

            // Rest of your resizing logic...

            std::cout << "Framebuffer data resized" << std::endl;
        }

        // Read the framebuffer data
        glReadPixels(0, 0, frameBufferWidth, frameBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, framebufferData.data());
        std::cout << "Framebuffer data read" << std::endl;

        // Assign the data to the NDI video frame
        video_frame.xres = targetWidth;
        video_frame.yres = targetHeight;
        video_frame.FourCC = NDIlib_FourCC_type_BGRA;
        video_frame.line_stride_in_bytes = targetWidth * 4; // Assuming 4 bytes per pixel

        // Assuming video_frame.p_data is already allocated and has enough space
        std::memcpy(video_frame.p_data, framebufferData.data(), framebufferData.size());
        std::cout << "Framebuffer data copied to NDI video frame" << std::endl;

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        std::cout << "Framebuffer unbound" << std::endl;
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

        std::cout << "Sent frame to NDI." << std::endl;

    });
}

void NDISender::update(){

}
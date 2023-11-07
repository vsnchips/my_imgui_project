#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <sstream>

#include "operators/isfRenderer.hpp"

// Function to create ImGui dialogs
void DoAllTheImGuis()
{
    ImGui::SetNextWindowBgAlpha(0.5f); // Set the alpha value for the window background

    ImGui::Begin("Window Dialog");
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Transparent Text");

    ImGui::Text("I am a Dialog");
    ImGui::End();
}

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

ISFRenderer* renderer = nullptr;
// Callback function to handle dropped files
void handleFileDrop(GLFWwindow* window, int count, const char* paths[]) {

    if (count > 0) {
        // Assuming only one file is dropped; you can handle multiple files if needed
        std::string shaderPath = paths[0];

        std::cout << "Drag/Drop recieved path " << shaderPath <<std::endl;

        // Create the ISFRenderer and load the shader
        if (renderer) {
            delete renderer; // Clean up the existing renderer, if any
        }
        renderer = ISFRenderer::createAndLoadISFShader(paths[0]);
    }
}

int main()
{
#pragma region init glfw
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        return -1;
    }
    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

#endif
    glfwWindowHint(GLFW_SAMPLES, 4); // Optional: enable anti-aliasing
    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

    // Request an alpha (transparency) channel
    glfwWindowHint(GLFW_ALPHA_BITS, 8); // 8 bits for alpha channel



#pragma endregion
#pragma region create window

    // Create the first GLFW window
    GLFWwindow *window1 = glfwCreateWindow(640, 480, "Window 1", nullptr, nullptr);
    if (!window1)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window1);
    glfwSwapInterval(1); // Enable vsync

    // Set the file drop callback
    glfwSetDropCallback(window1, handleFileDrop);

#pragma endregion
#pragma region init glew
    // Initialise GLEW
    bool err = glewInit() != GLEW_OK;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    int screen_width = 100;
    int screen_height = 100;
    glfwGetFramebufferSize(window1, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

#pragma endregion
#pragma region init ImGui
    // Initialize ImGui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(screen_width);
    io.DisplaySize.y = static_cast<float>(screen_height);

  #pragma region init imgui opengl renderer for window1

    // Initialize ImGui OpenGL3 renderer
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        // Handle initialization failure
        std::cout << "ImGui_ImplOpenGL3_Init failed";
        return 1;
    }
    if (!ImGui_ImplGlfw_InitForOpenGL(window1, true))
    {
        std::cout << "ImGui_ImplGlfw_InitForOpenGL failed";
        return 1;
    }
  #pragma endregion
#pragma endregion

#pragma region main loop
    while (!glfwWindowShouldClose(window1))
    {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window1, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.2f, 0.f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check if the renderer exists and render the quad
        if (renderer) {
            renderer->render();
        }

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DoAllTheImGuis(); // Pass window ID 1

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window1);
    }
#pragma endregion

#pragma region shutdown
    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
#pragma endregion
}

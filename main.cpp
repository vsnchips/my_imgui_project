#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <sstream>

// Function to create ImGui dialogs
void CreateImGuiDialog(int windowId)
{
    ImGui::Begin("Window Dialog");
    ImGui::Text("Window ID: %d", windowId);
    ImGui::End();
}

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{

#pragma region init glfw
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
#pragma endregion

#pragma region init glew
    // Initialise GLEW
    bool err = glewInit() != GLEW_OK;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

#pragma endregion
#pragma region ImGui context
    // Initialize ImGui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    std::cout << "Initialised ImGui";
#pragma endregion

#pragma window creation
    // Create the first GLFW window
    GLFWwindow *window1 = glfwCreateWindow(640, 480, "Window 1", nullptr, nullptr);
    if (!window1)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window1);
    glfwSwapInterval(1); // Enable vsync

    int screen_width;
    int screen_height;
    glfwGetFramebufferSize(window1, &screen_width, &screen_height);
    std::cout << "FrameBuffer Size" << screen_width << " " << screen_height;

    glViewport(0, 0, screen_width, screen_height);
    std::cout << "Created Viewport";

#pragma endregion

#pragma region init imgui opengl renderer
    // Initialize ImGui OpenGL3 renderer
    try
    {
        if (!ImGui_ImplGlfw_InitForOpenGL(window1, true))
        {
            throw std::runtime_error("ImGui_ImplGlfw_InitForOpenGL failed");
        }
        if (!ImGui_ImplOpenGL3_Init(glsl_version))
        {
            // Handle initialization failure
            throw std::runtime_error("ImGui_ImplOpenGL3_Init failed");
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    std::cout << "Initialised ImGui GLFW and OpenGL stuff";
#pragma endregion

#pragma region main loop
    try
    {
        while (!glfwWindowShouldClose(window1))
        {
            glfwPollEvents();
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            // feed inputs to dear imgui, start new frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // CreateImGuiDialog(1); // Pass window ID 1

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window1, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window1);
        }
#pragma endregion

#pragma region shutdown
        std::cout << "\nCLOSING\n";

        // Cleanup
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

#pragma endregion
    return 0;
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Function to create ImGui dialogs
void CreateImGuiDialog(int windowId) {
    ImGui::Begin("Window Dialog");
    ImGui::Text("Window ID: %d", windowId);
    ImGui::End();
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    // Create the first GLFW window
    GLFWwindow* window1 = glfwCreateWindow(640, 480, "Window 1", nullptr, nullptr);
    if (!window1) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window1);
    glfwSwapInterval(1); // Enable vsync

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window1, true);

    while (!glfwWindowShouldClose(window1)) {
        glfwPollEvents();

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        CreateImGuiDialog(1); // Pass window ID 1

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window1, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window1);
    }

    // Create the second GLFW window
    GLFWwindow* window2 = glfwCreateWindow(640, 480, "Window 2", nullptr, nullptr);
    if (!window2) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window2);
    glfwSwapInterval(1); // Enable vsync

    // Initialize ImGui for the second window
    ImGui_ImplGlfw_InitForOpenGL(window2, true);

    while (!glfwWindowShouldClose(window2)) {
        glfwPollEvents();

        // ImGui frame for the second window
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        CreateImGuiDialog(2); // Pass window ID 2

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window2, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window2);
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

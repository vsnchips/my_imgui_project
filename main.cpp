#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <sstream>

// Function to create ImGui dialogs
void CreateImGuiDialog(int windowId) {
    ImGui::Begin("Window Dialog");
    ImGui::Text("Window ID: %d", windowId);
    ImGui::End();
}

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {

	// Setup window
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        return -1;
    }
		// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char *glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
:w
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif



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

    // Initialize ImGui OpenGL3 renderer
    if (!ImGui_ImplOpenGL3_Init()) {
        // Handle initialization failure
        std::cout << "ImGui_ImplOpenGL3_Init failed";
        return 1;
    }
    if (!ImGui_ImplGlfw_InitForOpenGL(window1, true)){
        std::cout << "ImGui_ImplGlfw_InitForOpenGL failed";
        return 1;
    }

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

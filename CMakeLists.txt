cmake_minimum_required(VERSION 3.12)

project(GLFWImGuiTwoWindows)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

# Specify the directory containing ImGui source code
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui")
# Add the ImGui source code to your project
add_library(imgui STATIC
    ${IMGUI_DIR}/*.cpp
)

# Find and link ImGui
find_package(ImGui REQUIRED)
if (ImGui_FOUND)
    message(STATUS "Found ImGui: ${IMGUI_INCLUDE_DIRS}, ${IMGUI_LIBRARIES}")
else()
    message(FATAL_ERROR "ImGui not found. Please install ImGui or specify its location.")
endif()
include_directories(${IMGUI_INCLUDE_DIRS})

# Find and link GLFW
find_package(GLFW REQUIRED)
if (GLFW_FOUND)
    message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIRS}, ${GLFW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLFW not found. Please install GLFW or specify its location.")
endif()
include_directories(${GLFW_INCLUDE_DIRS})


# Add your source files here
add_executable(GLFWImGuiTwoWindows main.cpp)

# Link libraries
target_link_libraries(GLFWImGuiTwoWindows ${GLFW_LIBRARIES} ${IMGUI_LIBRARIES})
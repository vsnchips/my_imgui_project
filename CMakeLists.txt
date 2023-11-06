cmake_minimum_required(VERSION 3.12)

project(GLFWImGuiTwoWindows)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

# Find and link ImGui
find_package(ImGui REQUIRED)
if (ImGui_FOUND)
    message(STATUS "Found ImGui: ${IMGUI_INCLUDE_DIRS}, ${IMGUI_LIBRARIES}")
else()
    message(FATAL_ERROR "ImGui not found. Please install ImGui or specify its location.")
endif()
include_directories(${IMGUI_INCLUDE_DIRS})


# Add your source files here
add_executable(twoWindows main.cpp)

#Find and link GLFW
find_package(GLFW REQUIRED)
if (GLFW_FOUND)
    message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIRS} ${GLFW_LIBRARIES}")
    target_include_directories(twoWindows PRIVATE ${GLFW_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GLFW not found. Please install GLFW or specify its location.")
endif()

# Find and link GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_INCLUDE_DIRS}, ${GLEW_LIBRARIES}")
    target_include_directories(twoWindows PRIVATE ${GLEW_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GLEW not found. Please install GLEW or specify its location.")
endif()


# Link libraries
target_link_libraries(twoWindows ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${IMGUI_LIBRARIES})
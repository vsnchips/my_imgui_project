cmake_minimum_required(VERSION 3.12)

project(GLFWImGuiTwoWindows)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")


# Add your source files here
add_executable(twoWindows main.cpp)

## IMGUI!
# Include the subdirectory
add_subdirectory(extern/imgui)

# Example: Print the interface include directories and libraries of the ImGui target
get_target_property(imgui_interface_includes ImGui INTERFACE_INCLUDE_DIRECTORIES)
message("Interface include directories of ImGui: ${imgui_interface_includes}")

get_target_property(imgui_interface_libraries ImGui INTERFACE_LINK_LIBRARIES)
message("Interface libraries of ImGui: ${imgui_interface_libraries}")


#Find and link GLFW
find_package(GLFW REQUIRED)
if (GLFW_FOUND)
    message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIRS} ${GLFW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLFW not found. Please install GLFW or specify its location.")
endif()


# Find and link GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_INCLUDE_DIRS}, ${GLEW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLEW not found. Please install GLEW or specify its location.")
endif()

target_include_directories(twoWindows PRIVATE
${GLFW_INCLUDE_DIRS}
${GLEW_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/include
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/src
)

# Link libraries
target_link_libraries(twoWindows ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${IMGUI_LIBRARIES})
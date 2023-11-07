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
    message(STATUS "Found GLEW: ${GLEW_INCLUDE_DIRS}, ${GLEW_LIBRARY}")
else()
    message(FATAL_ERROR "GLEW not found. Please install GLEW or specify its location.")
endif()

# Define the header files for ImGui
set(IMGUI_HEADERS
    extern/imgui/imgui.h
    extern/imgui/imconfig.h
    extern/imgui/imgui_internal.h
    # Add other ImGui header files if needed
)

# Define the source files for ImGui
set(IMGUI_SOURCES
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
    # Add other ImGui source files if needed
)

# Create the ImGui library target
add_library(ImGui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Include the root directory of ImGui as an INTERFACE include directory
target_include_directories(ImGui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${GLFW_INCLUDE_DIRS}
)
target_include_directories(ImGui INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
)
target_link_libraries(ImGui ${GLFW_LIBRARIES})

#TODO:
#Find and link imnodes
target_include_directories(twoWindows PRIVATE
${GLFW_INCLUDE_DIRS}
${GLEW_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/include
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/src
)

# Link libraries
target_link_libraries(twoWindows ${GLFW_LIBRARIES} ${GLEW_LIBRARY} ImGui)
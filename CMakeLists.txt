cmake_minimum_required(VERSION 3.12)

project(GLFWImGuiTwoWindows)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# (Optional) Set the C and C++ compilers to Clang
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")

# Include custom CMake modules
if(APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules_apple")
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules_windows")
endif()


##### MAIN TARGET SOURCES #########
# Create the executable target
add_executable(twoWindows main.cpp)

# Glob source files in the 'operators/' directory
file(GLOB OPERATORS_SOURCE_FILES operators/*.cpp)

# Add the globbed source files to the target
target_sources(twoWindows PRIVATE ${OPERATORS_SOURCE_FILES})



########### IMGUI! ##################
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


find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_INCLUDE_DIRS}, ${GL}, ${OPENGL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenGL not found. Please install OpenGL or specify its location.")
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
    extern/imgui/imgui_tables.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
    # Add other ImGui source files if needed
)

# Create the ImGui library target
add_library(ImGui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Include the root directory of ImGui as an INTERFACE include directory
target_include_directories(ImGui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
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

target_compile_definitions(twoWindows PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
# Link libraries
target_link_libraries(twoWindows ImGui ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )
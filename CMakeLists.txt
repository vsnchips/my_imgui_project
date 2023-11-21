cmake_minimum_required(VERSION 3.12)

project(GLFWImGuiTwoWindows)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Optional) Set the C and C++ compilers to Clang
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
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
# Glob source files in the specified directories and their subdirectories
file(GLOB_RECURSE MODULE_SOURCES "modules/*.cpp")
message("MODULE_SOURCES " ${MODULE_SOURCES})

file(GLOB_RECURSE OPERATOR_SOURCES "operators/*.cpp")
message("OPERATOR_SOURCES" ${OPERATOR_SOURCES})

file(GLOB_RECURSE TYPE_SOURCES "types/*.cpp")
message("TYPE_SOURCES " ${TYPE_SOURCES})

file(GLOB_RECURSE WIDGET_SOURCES "widgets/*.cpp")
message("WIDGET_SOURCES " ${WIDGET_SOURCES})

set(ALL_SOURCES
    ${MODULE_SOURCES}
    ${OPERATOR_SOURCES}
    ${TYPE_SOURCES}
    ${WIDGET_SOURCES}
)

# Add the globbed source files to the target
target_sources(twoWindows PRIVATE ${ALL_SOURCES})


########### GLFW! ##################
#Find and link GLFW
find_package(GLFW REQUIRED)
if (GLFW_FOUND)
    message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIRS} ${GLFW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLFW not found. Please install GLFW or specify its location.")
endif()

########### GLEW! ##################
# Find and link GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_INCLUDE_DIRS}, ${GLEW_LIBRARY}")
else()
    message(FATAL_ERROR "GLEW not found. Please install GLEW or specify its location.")
endif()


########### OPENGL! ##################
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_INCLUDE_DIRS}, ${GL}, ${OPENGL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenGL not found. Please install OpenGL or specify its location.")
endif()


########### IMGUI! ##################
# Define the header files for ImGui
set(IMGUI_HEADERS
    extern/imgui/imgui.h
    extern/imgui/imconfig.h
    extern/imgui/imgui_internal.h
    # Add other ImGui header files if needed
    extern/imgui_toggle/imgui_toggle.h
)

# Define the source files for ImGui using file globbing
file(GLOB IMGUI_SOURCES
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
    # Add other ImGui source files if needed
    extern/imgui_toggle/*.cpp
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


########### JSON! ##############################################
# ChatGPT says to add the include path literally, first.
# Set the path to the nlohmann/json library
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/json/include")

# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")

# If you only include this third party in PRIVATE source files, you do not
# need to install it when your main project gets installed.
set(JSON_Install OFF CACHE INTERNAL "")

# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.
add_subdirectory(extern/json)

############## OSC #####################

find_package(liblo REQUIRED)
set(OSC_INCLUDE_DIRS ${LIBLO_INCLUDE_DIRS})
set(OSC_LIBS ${LIBLO_LIBRARY})

############## Add next libs here #####################
#TODO:
#Find and link imnodes


#TODO:
#Find and link imnodes

############### LINK UP THE EXE ######################

target_include_directories(twoWindows PRIVATE
"operators"
"modules"
"types"
"widgets"
${GLFW_INCLUDE_DIRS}
${GLEW_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/include
${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui_toggle
${LIBLO_INCLUDE_DIRS}
${JSON_INCLUDE_DIR}
)

target_compile_definitions(twoWindows PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
# Link libraries
target_link_libraries(twoWindows
  ImGui
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OSC_LIBS}
)
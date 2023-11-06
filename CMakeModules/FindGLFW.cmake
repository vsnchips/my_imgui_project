message("Searching for GLFW3 headers and libs.")
message("The CMAKE_SYSTEM_PROCESSOR var is:"${CMAKE_SYSTEM_PROCESSOR})

# Check if the platform is macOS

if(APPLE)
    # On macOS, set the search path to Homebrew's GLFW installation directory
    find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS /opt/homebrew/Cellar/glfw)

    # Find the GLFW library file
    find_library(GLFW_LIBRARY NAMES glfw PATHS /opt/homebrew/Cellar/glfw)
else()
    # On other platforms, you should provide the appropriate search paths or handle the error differently
    message(FATAL_ERROR "GLFW search paths not specified for this platform. Please configure them.")
endif()

# Check if GLFW_INCLUDE_DIR and GLFW_LIBRARY are found
if(GLFW_INCLUDE_DIR )
    message(STATUS "Found GLFW include directory: ${GLFW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "GLFW include directory not found. Please install GLFW or specify their locations.")
endif()

# Check if GLFW_INCLUDE_DIR and GLFW_LIBRARY are found
if(GLFW_LIBRARY)
    message(STATUS "Found GLFW library: ${GLFW_LIBRARY}")
else()
    message(FATAL_ERROR "GLFW library not found. Please install GLFW or specify their locations.")
endif()

set(GLFW_FOUND TRUE)
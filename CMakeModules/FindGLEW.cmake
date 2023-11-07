message("Searching for GLEW3 headers and libs.")
message("The CMAKE_SYSTEM_PROCESSOR var is:" ${CMAKE_SYSTEM_PROCESSOR})

# Check if the platform is macOS

if(APPLE)
    # On macOS, set the search path to Homebrew's GLEW installation directory
    # this is broken: find_path(GLEW_INCLUDE_DIR NAMES GL PATHS /opt/homebrew/Cellar/ NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH)
    set(GLEW_INCLUDE_DIRS /opt/homebrew/include)

    # Find the GLEW library file
    find_library(GLEW_LIBRARY NAMES GLEW PATHS /opt/homebrew/Cellar/glfw)
else()
    # On other platforms, you should provide the appropriate search paths or handle the error differently
    message(FATAL_ERROR "GLEW search paths not specified for this platform. Please configure them.")
endif()

# Check if GLEW_INCLUDE_DIR and GLEW_LIBRARY are found
if(GLEW_INCLUDE_DIRS )
    message(STATUS "Found GLEW include directory: ${GLEW_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "GLEW include directory not found. Please install GLEW or specify their locations.")
endif()

# Check if GLEW_INCLUDE_DIR and GLEW_LIBRARY are found
if(GLEW_LIBRARY)
    message(STATUS "Found GLEW library: ${GLEW_LIBRARY}")
else()
    message(FATAL_ERROR "GLEW library not found. Please install GLEW or specify their locations.")
endif()

set(GLEW_INC)

set(GLEW_FOUND TRUE)
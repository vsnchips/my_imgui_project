message("Searching for GLEW3 headers and libs.")
message("The CMAKE_SYSTEM_PROCESSOR var is:" ${CMAKE_SYSTEM_PROCESSOR})

# Check if the platform is macOS

# Set the search path to Homebrew's GLFW installation directory
find_path(GLEW_INCLUDE_DIRS NAMES GL/glew.h PATHS "C:\\Program Files\\glew-2.2.0\\include")

# Find the GLEW library file
find_library(GLEW_LIBRARY NAMES glew32 PATHS "C:\\Program Files\\glew-2.2.0\\lib\\Release\\x64")

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
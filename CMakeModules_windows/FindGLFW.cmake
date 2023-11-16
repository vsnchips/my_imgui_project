message("Searching for GLFW3 headers and libs.")
message("The CMAKE_SYSTEM_PROCESSOR var is:" ${CMAKE_SYSTEM_PROCESSOR})


# Set the search path to Homebrew's GLFW installation directory
find_path(GLFW_INCLUDE_DIRS NAMES GLFW/glfw3.h PATHS "C:\\Program Files\\glfw3\\glfw-3.3.8.bin.WIN64\\include")

# Find the GLFW library file
find_library(GLFW_LIBRARIES NAMES glfw3 PATHS "C:\\Program Files\\glfw3\\glfw-3.3.8.bin.WIN64\\lib-vc2022")

# Check if GLFW_INCLUDE_DIRS are found
if(GLFW_INCLUDE_DIRS )
    message(STATUS "Found GLFW include directory: ${GLFW_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "GLFW include directory not found. Please install GLFW or specify their locations.")
endif()

# Check GLFW_LIBRARIES are found
if(GLFW_LIBRARIES)
    message(STATUS "Found GLFW library: ${GLFW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLFW library not found. Please install GLFW or specify their locations.")
endif()

set(GLFW_FOUND TRUE)
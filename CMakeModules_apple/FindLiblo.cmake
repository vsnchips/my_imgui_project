message("Searching for LIBLO headers and libs.")

# Set the search path to Homebrew's GLFW installation directory
find_path(LIBLO_BUILT_INCLUDE_DIRS NAMES lo/lo.h  PATHS "${CMAKE_CURRENT_SOURCE_DIR}\\extern\\liblo\\")
set(LIBLO_SRC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}\\extern\\liblo") #for lo/lo_types etc
set(LIBLO_INCLUDE_DIRS ${LIBLO_SRC_INCLUDE_DIRS} ${LIBLO_BUILT_INCLUDE_DIRS})

# Find the LIBLO library file
find_library(LIBLO_LIBRARY NAMES liblo.dylib PATHS "${CMAKE_CURRENT_SOURCE_DIR}\\extern\\liblo")

# Check if LIBLO_INCLUDE_DIR and LIBLO_LIBRARY are found
if(LIBLO_INCLUDE_DIRS )
    message(STATUS "Found LIBLO include directory: ${LIBLO_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "LIBLO include directory not found. Please install LIBLO or specify their locations.")
endif()

# Check if LIBLO_INCLUDE_DIR and LIBLO_LIBRARY are found
if(LIBLO_LIBRARY)
    message(STATUS "Found LIBLO library: ${LIBLO_LIBRARY}")
else()
    message(FATAL_ERROR "LIBLO library not found. Please install LIBLO or specify their locations.")
endif()

set(LIBLO_FOUND TRUE)
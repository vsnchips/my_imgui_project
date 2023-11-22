message("Searching for NDI headers and libs.")

# Set the search path to Homebrew's GLFW installation directory
#find_path(NDI_INCLUDE_DIRS NAMES Processing.NDI.Lib.h PATHS "${CMAKE_CURRENT_SOURCE_DIR}\\extern\\ndi\\include")
find_path(NDI_INCLUDE_DIRS NAMES Processing.NDI.Lib.h PATHS "C:/Program Files/NDI/NDI 5 SDK/Include")

# Find the NDI library file
#find_library(NDI_LIBRARY NAMES libndi.dylib PATHS "${CMAKE_CURRENT_SOURCE_DIR}\\extern\\ndi\\lib\\macOS")
find_library(NDI_LIBRARY NAMES Processing.NDI.Lib.x64.lib PATHS "C:/Program Files/NDI/NDI 5 SDK/Lib/x64")

# Check if NDI_INCLUDE_DIR and NDI_LIBRARY are found
if( NDI_INCLUDE_DIRS )
    message(STATUS "Found NDI include directory: ${NDI_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "NDI include directory not found. Please install NDI or specify their locations.")
endif()

# Check if NDI_INCLUDE_DIR and NDI_LIBRARY are found
if(NDI_LIBRARY)
    message(STATUS "Found NDI library: ${NDI_LIBRARY}")
else()
    message(FATAL_ERROR "NDI library not found. Please install NDI or specify their locations.")
endif()

set(NDI_FOUND TRUE)
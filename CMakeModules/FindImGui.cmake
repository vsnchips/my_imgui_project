# Define a macro for finding ImGui
message( current source dir: ${CMAKE_CURRENT_SOURCE_DIR})
message( source dir: ${CMAKE_SOURCE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/imgui/)

set(IMGUI_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/imgui/)

set(ImGui_FOUND TRUE)
#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(ImGui REQUIRED_VARS ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui imgui)
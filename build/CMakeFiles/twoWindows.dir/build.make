# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/daston/play/my_imgui_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/daston/play/my_imgui_project/build

# Include any dependencies generated for this target.
include CMakeFiles/twoWindows.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/twoWindows.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/twoWindows.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/twoWindows.dir/flags.make

CMakeFiles/twoWindows.dir/main.cpp.o: CMakeFiles/twoWindows.dir/flags.make
CMakeFiles/twoWindows.dir/main.cpp.o: /Users/daston/play/my_imgui_project/main.cpp
CMakeFiles/twoWindows.dir/main.cpp.o: CMakeFiles/twoWindows.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/daston/play/my_imgui_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/twoWindows.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/twoWindows.dir/main.cpp.o -MF CMakeFiles/twoWindows.dir/main.cpp.o.d -o CMakeFiles/twoWindows.dir/main.cpp.o -c /Users/daston/play/my_imgui_project/main.cpp

CMakeFiles/twoWindows.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/twoWindows.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/daston/play/my_imgui_project/main.cpp > CMakeFiles/twoWindows.dir/main.cpp.i

CMakeFiles/twoWindows.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/twoWindows.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/daston/play/my_imgui_project/main.cpp -o CMakeFiles/twoWindows.dir/main.cpp.s

# Object files for target twoWindows
twoWindows_OBJECTS = \
"CMakeFiles/twoWindows.dir/main.cpp.o"

# External object files for target twoWindows
twoWindows_EXTERNAL_OBJECTS =

twoWindows: CMakeFiles/twoWindows.dir/main.cpp.o
twoWindows: CMakeFiles/twoWindows.dir/build.make
twoWindows: /opt/homebrew/lib/libglfw.dylib
twoWindows: CMakeFiles/twoWindows.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/daston/play/my_imgui_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable twoWindows"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/twoWindows.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/twoWindows.dir/build: twoWindows
.PHONY : CMakeFiles/twoWindows.dir/build

CMakeFiles/twoWindows.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/twoWindows.dir/cmake_clean.cmake
.PHONY : CMakeFiles/twoWindows.dir/clean

CMakeFiles/twoWindows.dir/depend:
	cd /Users/daston/play/my_imgui_project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/daston/play/my_imgui_project /Users/daston/play/my_imgui_project /Users/daston/play/my_imgui_project/build /Users/daston/play/my_imgui_project/build /Users/daston/play/my_imgui_project/build/CMakeFiles/twoWindows.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/twoWindows.dir/depend


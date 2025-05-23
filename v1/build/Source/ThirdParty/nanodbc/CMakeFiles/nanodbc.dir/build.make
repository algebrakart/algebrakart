# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /code/dev/algebrakart/v1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /code/dev/algebrakart/v1/build

# Include any dependencies generated for this target.
include Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/depend.make

# Include the progress variables for this target.
include Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/progress.make

# Include the compile flags for this target's objects.
include Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/flags.make

Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o: Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/flags.make
Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o: ../Source/ThirdParty/nanodbc/nanodbc/nanodbc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o -c /code/dev/algebrakart/v1/Source/ThirdParty/nanodbc/nanodbc/nanodbc.cpp

Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.i"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /code/dev/algebrakart/v1/Source/ThirdParty/nanodbc/nanodbc/nanodbc.cpp > CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.i

Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.s"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /code/dev/algebrakart/v1/Source/ThirdParty/nanodbc/nanodbc/nanodbc.cpp -o CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.s

# Object files for target nanodbc
nanodbc_OBJECTS = \
"CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o"

# External object files for target nanodbc
nanodbc_EXTERNAL_OBJECTS =

Source/ThirdParty/nanodbc/libnanodbc.a: Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/nanodbc/nanodbc.cpp.o
Source/ThirdParty/nanodbc/libnanodbc.a: Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/build.make
Source/ThirdParty/nanodbc/libnanodbc.a: Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libnanodbc.a"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && $(CMAKE_COMMAND) -P CMakeFiles/nanodbc.dir/cmake_clean_target.cmake
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nanodbc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/build: Source/ThirdParty/nanodbc/libnanodbc.a

.PHONY : Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/build

Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/clean:
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc && $(CMAKE_COMMAND) -P CMakeFiles/nanodbc.dir/cmake_clean.cmake
.PHONY : Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/clean

Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/depend:
	cd /code/dev/algebrakart/v1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /code/dev/algebrakart/v1 /code/dev/algebrakart/v1/Source/ThirdParty/nanodbc /code/dev/algebrakart/v1/build /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc /code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/ThirdParty/nanodbc/CMakeFiles/nanodbc.dir/depend


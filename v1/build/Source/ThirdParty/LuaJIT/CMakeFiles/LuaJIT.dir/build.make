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
include Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend.make

# Include the progress variables for this target.
include Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/progress.make

# Include the compile flags for this target's objects.
include Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/flags.make

Source/ThirdParty/LuaJIT/generated/lj_vm.S: Source/ThirdParty/LuaJIT/src/host/buildvm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating buildvm output: lj_vm.S"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m elfasm -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_vm.S

Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_base.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_math.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_bit.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_string.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_table.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_io.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_os.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_package.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_debug.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_jit.c
Source/ThirdParty/LuaJIT/generated/lj_bcdef.h: ../Source/ThirdParty/LuaJIT/src/lib_ffi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating buildvm output: lj_bcdef.h"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m bcdef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_bcdef.h src/lib_base.c src/lib_math.c src/lib_bit.c src/lib_string.c src/lib_table.c src/lib_io.c src/lib_os.c src/lib_package.c src/lib_debug.c src/lib_jit.c src/lib_ffi.c

Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_base.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_math.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_bit.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_string.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_table.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_io.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_os.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_package.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_debug.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_jit.c
Source/ThirdParty/LuaJIT/generated/lj_ffdef.h: ../Source/ThirdParty/LuaJIT/src/lib_ffi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating buildvm output: lj_ffdef.h"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m ffdef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_ffdef.h src/lib_base.c src/lib_math.c src/lib_bit.c src/lib_string.c src/lib_table.c src/lib_io.c src/lib_os.c src/lib_package.c src/lib_debug.c src/lib_jit.c src/lib_ffi.c

Source/ThirdParty/LuaJIT/generated/lj_libdef.h: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_base.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_math.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_bit.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_string.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_table.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_io.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_os.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_package.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_debug.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_jit.c
Source/ThirdParty/LuaJIT/generated/lj_libdef.h: ../Source/ThirdParty/LuaJIT/src/lib_ffi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating buildvm output: lj_libdef.h"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m libdef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_libdef.h src/lib_base.c src/lib_math.c src/lib_bit.c src/lib_string.c src/lib_table.c src/lib_io.c src/lib_os.c src/lib_package.c src/lib_debug.c src/lib_jit.c src/lib_ffi.c

Source/ThirdParty/LuaJIT/generated/lj_recdef.h: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_base.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_math.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_bit.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_string.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_table.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_io.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_os.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_package.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_debug.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_jit.c
Source/ThirdParty/LuaJIT/generated/lj_recdef.h: ../Source/ThirdParty/LuaJIT/src/lib_ffi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating buildvm output: lj_recdef.h"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m recdef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_recdef.h src/lib_base.c src/lib_math.c src/lib_bit.c src/lib_string.c src/lib_table.c src/lib_io.c src/lib_os.c src/lib_package.c src/lib_debug.c src/lib_jit.c src/lib_ffi.c

Source/ThirdParty/LuaJIT/generated/vmdef.lua: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_base.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_math.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_bit.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_string.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_table.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_io.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_os.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_package.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_debug.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_jit.c
Source/ThirdParty/LuaJIT/generated/vmdef.lua: ../Source/ThirdParty/LuaJIT/src/lib_ffi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Generating buildvm output: vmdef.lua"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m vmdef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/vmdef.lua src/lib_base.c src/lib_math.c src/lib_bit.c src/lib_string.c src/lib_table.c src/lib_io.c src/lib_os.c src/lib_package.c src/lib_debug.c src/lib_jit.c src/lib_ffi.c

Source/ThirdParty/LuaJIT/generated/lj_folddef.h: Source/ThirdParty/LuaJIT/src/host/buildvm
Source/ThirdParty/LuaJIT/generated/lj_folddef.h: ../Source/ThirdParty/LuaJIT/src/lj_opt_fold.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Generating buildvm output: lj_folddef.h"
	cd /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT && /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/src/host/buildvm -m folddef -o /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_folddef.h src/lj_opt_fold.c

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.o: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/flags.make
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.o: ../Source/ThirdParty/LuaJIT/src/ljamalg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.o"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/LuaJIT.dir/src/ljamalg.c.o   -c /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT/src/ljamalg.c

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/LuaJIT.dir/src/ljamalg.c.i"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT/src/ljamalg.c > CMakeFiles/LuaJIT.dir/src/ljamalg.c.i

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/LuaJIT.dir/src/ljamalg.c.s"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT/src/ljamalg.c -o CMakeFiles/LuaJIT.dir/src/ljamalg.c.s

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/flags.make
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o: Source/ThirdParty/LuaJIT/generated/lj_vm.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building ASM object Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && /usr/bin/cc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o -c /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/generated/lj_vm.S

# Object files for target LuaJIT
LuaJIT_OBJECTS = \
"CMakeFiles/LuaJIT.dir/src/ljamalg.c.o" \
"CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o"

# External object files for target LuaJIT
LuaJIT_EXTERNAL_OBJECTS =

Source/ThirdParty/LuaJIT/libLuaJIT.a: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/src/ljamalg.c.o
Source/ThirdParty/LuaJIT/libLuaJIT.a: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o
Source/ThirdParty/LuaJIT/libLuaJIT.a: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/build.make
Source/ThirdParty/LuaJIT/libLuaJIT.a: Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/code/dev/algebrakart/v1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C static library libLuaJIT.a"
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && $(CMAKE_COMMAND) -P CMakeFiles/LuaJIT.dir/cmake_clean_target.cmake
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LuaJIT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/build: Source/ThirdParty/LuaJIT/libLuaJIT.a

.PHONY : Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/build

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/clean:
	cd /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT && $(CMAKE_COMMAND) -P CMakeFiles/LuaJIT.dir/cmake_clean.cmake
.PHONY : Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/clean

Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_vm.S
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_bcdef.h
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_ffdef.h
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_libdef.h
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_recdef.h
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/vmdef.lua
Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend: Source/ThirdParty/LuaJIT/generated/lj_folddef.h
	cd /code/dev/algebrakart/v1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /code/dev/algebrakart/v1 /code/dev/algebrakart/v1/Source/ThirdParty/LuaJIT /code/dev/algebrakart/v1/build /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT /code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/ThirdParty/LuaJIT/CMakeFiles/LuaJIT.dir/depend

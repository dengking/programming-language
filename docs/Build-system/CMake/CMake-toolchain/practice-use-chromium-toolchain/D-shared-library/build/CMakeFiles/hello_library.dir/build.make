# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build

# Include any dependencies generated for this target.
include CMakeFiles/hello_library.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hello_library.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hello_library.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hello_library.dir/flags.make

CMakeFiles/hello_library.dir/src/Hello.cpp.o: CMakeFiles/hello_library.dir/flags.make
CMakeFiles/hello_library.dir/src/Hello.cpp.o: ../src/Hello.cpp
CMakeFiles/hello_library.dir/src/Hello.cpp.o: CMakeFiles/hello_library.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hello_library.dir/src/Hello.cpp.o"
	/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hello_library.dir/src/Hello.cpp.o -MF CMakeFiles/hello_library.dir/src/Hello.cpp.o.d -o CMakeFiles/hello_library.dir/src/Hello.cpp.o -c /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/Hello.cpp

CMakeFiles/hello_library.dir/src/Hello.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hello_library.dir/src/Hello.cpp.i"
	/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/Hello.cpp > CMakeFiles/hello_library.dir/src/Hello.cpp.i

CMakeFiles/hello_library.dir/src/Hello.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hello_library.dir/src/Hello.cpp.s"
	/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/Hello.cpp -o CMakeFiles/hello_library.dir/src/Hello.cpp.s

# Object files for target hello_library
hello_library_OBJECTS = \
"CMakeFiles/hello_library.dir/src/Hello.cpp.o"

# External object files for target hello_library
hello_library_EXTERNAL_OBJECTS =

libhello_library.dylib: CMakeFiles/hello_library.dir/src/Hello.cpp.o
libhello_library.dylib: CMakeFiles/hello_library.dir/build.make
libhello_library.dylib: CMakeFiles/hello_library.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libhello_library.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hello_library.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hello_library.dir/build: libhello_library.dylib
.PHONY : CMakeFiles/hello_library.dir/build

CMakeFiles/hello_library.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hello_library.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hello_library.dir/clean

CMakeFiles/hello_library.dir/depend:
	cd /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles/hello_library.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hello_library.dir/depend


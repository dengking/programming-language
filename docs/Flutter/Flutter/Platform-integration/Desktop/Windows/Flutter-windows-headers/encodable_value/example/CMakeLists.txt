# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (test)
# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 17)

# Add an executable
add_executable(main main.cc)
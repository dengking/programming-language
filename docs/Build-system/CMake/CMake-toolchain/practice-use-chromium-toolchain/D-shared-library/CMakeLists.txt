cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang")
set(CMAKE_CXX_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++")
set(CMAKE_LINKER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")


project(hello_library)

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(hello_library SHARED 
    src/Hello.cpp
)
add_library(hello::library ALIAS hello_library)

target_include_directories(hello_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(hello_binary
    src/main.cpp
)

# link the new hello_library target with the hello_binary target
target_link_libraries( hello_binary
    PRIVATE 
        hello::library
)

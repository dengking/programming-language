cmake_minimum_required(VERSION 3.5)

project(example)

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(example SHARED 
    example.cxx example_wrap.cxx
)



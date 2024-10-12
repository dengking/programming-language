# CMake examples



## [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html#cmake-tutorial)

[Step 1: A Basic Starting Point](https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html)

```cmake
# TODO 1: Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)
# TODO 2: Create a project named Tutorial
project(Tutorial)
# TODO 7: Set the project version number as 1.0 in the above project command
project(Tutorial VERSION 1.0)
# TODO 6: Set the variable CMAKE_CXX_STANDARD to 11
#         and the variable CMAKE_CXX_STANDARD_REQUIRED to True
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# TODO 8: Use configure_file to configure and copy TutorialConfig.h.in to
#         TutorialConfig.h
configure_file(TutorialConfig.h.in TutorialConfig.h)
# TODO 3: Add an executable called Tutorial to the project
# Hint: Be sure to specify the source file as tutorial.cxx
add_executable(Tutorial tutorial.cxx)
# TODO 9: Use target_include_directories to include ${PROJECT_BINARY_DIR}
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
)
```



## [pabloariasal](https://github.com/pabloariasal)/**[modern-cmake-sample](https://github.com/pabloariasal/modern-cmake-sample)** 





## [ttroy50](https://github.com/ttroy50)/**[cmake-examples](https://github.com/ttroy50/cmake-examples)**


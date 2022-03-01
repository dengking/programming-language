# CMake toolchain



## docs CMAKE_TOOLCHAIN_FILE[¶](https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html#cmake-toolchain-file)

Path to toolchain file supplied to [`cmake(1)`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)).



## stackoverflow [CMake: specifying build toolchain](https://stackoverflow.com/questions/5098360/cmake-specifying-build-toolchain)



### [A](https://stackoverflow.com/a/5099229)

Have a look [here](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/CrossCompiling): basically, you define a "toolchain file" that sets things like the system name, paths to compilers and so on. You then call `cmake` like so:

```SH]
cmake /path/to/src -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain/foo-bar-baz.cmake
```



## github [SX-Aurora](https://github.com/SX-Aurora)/**[CMake-toolchain-file](https://github.com/SX-Aurora/CMake-toolchain-file)**


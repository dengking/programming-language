# CMake toolchain



## 使用chromium clang来编译

指定编译器为chromium clang

有如下方法：

1、CMake-toolchain-file

```cmake
set(CMAKE_C_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang") set(CMAKE_CXX_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++")
```



2、 environment variable

```
export CC=/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang export CXX=/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++
```

## CMake toolchain file

### docs CMAKE_TOOLCHAIN_FILE[¶](https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html#cmake-toolchain-file)

Path to toolchain file supplied to [`cmake(1)`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)).



### stackoverflow [CMake: specifying build toolchain](https://stackoverflow.com/questions/5098360/cmake-specifying-build-toolchain)



#### [A](https://stackoverflow.com/a/5099229)

Have a look [here](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/CrossCompiling): basically, you define a "toolchain file" that sets things like the system name, paths to compilers and so on. You then call `cmake` like so:

```SH]
cmake /path/to/src -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain/foo-bar-baz.cmake
```

### stackoverflow [Switching between GCC and Clang/LLVM using CMake](https://stackoverflow.com/questions/7031126/switching-between-gcc-and-clang-llvm-using-cmake)





### github [SX-Aurora](https://github.com/SX-Aurora)/**[CMake-toolchain-file](https://github.com/SX-Aurora/CMake-toolchain-file)**


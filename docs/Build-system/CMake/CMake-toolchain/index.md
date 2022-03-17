# CMake toolchain

理想的写法应该是如chromium那样，定义专门的toolchain file：



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

## docs [cmake-toolchains(7)](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#id7)[¶](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cmake-toolchains-7)

### [Variables and Properties](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#id10)[¶](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#variables-and-properties)

As the linker is invoked by the compiler driver, CMake needs a way to determine which compiler to use to invoke the linker. This is calculated by the [`LANGUAGE`](https://cmake.org/cmake/help/latest/prop_sf/LANGUAGE.html#prop_sf:LANGUAGE) of source files in the target, and in the case of static libraries, the language of the dependent libraries. The choice CMake makes may be overridden with the [`LINKER_LANGUAGE`](https://cmake.org/cmake/help/latest/prop_tgt/LINKER_LANGUAGE.html#prop_tgt:LINKER_LANGUAGE) target property.

### [Cross Compiling using Clang](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#id15)[¶](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-using-clang)

Similarly, some compilers do not ship their own supplementary utilities such as linkers, but provide a way to specify the location of the external toolchain which will be used by the compiler driver. The [`CMAKE_<LANG>_COMPILER_EXTERNAL_TOOLCHAIN`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_EXTERNAL_TOOLCHAIN.html#variable:CMAKE__COMPILER_EXTERNAL_TOOLCHAIN) variable can be set in a toolchain file to pass the path to the compiler driver.



## CMake toolchain file

### docs CMAKE_TOOLCHAIN_FILE[¶](https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html#cmake-toolchain-file)

Path to toolchain file supplied to [`cmake(1)`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)).

### 其他素材

github [SX-Aurora](https://github.com/SX-Aurora)/**[CMake-toolchain-file](https://github.com/SX-Aurora/CMake-toolchain-file)**



### stackoverflow [CMake: specifying build toolchain](https://stackoverflow.com/questions/5098360/cmake-specifying-build-toolchain)



#### [A](https://stackoverflow.com/a/5099229)

Have a look [here](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/CrossCompiling): basically, you define a "toolchain file" that sets things like the system name, paths to compilers and so on. You then call `cmake` like so:

```SH]
cmake /path/to/src -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain/foo-bar-baz.cmake
```

### stackoverflow [Switching between GCC and Clang/LLVM using CMake](https://stackoverflow.com/questions/7031126/switching-between-gcc-and-clang-llvm-using-cmake)



## linker

[[LLVMdev] How to change the linker of clang](https://groups.google.com/g/llvm-dev/c/gFVH78ZpCX0)
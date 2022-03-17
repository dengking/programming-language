# Assembling a Complete Toolchain

clang仅仅是一个front end，要想完全替代gcc还需要结合本文的内容。



## compiler driver

下面是一些素材：

1、doc [Assembling a Complete Toolchain](https://clang.llvm.org/docs/Toolchain.html) 

2、[clang-cl What it is, how it works, and how to use it](https://llvm.org/devmtg/2014-04/PDFs/Talks/clang-cl.pdf)

3、[cmake-toolchains(7)](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#id7)[¶](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cmake-toolchains-7)



## doc [Assembling a Complete Toolchain](https://clang.llvm.org/docs/Toolchain.html) 

### [Introduction](https://clang.llvm.org/docs/Toolchain.html#id8)[¶](https://clang.llvm.org/docs/Toolchain.html#introduction)

### [Tools](https://clang.llvm.org/docs/Toolchain.html#id9)[¶](https://clang.llvm.org/docs/Toolchain.html#tools)

**Preprocessor**

**Parsing**

**IR generation**

**Compiler backend**

**Assembler**

**Linker**

Clang provides all of these pieces other than the linker.

> NOTE: 
>
> 一、使用什么替代？

#### [Assembler](https://clang.llvm.org/docs/Toolchain.html#id12)



#### [Linker](https://clang.llvm.org/docs/Toolchain.html#id13)

Clang can be configured to use one of several different linkers:

- GNU ld
- GNU gold
- LLVM’s [lld](https://lld.llvm.org/)
- MSVC’s link.exe

> NOTE: 
>
> 如何去配置呢？



### [Runtime libraries](https://clang.llvm.org/docs/Toolchain.html#id14)[¶](https://clang.llvm.org/docs/Toolchain.html#runtime-libraries)

A number of different runtime libraries are required to provide different layers of support for C family programs. Clang will implicitly link an appropriate implementation of each runtime library, selected based on target defaults or explicitly selected by the `--rtlib=` and `--stdlib=` flags.

> NOTE: 需要做的是使用`--rtlib=` and `--stdlib=` flags来使用llvm的library。

#### [Compiler runtime](https://clang.llvm.org/docs/Toolchain.html#id15)[¶](https://clang.llvm.org/docs/Toolchain.html#compiler-runtime)



##### compiler-rt (LLVM)[¶](https://clang.llvm.org/docs/Toolchain.html#compiler-rt-llvm)

##### libgcc_s (GNU)[¶](https://clang.llvm.org/docs/Toolchain.html#libgcc-s-gnu)

#### [Atomics library](https://clang.llvm.org/docs/Toolchain.html#id16)[¶](https://clang.llvm.org/docs/Toolchain.html#atomics-library)

##### compiler-rt (LLVM)[¶](https://clang.llvm.org/docs/Toolchain.html#id1)

##### libatomic (GNU)[¶](https://clang.llvm.org/docs/Toolchain.html#libatomic-gnu)

#### [Sanitizer runtime](https://clang.llvm.org/docs/Toolchain.html#id18)[¶](https://clang.llvm.org/docs/Toolchain.html#sanitizer-runtime)

#### [C standard library](https://clang.llvm.org/docs/Toolchain.html#id19)[¶](https://clang.llvm.org/docs/Toolchain.html#c-standard-library)

#### [C++ ABI library](https://clang.llvm.org/docs/Toolchain.html#id20)[¶](https://clang.llvm.org/docs/Toolchain.html#c-abi-library)

#### [C++ standard library](https://clang.llvm.org/docs/Toolchain.html#id21)[¶](https://clang.llvm.org/docs/Toolchain.html#id6)


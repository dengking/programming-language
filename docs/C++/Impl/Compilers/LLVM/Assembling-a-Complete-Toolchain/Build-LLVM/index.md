# Build LLVM

Google: llvm install，检索到了 Getting Started with the LLVM System[¶](https://llvm.org/docs/GettingStarted.html#getting-started-with-the-llvm-system)

## llvm docs Getting Started with the LLVM System[¶](https://llvm.org/docs/GettingStarted.html#getting-started-with-the-llvm-system)

### [Overview](https://llvm.org/docs/GettingStarted.html#id4)[¶](https://llvm.org/docs/GettingStarted.html#overview)

C-like languages use the [Clang](https://clang.llvm.org/) front end. This component compiles C, C++, Objective C, and Objective C++ code into LLVM bitcode – and from there into object files, using LLVM.

> NOTE: 
>
> 可以简单地理解: 
>
> clang 是front end
>
> LLVM 是back end

Other components include: the [libc++ C++ standard library](https://libcxx.llvm.org/), the [LLD linker](https://lld.llvm.org/), and more.

> NOTE: 
>
> 通过在 Assembling a Complete Toolchain[¶](https://clang.llvm.org/docs/Toolchain.html#assembling-a-complete-toolchain) 中描述的内容可知，可以将它们assemble成一个类似于gcc 的compile

## clang [Getting Started: Building and Running Clang](https://clang.llvm.org/get_started.html)

### Building Clang and Working with the Code

> NOTE: 
>
> 一、这一节讲述的完整的编译LLVM
>
> 二、其中的 "Building Clang and Working with the Code" 段中就提及了 "Get the required tools. See [Getting Started with the LLVM System - Requirements](https://llvm.org/docs/GettingStarted.html#requirements)."
>



## fuchsia

fuchsia [Build Clang toolchain](https://fuchsia.dev/fuchsia-src/development/build/toolchain)

## Android 

android [Android Clang/LLVM Toolchain](https://android.googlesource.com/toolchain/llvm_android/+/master/README.md)

## chromium

参见工程programming- language的`Google/Chromium/doc/Developers/LLVM/Chromium-clang/Build&test` 章节。


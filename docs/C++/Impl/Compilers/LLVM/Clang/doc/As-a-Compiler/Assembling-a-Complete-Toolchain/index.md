# Assembling a Complete Toolchain

clang仅仅是一个front end，要想完全替代gcc还需要结合本文的内容。

## doc [Assembling a Complete Toolchain](https://clang.llvm.org/docs/Toolchain.html)

### [Introduction](https://clang.llvm.org/docs/Toolchain.html#id8)[¶](https://clang.llvm.org/docs/Toolchain.html#introduction)

#### [Tools](https://clang.llvm.org/docs/Toolchain.html#id9)[¶](https://clang.llvm.org/docs/Toolchain.html#tools)

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

### [Assembler](https://clang.llvm.org/docs/Toolchain.html#id12)

### [Linker](https://clang.llvm.org/docs/Toolchain.html#id13)

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



## Linux 安装 llvm

Google: llvm install，检索到了 Getting Started with the LLVM System[¶](https://llvm.org/docs/GettingStarted.html#getting-started-with-the-llvm-system)



### llvm docs Getting Started with the LLVM System[¶](https://llvm.org/docs/GettingStarted.html#getting-started-with-the-llvm-system)

#### [Overview](https://llvm.org/docs/GettingStarted.html#id4)[¶](https://llvm.org/docs/GettingStarted.html#overview)

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

### clang [Getting Started: Building and Running Clang](https://clang.llvm.org/get_started.html)

#### Building Clang and Working with the Code

> NOTE: 
>
> 一、这一节讲述的完整的编译LLVM
>
> 二、其中的 "Building Clang and Working with the Code" 段中就提及了 "Get the required tools. See [Getting Started with the LLVM System - Requirements](https://llvm.org/docs/GettingStarted.html#requirements)."
>
> 

### addictivetips [How to install Clang on Ubuntu](https://www.addictivetips.com/ubuntu-linux-tips/clang-on-ubuntu/)

```C++
sudo apt install clang-12 --install-suggests
```

> NOTE: 
>
> 我第一次按照的时候，使用的国内的阿里云的源，结果报了如下错误:
>
> ```shell
> The following packages have unmet dependencies:
>  libclang-6.0-dev : Depends: libclang1-6.0 (= 1:6.0-1ubuntu2) but 1:6.0.1~svn334776-1~exp1~20190309042703.125 is to be installed
>                     Depends: libclang-common-6.0-dev (= 1:6.0-1ubuntu2) but 1:6.0.1~svn334776-1~exp1~20190309042703.125 is to be installed
> E: Unable to correct problems, you have held broken packages.
> ##[error]Process completed with exit code 100.
> ```
>
> 后来Google "ubuntu install clang failed"，发现了GitHub [actions](https://github.com/actions)/**[virtual-environments](https://github.com/actions/virtual-environments)** 的 [Installing clang from ubuntu package repository fails #46](https://github.com/actions/virtual-environments/issues/46) 中正好遇到了相同的问题:
>
> A: 
>
> > Following https://apt.llvm.org/ doesn't seem to work either
>
> A:
>
> > [#104 (comment)](https://github.com/actions/virtual-environments/issues/104#issuecomment-555806825)
> >
> > works
>
> 

### [actions](https://github.com/actions)/**[virtual-environments](https://github.com/actions/virtual-environments)** # [Add C++ support (GCC and Clang) on all platforms #104](https://github.com/actions/virtual-environments/issues/104)

On Ubuntu I use this to install it as `clang-9` (takes about a minute):

```sh
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key 2>/dev/null | sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main' -y
sudo apt-get update -q
sudo apt-get install -y clang-9 lld-9 libc++-9-dev libc++abi-9-dev clang-tools-9
```

> NOTE: 
>
> 初次看上述code，我发现它并没有install clang++，然后我看了 [**LLVM Debian/Ubuntu nightly packages**](https://apt.llvm.org/)，发现其中的install中，有没有clang++。



### [**LLVM Debian/Ubuntu nightly packages**](https://apt.llvm.org/)


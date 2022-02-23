# chromium clang

一、更加准确来说chromium使用的是一个定制化的clang，它实现了一些custom plugin。

二、从官方文档 [Toolchain support](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/toolchain_support.md) 中的内容可知，chromium团队中有专门的 "toolchain team"，他们专门负责clang、lld的维护工作。

三、chromium团队是如何编译LLVM的呢？如何获得chromium团队编译的"prebuilt [clang](http://clang.llvm.org/) binary"呢？

1、这在`Build&test` 中进行了介绍。

2、链接

http://commondatastorage.googleapis.com/chromium-browser-clang/index.html

## chromium use clang for all OS

一、llvm blog [Clang is now used to build Chrome for Windows](http://blog.llvm.org/2018/03/clang-is-now-used-to-build-chrome-for.html)

这篇文章详细地论述了chromium使用clang的原因。

二、arstechnica [Chrome on Windows ditches Microsoft’s compiler, now uses Clang](https://arstechnica.com/gadgets/2018/03/chrome-on-windows-ditches-microsofts-compiler-now-uses-clang/)

这篇文章其实是对llvm blog [Clang is now used to build Chrome for Windows](http://blog.llvm.org/2018/03/clang-is-now-used-to-build-chrome-for.html) 的解释。

三、chromium blog [Faster Chrome - Let The Compiler do the work](https://blog.chromium.org/2021/12/faster-chrome-let-the-compiler-do-the-work.html)

四、官方文档 [Toolchain support](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/toolchain_support.md) 

> This toolchain is used to build Chromium for 
>
> **7+ platforms** (Android, Chromecast, Chrome OS, Fuchsia, iOS, Linux, macOS, Windows) targeting 
>
> **4+ CPUs** (arm, arm64, x86, x64) in 
>
> **6+ build modes** (debug (component non-optimized), release (static build optimized), official build (very optimized and on some platforms LTO+CFI), asan+lsan, msan, tsan), 
>
> resulting in **130+** different test binaries.

## 官方文档

[Clang Compiler](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) 

[clang_sheriffing.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_sheriffing.md) 

[Updating Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) 

[Toolchain support](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/toolchain_support.md) 

## chromium doc [Clang Compiler](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md)

> NOTE: 
>
> 一、通过这篇文章可以看到，chromium是clang的重度用户:1
>
> 1、chromium use clang for all OS
>
> 2、clang plugin
>
> 3、clang tool

Chromium ships a prebuilt [clang](http://clang.llvm.org/) binary. It's just upstream clang built at a known-good revision that we bump every two weeks or so.

> NOTE: 通过官方文档 [Updating clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) 中的内容可知:
>
> > We distribute prebuilt packages of LLVM binaries, including clang and lld, that all developers and bots pull(拉取) at `gclient runhooks` time. 
>
> 在chromium中，并没有随包带上clang binary，而是由`gclient runhooks` 自动去拉取的
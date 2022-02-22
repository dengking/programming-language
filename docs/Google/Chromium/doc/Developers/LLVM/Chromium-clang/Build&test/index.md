# Build & test

一、chromium如何来build clang、如何来进行test。

二、发布物地址:

http://commondatastorage.googleapis.com/chromium-browser-clang/index.html

## chromium doc [Configurations supported by the toolchain team](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/toolchain_support.md)

> NOTE: 
>
> 从官方文档 [Toolchain support](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/toolchain_support.md) 中的内容可知，chromium团队中有专门的 "toolchain team"，他们专门负责clang、lld的维护工作。

## chromium doc [Updating Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) 

We distribute prebuilt packages of LLVM binaries, including clang and lld, that all developers and bots pull at `gclient runhooks` time. 

> NOTE: 
>
> 通过上面这段话可知: 在chromium中，并没有随包带上clang binary，而是由`gclient runhooks` 自动去拉取的



## chromium doc [Clang Sheriffing](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_sheriffing.md)

> NOTE: 
>
> 一、这篇文章主要讨论chromium如何来测试它编译出的clang的
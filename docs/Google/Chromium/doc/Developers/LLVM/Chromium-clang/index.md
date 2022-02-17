# chromium clang

更加准确来说chromium使用的是一个定制化的clang，它实现了一些custom plugin。

## chromium use clang for all OS

llvm blog [Clang is now used to build Chrome for Windows](http://blog.llvm.org/2018/03/clang-is-now-used-to-build-chrome-for.html)

这篇文章详细地论述了chromium使用clang的原因。

arstechnica [Chrome on Windows ditches Microsoft’s compiler, now uses Clang](https://arstechnica.com/gadgets/2018/03/chrome-on-windows-ditches-microsofts-compiler-now-uses-clang/)

这篇文章其实是对llvm blog [Clang is now used to build Chrome for Windows](http://blog.llvm.org/2018/03/clang-is-now-used-to-build-chrome-for.html) 的解释。

chromium blog [Faster Chrome - Let The Compiler do the work](https://blog.chromium.org/2021/12/faster-chrome-let-the-compiler-do-the-work.html)



## 官方文档

[Clang Compiler](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) 

[clang_sheriffing.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_sheriffing.md) 

[Updating Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) 



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
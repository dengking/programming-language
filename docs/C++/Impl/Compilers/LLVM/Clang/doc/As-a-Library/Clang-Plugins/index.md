# Clang Plugin



## docs [Clang Plugins](https://clang.llvm.org/docs/ClangPlugins.html#clang-plugins)

Clang Plugins make it possible to run extra user defined actions during a compilation. This document will provide a basic walkthrough of how to write and run a Clang Plugin.

> NOTE: 
>
> 显然，clang plugin不能想clang tool那样独立运行的，从它的名字可知: 它是clang的plugin；相反: clang tools是可以独立运行的

Clang Plugins run `FrontendActions` over code. See the [FrontendAction tutorial](https://clang.llvm.org/docs/RAVFrontendAction.html) on how to write a `FrontendAction` using the `RecursiveASTVisitor`. In this tutorial, we’ll demonstrate how to write a simple clang plugin.

> NOTE: 
>
> 关于`FrontendAction`，参加: llvm [clang::FrontendAction (**abstract**)](https://clang.llvm.org/doxygen/classclang_1_1FrontendAction.html)
>
> 







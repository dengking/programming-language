# LibTooling

## docs [Tutorial for building tools using LibTooling and LibASTMatchers](https://clang.llvm.org/docs/LibASTMatchersTutorial.html) 

> NOTE: 
>
> 这篇文章非常好，它基本上展示了使用 LibTooling 和 LibASTMatchers 来实现一个"source-to-source translation tool" 的过程

This document is intended to show how to build a useful source-to-source translation tool based on Clang’s [LibTooling](https://clang.llvm.org/docs/LibTooling.html). It is explicitly aimed at people who are new to Clang, so all you should need is a working knowledge of C++ and the command line.

> NOTE: 
>
> 一、"source-to-source translation tool" 正是我所需要的

## docs [LibTooling](https://clang.llvm.org/docs/LibTooling.html#libtooling)

> NOTE: 
>
> 一、这篇文章的内容非常一般
>
> 二、
>
> llvm [clang::FrontendAction (**abstract**)](https://clang.llvm.org/doxygen/classclang_1_1FrontendAction.html)
>
> 它的sublass中包含有 [**clang::CodeGenAction**](https://clang.llvm.org/doxygen/classclang_1_1CodeGenAction.html)
>
> [**clang::SyntaxOnlyAction**](https://clang.llvm.org/doxygen/classclang_1_1SyntaxOnlyAction.html) 
>
> 


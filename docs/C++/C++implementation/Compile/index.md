# 关于本章

1、在前面章节已经描述了compile model，总结了compiler是如何编译生成最终的object file的

2、本章继续深入，探讨C++ compiler编译一个source file的细节，显然这会涉及非常多的内容。

3、对C++而言，它的implementation主要是由compiler来完成。



## Compiler的局限

> NOTE: 首先介绍compiler的局限性



> TODO:
>
> 1、需要增加preshing的文章的链接，其中有很多关于这个topic的描述



1、compiler是无法知道function是否会被multithread执行的

2、compiler是无法知道一个variable是否是shared variable，即会被multithread来执行read、write

3、compiler能够在多大程度上了解我们的program: 它不知道multithread。

C++11引入了memory model，给予了programmer解决上述问题的手段。



## 编译流程

参见 `Compile-flow` 章节。



## Compiler optimization 

参见 `Optimization` 章节。



## Implementation of polymorphism

参见 `C++\Guide\Polymorphism` 章节。



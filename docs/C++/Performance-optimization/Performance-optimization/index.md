# 关于本章

本章讨论performance optimization这个主题的内容。

High performance涉及到发出多的内容，可以说是贯彻了整个computer science。

将从如下几个方面来进行讨论:

1) High performance server

2) Performance optimization in programming language

## wikipedia [Program optimization](https://en.wikipedia.org/wiki/Program_optimization)

> NOTE: 
>
> 比较权威的介绍

## Optimization principle

参见文章 《Optimization-principle》。

Optimization principle贯穿了computer science。

## Optimization的方向/how to optimize

下面给出一些optimize的方向，以及如何来实现。

### 避免频繁的system call

tag-avoid-system-call

####  Why system call slow?

参见`Linux-OS` 的 `System-call`。

#### How to do?

1、使用pool design pattern

避免频繁system call，参见 `Software-design\Design-pattern\Pool`。

2、使用lightweight、userspace替代方案来替代system call

> tag-avoid-system-call-lightweight-userspace

light weight、user space的来实现功能，典型例子：使用`std::atomic_flag`来实现spinlock，使用spinning lock，避免kernel将thread suspend，然后thread进入wait/sleeping。

### Concurrency的优化

parallel computing的崛起，促进了concurrent programming，因此concurrency的优化非常重要，在工程parallel-computing的`Concurrent-computing\Concurrency-control\How-to-optimize`章节中，对这个问题进行了深入讨论。



### Memory的优化

非常重要的一个方面，其实和前面的内容是有重叠的。并且涉及到的内容是非常多的。

#### How to do?

1、避免copy

tag-avoid-copy

在下面章节中也探讨了这个topic:

a、工程programming language的相关章节

2、避免system call

tag-avoid-system-call-new-malloc

a、SSO

3、string的优化



### Hardware层面的优化

#### How to do?

1、NUMA

2、千兆网卡

3、并发编程



### 将run time computation转移到compile-time

cpp core guideline中有相关的内容



### Algorithm and DS

这在工程discrete中进行讨论；



## Application

1、参见工程programming-language的`C-family\C++\Guide\Performance-optimization`章节。
# Performance  optimization

本章讨论performance optimization这个主题的内容。Performance optimization涉及到非常多的内容，可以说是贯彻了整个computer science。

一、无论是compiler还是programmer，都需要遵循optimization principle。因此，我们后续谈论optimization的时候，会从两个方面来进行描述:

1、compiler(实现层)执行的optimization

2、programmer执行的optimization

二、C++的一个发展方向: performance optimization。

本文对C++ Performance  optimization进行总结。

三、C++ Zero-overhead-principle



## wikipedia [Program optimization](https://en.wikipedia.org/wiki/Program_optimization)

> NOTE: 
>
> 比较权威的介绍

## Optimization principle

参见文章 《Optimization-principle》。

Optimization principle贯穿了computer science。

## Optimization的方向/how to optimize

C++中的很多Optimization technique和`Software-analysis\Performance-optimization`总结的各种Optimization technique是一脉相承的。



### Memory的优化

非常重要的一个方面，其实和前面的内容是有重叠的。并且涉及到的内容是非常多的。



#### 1、避免copy

> tag-avoid-copy

参见下面的"Avoid copy optimization in C++"。

#### 2、避免system call

> tag-avoid-system-call-new-malloc

#### 3、SSO

参见 `C++\Performance-optimization\Memory\Small-Buffer-Optimization` 章节；



### Cache optimization

后面提及的"绑核"能够一定程度上提高cache hit。

### 避免频繁的system call

####  Why system call slow?

参见`Linux-OS` 的 `System-call`。

#### How to do?

1、使用pool design pattern

避免频繁system call，参见 `Software-design\Design-pattern\Pool`。

2、使用lightweight、userspace替代方案来替代system call

> tag-avoid-system-call-lightweight-userspace

light weight、user space的来实现功能，典型例子：使用`std::atomic_flag`来实现spinlock，使用spinning lock，避免kernel将thread suspend，然后thread进入wait/sleeping。



## Avoid copy optimization in C++

Performance optimization中，"avoid copy"是非常重要的一种优化思路，C++无论是在实现(compiler)还是在language层、还是在STL中，都贯穿了这个思路，本文将对C++ avoid copy进行总结。

### Placement new

参见 `Resource-management\Memory-management\Allocation-and-deallocation\C++\new-operator\Placement-new` 章节。

### Emplace/construct in place

C++ emplace可以作为一个例子，另外它的实现还依赖于placement new。参见`C++\Library\Standard-library\STL\Containers-library\Common-issue\Emplace`章节。



### View/non-owning type

1、`string_view`

参见 `String\C++string\basic_string_view` 章节

2、`span`

参见 `C++\Library\Standard-library\STL\Containers-library\View\span` 章节

#### `std::string` VS  `const char *` VS `std::string_view`

`std::string` 是典型的deep copy。

`const char *` 和 `std::string_view` 是典型的non-copy



### Move semantic

参见 `C++\Language-reference\Reference\Move-semantic` 章节。



### Avoid copy optimization in compiler 

本节总结compiler对Avoid copy optimization 技术的运用。

#### Copy elision

参见 `C-family\Common\From-source-code-to-exec\Compile\Optimization\Copy-elision` 章节。

#### RVO

参见 "RVO" 章节。



### Compiler type deduction

#### reference

C++compiler的template argument，deduction使用reference是符合optimization原则的



## Concurrent/parallel computing/Concurrency的优化

distributed computing、concurrent programming，因此concurrency的优化非常重要，参见: 

一、工程`parallel-computing`的如下章节中，对这个问题进行了探讨:

1、`Concurrent-computing\Concurrency-control\How-to-optimize`章节



二、`C++\Library\Standard-library\Thread`

三、C++17 execution policy



## Programmers give hint to compiler

本节标题的含义是: 给编译器提示；虽然当今compiler比较强大，但是毕竟它不是万能的，作为programmer，我们可以给予它一些hint，让能够满足我们的目的: 

1) 让compiler能够更好地优化code

关于此的例子有:

`inline`

`likely` 和 `unlikely`

2) 让compiler实现static polymorphism，选择(statically)最优的实现



## Compile-time computation

将run time computation转移到compile-time，CppCoreGuideline中有相关的内容；

参见`Compile-time-computation`章节。

### draft

含义是: 让compiler选择(statically)正确的实现

例子:

1 multiple dispatch，参见

- `Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Behavioral-pattern\Visitor-pattern`
- `C++\Pattern\Visitor-pattern`

2 primary template 和 specialization

当两者处于不同header file的时候，在使用的时候，需要将它们都包含到同一个translation unit中才能够生效。

3 CRTR都是give hint to compiler

它都是generic programming中的技巧，参见 `Generic-programming\Curiously-recurring-template-pattern` 章节。

4 tag dispatch



## 绑核提高cache hit

在如下software中，都使用了这种优化方法:

1、Redis

https://github.com/redis/redis/blob/unstable/src/server.c#L6395

```C++
void redisSetCpuAffinity(const char *cpulist) {
#ifdef USE_SETCPUAFFINITY
    setcpuaffinity(cpulist);
#else
    UNUSED(cpulist);
#endif
}
```



2、Nginx

参见 zhihu [nginx master-worker进程工作原理](https://zhuanlan.zhihu.com/p/96757160)。

## Hardware层面的优化

1、NUMA

2、千兆网卡、高性能网卡

3、并发编程、多核





## Algorithm and DS

这在工程discrete中进行讨论；


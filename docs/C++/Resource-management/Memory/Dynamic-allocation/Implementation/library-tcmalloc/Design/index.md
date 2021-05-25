# [TCMalloc : Thread-Caching Malloc](https://google.github.io/tcmalloc/design.html)

## Motivation

> NOTE: 
>
> 1、"意图决定最终的结果"

TCMalloc is a memory allocator designed as an alternative to the system default allocator that has the following characteristics:

1、Fast, uncontended allocation and deallocation for most objects. Objects are cached, depending on mode, either per-thread, or per-logical-CPU. Most allocations do not need to take locks, so there is low contention and good scaling for multi-threaded applications.

> NOTE:
>
> 1、"uncontended"是源自于per-thread cache，它的这种lock-free的实现思路是相对常见的

2、Flexible use of memory, so freed memory can be reused for different object sizes, or returned to the OS.

3、Low per object memory overhead by allocating “pages” of objects of the same size. Leading to space-efficient representation of small objects.

4、Low overhead sampling, enabling detailed insight into applications memory usage.

## Overview

The following block diagram shows the rough internal structure of TCMalloc:

![Diagram of TCMalloc internal structure](https://google.github.io/tcmalloc/images/tcmalloc_internals.png)

We can break TCMalloc into three components. The front-end, middle-end, and back-end. We will discuss these in more details in the following sections. A rough breakdown of responsibilities is:

1、The front-end is a cache that provides fast allocation and deallocation of memory to the application.

2、The middle-end is responsible for refilling the front-end cache.

3、The back-end handles fetching memory from the OS.

Note that the front-end can be run in either per-CPU or legacy per-thread mode, and the back-end can support either the hugepage aware pageheap or the legacy pageheap.
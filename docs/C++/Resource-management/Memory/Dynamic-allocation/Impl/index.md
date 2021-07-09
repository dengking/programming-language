# Implementation of dynamic memory allocator

## Chunky memory management

很多memory allocator使用的是chunky memory management策略。



## 选型 and tradeoff

考虑的因素包括:

1、multiple thread

2、fragment

### 素材

1、stackoverflow [Multithreaded Memory Allocators for C/C++](https://stackoverflow.com/questions/147298/multithreaded-memory-allocators-for-c-c)

2、Redis选择jmalloc



## 对比

### jemalloc arena VS tcmalloc thread cache

1、都是为了提高 scalability、降低contention
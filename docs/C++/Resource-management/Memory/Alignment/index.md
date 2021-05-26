# 关于本章

本章主要讨论C和C++中涉及的和alignment相关的内容，关于hardware层级的alignment相关的内容，参见工程[Hardware](https://dengking.github.io/Hardware/)的[Memory-alignment](https://dengking.github.io/Hardware/CPU/Memory-access/Memory-alignment/)章节。

## 相关内容

alignment涉及到的内容非常广泛。

### Dynamic allocation and alignment

参见 `Dynamic-allocation` 章节，其中对此进行了说明。

### struct pack

参见 `struct-pack` 章节。



## Stricter alignment  

stricter alignment  意味着 alignment  更大，比如`char`的alignment是1，`int`的alignment是4，显然更大；



## C++对alignment的支持、演进

### Object alignment requirement

参见 `Object-alignment-requirement` 章节。

### 概述

参见: 

1、wikipedia [C++11#Control and query object alignment](https://en.wikipedia.org/wiki/C++11#Control_and_query_object_alignment)

2、`Object-alignment-requirement` 章节



## TODO

### Linux unaligned memory access

https://www.kernel.org/doc/Documentation/unaligned-memory-access.txt



### alexonlinux [Aligned vs. unaligned memory access](http://www.alexonlinux.com/aligned-vs-unaligned-memory-access)



### unaligned pointer to large area

https://groups.google.com/forum/#!topic/comp.lang.c/YEfswjndH1s


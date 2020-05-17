# The Boost Graph Library

在工程data-structure中，我们介绍了graph的概念，本章介绍c++中的实现：boost的graph library。

## [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/index.html)

> 通过原文的第一段我们可以看出，BGL所采用的设计思想和STL的设计思想是相同的，核心思想是generic-programming，在原文对两者进行了对比。

### Genericity in the Boost Graph Library

Like the STL, there are three ways in which the BGL is generic.

#### Algorithm/Data-Structure Interoperability

> NOTE: 简而言之就是使用iterator、adapter pattern

#### Extension through Visitors

> NOTE: 简而言之就是visitor pattern

#### Vertex and Edge Property Multi-Parameterization

> NOTE: graph结构是比list等结构要复杂的，它有node、edge，node和edge有各自的一些property，在BGL中，这些都parameterization了，用户可以通过template parameter对它们进行调整。
>
> 更多的parameter，能够带来BGL的genericity，同时它也带来了实现的复杂、使用的复杂。

### Algorithms

> NOTE: core algorithm pattern是基础的algorithm，它是其他几种algorithm的building block。



### Data Structures

> NOTE: 本段所描述的是如何来存储图

# [A Quick Tour of the Boost Graph Library](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/quick_tour.html)

![](./analogy.gif)
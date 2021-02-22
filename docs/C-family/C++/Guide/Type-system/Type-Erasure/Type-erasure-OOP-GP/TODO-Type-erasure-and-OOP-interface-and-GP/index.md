# Type erasure、OOP interface、generic programming

本文讨论如下三种technique:

1、type erasure: 去除type，提供统一的type; 不依赖inheritance、implementation; dynamic polymorphism;

2、OOP interface: 统一type; 依赖于inheritance、implementation; dynamic  polymorphism;

Java主要是利用它 来实现generic  programming

3、generic programming: 抽象出type，其实是不同的type; 不依赖inheritance、implementation; static polymorphism;

> NOTE: 上述三种technique对type的处理是不同的

上述三种technique都能够实现:

1、consistent interface

2、make code generic、促进code reuse

3、program to abstraction and polymorphism



## 如何进行tradeoff?

其实前面已经对它们的差异进行了梳理。
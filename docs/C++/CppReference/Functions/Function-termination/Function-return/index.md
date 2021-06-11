# Function return

## Function return type

对于function return type:

一、当前就已经指定

二、late-specified return type(主要在generic forwarding function template中)，这是一种static polymorphism。

1、C++ 11 `decltype` 引入

2、依赖于compiler type inference

> NOTE: 
>
> 关于此，参见 microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)

## 发展概述

在 stackoverflow [c++11: Templated wrapper function](https://stackoverflow.com/questions/25495448/c11-templated-wrapper-function) 中，非常好的总结了C++ function return的发展历程，其中给出了非常好的example，值得阅读。



### Late-specified return type 

> NOTE:
>
> 1、"Late-specified return type"是取自 microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)。

#### C++11

1、auto + `decltype` + trailing return type

2、`std::result_of`



#### C++14

1、Return-type-deduction

对C++11的syntax进行了简化。

microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019) # [Decltype and Auto](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-auto)

> In C++14, you can use `decltype(auto)` with no **trailing return type** to declare a **template function** whose **return type** depends on the types of its **template arguments**.



#### draft

C++在function return type inference上的优化:

1) C++ 11 `auto` + `decltype` + trailing return type

2) C++14 `decltype(auto)`
# Variadic multiple inheritance

需要使用template来抽象化地描述C++ class hierarchy，其实这是有一点难度的，但是C++11的很多新特性使得这成为了可能。那如何实现呢？参见下面文章:

1、fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

## Grammar

在 "cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack) # Base specifiers and member initializer lists" 中，给出了它的grammar。

## Application

1、Variadic CRTP

2、Variadic mixin 


# Mixin multiple

一、它的实现依赖的技巧:

1、Variadic multiple inheritance

2、Variadic CRTP(可能会使用)

二、它应该只能够是mixin from above



三、example

1、"cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack) # Base specifiers and member initializer lists "

其中给出的例子就是典型的mixin multiple

2、fluentcpp [Variadic CRTP: An Opt-in for Class Features, at Compile Time](https://www.fluentcpp.com/2018/06/22/variadic-crtp-opt-in-for-class-features-at-compile-time/) 中提出的: 

[`NamedType`](http://github.com/joboccara/NamedType) library
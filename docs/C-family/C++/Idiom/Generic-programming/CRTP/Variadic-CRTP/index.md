# Variadic CRTP

1、这是在文章 fluentcpp [Variadic CRTP: An Opt-in for Class Features, at Compile Time](https://www.fluentcpp.com/2018/06/22/variadic-crtp-opt-in-for-class-features-at-compile-time/) 中提出的，它收录于 `fluentcpp-Variadic-CRTP` 章节。

2、在主要使用的是 [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack) 中的 "Base specifiers and member initializer lists" 语法点，即Variadic multiple inheritance

3、它主要用于实现mixin multiple
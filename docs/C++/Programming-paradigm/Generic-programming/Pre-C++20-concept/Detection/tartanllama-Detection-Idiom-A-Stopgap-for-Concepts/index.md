# tartanllama [Detection Idiom - A Stopgap for Concepts](https://blog.tartanllama.xyz/detection-idiom/)

> NOTE: 
>
> 1、"Stopgap"的含义是"临时替代品"

[Concepts](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4641.pdf) is a proposed C++ feature which allows succinct, expressive, and powerful constraining of templates. They have been threatening to get in to C++ for a long time now, with the first proposal being rejected for C++11. They were finally [merged in to C++20](https://herbsutter.com/2017/07/15/trip-report-summer-iso-c-standards-meeting-toronto/) a few months ago, which means we need to hold on for another few years before they’re in the official standard rather than a [Technical Specification](https://stackoverflow.com/questions/21342077/what-is-c-technical-specification). In the mean time, there have been various attempts to implement parts of concepts as a library so that we can have access to some of the power of Concepts without waiting for the new standard. The [detection idiom](http://en.cppreference.com/w/cpp/experimental/is_detected) – designed by [Walter Brown](http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4502.pdf) and part of the [Library Fundamentals 2 Technical Specification](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4617.pdf) – is one such solution. This post will outline the utility of it, and show the techniques which underlie its implementation.

## A problem



## Solutions



## Metaprogramming demystified

### Type traits and `_v` and `_t` suffixes



### `decltype` specifiers



### `std::declval` trickery



### SFINAE and `std::enable_if`



### Expression SFINAE



### `void_t` magic
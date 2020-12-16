# 关于本章

本章讨论c++语言的发展，演进，重点关注各个版本的c++语言的new feature。

## C++ 版本计划

在[cppreference C++11](https://en.cppreference.com/w/cpp/11)中介绍了：

> C++ updates every 3 years regularly



## [C++ Standards Committee Papers](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/)

> NOTE: 相当于python [PEP](https://www.python.org/dev/peps/)。



[Final Committee Draft](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2011/n3249.html)

最终标准化草案



## What is new in C++

python的官方文档的“What’s New in Python[¶](https://docs.python.org/3/whatsnew/index.html#what-s-new-in-python)”会详细的罗列python的各个版本的what is new，[cppreference](https://en.cppreference.com/w/cpp)中也有类似的、专门就某一版本的改进的、引入的新特性进行说明的章节，同时它也把这些杂糅进了各个具体章节中，阅读这些有利于开发者掌握这门语言的演进轨迹，下面收集了关于这方面的一些素材：

| 素材                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| cppreference [version](https://en.cppreference.com/w/cpp/11) | 在cppreference [version](https://en.cppreference.com/w/cpp/11)中对各个版本c++的改进进行了详细说明，是官方版的what is new in c++。 |
| cppreference [C++ compiler support](https://en.cppreference.com/w/cpp/compiler_support) | C++ compiler support，仔细阅读原文就会发现它所讨论的是各种c++ compiler对各个版本c++所引入的feature的支持，并且，它的罗列是非常详细、以时间为顺序的，所以可以作为what is new in c++。 |
| 维基百科[C++11](https://en.wikipedia.org/wiki/C%2B%2B11)     | 维基百科对各个版本的C++进行了总结，可以作为What is new in C++。 |



## C++发展方向

C++不断引入新的特性，我们需要从更高地层次来思考这些特性对C++的具体改进，这样我们能够梳理清楚C++的发展脉络，下面是我总结的一些方面:

### Compile-time computation

参见`C-and-C++\Compile-time-and-run-time`章节。

### Template

C++的一个发展发现是对template-meta programming的强化。

| feature                                                      | 引入版本 | 章节                                               |
| ------------------------------------------------------------ | -------- | -------------------------------------------------- |
| [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) | C++20    |                                                    |
| [Abbreviated function templates](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template) | C++20    |                                                    |
| [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) | C++17    |                                                    |
| Type transformation `_t` aliases [¶](https://isocpp.org/wiki/faq/cpp14-library#type-transformation-aliases) [Δ](https://isocpp.org/wiki/faq/cpp14-library#) | C++14    | `C++\What-is-new-in-C++\C++14\TransformationTrait` |



### Value-semantic

充分发挥value-semantic。

| feature                   | 引入版本 | 章节                                                 | 说明 |
| ------------------------- | -------- | ---------------------------------------------------- | ---- |
| move semantic             | C++11    | 参见`C++\Language-reference\Reference\Move-semantic` |      |
| Temporary materialization | C++17    | 参见参见`C++\Guide\Temporary`                        |      |
|                           |          |                                                      |      |



### User-defined literal

| feature                                                      | 引入版本 | 章节 |
| ------------------------------------------------------------ | -------- | ---- |
| [User-defined literals for `std::` types](https://isocpp.org/wiki/faq/cpp14-library#std-udls) | C++14    |      |
|                                                              |          |      |
|                                                              |          |      |



### Uniform

C++的一个非常重要的方向是uniform，uniform促进了对各种差异的abstraction，促进了generic programming。主要包括如下方面:

1 对很多implementation-specific进行uniform，提供uniform/consistent syntax，这包括:

1) C++ 11 attribute: `C++\Language-reference\Attribute`

2) C++11 uniform initialization: `C++\Language-reference\Initialization\Uniform-initialization`

2 对复杂的、常用的运算进行uniform，提供uniform/consistent interface

"统一"意味着"抽象"，因为着需要polymorphism。

1) C++17 `std::invoke`

http://www.cplusplus2017.info/c17-stdinvoke-stdapply/ 

> `std::invoke` it’s universal interface for calling anything it’s callable.

2) `std::decay`

### C++ Has Become More Pythonic

参见`./preshing-C++Has-Become-More-Pythonic`。


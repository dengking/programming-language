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

python的官方文档的“What’s New in Python[¶](https://docs.python.org/3/whatsnew/index.html#what-s-new-in-python)”会详细的罗列python的各个版本的what is new，[cppreference](https://en.cppreference.com/w/cpp)中也有类似的、专门就某一版本的改进的、引入的新特性进行说明的章节，同时它也把这些杂糅进了各个具体章节中，阅读这些有利于开发者掌握这门语言的演进轨迹，下面收集了关于这方面的一些内容：

| 素材                                                         | 说明                                                         |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| cppreference [version](https://en.cppreference.com/w/cpp/11) | 在cppreference [version](https://en.cppreference.com/w/cpp/11)中对各个版本c++的改进进行了详细说明，是官方版的what is new in c++。 |      |
| cppreference [C++ compiler support](https://en.cppreference.com/w/cpp/compiler_support) | C++ compiler support，仔细阅读原文就会发现它所讨论的是各种c++ compiler对各个版本c++所引入的feature的支持，并且，它的罗列是非常详细、以时间为顺序的，所以可以作为what is new in c++。 |      |
| 维基百科[C++11](https://en.wikipedia.org/wiki/C%2B%2B11)     | 维基百科对各个版本的C++进行了总结，可以作为What is new in C++。 |      |

### C++发展方向:

### compile-time computation

C++的一个发展发现就是充分发挥compile-time computation。

| feature        | 引入版本 | 章节                                                         | 说明 |
| -------------- | -------- | ------------------------------------------------------------ | ---- |
| `constexpr`    | C++11    | 参见`C++\Language-reference\Expressions\Constant-expressions\constexpr\Constexpr-specifier.md` |      |
| `constexpr if` | C++17    | 参见`C++\Language-reference\Statements\Selection-statements\Constexpr-if.md` |      |
|                |          |                                                              |      |

### template-meta programming

C++的一个发展发现是对template-meta programming的强化。

| feature                                                      | 引入版本 | 章节 |
| ------------------------------------------------------------ | -------- | ---- |
| [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) | C++20    |      |
| [Abbreviated function templates](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template) | C++20    |      |
| [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) | C++17    |      |



### value-semantic

充分发挥value-semantic。

| feature                   | 引入版本 | 章节                                                 | 说明 |
| ------------------------- | -------- | ---------------------------------------------------- | ---- |
| move semantic             | C++11    | 参见`C++\Language-reference\Reference\Move-semantic` |      |
| Temporary materialization | C++17    | 参见参见`C++\Guide\Temporary`                        |      |
|                           |          |                                                      |      |




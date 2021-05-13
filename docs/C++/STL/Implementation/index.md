# 关于本章

本章讨论C++标准库的实现，在`Theory\Programming-language\Design-of-programming-language`中，我们已经知道：

一门programming language的设计，可以分为两大块：

- language 
- standard library

关于language的实现，在`C-family-language\C-and-C++\From-source-code-to-exec\Compile\Implementation`中已经描述了，本章描述的是C++标准库的实现，其实它和language的implementation是对应的，显然，它有：

| implementation                                               |      |
| ------------------------------------------------------------ | ---- |
| [llvm-project](https://github.com/llvm/llvm-project)/[libcxx](https://github.com/llvm/llvm-project/tree/main/libcxx) |      |
| [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3) |      |
| [microsoft](https://github.com/microsoft)/**[STL](https://github.com/microsoft/STL)** |      |


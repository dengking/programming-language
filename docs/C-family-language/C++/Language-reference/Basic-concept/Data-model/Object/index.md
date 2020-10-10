# 关于本章

阅读[cppreference.com](https://en.cppreference.com/)中关于C、C++的描述，可以发现object的是这两门语言中非常核心的概念，是完整地认知C、C++的基础。本文讨论C++中object的概念以及与object相关的各种内容，本章讨论的内容如下：

| 主题                                 | 概述                                                         | 链接                                                         |
| ------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| object                               | 主要讨论C++中object的概念                                    | [Object](./Object.md)                                        |
| object storage duration and lifetime | object storage描述的是C++ object位于内存在的位置：stack、heap；<br>object lifetime与object storage是两个相互依存的概念，<br>在[Object](https://en.cppreference.com/w/cpp/language/object)中这样描述两者的关系：<br>[lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary); | [Object-storage-and-lifetime](./Object-storage-and-lifetime.md) |
| object layout                        | 前面讨论了C++ object的storage，object layout则讨论的是object在内存中的布局 | [Object-layout](./Object-layout/index.md)                    |


# 关于本章

阅读[cppreference.com](https://en.cppreference.com/)中关于C、C++的描述，可以发现object的是这两门语言中非常核心的概念，是完整地认知C、C++的基础。本文讨论C++中object的概念以及与object相关的各种内容，本章讨论的内容如下：

C++ object：，

object storage：

| 主题                                       | 概述                                                         | 链接                                                         |
| ------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| object                                     | 主要讨论C++中object的概念                                    | [Object](./Object.md)                                        |
| object storage and lifetime   and  linkage | object storage描述的是C++ object位于内存在的位置：stack、heap、data segmentation；object lifetime与object storage是两个相互依存的概念，在[Object](https://en.cppreference.com/w/cpp/language/object)中这样描述两者的关系：[lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary); linkage是一个与object storage密切关联的概念，所以放到这里和它们一并进行描述。需要注意的是，linkage是一个静态概念，而object是一个动态概念。 | [Object-storage-and-lifetime](./Object-storage-and-lifetime.md) |
| object layout                              | 前面讨论了C++ object的storage，object layout则讨论的是object在内存中的布局 | [Object-layout](./Object-layout/index.md)                    |


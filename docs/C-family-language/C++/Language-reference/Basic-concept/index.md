# 关于本章

本章对应的是cppreference的[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)章节，这一章的内容非常重要，它描述了c++ programming language的基本概念，这些基本概念是后续内容的基石。

c++语言是复杂的，所以本章的内容也是庞杂的，我们需要有清晰的思路来组织这些内容，下面是我的思路：

以总分的思路，即先综述（建立高屋建瓴的视角），然后逐个描述。主要内容如下：



| 章节                                                         | 内容简介                                                     |              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------ |
| [Basic-concepts](./Basic-concepts.md)                        | 综述，以cppreference [Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)为蓝本 |              |
| [Reference-and-Value-Semantics](./Reference-and-Value-Semantics/index.md) | C++同时支持reference semantic和value semantic，充分理解两者是掌握C++语言所必须的，它是后面很多内容的基础，所以将它放到交前的位置 |              |
| [Data-model](./Data-model/index.md)                          | 描述data model，核心概念是object，以及由它而发散出的一些列内容；它对应的是value semantic | run-time     |
| [Organization](./Organization/index.md)                      | 描述C++ program的organization，其中描述了name-lookup，它能够帮助理解很多C++ compile error。 | compile-time |
| [Type-system](./Type-system/index.md)                        | 描述C++的type system                                         | compile-time |
| Declarations                                                 |                                                              | compile-time |
| Execution-model                                              |                                                              | run-time     |



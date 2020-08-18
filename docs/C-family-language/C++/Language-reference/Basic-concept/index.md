# 关于本章

本章对应的是cppreference的[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)章节，这一章的内容非常重要，它描述了c++ programming language的基本概念，这些基本概念是后续内容的基石。

c++语言是复杂的，所以本章的内容也是庞杂的，我们需要有清晰的思路来组织这些内容，下面是我的思路：

以总分的思路，即先综述（建立高屋建瓴的视角），然后逐个描述。主要内容如下：



| 章节                                                         | 内容简介                                                     |              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------ |
| [Basic-concepts](./Basic-concepts.md)                        | 综述，以cppreference [Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)为蓝本 |              |
| [Reference-and-Value-Semantics](./Reference-and-Value-Semantics/index.md) | C++同时支持reference semantic和value semantic，充分理解两者是掌握C++语言所必须的，它是后面很多内容的基础，所以将它放到较前的位置 |              |
| [Data-model](./Data-model/index.md)                          | 描述data model，核心概念是object，以及由它而发散出的一些列内容；它对应的是value semantic | run-time     |
| [Organization](./Organization/index.md)                      | 描述C++ program的organization，其中描述了name-lookup，它能够帮助理解很多C++ compile error。 | compile-time |
| [Type-system](./Type-system/index.md)                        | 描述C++的type system，其中还描述了C++ RTTI、reflection等等   | compile-time |
| Declarations                                                 | 描述C++ declarations、specifiers                             | compile-time |
| [Execution-model](./Execution-model/index.md)                | 描述C++ execution model                                      | run-time     |



## object 和 function

object 和 function构成了C-family language run-time的主要内容。

function 和 object的相同之处在于：

- 都位于memory中（按照the stored-program computer的实现，显然function是位于memory中的），所以它们存在storage
- 都是run-time概念（object在run-time被manipulation、function在run-time被执行）
- 都有type
- 都存在linkage（这在文章`C++\Language-reference\Basic-concept\Data-model\ObjectObject-storage-duration-and-lifetime.md` “linkage and storage duration of function”段中进行了探讨）

由于function 和 object 都 位于memory中，所以存在：

pointer to object、pointer to function。

reference to function、reference to function。

不同的programming language对于function的支持是不同的：

- 在C-family language 中，object is first class citizen, but function is not
- 在python中，everything is an object, so be function

在C-family language中，对function支持的操作包括：

- invokation
- refer to

在C-family language中，function不是object，所以：

- function 没有 storage duration，其实对于function而言，考虑它的storage duration并没有太大的意义，所以C-family language的designer并没有设计描述function storage duration的specifier、并没有提供供programmer对function的storage duration进行控制的操作（这在文章`C++\Language-reference\Basic-concept\Data-model\ObjectObject-storage-duration-and-lifetime.md` “linkage and storage duration of function”段中进行了探讨）
- 无法create function


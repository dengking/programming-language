# 关于本章

本章对应的是cppreference的[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)章节，这一章的内容非常重要，它描述了c++ programming language的基本概念，这些基本概念是后续内容的基石。

## 内容简介

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

## Extend to OOP

在`C-family-language\C-and-C++\VS-C-VS-C++.md`中，我们已经提及了"C++支持OOP，因此很多概念就需要向OOP扩展"，下面具体地罗列了C++向OOP中的扩展:

| 扩展点                             | 说明                         | 章节                                                         |
| ---------------------------------- | ---------------------------- | ------------------------------------------------------------ |
| function and data model向OOP的扩展 | member function、member data | 参见下一段中的“Extend to OOP”小节; <br>                      |
| pointer向OOP的扩展                 | 支持pointer to member        | `C-and-C++\Pointer-and-array\Pointer\Pointer.md`             |
| static向OOP的扩展                  | 支持static member            | `C-and-C++\Static`                                           |
| `const`向OOP的扩展                 | 支持const member             | `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-qualifier` |
| `constexpr` 向OOP的扩展            |                              |                                                              |
| enum向OOP的扩展                    | 支持enum class               | `C++\Language-reference\Enum`                                |

后续对它们的描述将统一按照pointer的中描述思路来进行: 首先描述Non-OOP，然后描述OOP。

## Function and data model

在工程hardware的`Computer-architecture\Stored-program-computer.md`中，我们介绍了Function and data model，现在使用它来对C++进行描述。C++使用object来描述data，显然object 和 function构成了C-family language run-time的主要内容。

### Extend to OOP

C++支持OOP，因此相对于C而言，它的很多概念都需要向OOP扩展，OOP只是一种programming paradigm，最终的program还是由function和data组成: 

| 组成     | 说明                                                         |
| -------- | ------------------------------------------------------------ |
| function | 包括free function、member method，关于member method是function的讨论，参见 |
| data     | 包括普通object、member object                                |

### Thoughts

下面是一些关于function、object的想法。

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



### Uniform function model

> **关于Uniform function model和Function and data model的说明:** 
>
> 其实Function and data model就是一个非常统一的模型（进行了很高的抽象、囊括了非常多的情况），本节只是结合C++对function进行详细、具体的说明。

本节标题的含义是: 统一函数模型，它的含义是: C++中，member method、operator都可以看做是一种function。下面分情况对Operator is a kind of function、member method is a kind of function进行详细说明: 

**Operator is a kind of function**

参见 `C++\Language-reference\Expressions\Operators\index.md` ，其中对此进行了详细说明。

**Member method is a kind of function**

参见`C++\Language-reference\Classes\Members\Function-member\index.md`，其中对此进行了详细说明。


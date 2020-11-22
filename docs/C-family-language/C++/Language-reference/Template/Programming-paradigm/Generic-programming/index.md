# Generic programming

在`Theory\Programming-paradigm\Generic-programming`中讨论了generic programming，其中所描述的是思想，其中已经涉及了C++中的generic programming。

在C++ Core Guidelines: [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)中，对C++中Generic programming的介绍如下:

> Generic programming is programming using types and algorithms parameterized by types, values, and algorithms. In C++, generic programming is supported by the `template` language mechanisms.

简而言之，C++中的generic programming是通过template来实现的。

在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 介绍了一些GP technique，我觉得它是关于C++ GP介绍的非常好的的文章，它向我们展示了GP的核心technique，在`C++\Library\Boost\Boost-community\Generic-programming-technique`中收录了这篇文章。



## 发展概述: 如何描述concept?

在generic programming中，非常重要的一点是:如何描述concept? 本节对此进行梳理。

### 文档中的描述: Named requirements

> NOTE: 本节标题的含义是: 在设计/技术文档中的描述方式是: named requirements，这种描述方式一般采用的是偏向于自然语言的，它是informal的

在`Theory\Programming-paradigm\Generic-programming\Type-requirement`的《Specification-of-type-requirement》章节中，已经提及了C++的named requirement，按照其中的思想来理解cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)是非常容易的。

需要注意的是，C++ named requirement是generic programming的concept，在c++20中它们都可以使用concept feature进行formalize。（基本上）named requirement是通过对应的magic function来实现的，也就是它是基于behavior，而非基于type的。

Named requirement或者说concept是基于behavior的，behavior常常是以expression的方式来进行表达，所以可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中使用expression来表达这些concept。

### 代码中的描述

> NOTE: 本节讨论如何来编码实现named requirements，它是formal的。

|           | 实现方式                                              |
| --------- | ----------------------------------------------------- |
| pre C++20 | SFINAE + `enable_if` + `type_trait`，后面简称为SFINAE |
| C++20     | concept                                               |

### SFINAE VS concept

SFINAE是一种事前检查，C++ concept也是一种事前检查，不过C++ concept的语法更加清晰易读。



#### stackoverflow [Will Concepts replace SFINAE?](https://stackoverflow.com/questions/28133118/will-concepts-replace-sfinae)

> NOTE: 问题中的SFINAE，应该指的是SFINAE + `enable_if` + `type_trait` idiom。

[A](https://stackoverflow.com/a/42072950)

With [concepts](http://en.cppreference.com/w/cpp/language/constraints) taking shape and being implemented in compilers (currently only gcc >= 6.1 with option `-fconcepts`), it appears that they can do everything that you could have done using [SFINAE](http://en.cppreference.com/w/cpp/language/sfinae) only much better (much clearer expression of intent and producing concise(简明的) error messages), and much more (such as abbreviated templates).

So the simple answer is **YES**.
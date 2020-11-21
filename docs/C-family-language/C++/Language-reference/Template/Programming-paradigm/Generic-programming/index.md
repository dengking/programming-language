# Generic programming



在`Theory\Programming-paradigm\Generic-programming`中讨论了generic programming，其中所描述的是思想，其中已经涉及了C++中的generic programming。



在C++ Core Guidelines: [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)中，对C++中Generic programming的介绍如下:

> Generic programming is programming using types and algorithms parameterized by types, values, and algorithms. In C++, generic programming is supported by the `template` language mechanisms.

简而言之，C++中的generic programming是通过template来实现的。

在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 介绍了一些GP technique。

我觉得关于C++ GP介绍的最好的文章是：boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)。



## 发展概述

### 如何描述concept?

在generic programming中，非常重要的一点是:如何描述concept? 本节对此进行梳理。

#### 文档中的描述: Named requirements

> NOTE: 本节标题的含义是: 在设计/技术文档中的描述方式是: named requirements，这种描述方式一般采用的是偏向于自然语言的

在`Theory\Programming-paradigm\Generic-programming\Type-requirement`的《Specification-of-type-requirement》章节中，已经提及了C++的named requirement，按照其中的思想来理解cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)是非常容易的。

需要注意的是，C++ named requirement是generic programming的concept，在c++20中它们都可以使用concept feature进行formalize。（基本上）named requirement是通过对应的magic function来实现的，也就是它是基于behavior，而非基于type的。

Named requirement或者说concept是基于behavior的，behavior常常是以expression的方式来进行表达，所以可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中使用expression来表达这些concept。

#### 代码中的描述

> NOTE: 本节讨论如何来编码实现named requirements。

|           | 实现方式                            |
| --------- | ----------------------------------- |
| pre C++20 | SFINAE + `enable_if` + `type_trait` |
| C++20     | concept                             |






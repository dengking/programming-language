# Generic programming

## C++ generic programming

在`Theory\Programming-paradigm\Generic-programming`中讨论了generic programming，其中所描述的是思想，其中已经涉及了C++中的generic programming。

在C++ Core Guidelines: [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)中，对C++中Generic programming的介绍如下:

> Generic programming is programming using types and algorithms parameterized by types, values, and algorithms. In C++, generic programming is supported by the `template` language mechanisms.

简而言之，C++中的generic programming是通过template来实现的。

在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 介绍了一些GP technique，我觉得它是关于C++ GP介绍的非常好的的文章，它向我们展示了GP的核心technique，在`C++\Library\Boost\Boost-community\Generic-programming-technique`中收录了这篇文章。



## 发展概述: 如何描述type requirement/concept?

> NOTE: 在`Theory\Programming-paradigm\Generic-programming`中我们知道，generic programming中将type requirement称为concept。
>
> 因此本节标题中为了避免混淆，两者都用上了。

在generic programming中，非常重要的一点是描述type requirement/concept；具体到C++，则是指在使用template的过程中，我们需要对template parameter添加type requirement/restriction/constrain，比如希望它**具备某种特性**/**满足某种条件**，各个版本的C++都提供了解决方法，本文对各种可用的方法进行总结。

> 显然这就是`C++` concept所要解决的问题，它是`C++`20的feature，那在此之前，我们要如何实现呢？

Type requirement/concept也是C++中用于控制static polymorphism的，因此它在C++中尤为重要，本节对此进行梳理。

> NOTE: 关于Type requirement/concept用于控制static polymorphism，参见如下章节:
>
> 1) `C++\Language-reference\Template\SFINAE`
>
> 2) `C++\Language-reference\Template\Programming-paradigm\Idioms\Polymorphism`

### Type requirement/concept examples

Type requirement/concept基本上涉及了所有与type相关的内容，C++对此提供了丰富的支持，下面列举一些例子: 

#### Behavior

支持哪些operation，显然这是behavior，可以通过expression来进行表达；参见:

1) `./Named-requirements`

#### Member

Type是否有指定member，比如typedef。

#### Type relation

是否满足inheritance关系。

#### ......



### C++11

`decltype`、`std::declval`: 让programmer能够方便地表达expression、member，再借助SFINE，从而便利的实现**compile-time detection**/**compile-time introspection**/**compile-time reflection**。

`<type_traits>`

`std::enable_if`

C++17

`std::void_t`





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
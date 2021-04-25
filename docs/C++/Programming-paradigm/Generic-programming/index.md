# C++ generic programming

在`Theory\Programming-paradigm\Generic-programming`中讨论了generic programming，其中所描述的是思想，其中已经涉及了C++中的generic programming。

在C++ Core Guidelines: [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)中，对C++中Generic programming的介绍如下:

> Generic programming is programming using types and algorithms parameterized by types, values, and algorithms. In C++, generic programming is supported by the `template` language mechanisms.

简而言之，C++中的generic programming是通过template来实现的。

## boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 

1、在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 介绍了一些GP technique，我觉得它是关于C++ GP介绍的非常好的的文章，我是通过这篇文章开始了C++ GP的学习的，它向我们展示了GP的核心technique，在`C++\Library\Boost\Boost-community\Generic-programming-technique`中收录了这篇文章。

2、后面的很多内容都是基于boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 中的概念的，尤其是其中的valid expression章节

## Type requirement/concept

> NOTE: 在`Theory\Programming-paradigm\Generic-programming`中我们知道，generic programming中将type requirement称为concept。
>
> 因此本节标题中为了避免混淆，两者都用上了。

1、在generic programming中，非常重要的一点是描述type requirement/concept；具体到C++，则是指在使用template的过程中，我们需要对template parameter添加type requirement/restriction/constrain，比如希望它**具备某种特性**/**满足某种条件**，各个版本的C++都提供了解决方法，本文对各种可用的方法进行总结。

2、Type requirement/concept也是C++中用于控制static polymorphism的，因此它在C++中尤为重要，本节对此进行梳理。

> NOTE: 关于Type requirement/concept用于控制static polymorphism，参见如下章节:
>
> 1) `C++\Language-reference\Template\SFINAE`
>
> 2) `C++\Language-reference\Template\Programming-paradigm\Idioms\Polymorphism`

3、Type requirement的对应的是static reflection

### Classification

Type requirement/concept基本上涉及了所有与type相关的内容，C++对此提供了丰富的支持，下面是对它的分类: 

#### Behavior-based

对于behavior-based的type requirement的，behavior常常是以**valid expression**的方式来进行表达，所以可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中使用**valid expression**来表达这些concept。参见:

1) `./Named-requirements`，在`./Named-requirements`中对此进行了详细的说明

#### Member

Type是否有指定member，比如typedef。

#### Type relation

是否满足inheritance关系。

#### ......

### 发展概述

本节描述generic programming的发展概述，下面是一些素材:

1) 

|           | 实现方式                                              |
| --------- | ----------------------------------------------------- |
| pre C++20 | SFINAE + `enable_if` + `type_trait`，后面简称为SFINAE |
| C++20     | concept                                               |

2) boost [Declval](https://www.boost.org/doc/libs/1_54_0/libs/utility/doc/html/declval.html)

这篇文章非常好，它告诉了我们在C++20之前，如何来实现C++20 concept的功能。

#### C++11

`decltype`、`std::declval`: 让programmer能够方便地表达valid expression，再借助SFINE，从而便利地:

1) 实现**compile-time detection**/**compile-time introspection**/**compile-time reflection**

2) 描述type requirement

`<type_traits>`

`std::enable_if`

#### C++17

`std::void_t`

#### C++20

C++20 引入了concept feature，参见 `./Concepts`



## SFINAE VS concept

1) SFINAE是一种事前检查，C++ concept也是一种事前检查，不过C++ concept的语法更加清晰易读。

2) 都能够用于对static polymorphism进行控制

### stackoverflow [Will Concepts replace SFINAE?](https://stackoverflow.com/questions/28133118/will-concepts-replace-sfinae)

> NOTE: 问题中的SFINAE，应该指的是SFINAE + `enable_if` + `type_trait` idiom。

[A](https://stackoverflow.com/a/42072950)

With [concepts](http://en.cppreference.com/w/cpp/language/constraints) taking shape and being implemented in compilers (currently only gcc >= 6.1 with option `-fconcepts`), it appears that they can do everything that you could have done using [SFINAE](http://en.cppreference.com/w/cpp/language/sfinae) only much better (much clearer expression of intent and producing concise(简明的) error messages), and much more (such as abbreviated templates).

So the simple answer is **YES**.
# C++ polymorphism

在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism`中讨论了各种polymorphism，本章讨论C++中如何实现polymorphism，这是一个较大的话题。C++中提供了多种polymorphism:

下面根据static/dynamic进行分类：

| static                    | dynamic                     |
| ------------------------- | --------------------------- |
| - overload <br>- template | - sub-type / virtual method |

下面是根据 wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))中的分类方法进行的分类: 

| classification                                               |                          |
| ------------------------------------------------------------ | ------------------------ |
| [**Ad hoc polymorphism**](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) | overload                 |
| [**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) | template                 |
| [**Subtyping**](https://en.wikipedia.org/wiki/Subtyping)     | subtype / virtual method |

无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑从set  of candidates中的选择哪一个来作为最终的实现。显然这有一个**比较**/**排序**的过程，通俗来讲，选择最“**合适**” 的那一个，那到底哪个最**合适**呢？不同的polymorphism有不同的标准。

> NOTE: 这段话在 `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation` 中也有使用 。

Function overload、template specialization、OOP subclass都是对某种“特殊情况的说明”，compiler在进行选择(select)/派发(dispatch)/resolve的时候，应该选择**最最特殊的实现**: 

| Type of polymorphism  | 最最特殊的                      | 样例                                                         |
| --------------------- | ------------------------------- | ------------------------------------------------------------ |
| sub-type polymorphism | most derived object             | cppreference [Object#Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects) |
| template              | most specialized specialization | cppreference [Partial template specialization#Partial ordering](https://en.cppreference.com/w/cpp/language/partial_specialization#Partial_ordering) |
| overload              | most matched                    | cppreference [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) |

C++ overload 、template specialization、virtual method和Python attribute find的实现都可以按照在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation` 的 "Dispatch的搜索和排序" 中描述的过程来进行描述的。

## Static polymorphism

### Static polymorphism and template metaprogramming

C++的static polymorphism是由compiler来实现，C++的允许programmer通过**template metaprogramming**(编写metaprogram)来对**static polymorphism**的过程进行控制，这是C++的核心特性之一，这些内容在下面章节中进行了总结:

1) `C++\Language-reference\Template\SFINAE`

SFINAE是C++实现"通过**template metaprogramming**(编写metaprogram)来对**static polymorphism**的过程进行控制"的基础。

2) `C++\Language-reference\Template\Programming-paradigm\Idioms`

其中总结了一些其他technique。

### Implementation of template

在`C++\Language-reference\Template\Implementation`章节对此进行了深入分析。



### Implementation of overload 

在`C++\Language-reference\Functions\Function-overload\Overload-resolution`章节对此进行深入分析。

### Overload + template

在如下章节中进行了分析:

1、`C++\Language-reference\Template\Function-template`

2、`Specialization-and-overload`

## Implementation of dynamic polymorphism

在`C++\Language-reference\Classes\Subtype-polymorphism\Implementation`章节对此进行深入分析。

## Mix static polymorphism and dynamic polymorphism

需要注意的是，C++并没有绝对地区分 static 和 dynamic，C++是允许 mix static polymorphism and dynamic polymorphism，这增加了C++的 power同时不可避免的增加了C++的complexity。下面是mix static polymorphism and dynamic polymorphism的方式: 

### Overload of virtual function

我们已经知道，在C++中，virtual function对应的是dynamic polymorphism，而overload对应的是static polymorphism，那 "overload of virtual function" 该如何理解呢？C++中是否支持 "overload of virtual function" 呢？在 `C++\Language-reference\Functions\Function-overload\Overload-in-OOP` 章节对此进行了描述。

### Virtual function in class template

参见 `Template-and-virtual` 章节。
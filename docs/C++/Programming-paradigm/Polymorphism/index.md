# C++ polymorphism

在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism`中讨论了各种polymorphism，本章讨论C++中如何实现polymorphism，这是一个较大的话题。C++中提供了多种polymorphism:

## Classification

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
| External polymorphism                                        |                          |

无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑从set  of candidates中的选择哪一个来作为最终的实现。显然这有一个**比较**/**排序**的过程，通俗来讲，选择最“**合适**” 的那一个，那到底哪个最**合适**呢？不同的polymorphism有不同的标准。

> NOTE: 这段话在 `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation` 中也有使用 。

Function overload、template specialization、OOP subclass都是对某种“特殊情况的说明”，compiler在进行选择(select)/派发(dispatch)/resolve的时候，应该选择**最最特殊/合适的实现**: 

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

在`C++\Language-reference\Classes\Implementation`章节对此进行深入分析。



## Mix static polymorphism and dynamic polymorphism

[CppCoreGuidelines T.5: Combine generic and OO techniques to amplify(放大) their strengths, not their costs](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t5-combine-generic-and-oo-techniques-to-amplify-their-strengths-not-their-costs)  

需要注意的是，C++并没有绝对地区分 static 和 dynamic，C++是允许 mix static polymorphism and dynamic polymorphism，这增加了C++的 power同时不可避免的增加了C++的complexity。



下面是mix static polymorphism and dynamic polymorphism的方式: 

### Overload of virtual function

我们已经知道，在C++中，virtual function对应的是dynamic polymorphism，而overload对应的是static polymorphism，那 "overload of virtual function" 该如何理解呢？C++中是否支持 "overload of virtual function" 呢？答案是: 支持的。参见:

1、在 `C++\Language-reference\Functions\Function-overload\Overload-in-OOP` 章节对此进行了描述。

2、C++ visitor pattern，参见:

a、`Theory\Programming-paradigm\OOP\Design-pattern\Behavioral-pattern\Visitor-pattern` 章节

b、`C++\Pattern\Visitor-pattern`

### Virtual function in class template

参见 `Template-and-virtual` 章节。

### OOP interface+template implementation

TODO: [CppCoreGuidelines T.5: Combine generic and OO techniques to amplify(放大) their strengths, not their costs](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t5-combine-generic-and-oo-techniques-to-amplify-their-strengths-not-their-costs) 中给出了例子

## External Polymorphism and Internal Polymorphism

Google了一下，发现下面这篇文章的介绍是比较好的。



1、agnihotris [External Polymorphism (Adapter Design pattern) vs InternalPolymorphism](https://agnihotris.wordpress.com/2013/09/22/external-polymorphism-design-pattern-vs-internalpolymorphism/)

**Internal Polymorphism :-** When you call a function based on its dynamic type of objects by using virtual table or vtable . The only methods declared as virtual are polymorphic in c++.

In internal polymorphism function bindings are part of class definition of the object .

> NOTE: internal polymorphism是指C++ programming language内置支持的

**External polymorphism :-** Here functions and their bindings are  defined independently of a class.

This allows classes which are not related by inheritance and/or have no virtual methods to be treated polymorphically. Thus unrelated classes can be treated in common manner by s/w that uses them.

> NOTE: external polymorphism其实是custom polymorphism，是由programmer自己创建的

This pattern capitalizes on both c++ features and on other basic patterns (Adapter or Decorator) to give appearance of polymorphic behavior of otherwise unrelated class.

2、External-Polymorphism

参见 `External-Polymorphism` 章节。


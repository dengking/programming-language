# 关于本章

一、Template是C++的核心特性，本章介绍C++ template。

二、在gotw [Two-Phase or Not Two-Phase: The Story of Dependent Names in Templates](http://www.gotw.ca/gotw/087.htm)中有这样的描述:

> *In the land of C++, there are two towns: The village of traditional nontemplate C++ code, and the hamlet of templates.* 

三、template and programming paradigm

1、C++使用template来实现generic programming

2、Template meta-programming paradigm

参见 `C++\Programming-paradigm\TMP` 章节，其中对C++ template meta programming有着非常好的论述，尤其其中的"meta-program"概念，是非常重要的；

显然meta-program是一种compile-time computation；

它是C++灵活性的来源；

四、Template and abstraction、polymorphism

1、参见`C++\Guide\Polymorphism`章节。

2、`C++\Programming-paradigm\TMP`

template本身是一种抽象机制；

## wikipedia [Template (C++)](https://en.wikipedia.org/wiki/Template_(C%2B%2B))

### [Technical overview](https://en.wikipedia.org/wiki/Template_(C%2B%2B)#Technical_overview)

There are three kinds of templates: *function templates*, *class templates* and, since [C++14](https://en.wikipedia.org/wiki/C%2B%2B14), *variable templates*. Since [C++11](https://en.wikipedia.org/wiki/C%2B%2B11), templates may be either [variadic](https://en.wikipedia.org/wiki/Variadic_template) or non-variadic; in earlier versions of C++ they are always non-variadic.

> NOTE: 两种分类方式
>
> 分类方式一：
>
> *function templates*
>
> *class templates* 
>
> *variable templates* （since [C++14](https://en.wikipedia.org/wiki/C%2B%2B14)）
>
> 分类方式二：
>
> [variadic](https://en.wikipedia.org/wiki/Variadic_template)
>
> non-variadic
>
> 

## cppreference [Templates](https://en.cppreference.com/w/cpp/language/templates)

Templates are parameterized by one or more [template parameters](https://en.cppreference.com/w/cpp/language/template_parameters), of three kinds: 

1) type template parameters

2) non-type template parameters

3) template template parameters.

> NOTE: 虽然上述列举了三种template parameter，但是，可以简单的认为c++模板主要允许programmer参数化：
>
> 1、type，type template parameter（duck type，参见`Theory\Programming-paradigm\Generic-programming\Template-is-behavior-based`）
>
> 2、value，non-type template parameter
>
> 至于template template parameter，它其实可以归入上述两者之一




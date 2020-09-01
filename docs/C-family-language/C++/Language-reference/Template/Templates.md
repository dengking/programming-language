# Templates

template是c++的关键特性。

## 维基百科[Template (C++)](https://en.wikipedia.org/wiki/Template_(C%2B%2B))

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

Templates are parameterized by one or more [template parameters](https://en.cppreference.com/w/cpp/language/template_parameters), of three kinds: type template parameters, non-type template parameters, and template template parameters.

> NOTE: 虽然上述列举了三种template parameter，但是，可以简单的认为c++模板主要允许programmer参数化：
>
> - type，type template parameter（duck type，参见`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`）
> - value，non-type template parameter
>
> 至于template template parameter，它其实可以归入上述两者之一




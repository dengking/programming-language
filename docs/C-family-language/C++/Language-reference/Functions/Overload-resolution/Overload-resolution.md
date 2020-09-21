# Overload resolution



## cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)

In order to compile a function call, the compiler must first perform [name lookup](https://en.cppreference.com/w/cpp/language/lookup), which, for functions, may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl), and for function templates may be followed by [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction). If these steps produce more than one *candidate function*, then *overload resolution* is performed to select the function that will actually be called.

> NOTE: overload resolution非常复杂，基本上涵盖了C++的大部分内容；原文中，对于template function的部分涉及得不够多，下面的“Overload resolution and template function“章节进行了补充。

In general, the candidate function whose parameters match the arguments most closely is the one that is called.

> NOTE: 按照`C++\Guide\Implementation-of-polymorphism\index.md`中的说法，其实就是 most matched。

If a function cannot be selected by overload resolution (e.g. it is a [templated entity](https://en.cppreference.com/w/cpp/language/templates#Templated_entity) with a failed [constraint](https://en.cppreference.com/w/cpp/language/constraints)), it cannot be named or otherwise used.

> NOTE: 这段话其实所说明的就是：
>
> - SFINA
> - constrain

### Details



## Overload resolution and template function

需要梳理清楚overload resolution、template function之间的关系；substitution和overload resolution的发生顺序。

通过name lookup（对于function，还会涉及ADL），如果发现的function是一个function template，则需要对primary function template进行specializaiton: 首先进行[template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)，然后进行substitution；在进行substitution的时候，compiler还需要实现SFINAE，如果fail，则将这个specialization 丢弃。然后进行specializaiton resolution，还是进行overload resolution？按照之前的描述来看，是完成了overload resolution后，才进行specializaiton resolution。



### draft: cppreference [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)

This rule applies during overload resolution of function templates: When substituting the explicitly specified or deduced type for the template parameter fails, the specialization is discarded from the **overload set** instead of causing a compile error.



### draft: [Notes on C++ SFINAE#Overload Resolution](https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution)

When the compiler tries to compile a function call (simplified):

Perform a name lookup
For function templates the template argument values are deduced from the types of the actual arguments passed into the function.
All occurrences of the template parameter (in the return type and parameters types) are substituted with those deduced types.
When this process leads to invalid type (like `int::internalType`) the particular function is removed from the overload resolution set. (SFINAE)
At the end, we have a list of viable functions that can be used for the specific call. If this set is empty, then the compilation fails. If more than one function is chosen, we have an ambiguity. In general, the candidate function, whose parameters match the arguments most closely is the one that is called.



### draft: [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/) 

Function template的resolution算法，在文章 [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/) 中就总结地较好：

> while looking for the best overload, all candidates have to be considered. 

正如这篇文章中所讲述的，为了寻找到best overload，compiler采用了SFINAE策略，即它会尝试所有的candidate，关于SFINAE，参见`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`章节。





## 控制overload resolution

[More C++ Idioms/Tag Dispatching](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)中的preference ordering就是一个典型的例子。



## Name mangling

Name mangling发生于哪个阶段？



### wikipedia [Name mangling](https://en.wikipedia.org/wiki/Name_mangling)





## TO DO



https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/


https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution

https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/

http://foonathan.github.io/blog/2015/11/30/overload-resolution-4.html

https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/

### draft: implementation



#### 实现方式

cppreference [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)对此进行了描述。





匹配过程在下面的文章中有提及：

文章：[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

这篇文章中给出的例子非常好，“matches perfectly”、“overload candidate selection phase”

维基百科文章：[Function overloading](https://en.wikipedia.org/wiki/Function_overloading)

这篇文章中，将匹配过程描述为resolution。

与此相关的问题有：

- [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)



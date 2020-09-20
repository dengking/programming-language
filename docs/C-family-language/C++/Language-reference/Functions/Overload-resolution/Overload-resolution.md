# Overload resolution



## cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)









## 控制overload resolution

[More C++ Idioms/Tag Dispatching](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)中的preference ordering就是一个典型的例子。

## TO DO

### draft

https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/





### draft: overload resolution and template function

需要梳理清楚overload resolution、template function之间的关系；substitution和overload resolution的发生顺序。



overload resolution

在下面的文章中，谈及了overload resolution：
https://en.cppreference.com/w/cpp/language/sfinae
This rule applies during overload resolution of function templates: When substituting the explicitly specified or deduced type for the template parameter fails, the specialization is discarded from the overload set instead of causing a compile error.




https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution

When the compiler tries to compile a function call (simplified):

Perform a name lookup
For function templates the template argument values are deduced from the types of the actual arguments passed into the function.
All occurrences of the template parameter (in the return type and parameters types) are substituted with those deduced types.
When this process leads to invalid type (like int::internalType) the particular function is removed from the overload resolution set. (SFINAE)
At the end, we have a list of viable functions that can be used for the specific call. If this set is empty, then the compilation fails. If more than one function is chosen, we have an ambiguity. In general, the candidate function, whose parameters match the arguments most closely is the one that is called.



https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/

http://foonathan.github.io/blog/2015/11/30/overload-resolution-4.html



### draft: implementation

需要根据function name、type of argument来从set  of candidates中找到对应的实现。

#### 实现方式

cppreference [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)对此进行了描述。



[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)



匹配过程在下面的文章中有提及：

文章：[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

这篇文章中给出的例子非常好，“matches perfectly”、“overload candidate selection phase”

维基百科文章：[Function overloading](https://en.wikipedia.org/wiki/Function_overloading)

这篇文章中，将匹配过程描述为resolution。

与此相关的问题有：

- [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)



### draft: Resolution过程总结

首先是执行name lookup，对于function，还会涉及ADL；然后进行overload resolution，然后进行template specializaiton resolution。

function template的resolution算法，在文章[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)中就总结地较好：

> while looking for the best overload, all candidates have to be considered. 

正如这篇文章中所讲述的，为了寻找到best overload，compiler采用了SFINAE策略，即它会尝试所有的candidate，关于SFINAE，参见`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`章节。





##### TODO

上述分析的resolution过程，没有涉及SFINAE，后续需要结合具体的例子来进行说明。

TO READ:

- https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/


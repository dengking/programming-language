# Compiler编译的流程



## 完整流程

构建起一个完整的编译流程，对于理解C++非常重要。

| 步骤                      | 子步骤                                                       |      |
| ------------------------- | ------------------------------------------------------------ | ---- |
| Name lookup               |                                                              |      |
|                           | [ADL](https://en.cppreference.com/w/cpp/language/adl)        |      |
| Compile template          |                                                              |      |
|                           | Template argument deduction                                  |      |
|                           | Substitution(涉及 [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)  ) |      |
| Overload resolution       |                                                              |      |
|                           |                                                              |      |
| Specializaiton resolution |                                                              |      |
|                           |                                                              |      |
| Access checking           |                                                              |      |

说明:

1、上述步骤，从上到下依次执行

2、上一步骤的输出是下一步骤的输入

3、理解上述流程是掌握C++ static polymorphism(overload、specialization)实现的基础

### Name lookup

参见 `Name-lookup` 章节；



### Compile template

参见 `Compile-template` 章节；



### Overload resolution

参见 `Overload-resolution` 章节；



### Specializaiton resolution

参见 `Compile-template` 章节；



### Access checking

暂时没有专门的总结，参见 `Classes` 章节；







## Overload resolution and template function

> NOTE: 
>
> 1、这部分内容是从 `Overload-resolution` 章节迁移过来的

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

Function template的resolution算法，在文章thegreenplace  [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/) 中就总结地较好：

> while looking for the best overload, all candidates have to be considered. 

正如这篇文章中所讲述的，为了寻找到best overload，compiler采用了SFINAE策略，即它会尝试所有的candidate，关于SFINAE，参见`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`章节。









## 参考了如下文章:

### stackoverflow [Deletion of copy-ctor & copy-assignment - public, private or protected?](https://stackoverflow.com/questions/55205874/deletion-of-copy-ctor-copy-assignment-public-private-or-protected) # [A](https://stackoverflow.com/a/55206034)

> Name lookup and overload resolution happen before access checking. 

由此可知: Name lookup > Access checking。



### cppreference [Function template](https://en.cppreference.com/w/cpp/language/function_template)

> NOTE: 
>
> 1、这篇文章非常好，它其实是按照compiler编译function template的流程来进行编写的，通过这篇文章，大致可以梳理清楚compiler 编译 function template的流程

**Template argument deduction** 段

> Template argument deduction takes place after the function template [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (which may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)) and before [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

由此可知: Name lookup( [ADL](https://en.cppreference.com/w/cpp/language/adl)) > Template argument deduction  > overload resolution。



**Template argument substitution** 段

> Substitution failure (that is, failure to replace template parameters with the deduced or provided template arguments) of a function template removes the function template from the [overload set](https://en.cppreference.com/w/cpp/language/overload_resolution). This allows a number of ways to manipulate overload sets using template metaprogramming: see [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) for details.

由此可知: 上一步骤的输出是下一步骤的输入



**[Function overloads vs function specializations](https://en.cppreference.com/w/cpp/language/function_template#Function_overloads_vs_function_specializations)** 段

> Note that only **non-template** and **primary template overloads** participate in **overload resolution**. The **specializations** are not overloads and are not considered. Only after the **overload resolution** selects the best-matching **primary function template**, its specializations are examined to see if one is a better match.

由此可知: Overload resolution > Specializaiton resolution



### [GotW #100: Compilation Firewalls (Difficulty: 6/10)](https://herbsutter.com/gotw/_100/)

> C++ took the deliberate design decision to perform overload resolution before accessibility checking for safety reasons. For example, it was felt that changing the accessibility of a function from p*rivate* to *public* shouldn’t change the meaning of legal calling code.

它解释了为什么 Name lookup > Access checking。



### stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading) # [A](https://stackoverflow.com/a/7108123)




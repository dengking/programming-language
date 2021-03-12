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




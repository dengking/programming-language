# Constant expression

本章描述的是c++中的constant expression。本章首先描述constant expression概念，然后描述C++11中引入的`constexpr`，对这两者的介绍都是基于cppreference的，cppreference的对两者的讲解中会涉及到彼此，所以需要两者一起阅读。

传统的constant expression（比如`const int i = 0`）是非常容易理解的，但是从C++11开始，对这个概念进行了generalize（推广），引入了[`constexp`](https://en.cppreference.com/w/cpp/language/constexpr) 、[`consteval`](https://en.cppreference.com/w/cpp/language/consteval) ，显然越推广就越抽象，越难以理解，所以我第一次直接阅读cppreference的时候，并没有抓住引入这些新feature的意图所在，而是直接阅读cppreference中介绍这个语言特性的各种详细细节，花费了很多时间，但是最终还是没有完全搞清楚这个特性所解决的问题，这种做法是非常愚蠢的。应该是先阅读what's new in C++（一般是在这个特性的提案中描述这部分内容， 但是提案往往非常漫长，快捷的方式之一是阅读维基百科中所总结的: [C++11 # constexpr – Generalized constant expressions](https://en.wikipedia.org/wiki/C%2B%2B11#constexpr_%E2%80%93_Generalized_constant_expressions)），先搞清楚引入这些feature的意图、application，然后再去了解这些feature的grammar等详情。



tips: 

1、[CSDN-C++20：constexpr、consteval和constinit](https://blog.csdn.net/liuguang841118/article/details/127754252)



C++11 `constexpr` 和 C++20 `consteval`的引入，是为了强化C++的[Compile time function execution](https://en.wikipedia.org/wiki/Compile_time_function_execution)，关于C++中的compile-time context、compile time function execution，参见

1、`Compile-time&&run-time\Compile-time-computation`



## cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)

Defines an [expression](https://en.cppreference.com/w/cpp/language/expressions) that can be evaluated at **compile time**.

Such expressions can be used as non-type template arguments, array sizes, and in other contexts that require constant expressions

> NOTE: 
>
> 一、在[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中也给出了和上述类似的定义:
>
> > [constant expressions ](https://en.cppreference.com/w/cpp/language/constant_expression)can be evaluated at compile time and used in compile-time context (template arguments, array sizes, etc)
>
> 相比下来，[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中给出的定义是更好的，因为它更加具有概括性: 其中提及constant expression主要用于**compile-time context**，compile-time context是一个非常重要的概念，它概括了constant expression的应用场景，对于我们理解后文非常有裨益，
>
> 关于c++compile-time context，参见`C-family-language\C-and-C++\Compile-time-and-run-time\Compile-time-and-run-time`。
>
> 二、在如下文章中提及了constant expression：
>
> stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae) # [A](https://stackoverflow.com/a/12654277)
>
> > **The expressions include not only constant expressions** such as those that appear in array bounds or as nontype template arguments **but also general expressions** (i.e., non-constant expressions) **inside** `sizeof`, **`decltype`**, and other contexts that allow non-constant expressions.



| 分类                          | 简介 |
| ----------------------------- | ---- |
| Core constant expressions     |      |
| Integral constant expression  |      |
| Converted constant expression |      |
| Constant expression           |      |



### Core constant expressions

A *core constant expression* is any expression whose evaluation *would not* evaluate any one of the following: 

> NOTE: 原文给出的Core constant expression的定义是使用的排除法，初一看，原文枚举了几十条，让人难以理解。我觉得我们需要准确把握constant expression的核心要求是: "be evaluated at compile time"，所以任何涉及 run time的，都会导致它不满足constant expression的要求。

```C++
constexpr int n = std::numeric_limits<int>::max(); // OK: max() is constexpr
constexpr int m = std::time(nullptr); // Error: std::time() is not constexpr
```



```C++
constexpr double d1 = 2.0/1.0; // OK
constexpr double d2 = 2.0/0.0; // Error: not defined
constexpr int n = std::numeric_limits<int>::max() + 1; // Error: overflow
int x, y, z[30];
constexpr auto e1 = &y - &x; // Error: undefined
constexpr auto e2 = &z[20] - &z[3]; // OK
constexpr std::bitset<2> a; 
constexpr bool b = a[2]; // UB, but unspecified if detected
```

> NOTE: 从原文给出的例子来看，所谓“core constant expressions”应该就是类似上述的，定义一个constant，可能初始化这个constant，要求initializer is a constant initializer；

#### [Usable in constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Usable-in-constant-expressions)

> NOTE:  原文的这一段所描述的是哪些variable可以用在constant expression中。



### Integral constant expression

*Integral constant expression* is an expression of integral or **unscoped enumeration type** implicitly converted to a **prvalue**, where the converted expression is a *core constant expression*.

> NOTE: 



### Converted constant expression

> NOTE: 
>
> examples：
>
> - [static_assert](https://en.cppreference.com/w/cpp/language/static_assert) 的第一个入参
> - [Template non-type arguments](https://en.cppreference.com/w/cpp/language/template_parameters#Template_non-type_arguments)



### Constant expression

> NOTE: 原文的这一段难以理解
>
> 下面枚举了一些constant expression的例子：
>
> - [sizeof operator](https://en.cppreference.com/w/cpp/language/sizeof)
> - [Enumeration](https://en.cppreference.com/w/cpp/language/enum)
> - 类似于`const int i = 0;`



## constant expression in cppreference

在下面context中，需要constant expression。

### [static_assert](https://en.cppreference.com/w/cpp/language/static_assert) 



### [Template non-type arguments](https://en.cppreference.com/w/cpp/language/template_parameters#Template_non-type_arguments)

> For integral and arithmetic types, the template argument provided during instantiation must be a [converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of the template parameter's type (so certain implicit conversion applies).




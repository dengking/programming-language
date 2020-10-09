# Constant expressions





## cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)

Defines an [expression](https://en.cppreference.com/w/cpp/language/expressions) that can be evaluated at **compile time**.

Such expressions can be used as non-type template arguments, array sizes, and in other contexts that require constant expressions

> NOTE: 在[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中也给出了和上述类似的定义:
>
> > [constant expressions ](https://en.cppreference.com/w/cpp/language/constant_expression)can be evaluated at compile time and used in compile-time context (template arguments, array sizes, etc)
>
> 相比下来，[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中给出的定义是更好的，因为它更加具有概括性: 其中提及constant expression主要用于**compile-time context**，compile-time context是一个非常重要的概念，它概括了constant expression的应用场景，对于我们理解后文非常有裨益，
>
> 关于c++compile-time context，参见`C-family-language\C-and-C++\Compile-time-and-run-time\Compile-time-and-run-time.md`。



> 在如下文章中提及了constant expression：
>
> stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae) # [A](https://stackoverflow.com/a/12654277)
>
> > **The expressions include not only constant expressions** such as those that appear in array bounds or as nontype template arguments **but also general expressions** (i.e., non-constant expressions) **inside** `sizeof`, **`decltype`**, and other contexts that allow non-constant expressions.





| 分类                          |      |      |
| ----------------------------- | ---- | ---- |
| Core constant expressions     |      |      |
| Integral constant expression  |      |      |
| Converted constant expression |      |      |
| Constant expression           |      |      |



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

> NOTE: 原文，没有给出例子，无法理解

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






# Constant expressions



## cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)



> NOTE: 在[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中给出的constant expression的介绍较好，其中提及constant expression主要用于compile-time context，关于c++compile-time context，参见`C-family-language\C-and-C++\Compile-time-and-run-time\Compile-time-and-run-time.md`。

> 在如下文章中提及了constant expression：
>
> https://stackoverflow.com/a/12654277
>
> > **The expressions include not only constant expressions** such as those that appear in array bounds or as nontype template arguments **but also general expressions** (i.e., non-constant expressions) **inside** `sizeof`, **`decltype`**, and other contexts that allow non-constant expressions.

### Core constant expressions

A *core constant expression* is any expression whose evaluation *would not* evaluate any one of the following

> NOTE: 原文给出的Core constant expression的定义是使用的排除法，这是比较不好理解的。

#### [Usable in constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Usable-in-constant-expressions)

> NOTE:  原文的这一段所描述的是哪些variable可以用在constant expression中。

### Integral constant expression



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

### [static_assert](https://en.cppreference.com/w/cpp/language/static_assert) 

syntax如下：

```c++
static_assert ( bool_constexpr , message )
```

它的第一个入参是：`bool_constexpr`，解释是：

> a contextually converted constant expression of type `bool`

`static_assert` 的第一个入参往往是和 `type_traits` 中的 各种关于 type trait的函数一起使用的，也就是compile time的，所以是constant expression。

### [Template non-type arguments](https://en.cppreference.com/w/cpp/language/template_parameters#Template_non-type_arguments)

> For integral and arithmetic types, the template argument provided during instantiation must be a [converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of the template parameter's type (so certain implicit conversion applies).
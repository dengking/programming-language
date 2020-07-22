# Constant expressions



## cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)



> NOTE: 在[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中给出的constant expression的介绍较好，其中提及constant expression主要用于compile-time context，关于c++compile-time context，参见`C-family-language\C-and-C++\Compile-time-and-run-time\Compile-time-and-run-time.md`。
>

### Core constant expressions

A *core constant expression* is any expression whose evaluation *would not* evaluate any one of the following

> NOTE: 原文给出的Core constant expression的定义是使用的排除法，这是比较不好理解的。

#### [Usable in constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression#Usable-in-constant-expressions)

> NOTE:  原文的这一段所描述的是哪些variable可以用在constant expression中。

### Integral constant expression



### Converted constant expression



### Constant expression

> NOTE: 原文的这一段难以理解
>
> 下面枚举了一些constant expression的例子：
>
> - [sizeof operator](https://en.cppreference.com/w/cpp/language/sizeof)
> - [Enumeration](https://en.cppreference.com/w/cpp/language/enum)
> - 类似于`const int i = 0;`




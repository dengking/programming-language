# Constant expressions



## cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)



> NOTE: 在[Expressions#Other](https://en.cppreference.com/w/cpp/language/expressions#Other)章节中给出的constant expression的介绍较好，其中提及constant expression主要用于compile-time context，下面是总结的（部分）compile-time context：
>
> - template arguments
> - array sizes，参见[Array declaration](https://en.cppreference.com/w/cpp/language/array)
> - [switch statement](https://en.cppreference.com/w/cpp/language/switch)

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



## cppreference [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

https://stackoverflow.com/questions/19554841/how-to-use-const-cast

## `switch` and constant expression

http://www.cplusplus.com/forum/beginner/74845/

https://stackoverflow.com/questions/46977678/switch-case-error-the-value-of-is-not-usable-in-a-constant-expression



## modify a const variable

https://www.geeksforgeeks.org/how-to-modify-a-const-variable-in-c/

https://stackoverflow.com/questions/2006161/changing-the-value-of-const-variable-in-c
# 关于本章

"initialization"即“初始化”，本章描述c++中initialization的概念、如何进行initialization等内容。

## 发展概述

### C++98 

1、[Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)

2、[Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

3、[Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

### C++03 

1、[Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization) 

弥补了[Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)中，indeterminate value(中间值) undefined behavior。

### C++11 

1、[List initialization (since C++11)](https://en.cppreference.com/w/cpp/language/list_initialization)

它又被称为uniform initialization，它统一了syntax、消除了syntax ambiguity，进行了simplify。

## 思路

C++的initialization不仅种类繁多而且规则冗杂，下面是比较好的整理思路:

首先需要区分每种initialization的syntax，因为compiler根据syntax来决定采用何种initialization；

其次是需要搞清楚每种initialization的**effect**，对effect的讨论主要集中在对下面三种type的effect: 

| type           | 注解        |
| -------------- | ----------- |
| built-in type  | int、char等 |
| OOP class type | class       |
| aggregate type | array       |

各种initialization的差异重要体现在它们的**effect**上。

## 区分

C++的initialization章节中，出现了非常多的initialization术语，如何对它们进行区分、记忆呢？

### 带参 与 不带参的initialization

这在cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization)章节有着非常好的总结。

### Default initialization VS value initialization

参见:

1、akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)

遵循 CppCoreGuidelines [ES.20: Always initialize an object](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-always)，阅读完成后，我的一个想法是: 我们总是需要value initialization，C++11 的 uniform initialization `{}` 让我们以uniform、consistent syntax来实现 value initialization。



#### stackoverflow [Do the parentheses after the type name make a difference with new?](https://stackoverflow.com/questions/620137/do-the-parentheses-after-the-type-name-make-a-difference-with-new)

> NOTE: 
>
> 1、非常能够体现 Default initialization VS value initialization

[A](https://stackoverflow.com/a/620402)

Let's get pedantic, because there are differences that can actually affect your code's behavior. Much of the following is taken from comments made to an ["Old New Thing" article](https://devblogs.microsoft.com/oldnewthing/20061214-02/?p=28713).

Sometimes the memory returned by the new operator will be initialized, and sometimes it won't depending on whether the type you're newing up is a [POD (plain old data)](https://stackoverflow.com/questions/146452/what-are-pod-types-in-c), or if it's a class that contains POD members and is using a compiler-generated default constructor.

- In C++1998 there are 2 types of initialization: zero and default
- In C++2003 a 3rd type of initialization, **value initialization** was added.

Assume:

```cpp
struct A { int m; }; // POD
struct B { ~B(); int m; }; // non-POD, compiler generated default ctor
struct C { C() : m() {}; ~C(); int m; }; // non-POD, default-initialising m
```

In a C++98 compiler, the following should occur:

- `new A` - indeterminate value
- `new A()` - zero-initialize
- `new B` - default construct (B::m is uninitialized)
- `new B()` - default construct (B::m is uninitialized)
- `new C` - default construct (C::m is zero-initialized)
- `new C()` - default construct (C::m is zero-initialized)

In a C++03 conformant compiler, things should work like so:

- `new A` - indeterminate value
- `new A()` - value-initialize A, which is zero-initialization since it's a POD.
- `new B` - default-initializes (leaves `B::m` uninitialized)
- `new B()` - value-initializes B which zero-initializes all fields since its default ctor is compiler generated as opposed to user-defined.
- `new C` - default-initializes C, which calls the default ctor.
- `new C()` - value-initializes C, which calls the default ctor.

> NOTE: 
>
> initialization 和 default ctor之间的对应关系

So in all versions of C++ there's a difference between `new A` and `new A()` because `A` is a POD.

And there's a difference in behavior between C++98 and C++03 for the case `new B()`.

This is one of the dusty corners(灰暗的角落) of C++ that can drive you crazy. When constructing an object, sometimes you want/need the parens, sometimes you absolutely cannot have them, and sometimes it doesn't matter.

### Directed initialization 和 value initialization

directed initialization 和 value initialization 的syntax非常类似，区别在于directed initialization提供了参数。



### Direct initialization 和 Copy initialization

cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) 

> Direct-initialization is more permissive(自由、宽松) than copy-initialization: 
>
> copy-initialization only considers non-[explicit](https://en.cppreference.com/w/cpp/language/explicit) constructors and non-explicit user-defined [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator), while 
>
> direct-initialization considers all constructors and all user-defined conversion functions.

如果只有explicit，那么是无法使用copy initialization的。



### 素材

stackoverflow [What do the following phrases mean in C++: zero-, default- and value-initialization?](https://stackoverflow.com/questions/1613341/what-do-the-following-phrases-mean-in-c-zero-default-and-value-initializat)
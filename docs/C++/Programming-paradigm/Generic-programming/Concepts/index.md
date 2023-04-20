# 关于本章

本章讨论C++ concept特性。

## 为什么添加concept特性？

在`Theory\Programming-language\Design-of-programming-language.md#What's new in the programming language`中，提出了“首先搞清楚设计者引入这些新特性的意图、这些新特性所能够解决的问题，然后再去了解它们的grammar”，所以在学习concept特性的grammar之前，我们实现要搞清楚c++的designer引入这个feature的原因。关于这个问题，我首先从generic programming的角度来进行分析，我觉得这个角度是最高的、最本质的，后面会结合具体的问题来进行分析。

在 [generic programming](https://en.wikipedia.org/wiki/Generic_programming) 中，就已经提出了“[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))”的概念，但是C++直到C++20版本才显式地支持它了，在之前的版本，programmer能够通过类似于`SFINAE + enable_if + type_trait` 的方式来间接地表达它，那我们就需要考虑:

1) 为什么添加concept特性?

2) concept相较于 `SFINAE + enable_if + type_trait` 的优势是什么?

3) concept能否完全地实现 `SFINAE + enable_if + type_trait`  提供的功能?

> NOTE: 前面的这几个问题其实是存在一定的重复的。

### Complete support for generic programming

阅读wikipedia [generic programming](https://en.wikipedia.org/wiki/Generic_programming)，我们会发现，[generic programming](https://en.wikipedia.org/wiki/Generic_programming)中已经提出了[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))概念，[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))是完整地实现generic programming所必须的，关于这一点，在 [Generic Programming in ConceptC++](http://www.generic-programming.org/languages/conceptcpp.html) 中进行了非常详细地说明，下面截取了其中一部分：

> ConceptC++ is an extension to the C++ Programming Language that introduces first-class support for Generic Programming. It is the first attempt to introduce complete support for Generic Programming into a mainstream programming language. ConceptC++ is a strong candidate for inclusion into the upcoming ANSI/ISO C++ Standard, dubbed C++0x. 



C++20之前，这个语言并没有显式定义concept的语法，C++20引入了这个feature，允许用户显式地定义concept。



### Summary

concept是explicit的，它是对named requirement的formal definition，它能够让我们的code更加地expressive、readable。

## wikipedia [Concepts (C++)](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B))

> NOTE: 
>
> wikipedia [Concepts (C++)](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)) 对concept的介绍是比较简洁清晰的，可以作为prime来进行阅读。
>
> C++ concept是对GP concept的一种实现，同时结合C++的实现进行了很多的扩展。

Concepts are named **[Boolean](https://en.wikipedia.org/wiki/Boolean_value) predicates** on template parameters, evaluated at [compile time](https://en.wikipedia.org/wiki/Compile_time). A concept may be associated with a template (

[class](https://en.wikipedia.org/wiki/Class_(C%2B%2B)) template, 

[function](https://en.wikipedia.org/wiki/Function_(computer_programming)) template, 

[member function](https://en.wikipedia.org/wiki/Member_function) of a class template,

[variable template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)#Variable_templates), 

[alias template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)#Template_aliases)), in which case it serves as a *constraint*: it limits the set of arguments that are accepted as template parameters.


### [Main uses](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Main_uses)

> NOTE: 
>
> 一、原文的main use章节的总结是非常好的

The main uses of concepts are:

1) Introducing type-checking to template programming

> NOTE: 
>
> 一、type requirement是GP的核心内容

2) Simplified compiler diagnostics for failed template instantiations



3) Selecting **function template overloads** and **class template specializations** based on type properties

> NOTE: 
>
> 一、static polymorphism，能够替代SFINAE

4) Constraining automatic type deduction

> NOTE: 
>
> 1、automatic type deduction是C++17引入的

### Constraint types and usage

There are five different places in a function template signature where a constraint can be used (labeled below as C1 to C5):

```c++
template<C1 T>
requires C2<T>
C3 auto Fun(C4 auto param) requires C5<T>;
```

1、`C1`: A type-constraint. This kind replaces `class` or [`typename`](https://en.wikipedia.org/wiki/Typename) for declaring a [type template parameter](https://en.wikipedia.org/wiki/TypeParameter). When using a concept instead of the former two the type is constraint.

2、`C2`: A requires-clause. Whenever a type-constraint does not work, for example, because the concept takes multiple parameters, a requires-clause can be used to apply more elaborated constraints.

> NOTE:
>
> 一、"elaborated"在这里的含义是"精细的"

3、`C3 / C4`: A **constrained placeholder type**. The same syntax is available for [placeholder](https://en.wikipedia.org/wiki/Type_inference) variable aka. `auto` variable. C++20 **added abbreviated function templates** which use `auto` as a **placeholder type** in the parameter declaration.[[2\]](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#cite_note-2) A **constrained placeholder type** allows to put constraints on the **automatically deduced return type** of a function or a variable.

4、`C5`: A **trailing requires-clause**. This form is similar to `C2` with one notable exception. A **trailing requires-clause** can be applied to a function in a class template. This allows the function to remain a regular, template-free function, which can be enabled or disabled depending on the functions trailing requires-clause.

The constraint forms `C1` and `C2` can be used in all kinds of templates.

### Example: equality_comparable

The following is a declaration of the concept "`equality_comparable`" from the `<concepts>` header of a C++20 standard library. This concept is satisfied by any type `T` such that for [lvalues](https://en.wikipedia.org/wiki/Value_(computer_science)) `a` and `b` of type `T`, the expressions `a==b` and `a!=b` as well as the reverse `b==a` and `b!=a` compile, and their results are convertible to a type that satisfies the concept "boolean-testable":

```c++
// The following concept is an implementation detail used to build equality_comparable
template<typename T, typename U>
concept weakly_equality_comparable_with = requires(const remove_reference<T>& a, const remove_reference<U>& b) {
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
    { b == a } -> std::same_as<bool>;
    { b != a } -> std::same_as<bool>;
};

template<class T>
concept equality_comparable = weakly_equality_comparable_with<T, T>;
```

A function template constrained on this concept may be declared as follows:

```c++
void f(const equality_comparable auto&); // constrained abbreviated function template declaration using a constrained placeholder type (C4 from above)
```

or

```c++
template <equality_comparable T>
void f(const T&); // constrained function template declaration using a type constraint (C1 from above)
```

And may be called as usual:

```c++
f(42); // OK, int satisfies equality_comparable
```



### [Compiler diagnostics](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Compiler_diagnostics) 

If a programmer attempts to use a template argument that does not satisfy the requirements of the template, the compiler will generate an error. When concepts are not used, such errors are often difficult to understand because the error is not reported in the context of the call, but rather in an internal, often deeply nested, implementation context where the type was used.

For example, `std::sort` requires that its first two arguments be [random-access iterators](https://en.wikipedia.org/wiki/Random-access_iterator). If an argument is not an iterator, or is an iterator of a different category, an error will occur when `std::sort` attempts to use its parameters as bidirectional iterators:

```c++
#include <iostream>
#include <algorithm>
#include <list>

int main() {
    std::list<int> l = {2, 1, 3};
    std::sort(l.begin(), l.end());
}
```

> NOTE:
>
> 一、`g++ --std=c++11 test.cpp`
>
> 编译错误如下：
>
> ```c++
> In file included from /usr/include/c++/4.8.2/algorithm:62:0,
>                  from test.cpp:2:
> /usr/include/c++/4.8.2/bits/stl_algo.h: In instantiation of ‘void std::sort(_RAIter, _RAIter) [with _RAIter = std::_List_iterator<int>]’:
> test.cpp:8:33:   required from here
> /usr/include/c++/4.8.2/bits/stl_algo.h:5461:22: error: no match for ‘operator-’ (operand types are ‘std::_List_iterator<int>’ and ‘std::_List_iterator<int>’)
>      std::__lg(__last - __first) * 2);
> 
> ```
>
> 

If concepts are used, the error can be detected and reported in the context of the call:

```
error: cannot call function 'void std::sort(_RAIter, _RAIter) [with _RAIter = std::_List_iterator<int>]'
note:   concept 'RandomAccessIterator()' was not satisfied
```

> NOTE: 
>
> 一、在cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) 中对此有说明，因为
>
> > Violations of **constraints** are detected at compile time, early in the template instantiation process, which leads to easy to follow error messages.

### [Overload resolution](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Overload_resolution)

> NOTE: 
>
> 一、concept可以作为 [SFINAE](https://en.wikipedia.org/wiki/SFINAE) and [tag dispatching](https://en.wikipedia.org/w/index.php?title=Tag_dispatching&action=edit&redlink=1) 的替代方案，它能够实现相同的功能

Concepts can be used to choose **function template overloads** and **class template specializations** based on properties of their template arguments, as an alternative to [SFINAE](https://en.wikipedia.org/wiki/SFINAE) and [tag dispatching](https://en.wikipedia.org/w/index.php?title=Tag_dispatching&action=edit&redlink=1). If an argument satisfies more than one concept, the overload associated with the **more constrained concept** is chosen.



### [Type deduction](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Type_deduction)

Concepts may be used instead of the unconstrained type deduction placeholder `auto` in variable declarations and function return types:

```C++
auto     x1 = f(y); // the type of x1 is deduced to whatever f returns
Sortable auto x2 = f(y); // the type of x2 is deduced, but only compiles if it satisfies Sortable
```





## See also

https://www.modernescpp.com/index.php/c-20-concepts-the-details

https://www.drdobbs.com/cpp/the-c0x-remove-concepts-decision/218600111

https://www.modernescpp.com/index.php/c-20-define-the-concept-regular-and-semiregular
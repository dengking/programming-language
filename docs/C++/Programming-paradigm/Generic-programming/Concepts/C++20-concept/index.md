# Concepts

是在阅读cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)的时候，发现了[concepts](https://en.cppreference.com/w/cpp/language/constraints)特性的。



## cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) 

> NOTE: 
>
> 一、查看右侧的目录可以知道，原文的行文思路是：
>
> 首先介绍constraint和concept的概念，然后对concept进行介绍，然后对constraint进行介绍。
>
> 二、两者之间的关系:
>
> 1、use concept to constrain
>
> 2、concept需要独立地进行定义，constrain将concept用于class template、function template等等

[Class templates](https://en.cppreference.com/w/cpp/language/class_template), [function templates](https://en.cppreference.com/w/cpp/language/function_template), and non-template functions (typically members of class templates) may be associated with a *constraint*, which specifies the requirements on template arguments, which can be used to select the most appropriate function overloads and template specializations.

> NOTE:
>
> 一、concept是static polymorphism、static dispatch、static select

Named sets of such requirements are called *concepts*. Each concept is a predicate, evaluated at compile time, and becomes a part of the interface of a template where it is used as a **constraint**:

> NOTE:
>
> 一、这段话表述了concept和constrain的关系：我们可以定义concept，然后将这个concept用作constraint。



```c++
#include <string>
#include <cstddef>
#include <concepts>

// Declaration of the concept "Hashable", which is satisfied by any type 'T'
// such that for values 'a' of type 'T', the expression std::hash<T>{}(a)
// compiles and its result is convertible to std::size_t
template<typename T>
concept Hashable = requires(T a)
{
{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

struct meow {};

// Constrained C++20 function template:
template<Hashable T>
void f(T) {}
//
// Alternative ways to apply the same constraint:
// template<typename T>
//     requires Hashable<T>
// void f(T) {}
//
// template<typename T>
// void f(T) requires Hashable<T> {}
//
// void f(Hashable auto /*parameterName*/) {}

int main()
{
    using std::operator""s;

    f("abc"s);    // OK, std::string satisfies Hashable
    // f(meow{}); // Error: meow does not satisfy Hashable
}
```

Violations of constraints are detected at compile time, early in the template instantiation process, which leads to easy to follow error messages

> NOTE:
>
> 一、这是使用constrains的主要原因之一



The intent of concepts is to model **semantic categories** (Number, Range, RegularFunction) rather than **syntactic restrictions** (HasPlus, Array). According to [ISO C++ core guideline T.20](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t20-avoid-concepts-without-meaningful-semantics), "The ability to specify a meaningful semantics is a defining characteristic of a true concept, as opposed to a syntactic constraint."

> NOTE: 
>
> 一、concept提供给programmer来表达semantic的



### [Concepts](https://en.cppreference.com/w/cpp/language/constraints#Concepts)



Concepts cannot:

1、... recursively refer to themselves 

2、... cannot be constrained

3、... explicit instantiations

4、... explicit specializations

5、... partial specializations of concepts (the meaning of the original definition of a constraint cannot be changed).



```c++
// concept
template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

template<class T>
concept C1 = true;
```

> NOTE:
>
> 一、没有`requires`，直接true、false



```c++
template<typename T>
concept V = V<T*>; // error: recursive concept
```



```c++
template<class T>
concept C1 = true;
template<C1 T>
concept Error1 = true; // Error: C1 T attempts to constrain a concept definition
template<class T> requires C1<T>
concept Error2 = true; // Error: the requires-clause attempts to constrain a concept
```





### [Constraints](https://en.cppreference.com/w/cpp/language/constraints#Constraints)

> NOTE:
>
> 一、`requires` constrains

A constraint is a sequence of logical operations and operands that specifies requirements on template arguments. They can appear within [requires expressions](https://en.cppreference.com/w/cpp/language/requires) or directly as bodies of concepts.

There are three types of constraints:

\1) conjunctions

\2) disjunctions

\3) atomic constraints



#### Conjunctions

The conjunction of two constraints is formed by using the `&&` operator in the constraint expression:

```c++
template<class T>
concept Integral = std::is_integral<T>::value;
template<class T>
concept SignedIntegral = Integral<T> && std::is_signed<T>::value;
template<class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;
```



#### Disjunctions



#### Atomic constraints



#### Constraint normalization



### [Requires clauses](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses) 

The keyword [`requires`](https://en.cppreference.com/w/cpp/keyword/requires) is used to introduce a *requires-clause*, which specifies constraints on template arguments or on a function declaration.



### [Requires expressions](https://en.cppreference.com/w/cpp/language/constraints#Requires_expressions)



#### Simple requirements



#### Type requirements



#### Compound Requirements



#### Nested requirements



### [Partial ordering of constraints](https://en.cppreference.com/w/cpp/language/constraints#Partial_ordering_of_constraints)


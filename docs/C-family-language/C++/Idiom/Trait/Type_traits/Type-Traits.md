# C++ Type Traits

在c++标准库有[Type traits](https://en.cppreference.com/w/cpp/types#Type_traits)库，本文对它的设计理念以及作用进行分析。

## drdobbs [C++ Type Traits](https://www.drdobbs.com/cpp/c-type-traits/184404270)

Generic programming -- that is, writing code that works with any data type meeting a set of **requirements** -- has become the method of choice for delivering reusable code. However, there are times in generic programming when **generic** just isn't good enough -- sometimes the differences between types are too great for an efficient generic implementation. This is when the **traits technique** becomes important. By encapsulating those properties that need to be considered on a type-by-type basis inside a traits class, you can minimize the amount of code that has to differ from one type to another, and maximize the amount of **generic code**.

> NOTE: 关于Generic programming，参见`Theory\Programming-paradigm\Generic-programming`章节，其中也描述了**generic** 概念

For example, when working with character strings, one common operation is to determine the length of a null-terminated string. Clearly, it's possible to write generic code that can do this, but it turns out that there are much more efficient methods available. The C library functions `strlen` and `wcslen`, for instance, are usually written in assembler, and with suitable hardware support can be considerably faster than a generic version written in C++. The authors of the C++ Standard Library realized this, and abstracted the properties of `char` and `wchar_t` into the class `char_ traits`. Generic code that works with character strings can simply use `char_ traits<>::length` to determine the length of a null-terminated string, safe in the knowledge that specializations of `char_traits` will use the most appropriate method available to them.



### Type Traits

Class `char_traits` is a classic example of a collection of type-specific properties wrapped up in a single class -- what Nathan Myers terms a "baggage class." In the [Boost type-traits library](http://www.boost.org/doc/libs/1_53_0/libs/type_traits/doc/html/index.html), we have written a set of very specific traits classes, each of which encapsulate a single **trait** from the C++ type system. For example, is a type a pointer or a reference type, or does a type have a trivial constructor, or a `const` qualifier? The type-traits classes share a unified design. Each class has a single member called ` value` -- a **compile-time constant** that is `true `if the type has the specified property, and `false `otherwise. These classes can be used in generic programming to determine the properties of a given type and introduce optimizations that are appropriate for that case.

> NOTE: 可以将trait理解为“特征
>
> **compile-time constant**使compiler可以选择more specialized specialization。
>
> 上面这段话中描述的方法的一个典型的例子就是accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)中的`supports_optimised_implementation`

The type-traits library also contains a set of classes that perform a specific transformation on a type; for example, they can remove a top-level `const` or `volatile `qualifier from a type. Each class that performs a transformation defines a single `typedef`-member `type `that is the result of the transformation. All of the type-traits classes are defined inside namespace `boost`; for brevity, this namespace qualification is omitted in most of the code samples presented here.

### Implementation

Beginning with possibly the simplest class in the library, `is_void` has a member `value `that is `true `only if `T` is void; see Listing One. Here we have defined the primary version of the template class `is_void`, and provided a full specialization when `T` is void.

##### Listing One

```c++
template <typename T> 
struct is_void
{ static const bool value = false; };
 
template <> 
struct is_void<void>
{ static const bool value = true; };
```

While full specialization of a template class is an important technique, you sometimes need a solution that is halfway between a **fully generic solution**, and a **full specialization**. This is exactly the situation for which the standards committee defined **partial template-class specialization**. To illustrate, consider the class `boost::is_pointer`. Here we needed a primary version that handles all the cases where `T` is not a pointer, and a partial specialization to handle all the cases where `T` is a pointer; see Listing Two.

> NOTE: 上述`struct is_void<void>`就是典型的full specialization。上面这段话中的**fully generic solution**，所指的是一个纯粹的目标，如`template <typename T> struct is_void`。在**full specialization**和**fully generic solution**之间的就是**partial template-class specialization**。

##### Listing Two

```c++
template <typename T> 
struct is_pointer 
{ static const bool value = false; };
  
template <typename T> 
struct is_pointer<T*> 
{ static const bool value = true; };
```



## [A quick primer on type traits in modern C++](https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp)

**Type traits** are a clever technique used in C++ template metaprogramming that gives you the ability to inspect and transform the properties of *types*.

This is extremely useful in **conditional compilation**, where you instruct the compiler to pick the right path according to the type in input. We will see an example shortly.

> NOTE: **conditional compilation**

### What is a type trait?



### How do I use a type trait?

```c++
#include <iostream>
#include <type_traits>

class Class {};

int main() 
{
    std::cout << std::is_floating_point<Class>::value << '\n';
    std::cout << std::is_floating_point<float>::value << '\n';
    std::cout << std::is_floating_point<int>::value << '\n';
}
```

### How does it work exactly?

In the snippet above you are passing three different types to the template struct `std::is_floating_point`: a custom `Class` type, a `float` and an `int`. The compiler, as with any regular template stuff, will generate three different structs for you under the hood:

```c++
struct is_floating_point_Class {
    static const bool value = false;
};

struct is_floating_point_float {
    static const bool value = true;
};

struct is_floating_point_int {
    static const bool value = false;
};
```

At this point it's just a matter of reading the `value` member inside those structs created by the compiler. Being static, you have to access the member constant with the `::` syntax. Just keep in mind that this is template metaprogramming, so everything takes place at *compile time*.

> NOTE: type trait的实现


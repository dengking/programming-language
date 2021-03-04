# Type inference



## wikipedia C++11 # [Type inference](https://en.wikipedia.org/wiki/C++11#Type_inference)

In C++03 (and C), to use a variable, its type must be specified explicitly. However, with the advent of **template types** and **template metaprogramming** techniques, the type of something, particularly the well-defined **return value of a function**, may not be easily expressed. Thus, storing intermediates in variables is difficult, possibly needing knowledge of the internals of a given metaprogramming library.

> NOTE: 上面描述的难以描述type的问题，在wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype)中描述了这种情况的一个典型的案例:
>
> It is sometimes desirable to write a **generic forwarding function** that returns the same type as the wrapped function, regardless of the type it is instantiated with. Without `decltype`, it is not generally possible to accomplish this.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) An example, which also utilizes the *[trailing-return-type](https://en.wikipedia.org/wiki/Trailing-return-type)*:[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8)
>
> ```C++
> #include <iostream>
> int g_IntValue = 1;
> int& foo(int& i)
> {
> 	std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	return g_IntValue;
> }
> float foo(float& f)
> {
> 	std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	return 0.0;
> }
> 
> template<class T>
> auto transparent_forwarder(T& t) -> decltype(foo(t))
> {
> 	return foo(t);
> }
> 
> int main()
> {
> 	int i = 0;
> 	float j = 0.0;
> 	transparent_forwarder(i);
> 	transparent_forwarder(j);
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 在`./decltype`章节中描述了更多的案例。
>
> 解决思路是由compiler来进行type inference。

C++11 allows this to be mitigated(缓解) in two ways. 

### First approche: `auto`

First, the **definition** of a variable with an explicit initialization can use the `auto` keyword.[[11\]](https://en.wikipedia.org/wiki/C++11#cite_note-11)[[12\]](https://en.wikipedia.org/wiki/C++11#cite_note-12) This creates a variable of the specific type of the initializer:

```c++
auto some_strange_callable_type = std::bind(&some_function, _2, _1, some_object);
auto other_variable = 5;
```

The type of `some_strange_callable_type` is simply whatever the particular template function override of `std::bind` returns for those particular arguments. This type is easily determined procedurally(自动的) by the compiler as part of its **semantic analysis** duties, but is not easy for the user to determine upon inspection. The type of `other_variable` is also well-defined, but it is easier for the user to determine. It is an `int`, which is the same type as the integer literal.

This use of the keyword `auto` in C++ re-purposes the semantics of this keyword, which was originally used in the typeless predecessor language [B ](https://en.wikipedia.org/wiki/B_(programming_language)#Examples)in a related role of denoting an untyped [automatic variable](https://en.wikipedia.org/wiki/Automatic_variable) definition.

`auto` is also useful for reducing the verbosity of the code. For instance, instead of writing

```C++
for (std::vector<int>::const_iterator itr = myvec.cbegin(); itr != myvec.cend(); ++itr)
```

the programmer can use the shorter

```C++
for (auto itr = myvec.cbegin(); itr != myvec.cend(); ++itr)
```

which can be further compacted since "`myvec`" implements begin/end iterators:

```C++
for (auto& x : myvec)
```

This difference grows as the programmer begins to nest containers, though in such cases `typedef`s are a good way to decrease the amount of code.

### Second approche: `decltype` 

Further, the keyword `decltype` can be used to determine the type of **expression** at compile-time. For example:

```
int some_int;
decltype(some_int) other_integer_variable = 5;
```

This is more useful in conjunction with `auto`, since the type of auto variable is known only to the compiler. However, `decltype` can also be very useful for expressions in code that makes heavy use of [operator overloading](https://en.wikipedia.org/wiki/Operator_overloading) and specialized types.

> NOTE:  最后一句话没有搞懂
> 

The type denoted by `decltype` can be different from the type deduced by `auto`.

```C++
#include <vector>
int main()
{
    const std::vector<int> v(1);
    auto a = v[0];        // a has type int
    decltype(v[0]) b = 1; // b has type const int&, the return type of
                          //   std::vector<int>::operator[](size_type) const
    auto c = 0;           // c has type int
    auto d = c;           // d has type int
    decltype(c) e;        // e has type int, the type of the entity named by c
    decltype((c)) f = c;  // f has type int&, because (c) is an lvalue
    decltype(0) g;        // g has type int, because 0 is an rvalue
}
```



## 发展概述

### C++11

1、`auto`

2、`decltype`



### C++14

C++14 return type deduction for function 特性，参见: 

1、[C++14](https://en.cppreference.com/w/cpp/14) # [return type deduction for functions](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction_.28since_C.2B.2B14.29)

C++14 `decltype(auto)`

对C++ 11 `auto` + `decltype` + trailing return type的简化，参见`./decltype`。



### C++17

C++17 新增 class template argument deduction 特性，参见: 

1、cppreference [C++17](https://en.cppreference.com/w/cpp/17) # [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)

2、wikipedia C++17

C++17 新增 "Use of auto as the type for a non-type template parameter"，参见

1、cppreference [C++17](https://en.cppreference.com/w/cpp/17) # non-type [template parameters](https://en.cppreference.com/w/cpp/language/template_parameters) declared with `auto`

2、wikipedia C++17

## 为什么需要type inference? 

从上面的**发展概述**来看，C++的一个非常重要的方向是对"type inference"的增强，那我们就需要思考: "为什么需要type inference"。

### 更好的支持generic programming

通过上面的描述可以看出，**type inference**是**generic programming**所必须的，在下面文章中对此进行了说明:

1) 在wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype)中对此有着很好的总结: 

> With the introduction of [templates](https://en.wikipedia.org/wiki/Template_(programming)) into the C++ programming language, and the advent of [generic programming](https://en.wikipedia.org/wiki/Generic_programming) techniques pioneered by the [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library), the need for a mechanism for obtaining the type of an [expression](https://en.wikipedia.org/wiki/Expression_(programming)), commonly referred to as `typeof`, was recognized. In generic programming, it is often difficult or impossible to express types that depend on template parameters,[[2\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1478-2)[[3\]](https://en.wikipedia.org/wiki/Decltype#cite_note-cleanup-3) in particular the return type of function template instantiations.[[2\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1478-2)
>
> One of the cited main motivations for the `decltype` proposal was the ability to write **perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates**.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) 

2) `./decltype`

一个典型的需求就是: **perfect forwarding function template**。

### Simplify C++/ Make C++ more expressive

减轻programmer的工作量，让programmer写更少的code。



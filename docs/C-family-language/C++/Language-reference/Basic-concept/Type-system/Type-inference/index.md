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

`auto`



`decltype`



### C++14

C++14 return type deduction for function 特性，参见: 

- [C++14](https://en.cppreference.com/w/cpp/14) # [return type deduction for functions](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction_.28since_C.2B.2B14.29)



C++14 `decltype(auto)`

对C++ 11 `auto` + `decltype` + trailing return type的简化，参见`./decltype`。



### C++17

C++17 新增 class template argument deduction 特性，参见: 

- cppreference [C++17](https://en.cppreference.com/w/cpp/17) # [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)

- Wikipedia C++17

C++17 新增 "Use of auto as the type for a non-type template parameter"，参见

- cppreference [C++17](https://en.cppreference.com/w/cpp/17) # non-type [template parameters](https://en.cppreference.com/w/cpp/language/template_parameters) declared with `auto`
- Wikipedia C++17

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

### Make C++ more expressive

减轻programmer的工作量，让programmer写更少的code。





## cppreference [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)



Template argument deduction takes place after the function template [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (which may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)) and before [template argument substitution](https://en.cppreference.com/w/cpp/language/function_template#Template_argument_substitution) (which may involve [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)) and [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

> NOTE: 需要将这些流程给串联起来

### Deduction from a function call



If there are multiple parameters, each `P/A` pair is deduced **separately** and the deduced template arguments are then **combined**. If deduction fails or is ambiguous for any `P/A` pair or if different pairs yield different deduced template arguments, or if any template argument remains neither deduced nor explicitly specified, compilation fails.

> NOTE: 上面这段话非常重要，它描述了deduction的机制



#### Non-deduced contexts

> NOTE: 关于non-deduced contexts，是我在阅读如下文章时碰到的：
>
> bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)，其中对`std::forward`的实现的分析中有如下描述：
>
> > The type `T` is not deduced, therefore we had to specify it when **using** `std::forward`.
>
> thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)，其中对`std::forward`的实现的分析中有如下描述：
>
> > Another thing I want to mention is the use of `std::remove_reference<T>`. In fact, if you think about it, `forward` could do without it. **Reference collapsing** does the job already, so `std::remove_reference<T>` is superfluous（多余的）. It's there to turn the `T& t` into a **non-deducing context** (according to the C++ standard, section 14.8.2.5), thus forcing us to explicitly specify the **template parameter** when calling `std::forward`.
>
> 这让我思考：如何来强制不进行deduction。
>
> 在某些情况下，programmer是不想启用deduction的，所以C++提供了给了programmer这种控制能力，这是C++灵活性的体现。
>
> ## Disable argument-based template parameter deduction for functions
>
> stackoverflow [Better way to disable argument-based template parameter deduction for functions?](https://stackoverflow.com/questions/37737487/better-way-to-disable-argument-based-template-parameter-deduction-for-functions)
>
> You can do it by putting T in non deducible context (to the left of `::`), and use [std::common_type](http://en.cppreference.com/w/cpp/types/common_type) from `<type_traits>`.
>
> example:
>
> ```cpp
> template <typename T> void f(typename std::common_type<T>::type obj) {std::cout << obj;}
> ```

In the following cases, the types, templates, and non-type values that are used to compose `P` do not participate in **template argument deduction**, but instead use the **template arguments** that were either deduced elsewhere or explicitly specified. If a template parameter is used only in **non-deduced contexts** and is not explicitly specified, template argument deduction fails.

1) The *nested-name-specifier* (everything to the left of the scope resolution operator `::`) of a type that was specified using a [qualified-id](name.html#Qualified_identifiers):

##### Example 1

```c++
#include <vector>
#include <complex>
// the identity template, often used to exclude specific arguments from deduction
// (available as std::type_identity as of C++20)
template<typename T> struct identity
{
	typedef T type;
};
template<typename T> void bad(std::vector<T> x, T value = 1)
{
}
template<typename T> void good(std::vector<T> x, typename identity<T>::type value = 1)
{
}

int main()
{
	std::vector<std::complex<double>> x;
	// bad(x, 1.2);  // P1 = std::vector<T>, A1 = std::vector<std::complex<double>>
				  // P1/A1: deduced T = std::complex<double>
				  // P2 = T, A2 = double
				  // P2/A2: deduced T = double
				  // error: deduction fails, T is ambiguous
	good(x, 1.2); // P1 = std::vector<T>, A1 = std::vector<std::complex<double>>
				  // P1/A1: deduced T = std::complex<double>
				  // P2 = identity<T>::type, A2 = double
				  // P2/A2: uses T deduced by P1/A1 because T is to the left of :: in P2
				  // OK: T = std::complex<double>
}

```

> NOTE: 上述`identity<T>::type`就是 **non-deduced contexts** 。



##### Example 2

关于此的另外一个例子就是`std::forward`，这在bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)、thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中都有描述；



2) The expression of a decltype-specifier(since C++11):

> NOTE: 关于这一点，是在阅读stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work) 时，其中提及的。这一点，收录在`C++\Language-reference\Basic-concept\Type-system\decltype\decltype.md` 中。

#### Deduction from a type



### Other contexts
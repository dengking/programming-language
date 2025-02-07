# User-defined conversion 

本章讨论C++中的user-defined conversion，它包括:

1、`User-defined-conversion-function`

2、`Converting-constructor`



## Converting constructor VS User-defined conversion function

1、Explicit constructor/Converting constructor: 使用其他类型的object，来构造本类型的object

2、User-defined conversion function: 将本类型的object转换为其他类型的object，它会构建一个新的object



## Explicit and implicit

C++11的`explicit` specifier指定conversion为explicit。

Implicit conversion:

- converting constructor
- implicit user-defined conversion function



Explicit conversion:

- explicit constructor

- explicit user-defined conversion function

### C++11 explicit specifier

参见 `C++11-Explicit-specifier` 章节

1、[explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) 不是 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

2、一个class，应该不能够同时既定义 [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit)  又定义 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) ，从下面的描述来看，两者之间存在着一定的对立。



## [`explicit` specifier/ Explicit constructor](https://en.cppreference.com/w/cpp/language/explicit)





## (Implicit) converting constructor



### cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

A **constructor** that is **not** declared with the specifier [explicit](https://en.cppreference.com/w/cpp/language/explicit) and which can be called with **a single parameter**(until C++11) is called a ***converting constructor***.

> NOTE: 从C++11开始，对"single parameter"放开了限制，允许有多个parameter，后面描述了这样的example

Unlike **explicit constructors**, which are only considered during [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (which includes [explicit conversions](https://en.cppreference.com/w/cpp/language/explicit_cast) such as [static_cast](https://en.cppreference.com/w/cpp/language/static_cast)), **converting constructors** are also considered during [copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization), as part of [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast).

> NOTE: 
>
> 上述  [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast) 其实就是 [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) 
>
> 上面这段话的意思是: **converting constructors** 既在 [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) 中被考虑又在 [copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) 中被考虑 

It is said that a **converting constructor** specifies an **implicit conversion** from the types of its arguments (if any) to the type of its class. Note that non-explicit [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) also specifies an **implicit conversion**.

Implicitly-declared and user-defined non-explicit [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor) and [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) are **converting constructors**.

#### Example

```c++
#include <iostream>

struct A {
    // converting constructor (since C++11)
    A() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    // converting constructor
    A(int) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    // converting constructor (since C++11)
    A(int, int) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

struct B {
    explicit B() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    explicit B(int) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    explicit B(int, int) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

int main() {
  A a1 = 1;       // OK: copy-initialization selects A::A(int)
  A a2(2);        // OK: direct-initialization selects A::A(int)
  A a3{4, 5};     // OK: direct-list-initialization selects A::A(int, int)
  A a4 = {4, 5};  // OK: copy-list-initialization selects A::A(int, int)
  A a5 = (A)1;    // OK: explicit cast performs static_cast, direct-initialization

  //  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
  B b2(2);     // OK: direct-initialization selects B::B(int)
  B b3{4, 5};  // OK: direct-list-initialization selects B::B(int, int)
  // B b4 = {4, 5};  // error: copy-list-initialization selected an explicit constructor
  //        B::B(int, int)
  B b5 = (B)1;  // OK: explicit cast performs static_cast, direct-initialization
  B b6;         // OK, default-initialization
  B b7{};       // OK, direct-list-initialization
  //  B b8 = {};     // error: copy-list-initialization selected an explicit constructor
  //        B::B()

  [](...) {}(a1, a4, a4, a5, b5);  // may suppress "unused variable" warnings
}

/**
A::A(int)
A::A(int)
A::A(int, int)
A::A(int, int)
A::A(int)
B::B(int)
B::B(int, int)
B::B(int)
B::B()
B::B()
**/
```



## User-defined conversion function



### cppreference [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator)

Enables [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) or [explicit conversion](https://en.cppreference.com/w/cpp/language/explicit_cast) from a [class type](https://en.cppreference.com/w/cpp/language/class) to another type.

#### Syntax

Conversion function is declared like a [non-static member function](https://en.cppreference.com/w/cpp/language/member_functions) or member [function template](https://en.cppreference.com/w/cpp/language/function_template) with no parameters, no explicit return type, and with the name of the form:

> NOTE: 原文对syntax的区分是依据“explicit”的

|      | Syntax                                                      | explanation                                                  |
| ---- | ----------------------------------------------------------- | ------------------------------------------------------------ |
| 1)   | `operator` *conversion-type-id*                             | participates in all [implicit](https://en.cppreference.com/w/cpp/language/implicit_cast) and [explicit conversions](https://en.cppreference.com/w/cpp/language/explicit_cast). |
| 2)   | `explicit operator` *conversion-type-id*                    | (since C++11)<br>participates in [direct-initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) and [explicit conversions](https://en.cppreference.com/w/cpp/language/explicit_cast) only. |
| 3)   | `explicit (` *expression* `) operator` *conversion-type-id* | (since C++20)<br>[conditionally explicit](https://en.cppreference.com/w/cpp/language/explicit). |

> NOTE: 下面是注解:

|      | 注解                                                         |
| ---- | ------------------------------------------------------------ |
| 1)   |                                                              |
| 2)   | 在[Implicit conversions#Order of the conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion#Order_of_the-conversions) *implicitly convertible to* 段中给出了direct initialization的例子: <br/>Note that this is different from [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (`T2 t(e)`), where explicit constructors and conversion functions would additionally be considered. |
| 3)   |                                                              |

*conversion-type-id* is a [type-id](https://en.cppreference.com/w/cpp/language/type#Type_naming) except that function and array operators `[]` or `()` are not allowed in its declarator (thus conversion to types such as **pointer to array** requires a **type alias/typedef** or an **identity template**: see below). Regardless of typedef, *conversion-type-id* cannot represent an array or a function type.

> NOTE: 最后一句话其实说明的是：在C++中，是不允许：
>
> - conversion to array
> - conversion to function
>
> 但是conversion to pointer，能够实现:
>
> - conversion to pointer to array
> - conversion to pointer to function
>
> 下面是截取自原文中的例子，对上面这段话进行注解: 

```C++
#include <iostream>

struct X
{

//   Error: array operator not allowed in conversion-type-id
//   operator int(*)[3]() const { return nullptr; }
	using arr_t = int[3];
	operator arr_t*() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return nullptr;
	} // OK if done through typedef
//  operator arr_t () const; // Error: conversion to array not allowed in any case
};

int main()
{
	X x;

	int (*pa)[3] = x;  // OK
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```C++
> X::operator int (*)[3]() const
> ```
>
> 

**Specifier**:

> NOTE: 下面总结了所有可以使用的specifier:

| specifier                                                    | explanation                                                  | 注解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [`explicit`](https://en.cppreference.com/w/cpp/language/explicit) |                                                              | 前面已经说明                                                 |
| `const`                                                      |                                                              |                                                              |
| [`inline`](https://en.cppreference.com/w/cpp/language/inline) |                                                              |                                                              |
| [`virtual`](https://en.cppreference.com/w/cpp/language/virtual) |                                                              | 这说明conversion function是可以dynamic polymorphism的，这有那些用途？ |
| [`constexpr`](https://en.cppreference.com/w/cpp/language/constexpr) (since C++11) |                                                              |                                                              |
| [`consteval`](https://en.cppreference.com/w/cpp/language/consteval) (since C++20) |                                                              |                                                              |
| [`friend`](https://en.cppreference.com/w/cpp/language/friend) | `friend` requires a qualified name: `friend A::operator B();` | 如何使用？                                                   |



## Microsoft [User-Defined Type Conversions (C++)](https://docs.microsoft.com/en-us/cpp/cpp/user-defined-type-conversions-cpp?view=vs-2019)



## Conversion isn't considered for member access

stackoverflow [Why user-defined conversion is not implicitly taking place on the calling object](https://stackoverflow.com/questions/44699176/why-user-defined-conversion-is-not-implicitly-taking-place-on-the-calling-object) :

```C++
class A {
public:
   void func() const {}
};

class B {
public:
   // user-defined conversion operator to A
   operator A() const { return a_; }
private:
   A a_;
};
```



```c++
B b;
static_cast<A>(b).func(); // call func() on temporary instance of A
```



```C++
B b;
b.func(); // <-- error: 'class B' has no member named 'func'
```



[A](https://stackoverflow.com/a/44699334)

Conversion isn't considered for member access (§5.2.5/2 [expr.ref]).

> In either case, the *id-expression* shall name a member of the class or of one of its base classes

Here the *id-expression* is `func()`

So the compiler considers that `func` must be a member of `B` or a class that `B` derives from. Implicit conversion to other types that might have a `func` member is not considered.



## Example

Example1: overload conversion operator for custom type to `std::string`: 

来源: stackoverflow [C++ overloading conversion operator for custom type to std::string](https://stackoverflow.com/questions/3518145/c-overloading-conversion-operator-for-custom-type-to-stdstring)

> NOTE: 这个例子收录到了`C++\Language-reference\Functions\Overload-resolution\Ambiguous-overload.md`中



### Template user-defined conversion function

[std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) 

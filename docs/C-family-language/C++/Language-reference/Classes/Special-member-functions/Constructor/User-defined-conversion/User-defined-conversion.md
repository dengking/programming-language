# user-defined conversion 



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



### Microsoft [User-Defined Type Conversions (C++)](https://docs.microsoft.com/en-us/cpp/cpp/user-defined-type-conversions-cpp?view=vs-2019)



### Conversion isn't considered for member access

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



### Example

Example1: overload conversion operator for custom type to `std::string`: 

来源: stackoverflow [C++ overloading conversion operator for custom type to std::string](https://stackoverflow.com/questions/3518145/c-overloading-conversion-operator-for-custom-type-to-stdstring)

> NOTE: 这个例子收录到了`C++\Language-reference\Functions\Overload-resolution\Ambiguous-overload.md`中

## Converting constructor



### cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)
# cppreference [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

> NOTE: 
>
> 1、by-value的时候，都会涉及  [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

|                                    |                                                              |                 |
| ---------------------------------- | ------------------------------------------------------------ | --------------- |
| T object = other;                  |                                                              |                 |
| T object = {other} ; (until C++11) |                                                              |                 |
| f(other)                           | when [passing an argument](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_function_call_operator) to a function by value | pass-by-value   |
| return other;                      |                                                              | return-by-value |
| throw object;<br/>catch (T object) |                                                              | throw-by-value  |
| `T array[N] = {other};`            |                                                              |                 |

## Notes

### Constructor

Copy-initialization is less permissive than direct-initialization: [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) are not [converting constructors](https://en.cppreference.com/w/cpp/language/converting_constructor) and are not considered for copy-initialization.

```C++
struct Exp
{
	explicit Exp(const char*)
	{
	}
};

struct Imp
{
	Imp(const char*)
	{
	}
};

int main()
{
	// not convertible from const char*
	Exp e1("abc");  // OK
	Exp e2 = "abc"; // Error, copy-initialization does not consider explicit constructor

	// convertible from const char*
	Imp i1("abc");  // OK
	Imp i2 = "abc"; // OK
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp: 在函数‘int main()’中:
> test.cpp:19:11: 错误：请求从‘const char [4]’转换到非标量类型‘Exp’
>   Exp e2 = "abc"; // Error, copy-initialization does not consider explicit constructor
> ```
>
> 

### Implicit conversion and copy-initialization

[Implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) is defined in terms of copy-initialization: if an object of type `T` can be copy-initialized with expression `E`, then `E` is implicitly convertible to `T`.



In addition, the implicit conversion in copy-initialization must produce `T` directly from the initializer, while, e.g. direct-initialization expects an implicit conversion from the initializer to an argument of `T`'s constructor.

```C++
#include <string>

struct S
{
	S(std::string) // implicitly convertible from std::string
	{
	}
};


int main()
{
	S s1("abc"); // OK: conversion from const char[4] to std::string
	S s2 = "abc"; // Error: no conversion from const char[4] to S
	S s3 = "abc"s; // OK: conversion from std::string to S
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp: 在函数‘int main()’中:
> test.cpp:14:9: 错误：请求从‘const char [4]’转换到非标量类型‘S’
>   S s2 = "abc"; // Error: no conversion from const char[4] to S
> 
> ```
>
> 

### Copy-initialization and move-initialization

If *other* is an rvalue expression, [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) will be selected by overload resolution and called during copy-initialization. There is no such term as move-initialization.

### Equals sign, `=`, in copy-initialization VS assignment operator overload

The equals sign, `=`, in copy-initialization of a named variable is not related to the assignment operator. Assignment operator overloads have no effect on copy-initialization.


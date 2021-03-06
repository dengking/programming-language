# `constexpr` specifier

`constexpr`应该是constant expression的缩写，显然它是为了支持c++的constant expression特性而添加的，显然，它需要杜绝所有的run-time概念。



## What's in C++ # `constexpr`

wikipedia [C++11#`constexpr` – Generalized constant expressions](https://en.wikipedia.org/wiki/C++11#constexpr_%E2%80%93_Generalized_constant_expressions)，这一段所总结地非常好。

wikipedia [Compile time function execution#Examples](https://en.wikipedia.org/wiki/Compile_time_function_execution)

wikipedia [Template metaprogramming#Compile-time class generation](https://en.wikipedia.org/wiki/Template_metaprogramming#Compile-time_class_generation)：

> In [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) and [C++20](https://en.wikipedia.org/wiki/C%2B%2B20), [constexpr](https://en.wikipedia.org/wiki/Constexpr) and [`consteval`](https://en.cppreference.com/w/cpp/language/consteval) were introduced to let the compiler execute code. Using `constexpr` and `consteval`, one can use the usual recursive factorial definition with the non-templated syntax.

上述两篇中，给出的example是非常好的，非常具有启发性。



## `constexpr`发展史

### C++11

引入了`constexpr`

### C++14

1、stackoverflow [Difference between `constexpr` and `const`](https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const) # [A](https://stackoverflow.com/a/14117121)

> As of C++14, the rules are more relaxed, what is allowed since then inside a constexpr function: `asm` declaration, a `goto` statement, a statement with a label other than `case` and `default`, try-block, the definition of a variable of non-literal type, definition of a variable of static or thread storage duration, the definition of a variable for which no initialization is performed.
> 2、wikipedia [C++14](https://en.wikipedia.org/wiki/C%2B%2B14) # [Relaxed constexpr restrictions](https://en.wikipedia.org/wiki/C%2B%2B14#Relaxed_constexpr_restrictions)

### C++17

`constexpr if` statements 

参见 `C++\Language-reference\Statements\Selection-statements\Constexpr-if`章节。



### C++20

cppreference [C++20](https://en.cppreference.com/w/cpp/20)

> further relaxed constexpr

具体参见原文。



## cppreference [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr)

### Explanation

The `constexpr` specifier declares that it is possible to evaluate the value of the **function** or **variable** at **compile time**.

> NOTE: 这段话是非常核心地所在

Such **variables** and **functions** can then be used where only compile time [constant expressions](constant_expression.html) are allowed (provided that **appropriate function arguments are given**).

> NOTE: 需要对最后一句话进行说明：它的意思是：constexpr function的入参必须是能够在compile-time获得的。



### `constexpr` variable

A **constexpr variable** must satisfy the following requirements:

> NOTE: 原文给出了**constexpr variable**的若干requirements，总的来说，这些requirements是为了保证这个variable的值是可以在compile time evaluate得到的，所以它需要排除所有的run time 概念。

#### Examples

galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)

```c++
template <bool X>
struct not
{
    static constexpr bool value {!X};
};
```



```c++
static constexpr not(bool x) 
{
    return !x;
}
```

### `constexpr` function



#### Example: `Factorial`

来源: 维基百科[Compile time function execution#Examples](https://en.wikipedia.org/wiki/Compile_time_function_execution)

C++11之前：

```c++
#include<iostream>
template <int N>
struct Factorial {
  enum { value = N * Factorial<N - 1>::value };
};

template <>
struct Factorial<0> {
  enum { value = 1 };
};

// Factorial<4>::value == 24
// Factorial<0>::value == 1
void Foo() {
  int x = Factorial<0>::value;  // == 1
  std::cout<<x<<std::endl;
  int y = Factorial<4>::value;  // == 24
  std::cout<<y<<std::endl;    
}

int main()
{
    Foo();
}
```

> 编译: `g++ test.cpp`，输出如下
>
> ```
> 1
> 24
> ```
>
> 

C++11

```c++
#include <cstdio>

constexpr int Factorial(int n) { return n ? (n * Factorial(n - 1)) : 1; }

constexpr int f10 = Factorial(10);

int main() {
  printf("%d\n", f10);
  return 0;
}
```

> 编译: `g++ -std=c++11 test.cpp`，输出如下
>
> ```
> 3628800
> ```
>
> 

C++14

```c++
// Iterative factorial at compile time.
constexpr int Factorial(int n)
{
	int result = 1;
	while (n > 1)
	{
		result *= n--;
	}
	return result;
}

int main()
{
	constexpr int f4 = Factorial(4);  // f4 == 24
}

```

> NOTE: 编译 `g++ -std=c++11 test.cpp` 
>
> ```c++
> test.cpp: 在函数‘constexpr int Factorial(int)’中:
> test.cpp:10:1: 错误：body of constexpr function ‘constexpr int Factorial(int)’ not a return-statement
>  }
>  ^
> test.cpp: 在函数‘int main()’中:
> test.cpp:14:32: 错误：‘constexpr int Factorial(int)’ called in a constant expression
>   constexpr int f4 = Factorial(4);  // f4 == 24
> ```
>
> 

#### Example

来源: stackoverflow [Using a `constexpr` array size function](https://stackoverflow.com/a/7439261)

```C++
#include "stddef.h"
#include "stdio.h"
#include <iostream>
#include <typeinfo>

typedef ptrdiff_t Size;

using Size = ptrdiff_t;

template<class Type, Size n>
constexpr auto n_items(Type (&)[n]) -> Size
{
	return n;
}

// Example 1
void foo()
{
	int const x[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
	constexpr Size n = n_items(x);
	int y[n] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
	for (auto&& i : y)
	{
		std::cout << i << std::endl;
	}
	// Using y here.
}

int main()
{
	foo();
}
// g++ --std=c++11 test.cpp

```

#### Example

来源: cppreference `sizeof...` operator `make_array`



### constexpr constructor

> NOTE: 在cppreference [Constructors and member initializer lists#Explanation](https://en.cppreference.com/w/cpp/language/constructor#Explanation) 中有这样的描述：
>
> The constructors with a `constexpr` specifier make their type a [*LiteralType*](https://en.cppreference.com/w/cpp/named_req/LiteralType).
>
> 这段话，说明了在C++中，如何构建literal type。

#### Example

参见`C++\Language-reference\Expressions\Constant-expressions\constexpr\Literal-type`。



## microsoft [constexpr (C++)](https://docs.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=vs-2019)



## TODO

### stackoverflow [When should you use constexpr capability in C++11?](https://stackoverflow.com/questions/4748083/when-should-you-use-constexpr-capability-in-c11)


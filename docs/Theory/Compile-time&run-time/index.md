# Compile-time and run-time

Compile-time 和 run-time 是非常重要的两个概念，有非常非常多的概念是由它们衍生而来的，compile-time对应的是static，run-time对应的是dynamic，在计算机科学中，名称中带有static或dynamic的概念，很多都与它们相关。在本文的后面章节会对此进行总结。

从它们的名称可以看出，这两个概念是根据time的维度来进行划分的。

C++和C是严格区分compile-time和run-time的language，这两门语言关于这个主题的讨论在`C-family-language\C-and-C++\Compile-time-and-run-time`章节中。

## wikipedia [Runtime (program lifecycle phase)](https://en.wikipedia.org/wiki/Runtime_(program_lifecycle_phase))

## wikipedia [Compile time](https://en.wikipedia.org/wiki/Compile_time)

龙书的如下章节涉及到了两者：

- 1.6 Programming Language Basics
- 2.8.3 Static Checking
- Chapter 7 Run-Time Environments
- 8.3 Addresses in the Target Code



## 基于Compile-time 和 run-time 的概念

### Scope

See also: [Scope (computer science)](https://en.wikipedia.org/wiki/Scope_(computer_science))

Static scoping 和 dynamic scoping 

在本书1.6 Programming Language Basics中讨论。



### Storage Allocation

logical address space 也可以根据此来进行划分，在本书Chapter 7 Run-Time Environments中讨论。

code area、static area是compile-time分配；

stack area、heap area是run-time分配；



#### [Static variable](https://en.wikipedia.org/wiki/Static_variable) VS [Automatic variable](https://en.wikipedia.org/wiki/Automatic_variable) VS dynamic variable

关于这个主题，可以参见C++的实现: `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration`。

### Type

在C++中，区分static type、dynamic type，参见: `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system`。

### Name binding

参见: wikipedia [Name binding](https://en.wikipedia.org/wiki/Name_binding) 

[Late binding](https://en.wikipedia.org/wiki/Late_binding)

Static binding参见：

[Early binding](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism#Early_binding)



### Runtime error VS compile-time error 

参见维基百科[Runtime error detection](https://en.wikipedia.org/wiki/Runtime_error_detection)



### Static and dynamic polymorphism

参见工程[programming-language](https://dengking.github.io/programming-language/)的[Polymorphism](https://dengking.github.io/programming-language/Theory/Programming-paradigm/Object-oriented-programming/Polymorphism/index.md)。



### Function execution

提到function execution，我们自然而然地想到它是在run time被执行的，但是其实在现代programming language中也是支持compile time function execution，典型的就是C++。

#### wikipedia [Compile time function execution](https://en.wikipedia.org/wiki/Compile_time_function_execution)

> NOTE: 是在学习C++11的[constexpr](https://en.wikipedia.org/wiki/C++11#constexpr_%E2%80%93_Generalized_constant_expressions)特性时发现达到这个内容，知道有这样的一个过程是非常重要的，对于理解C++的很多新特性都非常重要。现在想想C++语言的很多新特性都是在compile time完成的。
>
> 后续，我们将compile time function execution，称为compile time computation。

**Compile-time function execution** (or **compile time function evaluation**, or **general constant expressions**) is the ability of a [compiler](https://infogalactic.com/info/Compiler), that would normally compile a function to machine code and execute it at [run time](https://infogalactic.com/info/Run_time_(program_lifecycle_phase)), to execute the function at [compile time](https://infogalactic.com/info/Compile_time). This is possible if the arguments to the function are known at compile time, and the function does not make any reference to or attempt to modify any global state (is a [pure function](https://infogalactic.com/info/Pure_function)).

**Examples**

> NOTE: 原文的[Examples](https://en.wikipedia.org/wiki/Compile_time_function_execution#Examples)段给出的例子非常好地说明了c++11引入的 [generalized constant expressions](https://en.wikipedia.org/wiki/C%2B%2B11#constexpr_–_Generalized_constant_expressions)  特性

In earlier versions of [C++](https://infogalactic.com/info/C%2B%2B), [template metaprogramming](https://infogalactic.com/info/Template_metaprogramming) is often used to compute values at compile time, such as:

```C++
#include<iostream>
template<int N>
struct Factorial
{
	enum
	{
		value = N * Factorial<N - 1>::value
	};
};

template<> struct Factorial<0>
{
	enum
	{
		value = 1
	};
};

// Factorial<4>::value == 24
// Factorial<0>::value == 1
void foo()
{
	int x = Factorial<0>::value; // == 1
	std::cout << x << std::endl;
	int y = Factorial<4>::value; // == 24
	std::cout << y << std::endl;
}
int main()
{
	foo();
}

```



```C++
#include <stdio.h>
 
constexpr int factorial(int n) {
    return n ? (n * factorial(n - 1)) : 1;
}
 
constexpr int f10 = factorial(10);
 
int main() {
    printf("%d\n", f10);
    return 0;
}
// g++ --std=c++11 test.cpp

```


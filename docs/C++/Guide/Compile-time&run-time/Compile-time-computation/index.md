# Compile time execution

1、在`Theory\Compile-time-and-run-time`中，我们知道了compile time function execution的概念，本文对C++ compile time function execution进行总结。

2、C++的一个发展方向就是充分发挥compile-time computation。

## wikipedia [Compile time function execution](https://en.wikipedia.org/wiki/Compile-time_function_execution)

> NOTE: 
>
> 这篇文章在 `Theory\Compile-time-and-run-time` 中也收录了

**Compile-time function execution** (or **compile time function evaluation**, or **general constant expressions**) is the ability of a [compiler](https://en.wanweibaike.com/wiki-Compiler), that would normally compile a function to machine code and execute it at [run time](https://en.wanweibaike.com/wiki-Run_time_(program_lifecycle_phase)), to execute the function at [compile time](https://en.wanweibaike.com/wiki-Compile_time). This is possible if the arguments to the function are known at compile time, and the function does not make any reference to or attempt to modify any global state (is a [pure function](https://en.wanweibaike.com/wiki-Pure_function)).

## 方式

本文标题的含义是: 编译时时执行，它是C++的一个非常强大的特性，C++支持如下方式的compile time execution: 

### 1) Constant expression

参见: `C++\Language-reference\Expressions\Constant-expressions`。

### 2) TMP 

TMP metaprogram可以以如下方式来实现compile time function execution:

1) functional programming，参见: `C++\Language-reference\Template\TMP-and-FP`。

2) TMP metafunction，参见`C++\Idiom\TMP\Metafunction`，它其实不是严格意义上的function，但是它实现了类似于function的功能，所以将它放到这里。一些metafunction能够evaluation to constant at compile time，比如`std::is_same`。

3) recursive variadic function，参见`Recursive-variadic-function`章节。

### 3) Static initialization

1、参见`C++\Language-reference\Initialization`章节。

2、`Static-and-dynamic-initialization` 章节。

## 发展概述

C++的一个发展发现就是充分发挥compile-time computation。

| feature                                                      | 引入版本 | 章节                                                         | 说明  |
| ------------------------------------------------------------ | -------- | ------------------------------------------------------------ | ----- |
| `constexpr`                                                  | C++11    | 参见`C++\Language-reference\Expressions\Constant-expressions\constexpr\Constexpr-specifier` | 非TMP |
| variadic template                                            | C++11    |                                                              |       |
| `constexpr if`                                               | C++17    | 参见`C++\Language-reference\Statements\Selection-statements\Constexpr-if.md` | 非TMP |
| fold expression                                              | C++17    |                                                              | TMP   |
| [`consteval`](https://en.cppreference.com/w/cpp/language/consteval), [`constinit`](https://en.cppreference.com/w/cpp/language/constinit) | C++20    |                                                              |       |

Compile time function execution给予了C++非常强大的灵活性、安全性、高性能，下面对上面描述的几种方式进行总结: 

方式 1) `constexp` function 与 C++ program使用的是相同的language，定义、调用就像是普通C++ function，因此非常容易使用；它是更加接近于Python metaprogram的，因为Python metaprogram和Python program也是使用的相同的Python language。

方式 2) TMP metaprogram，其实可以看做是使用的另外一种language，它的定义、调用都有着比较特殊grammar，不是非常容易使用，需要进行特殊的学习；





## 为什么需要compile time execution？

1、Compile time function是C++ programmer编写来给compiler来执行的，它们可以看做是metaprogram，显然compile time function属于metaprogramming。Compile time execution给予了C++非常强大的灵活性、高性能。

> NOTE: 这说明C++的metaprogramming不仅仅是template metaprogramming，还包括`constexpr`。

2、stroustrup [Foundations of C++](https://www.stroustrup.com/ETAPS-corrected-draft.pdf) # Compile-time Computation

3、CppCoreGuidelines [P.5: Prefer compile-time checking to run-time checking](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p5-prefer-compile-time-checking-to-run-time-checking)

4、参见 `Use-static-info-to-optimize`

5、CppCoreGuidelines [Per.11: Move computation from run time to compile time](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#per11-move-computation-from-run-time-to-compile-time)

6、static initialization VS dynamic initialization

a、在 pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/) 中，进行了深入讨论

b、C++11 `constexpr`、C++20 `constinit`


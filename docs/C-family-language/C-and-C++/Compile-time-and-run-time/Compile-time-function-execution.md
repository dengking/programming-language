# Compile time function execution

在`Theory\Compile-time-and-run-time`中，我们知道了compile time function execution的概念，本文对C++ compile time function execution进行总结。



## 方式

本文标题的含义是: 编译时时函数执行，它是C++的一个非常强大的特性，C++支持如下方式的compile time function execution: 

### 1) Constant expression

参见: `C++\Language-reference\Expressions\Constant-expressions`。

### 2) TMP 

TMP metaprogram可以以如下方式来实现compile time function execution:

1) functional programming，参见: `C++\Language-reference\Template\TMP-and-FP`。

2) TMP metafunction，参见`C++\Idiom\TMP\Metafunction`，它其实不是严格意义上的function，但是它实现了类似于function的功能，所以将它放到这里。

### 3) Static initialization

参见`C++\Language-reference\Initialization`章节。



## 演进

C++的一个发展发现就是充分发挥compile-time computation。

| feature        | 引入版本 | 章节                                                         | 说明 |
| -------------- | -------- | ------------------------------------------------------------ | ---- |
| `constexpr`    | C++11    | 参见`C++\Language-reference\Expressions\Constant-expressions\constexpr\Constexpr-specifier.md` |      |
| `constexpr if` | C++17    | 参见`C++\Language-reference\Statements\Selection-statements\Constexpr-if.md` |      |
|                |          |                                                              |      |



Compile time function execution给予了C++非常强大的灵活性，下面对上面描述的几种方式进行总结: 

方式 1) `constexp` function 与 C++ program使用的是相同的language，定义、调用就像是普通C++ function，因此非常容易使用；它是更加接近于Python metaprogram的，因为Python metaprogram和Python program也是使用的相同的Python language。

方式 2) TMP metaprogram，其实可以看做是使用的另外一种language，它的定义、调用都有着比较特殊grammar，不是非常容易使用，需要进行特殊的学习；





## 为什么需要compile time function execution？

Compile time function是C++ programmer编写来给compiler来执行的，它们可以看做是metaprogram，显然compile time function属于metaprogramming。Compile time function execution给予了C++非常强大的灵活性、高性能



## Application




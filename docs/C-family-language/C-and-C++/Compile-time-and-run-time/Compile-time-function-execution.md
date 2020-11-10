# Compile time function execution

在`Theory\Compile-time-and-run-time`中，我们知道了compile time function execution的概念，本文对C++ compile time function execution进行总结。

## 为什么需要compile time function execution？

Compile time function是C++ programmer编写来给compiler来执行的，它们可以看做是metaprogram，显然compile time function属于metaprogramming。Compile time function execution给予了C++非常强大的灵活性。





## SUMMARY of C++ template metaprogramming

从meta programming的角度来看，c++ program可以分为

- c++ program
- metaprogram，metaprogram是compiler-time computation

在`Theory\Programming-paradigm\Metaprogramming\Metaprogramming.md`中对这种观点进行了总结。

在下面文章中使用了这个观点

microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)中有这样的描述

> To enable compilers as well as **C++ programs** and **metaprograms** to reason about the suitability of any given type for operations that depend on a particular memory layout, C++14 introduced three categories of simple classes and structs

文章galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)中对这个观点进行了非常深刻的介绍。



### Short Intro to C++ metaprogramming

在阅读文章galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)时，其中的Short Intro段对c++的metaprogramming进行了较好的总结：

> Looking at complex C++ meta programs, which appear seemlessly（无缝的） embedded into normal program code, is confusing at first. It is like looking at brain fuck code. That is, because the **meta programming syntax** is ugly and bloated. *Why is it so ugly?* The answer is simply, that C++ was not designed from the very beginning to contain a **meta programming language**. The language evolved, controlled by a consortium(联盟) which always tried to keep newer language standards backwards compatible to older ones. One day, people realized, that this growing little template engine is actually *Turing complete*. Soon, people started to write really crazy **meta programs** with it, which were able to elevate(提升) implementations of C++ libraries to a level of unprecedented **usability**, **versatility**(多功能性) and **elegance** (from the perspective of a user which has not seen the implementation). Data structures and functions can be implemented in a way, where they do magic of all kinds on any user provided type - with no overhead at **runtime**, because all the magic happens at compile time, resulting in completely fixed and optimized run time code.



`constexpr` function、trait等都是meta program的典型例子，在c++中，meta program是compile-time computation。我们将它们嵌入到normal program code中，来对program的生成进行控制，比如可以实现polymorphism，它们给予了C++非常强大的灵活性。



### Application of template metaprogramming

维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)中已经总结了部分Application of template metaprogramming，下面是一些补充内容：

#### SFINAE

- 控制overload resolution，即function template SFINAE，在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if\SFINAE\Function-template-SFINAE.md`中对此进行了分析
- 控制template specification，即class template SFINAE，在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if\SFINAE\Clasas-template-SFINAE.md`中对此进行了分析





### Idiom of template metaprogramming

在`C-family-language\C++\Idiom\Template-metaprogramming`中介绍了Idiom of template metaprogramming。









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





## Application


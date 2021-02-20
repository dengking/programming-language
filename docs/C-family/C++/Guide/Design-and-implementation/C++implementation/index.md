# C++ implementation

本章对C++ implementation进行整理，显然这会涉及非常多的内容。对C++而言，它的implementation主要是由compiler来完成。

## Compiler的局限

> NOTE: 首先介绍compiler的局限性

1、compiler是无法知道function是否会被multithread执行的

2、compiler是无法知道一个variable是否是shared variable，即会被multithread来执行read、write

3、compiler能够在多大程度上了解我们的program: 它不知道multithread。

C++11引入了memory model，给予了programmer解决上述问题的手段。

## Compiler optimization 

### Principle

compiler AS-IF。那它的基准是什么？基准是：value computation、side effect，显然基准是 programming language standard中进行描述的，那标准是如何来进行描述的呢？

C++ programming language是如何描述基准的，它留下了充足的optimization空间。

### 有哪些optimization？

compiler会进行哪些optimization

expression、statement

## Topics

### Compiler编译的流程

构建起一个完整的编译流程，对于理解C++非常重要。

| 步骤                        | 子步骤                                                | 章节          |
| --------------------------- | ----------------------------------------------------- | ------------- |
| Name lookup                 |                                                       | `Name-lookup` |
|                             | [ADL](https://en.cppreference.com/w/cpp/language/adl) |               |
| Template argument deduction |                                                       |               |
|                             |                                                       |               |
| Overload resolution         |                                                       |               |
|                             |                                                       |               |
| Access checking             |                                                       |               |

说明:

1、上述步骤，从上到下依次执行

2、上述table的绘制，参考了如下文章:

a、stackoverflow [Deletion of copy-ctor & copy-assignment - public, private or protected?](https://stackoverflow.com/questions/55205874/deletion-of-copy-ctor-copy-assignment-public-private-or-protected) # [A](https://stackoverflow.com/a/55206034)

> Name lookup and overload resolution happen before access checking. 

由此可知: Name lookup > Access checking。



b、cppreference [Function template](https://en.cppreference.com/w/cpp/language/function_template)

> Template argument deduction takes place after the function template [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (which may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)) and before [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

由此可知: Name lookup( [ADL](https://en.cppreference.com/w/cpp/language/adl)) > Template argument deduction  > overload resolution。



c、[GotW #100: Compilation Firewalls (Difficulty: 6/10)](https://herbsutter.com/gotw/_100/)

> C++ took the deliberate design decision to perform overload resolution before accessibility checking for safety reasons. For example, it was felt that changing the accessibility of a function from p*rivate* to *public* shouldn’t change the meaning of legal calling code.

它解释了为什么 Name lookup > Access checking。



### Implementation of polymorphism

参见 `C++\Guide\Polymorphism` 章节。
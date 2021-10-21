# Implementation of generic programming

本章总结、对比下面三种主流的programming language对generic programming的支持与实现。

C++ template、Python duck typing、Java/C# generics，这是这三门主流programming language实现**GP**的方式，它们之间是有着可比性的: 

它们都能够实现**GP**，下面是它们的对比: 

|                      | C++ template                                                 | Python duck typing                                           | Java/C# generics                                             |
| -------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Type requirement** | concept(C++20) <br>参见: cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) | type hint(Python 3.5) <br>参见: [`typing`](https://docs.python.org/3/library/typing.html#module-typing) — Support for type hints[¶](https://docs.python.org/3/library/typing.html#module-typing) | [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification) <br>参见: [Bounded Type Parameters](https://docs.oracle.com/javase/tutorial/java/generics/bounded.html) |
| **Static/dynamic**   | static                                                       | dynamic                                                      | 暂时不确定                                                   |
|                      | behavior-based                                               | behavior-based                                               | behavior-based                                               |
| **章节**             | `C++\Language-reference\Template`                            |                                                              | `./Type-requirement`                                         |

在generic programming中，非常重要的一点是type requirement，在`./Type-requirement`中会进行详细的说明。



### Python

Python通过duck type来实现generic programming，duck type是python的核心，python的很多standard library都是建立在这个机制上，python通过`able`来define the expectations of type。

### C++

`C++`通过template来实现generic programming，template是c++的核心，C++的很多standard library都是建立在此机制上，C++通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来define the expectations of type。

### Java

Java通过interface、generics来实现generic programming，Java的很多standard library都是建立在此机制上，Java通过generics来define the expectations of type。


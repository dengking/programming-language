# Template and duck typing and generics

C++ template、Python duck typing、Java/C# generics，这是这三门主流programming language实现**GP**的方式，它们之间是有着可比性的: 

它们都能够实现**GP**，下面是它们的对比: 

|                      | C++ template                                                 | Python duck typing                                           | Java/C# generics                                             |
| -------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Type requirement** | concept(C++20) <br>参见: cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) | type hint(Python 3.5) <br>参见: [`typing`](https://docs.python.org/3/library/typing.html#module-typing) — Support for type hints[¶](https://docs.python.org/3/library/typing.html#module-typing) | [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification) <br>参见: [Bounded Type Parameters](https://docs.oracle.com/javase/tutorial/java/generics/bounded.html) |
| Static/dynamic       | static                                                       | dynamic                                                      | 暂时不确定                                                   |
|                      | behavior-based                                               | behavior-based                                               | inheritance-based                                            |



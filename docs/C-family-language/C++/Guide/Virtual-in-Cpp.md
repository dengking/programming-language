# Virtual in Cpp

本文对C++中的“virtual”进行总结。

## virtual method and virtual data

在C++，polymorphism是行为（function），只有virtual method、virtual table，而不是值的。

那C++中能否实现virtual data呢？这个问题在What is “`virtual` data,” and how-can / why-would I use it in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)中讨论了，这个讨论收录在了`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics\Value-and-reference-semantics.md`中。



### Virtual method

本节标题的含义是：使用`virtual`来实现dynamic polymorphism。需要注意的是，在C++中，只有通过reference-semantic才能够实现dynamic polymorphism，关于这一点，在`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics`中进行了讨论。



|                         | 章节                                                         | 说明 |
| ----------------------- | ------------------------------------------------------------ | ---- |
| Virtual Constructor     | `C++\Idiom\OOP\Virtual-Constructor`                          |      |
| Virtual destructor      | `C++\Language-reference\Classes\Special-member-functions\Destructor\Virtual-destructor.md` |      |
| Virtual Friend-Function | `C++\Idiom\OOP\Virtual-Friend-Function`                      |      |
| Virtual function        | `C++\Language-reference\Classes\Subtype-polymorphism\Inheritance\Virtual-function` |      |
| Virtual data            |                                                              |      |
|                         |                                                              |      |

### Virtual data



## Virtual inheritance




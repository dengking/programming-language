# Static type and dynamic type



## pythonconquerstheuniverse [Static vs. dynamic typing of programming languages](https://pythonconquerstheuniverse.wordpress.com/2009/10/03/static-vs-dynamic-typing-of-programming-languages/)

*Updated 2010-10-20 — added a bit more information about Boo’s type inferencing.*
*Updated 2012-04-08 — This post is an appendix to a post comparing Java and Python. Some comments on this post are actually comments on that other post.*

There is widespread confusion or disagreement about the meanings of the words *static, dynamic, strong* and *weak* when used to describe the type systems of programming languages.

Here is a description of the way (or at least one of the ways) these terms are most commonly used.
| | |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| In a **statically typed language**, every variable name is bound both to a **type** (at **compile time**, by means of a data **declaration**) to an **object**.The binding to an object is optional — if a name is not bound to an object, the name is said to be *null.*Once a variable name has been bound to a type (that is, declared) it can be bound (via an assignment statement) only to objects of that type; it cannot ever be bound to an object of a different type. An attempt to bind the name to an object of the wrong type will raise a type exception. | In a **dynamically typed language**, every variable name is (unless it is null) bound only to an object.Names are bound to objects at **execution time** by means of assignment statements, and it is possible to bind a name to objects of different types during the execution of the program. |
| ![static_typing](https://pythonconquerstheuniverse.files.wordpress.com/2009/10/static_typing.png?w=640) | ![dynamic_typing](https://pythonconquerstheuniverse.files.wordpress.com/2009/10/dynamic_typing.png?w=640) |

Here is an example.



In a statically-typed language, the following sequence of statements (which binds an integer object, then a string object, to the name **employeeName**) is illegal. If **employeeName** had been declared to be an **int**, then the second statement would be illegal; if it had been declared to be a **String**, then the first statement would be illegal. But in a dynamically-typed language this sequence of statements is perfectly fine.

```
`employeeName = 9 employeeName = "Steve Ferg"`
```

Python is a **dynamically-typed language**. Java is a **statically-typed language**.







## [Is it possible to have a dynamically typed language without duck typing? [closed]](https://softwareengineering.stackexchange.com/questions/259941/is-it-possible-to-have-a-dynamically-typed-language-without-duck-typing)





## [static vs dynamic vs strong vs weak vs duck typing](https://www.koffeinfrei.org/2012/03/19/static-vs-dynamic-vs-strong-vs-weak-vs-duck-typing/)



思考 : strong type和duck type是否冲突？

<https://devopedia.org/duck-typing>



## C++ static type and runtime type

C++是典型的区分static type和runtime type的语言。

在文章[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)中，提出了static type和dynamic type的概念。
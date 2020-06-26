# 关于本章

本章描述c++中关于exception的一些idiom

| idiom                                               | example | summary                                                      |
| --------------------------------------------------- | ------- | ------------------------------------------------------------ |
| [Polymorphic-Exception](./Polymorphic-Exception.md) |         | exception class之间存在着inheritance关系，那如何polymorphism throw exception并且polymorphism catch exception呢？这是这个idiom所解决的问题；`throw *this`即由exception object来throw，类似于visitor pattern。 |
| [Construction-Tracker](./Construction-Tracker.md)   |         | 这个idiom使用的场景是比较狭隘的：when initialization of two or more objects in the constructor's **initialization list** can throw the same exception type；它使用一个**counter** 来track construction of objects。 |


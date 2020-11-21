# Behavior

在`Theory\Programming-paradigm\Object-oriented-programming\index.md`的“Behavior and data member”章节中已经说明了:

> 后续为了描述的统一性，使用**behavior**来表示method、function member

之所以采用这个词语是基于如下原因:

| 文章                                                         |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `Theory\Programming-paradigm\Object-oriented-programming\Thinking-in-java\Introduction-to-Objects.md` | TODO: 将object看做是computer，"behavior"                     |
| `C++\Language-reference\Classes\Subtype-polymorphism\index.md` | 在C++，dynamic polymorphism是behavior（function），只有virtual method（virtual table），而不是data/value的。 |
| `Theory\Design-pattern\OOP-design-pattern\Behavioral-pattern\index.md` |                                                              |
| wikipedia [Behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping) |                                                              |
| `Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Subtyping-polymorphism\index.md` | "Subtyping polymorphism is based on behavior"                |
| Wikipedia method overriding                                  | override的是method，method是behavior                         |

下面几个词在OOP中是经常出现的，我觉得它们的有着相似的内涵，且它们都与behavior有关:

|           | 简介                                                   | 链接                                                         |
| --------- | ------------------------------------------------------ | ------------------------------------------------------------ |
| protocol  | 约定好protocol，其实就是约定好interface                | [Protocol (object-oriented programming)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)) |
| contract  | design by contract，其实很大一部分内容是约定好protocol | wikipedia [Design by contract](https://en.wikipedia.org/wiki/Design_by_contract) <br> |
| interface | interface是对behavior的formal description              |                                                              |



## Behavior and interface

前面已经提及了interface，本节描述它和behavior的关系。

### Interface in OOP

Interface在OOP中占据着核心地位，关于interface，参见:

- cs.utah.edu [Interfaces](https://www.cs.utah.edu/~germain/PPS/Topics/interfaces.html) 
- stackoverflow [What is the definition of “interface” in object oriented programming](https://stackoverflow.com/questions/2866987/what-is-the-definition-of-interface-in-object-oriented-programming)

### "program to interface" 

参见: `Theory\Design-pattern\OOP-design-pattern\Principle\index.md`



### Interface is formal description of behavior

本节标题的含义是interface是对behavior的formal description。



### The interface principle

这是[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)的文章[What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)中提出的一个观点，虽然它描述的是C++世界，但是我觉得它对interface的阐释是比较好的，这篇文章收录在了`C++\Language-reference\Classes\The-interface-principle.md`。
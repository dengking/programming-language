# Behavior-based and interface



## 为什么采用"behavior"这个词?

本节解释为什么采用"behavior"这个词语。

在`Theory\Programming-paradigm\Object-oriented-programming\index.md`的“Behavior and data member”章节中已经说明了:

> 后续为了描述的统一性，使用**behavior**来表示method、function member

之所以采用这个词语是基于如下原因:

| 文章                                                         |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `Theory\Programming-paradigm\Object-oriented-programming\Thinking-in-java\Introduction-to-Objects.md` | TODO: 将object看做是computer，"behavior"                     |
| `C++\Language-reference\Classes\Subtype-polymorphism\index.md` | 在C++，dynamic polymorphism是behavior（function），只有virtual method（virtual table），而不是data/value的。 |
| `Theory\Design-pattern\OOP-design-pattern\Behavioral-pattern\index.md` | 可以看到，它使用的是behavior这个词                           |
| wikipedia [Behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping) |                                                              |
| `Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Subtyping-polymorphism\index.md` | "Subtyping polymorphism is based on behavior"                |
| Wikipedia method overriding                                  | override的是method，method是behavior                         |



## Behavior and interface

前面已经提及了interface，本节描述它和behavior的关系。

### The interface principle

这是[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)的文章[What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)中提出的一个观点，虽然它描述的是C++世界，但是我觉得它对interface的阐释是比较好的，这篇文章收录在了`gotw-What's-In-a-Class-The-Interface-Principle` 章节。



### Interface is a description of behavior

本节标题的含义是interface是对behavior的description，或者说实现: 

1、使用interface来实现behavior，interface可以是explicit的，也可以是implicit

2、design by contact



### "program to interface" 

参见: `Theory\Design-pattern\OOP-design-pattern\Principle\index.md`

#### Interface in OOP

Interface在OOP中占据着核心地位，关于interface，参见:

1、cs.utah.edu [Interfaces](https://www.cs.utah.edu/~germain/PPS/Topics/interfaces.html) 

2、stackoverflow [What is the definition of “interface” in object oriented programming](https://stackoverflow.com/questions/2866987/what-is-the-definition-of-interface-in-object-oriented-programming)





## Behavior-based in programming paradigm

### GP

在Generic programming中，提出了"Type requirement is more about behavior-based"，参见 `Theory\Programming-paradigm\Generic-programming\Implementation\Type-requirement` 章节。

### OOP

在object oriented programming: "Polymorphism is more about behavior-based"，参见 `Theory\Programming-paradigm\Common\Abstraction-and-polymorphism\Polymorphism\Implementation` 章节。


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



### Interface is a description of behavior

本节标题的含义是interface是对behavior的description，或者说实现: 

1、使用interface来实现behavior，interface可以是explicit的，也可以是implicit

2、design by contact



## Behavior-based in programming paradigm

### GP

在Generic programming中，提出了"Type requirement is more about behavior-based"，参见 `Theory\Programming-paradigm\Generic-programming\Implementation\Type-requirement` 章节。

### OOP

在object oriented programming: "Polymorphism is more about behavior-based"，参见 `Theory\Programming-paradigm\Common\Abstraction-and-polymorphism\Polymorphism\Implementation` 章节。


# 关于本章

本章的标题是"assemble"，中文意思是"组装、装配、聚集"。它是源自于wikipedia [Object composition](https://en.wikipedia.org/wiki/Object_composition)中的这段话:

> [Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) is based on [objects](https://en.wikipedia.org/wiki/Object_(computer_science)) to [encapsulate](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) data and behavior. It uses two main techniques for assembling and composing functionality into more complex ones, **sub-typing** and **object composition**.[[2\]](https://en.wikipedia.org/wiki/Object_composition#cite_note-2) 

显然，本章所要探讨的话题是: OOP中的能够实现“assembling and composing functionality into more complex ones”的technique；



上面这段话中，已经告诉了我们两个答案，并且这两个是当前OOP中的主流的technique，除此之外，还有一些其他的technique，下面是本章将要讨论的technique:

| technique   | 简介                                                         | 章节                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| inheritance | "继承", 又称为"sub-typing"                                   | `Theory\Programming-paradigm\Object-oriented-programming\Assemble\Inheritance` |
| composition | "组合"                                                       | `Theory\Programming-paradigm\Object-oriented-programming\Assemble\Composition` |
| mixin       | "混入"，其实它是基于inheritance的，不过有它的独特之处，<br>所以将它单独进行描述 |                                                              |

除了上面列出的这些technique外，不断地涌现出了新的technique；

通过上述technique，我们可以实现assemble，进而实现了code reuse；

## inheritance and composition

这是目前主流的technique；

### inheritance VS composition

虽然两种都能够实现assemble，但是从design的角度来看，它们是有着差异的，在`Composition-over-inheritance`中对它们进行了对比。



### delegation and inheritance

在维基百科[Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)中有这样的描述: 

> [Delegation](https://en.wikipedia.org/wiki/Delegation_(programming)) is another language feature that can be used as an alternative to inheritance.
>

在wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)中，也介绍了使用delegation来实现inheritance的内容。

在`Theory\Programming-paradigm\Object-oriented-programming\Assemble\Composition\index.md`中将具体介绍如何来实现这一点。



### Combination explode

对于combination explode，composition有着天然的优势，典型例子:

1、decorator pattern中的: pizza + topping
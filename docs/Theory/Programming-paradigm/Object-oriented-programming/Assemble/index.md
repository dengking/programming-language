# 关于本章

本章的标题是"assemble"，中文意思是"组装、装配、聚集"。它是源自于wikipedia [Object composition](https://en.wikipedia.org/wiki/Object_composition)中的这段话:

> [Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) is based on [objects](https://en.wikipedia.org/wiki/Object_(computer_science)) to [encapsulate](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) data and behavior. It uses two main techniques for assembling and composing functionality into more complex ones, sub-typing and object composition.[[2\]](https://en.wikipedia.org/wiki/Object_composition#cite_note-2) 

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

虽然两种都能够实现assemble，但是从design的角度来看，它们是有着差异的，在`Theory\Design-pattern\OOP-design-pattern\Principle\Composition-over-inheritance`中对它们进行了对比。



### delegation and inheritance





## TODO 基于“结构化思维”来分析assemble

> 这段标题加一个TODO意在体现，这部分内容没有整理好，需要进一步整理

本章讨论的“assemble”对应了一种relation，这就提示了我们需要使用“结构化思维”来进行分析OOP相关的问题。

### 基于inheritance关系来思考

OOP中的inheritance关系是典型的**nesting关系**，因此它能够呈现出hierarchy structure（如果仅仅允许single-inheritance的话，则是tree），这就是我们常说的**class hierarchy** ，这是典型的以**结构化思维**来进行思考，基于class hierarchy structure来思考OOP中的内容，能够让我们快速的理解OOP中的许多内容，具体分析如下：

> TODO 需要补充一些关于class hierarchy的文章的链接



#### Hierarchy-based implementation

本节标题的含义是：基于hierarchy的实现，OOP中的很多内容都是基于hierarchy来实现的：

##### Initialization  and deinitialization  order

|                                            | 说明                                                       |
| ------------------------------------------ | ---------------------------------------------------------- |
| initialization order（construction order） | object的构造，涉及基类的构造，这就涉及initialization order |
| deinitialization/destruction order         | object                                                     |

> TODO 需要添加C++中的的链接



##### Implementation of polymorphism

function find order：沿着hierarchy进行搜索。

需要结合Python、C++的implementation来进行说明：

- C++ vtable
- Python C3 serialization

> TODO 需要添加链接
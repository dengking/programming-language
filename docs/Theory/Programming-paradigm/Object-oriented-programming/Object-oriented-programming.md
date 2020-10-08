# Object-oriented programming

本文主要基于维基百科[Object-oriented programming。如果有一定的编程经验的话，可以较好理解，如果没有OOP的经验，可以阅读下一章[Thinking-in-java](./Class-based-OOP/Thinking-in-java/index.md)，其中对OOP理论有非常好的描述。OOP有如下流派：

| 流派                                                         | example                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Class-based programming](https://en.wikipedia.org/wiki/Class-based_programming) | 典型代表有C++、Java、Python                                  |
| [Prototype-based programming](https://en.wikipedia.org/wiki/Prototype-based_programming) | 典型代表就是 [JavaScript](https://en.wikipedia.org/wiki/JavaScript) |



## 维基百科[Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)

**Object-oriented programming** (**OOP**) is a [programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm) based on the concept of "[objects](https://en.wikipedia.org/wiki/Object_(computer_science))", which can contain [data](https://en.wikipedia.org/wiki/Data), in the form of [fields](https://en.wikipedia.org/wiki/Field_(computer_science)) (often known as *attributes* or *properties*), and code, in the form of procedures (often known as *[methods](https://en.wikipedia.org/wiki/Method_(computer_science))*). A feature of objects is an object's procedures that can access and often modify the data fields of the object with which they are associated (objects have a notion of "[this](https://en.wikipedia.org/wiki/This_(computer_programming))" or "self"). In OOP, computer programs are designed by making them out of objects that interact with one another.



### [Features](https://en.wikipedia.org/wiki/Object-oriented_programming#Features)

> NOTE: 原文的本段说明非常多我们平时所听到的与OOP相关的概念。我将与class-base OOP相关的内容放到了[Class-based OOP](./Class-based-OOP/Class-based-OOP.md)中。

#### [Shared with non-OOP predecessor languages](https://en.wikipedia.org/wiki/Object-oriented_programming#Shared_with_non-OOP_predecessor_languages)

#### [Objects and classes](https://en.wikipedia.org/wiki/Object-oriented_programming#Objects_and_classes)

> NOTE: 在[Class-based OOP](./Class-based-OOP/Class-based-OOP.md)中会对此进行介绍。

#### [Class-based vs prototype-based](https://en.wikipedia.org/wiki/Object-oriented_programming#Class-based_vs_prototype-based)



#### [Dynamic dispatch/message passing](https://en.wikipedia.org/wiki/Object-oriented_programming#Dynamic_dispatch/message_passing)

> NOTE: 这是OOP中非常主要的一个概念

It is the responsibility of the object, not any external code, to select the procedural code to execute in response to a method call, typically by looking up the method at **run time** in a table associated with the object. This feature is known as [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch), and distinguishes an object from an [abstract data type](https://en.wikipedia.org/wiki/Abstract_data_type) (or module), which has a **fixed (static)** implementation of the operations for all instances. If the **call variability**（可变性） relies on more than the single type of the object on which it is called (i.e. at least one other parameter object is involved in the method choice), one speaks of [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch).

A method call is also known as *[message passing](https://en.wikipedia.org/wiki/Message_passing)*. It is conceptualized as a message (the name of the method and its input parameters) being passed to the object for dispatch.

#### [Encapsulation](https://en.wikipedia.org/wiki/Object-oriented_programming#Encapsulation)



#### [Composition, inheritance, and delegation](https://en.wikipedia.org/wiki/Object-oriented_programming#Composition,_inheritance,_and_delegation)

> NOTE: 在`Theory\Programming-paradigm\Object-oriented-programming\Assemble`章节，对这部分内容进行了专门的介绍。

Objects can contain other objects in their instance variables; this is known as [object composition](https://en.wikipedia.org/wiki/Object_composition). For example, an object in the `Employee` class might contain (either directly or through a pointer) an object in the `Address` class, in addition to its own instance variables like "first_name" and "position". Object composition is used to represent "**has-a**" relationships.

Languages that support classes almost always support [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)). This allows classes to be arranged in a hierarchy that represents "**is-a-type-of**" relationships. 

Subclasses can override the methods defined by superclasses.



[Delegation](https://en.wikipedia.org/wiki/Delegation_(programming)) is another language feature that can be used as an alternative to inheritance.



#### [Polymorphism](https://en.wikipedia.org/wiki/Object-oriented_programming#Polymorphism)

> NOTE: 在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism`章节将对它进行详细说明。

#### [Open recursion](https://en.wikipedia.org/wiki/Object-oriented_programming#Open_recursion)



### [OOP languages](https://en.wikipedia.org/wiki/Object-oriented_programming#OOP_languages)

> NOTE: 我们常常会听到“纯面向对象”、“部分支持明细对象”等说法，原文的这一段对此进行了说明。

Concerning the degree of **object orientation**, the following distinctions can be made:

- Languages called "pure" OO languages, because everything in them is treated consistently as an **object**, from primitives such as characters and punctuation, all the way up to whole classes, prototypes, blocks, modules, etc. They were designed specifically to facilitate, even enforce, OO methods. Examples: [Python](https://en.wikipedia.org/wiki/Python_(programming_language))
- Languages designed mainly for OO programming, but with some procedural elements. Examples: [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B)



## Class-based vs prototype-based

关于Class-based vs prototype-based，在维基百科[Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)的[Class-based vs prototype-based](https://en.wikipedia.org/wiki/Object-oriented_programming#Class-based_vs_prototype-based)段中有总结。下面是我根据自己的一些经验所总结的：

### 从type system的角度来进行对比

OOP的class-based流派，每个**class**对应一种**类型**，往往是**强类型**语言使用，比如c++，Java等。

OOP的prototype-based流派，并没有class的概念，往往是weak类型语言使用，比如JavaScript。

## Behavior and data member

后续为了描述的统一性，使用**behavior**来表示method、function member，使用data member来表示“成员变量”；

在`Theory\Programming-paradigm\Object-oriented-programming\Behaivor.md`中对behavior进行了总结。



## OOP and “function and data model”

在工程hardware的`Computer-architecture\Stored-program-computer.md`中，我们介绍了Function and data model。OOP的object是符合“function and data model”的，下面描述了它们的对应关系: 

| OOP的object     | function and data model |
| --------------- | ----------------------- |
| function member | function                |
| data member     | data                    |

关于C++ OOP and “function and data model”，参见`C-family-language\C++\Language-reference\Basic-concept\index.md`。


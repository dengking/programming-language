# Class-based OOP

我们已经知道了class-based OOP是OOP的一个流派，本文将对class-based OOP进行详细分析。

## 维基百科[Class-based programming](https://en.wikipedia.org/wiki/Class-based_programming)



> 原文在很多下面这段话我觉得非常具有启发性

Object-oriented programming is more than just classes and objects; it's a whole programming paradigm based around *objects* (data structures) that contain data fields and methods. It is essential to understand this; using classes to organize a bunch of unrelated methods together is not object orientation.

Junade Ali, *Mastering PHP Design Patterns*



### Encapsulation



### Inheritance





## 三大特性

我们平时所说的“面向对象编程”的三大特性，更加严格地说应该是class-based OOP的三大特性。这三大特性貌似并没有形式化地定义，应该是约定俗成的一种说法。

本段参考的内容有：

- [Features of Object-Oriented Programming Languages](http://www.cs.mun.ca/~donald/bsc/node12.html)
- [object-oriented programming](https://www.pcmag.com/encyclopedia/term/object-oriented-programming)



## 概念

> NOTE: 本段内容截取自维基百科[Object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)的[Objects and classes](https://en.wikipedia.org/wiki/Object-oriented_programming#Objects_and_classes)段。

Languages that support object-oriented programming (OOP) typically use [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) for code reuse and extensibility in the form of either [classes](https://en.wikipedia.org/wiki/Class-based_programming) or [prototypes](https://en.wikipedia.org/wiki/Prototype-based_programming). Those that use classes support two main concepts:

- [Classes](https://en.wikipedia.org/wiki/Class_(computer_science)) – the definitions for the data format and available procedures for a given type or class of object; may also contain data and procedures (known as class methods) themselves, i.e. classes contain the data members and member functions
- [Objects](https://en.wikipedia.org/wiki/Object_(computer_science)) – instances of classes

Objects sometimes correspond to things found in the real world. For example, a graphics program may have objects such as "circle", "square", "menu". An online shopping system might have objects such as "shopping cart", "customer", and "product".[[7\]](https://en.wikipedia.org/wiki/Object-oriented_programming#cite_note-7) Sometimes objects represent more abstract entities, like an object that represents an open file, or an object that provides the service of translating measurements from U.S. customary to metric.

Object-oriented programming is more than just classes and objects; it's a whole programming paradigm based around [*sic*] *objects* (data structures) that contain data fields and methods. It is essential to understand this; using classes to organize a bunch of unrelated methods together is not object orientation.

Junade Ali, *Mastering PHP Design Patterns*[[8\]](https://en.wikipedia.org/wiki/Object-oriented_programming#cite_note-8)

Each object is said to be an [instance](https://en.wikipedia.org/wiki/Instance_(computer_science)) of a particular class (for example, an object with its name field set to "Mary" might be an instance of class Employee). Procedures in object-oriented programming are known as [methods](https://en.wikipedia.org/wiki/Method_(computer_science)); variables are also known as [fields](https://en.wikipedia.org/wiki/Field_(computer_science)), members, attributes, or properties. This leads to the following terms:

- [Class variables](https://en.wikipedia.org/wiki/Class_variable) – belong to the *class as a whole*; there is only one copy of each one
- [Instance variables](https://en.wikipedia.org/wiki/Instance_variable) or attributes – data that belongs to individual *objects*; every object has its own copy of each one
- [Member variables](https://en.wikipedia.org/wiki/Member_variable) – refers to both the class and instance variables that are defined by a particular class
- Class methods – belong to the *class as a whole* and have access only to class variables and inputs from the procedure call
- Instance methods – belong to *individual objects*, and have access to instance variables for the specific object they are called on, inputs, and class variables

Objects are accessed somewhat like variables with complex internal structure, and in many languages are effectively [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)), serving as actual references to a single instance of said object in memory within a heap or stack. They provide a layer of [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) which can be used to separate internal from external code. External code can use an object by calling a specific instance method with a certain set of input parameters, read an instance variable, or write to an instance variable. Objects are created by calling a special type of method in the class known as a [constructor](https://en.wikipedia.org/wiki/Constructor_(object-oriented_programming)). A program may create many instances of the same class as it runs, which operate independently. This is an easy way for the same procedures to be used on different sets of data.

Object-oriented programming that uses classes is sometimes called [class-based programming](https://en.wikipedia.org/wiki/Class-based_programming), while [prototype-based programming](https://en.wikipedia.org/wiki/Prototype-based_programming) does not typically use classes. As a result, a significantly different yet analogous terminology is used to define the concepts of *object* and *instance*.

In some languages classes and objects can be composed using other concepts like [traits](https://en.wikipedia.org/wiki/Trait_(computer_programming)) and [mixins](https://en.wikipedia.org/wiki/Mixin).
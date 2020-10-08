# OOP design principle

在进入具体的design pattern之前，有必要对oop中的design principle进行阐述，因为各种的design pattern其实都是贯彻着这些思想的，经典书籍[Design Patterns: Elements of Reusable Object-Oriented Software](https://en.wikipedia.org/wiki/Design_Patterns)对其有着非常好的总结，所以本文的很多内容都是从其中摘录的。

## wikipedia [Design Patterns: Elements of Reusable Object-Oriented Software](https://en.wikipedia.org/wiki/Design_Patterns)

> 开山之作。

### Introduction, Chapter 1

Chapter 1 is a discussion of [object-oriented](https://en.wikipedia.org/wiki/Object-oriented) design techniques, based on the authors' experience, which they believe would lead to good object-oriented software design, including:

1) "Program to an 'interface', not an '**implementation'**." (Gang of Four 1995:18)

> NOTE: interface是abstraction，implementation是concrete。关于abstraction、concrete，参见文章[Abstraction](https://dengking.github.io/Post/Abstraction/Abstraction/)，它是OOP的核心思想，SOLID中的“D”即[Dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)表达的是和这相同的含义。
>
> 它是实现[Loose coupling](https://en.wikipedia.org/wiki/Loose_coupling)的关键实现。

2) [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance): "Favor '[object composition](https://en.wikipedia.org/wiki/Object_composition)' over '[class inheritance](https://en.wikipedia.org/wiki/Inheritance_(computer_science))'." (Gang of Four 1995:20)



> NOTE: 下面的标题原文中并没有，是我添加上去的，便于阅读

### Program to an 'interface', not an 'implementation'

The authors claim the following as advantages of [interfaces](https://en.wikipedia.org/wiki/Interface_(computer_science)) over implementation:

1) clients remain unaware of the specific types of objects they use, as long as the object adheres to the interface

2) clients remain unaware of the classes that implement these objects; clients only know about the abstract class(es) defining the interface

Use of an interface also leads to [dynamic binding](https://en.wikipedia.org/wiki/Dynamic_dispatch) and [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_in_object-oriented_programming), which are central features of object-oriented programming.

> NOTE: OOP的核心特性：polymorphism。

> NOTE: 在`Theory\Programming-paradigm\Object-oriented-programming\Behaivor.md`中，对interface进行了描述。

### Composition over inheritance

> NOTE: 
>
> 这将在`Theory\Design-pattern\OOP-design-pattern\Principle\Composition-over-inheritance`中进行详细说明。
>
> 在`Theory\Programming-paradigm\Object-oriented-programming\Assemble`中，对Composition、inheritance进行了描述。



### Parameterized types

The authors also discuss so-called **parameterized types**, which are also known as [generics](https://en.wikipedia.org/wiki/Generic_programming) (Ada, Eiffel, [Java](https://en.wikipedia.org/wiki/Generics_in_Java), C#, VB.NET, and Delphi) or templates (C++). These allow any type to be defined without specifying all the other types it uses—the unspecified types are supplied as 'parameters' at the point of use.

The authors admit that delegation and parameterization are very powerful but add a warning:

### Aggregation

The authors further distinguish between '[Aggregation](https://en.wikipedia.org/wiki/Object_composition#Aggregation)', where one object 'has' or 'is part of' another object (implying that an aggregate object and its owner have identical lifetimes) and **acquaintance**, where one object merely 'knows of' another object. Sometimes acquaintance is called 'association' or the 'using' relationship. Acquaintance objects may request operations of each other, but they aren't responsible for each other. Acquaintance is a weaker relationship than aggregation and suggests much [looser coupling](https://en.wikipedia.org/wiki/Loose_coupling) between objects, which can often be desirable for maximum maintainability in a design.



## [Design by contract](https://en.wikipedia.org/wiki/Design_by_contract) and [Interface-based programming](https://en.wikipedia.org/wiki/Interface-based_programming)

两者其实本质上都在描述相同的内容，面向抽象，而不是面向具体。抽象是科学的思考方式，其实，这一段的描述，需要从对抽象的描述开始：解决问题，我们往往是先建立起抽象模型，这个抽象模型来解决具体的问题。

dynamic dispatch是连接抽象与具体的桥梁。

## TODO: wikipedia [Object-oriented design](https://en.wikipedia.org/wiki/Object-oriented_design)








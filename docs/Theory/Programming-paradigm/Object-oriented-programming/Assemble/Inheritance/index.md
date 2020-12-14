# Inheritance



## wikipedia [Inheritance (object-oriented programming)](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming))

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), **inheritance** is the mechanism of basing an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) or [class](https://en.wikipedia.org/wiki/Class_(computer_programming)) upon another object ([prototype-based inheritance](https://en.wikipedia.org/wiki/Prototype-based_programming)) or class ([class-based inheritance](https://en.wikipedia.org/wiki/Class-based_programming)), retaining similar implementation. 

> NOTE: 两种流派的OOP中都支持inheritance。

Also defined as deriving(派生) new classes ([sub classes](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#Subclasses_and_superclasses)) from existing ones such as **super class** or [base class](https://en.wikipedia.org/wiki/Fragile_base_class) and then forming them into a hierarchy of classes. 

In most **class-based object-oriented languages**, an object created through inheritance, a "child object", acquires(获得) all the properties and behaviors of the "parent object" , with the exception of: [constructors](https://en.wikipedia.org/wiki/Constructor_(object-oriented_programming)), destructor, [overloaded operators](https://en.wikipedia.org/wiki/Operator_overloading) and [friend functions](https://en.wikipedia.org/wiki/Friend_function) of the base class. 

**Inheritance** allows programmers to create classes that are built upon existing classes,[[1\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-1) to specify a new implementation while maintaining the same behaviors ([realizing an interface](https://en.wikipedia.org/wiki/Class_diagram#Realization/Implementation)), to reuse code and to independently extend original software via public classes and interfaces. The relationships of objects or classes through inheritance give rise to a [directed graph](https://en.wikipedia.org/wiki/Directed_graph).

Inheritance was invented in 1969 for [Simula](https://en.wikipedia.org/wiki/Simula)[[2\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-2) and is now used throughout many object-oriented programming languages such as [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), C++ or Python.

An **inherited class** is called a **subclass** of its parent class or super class. The term "inheritance" is loosely used for both **class-based** and **prototype-based** programming, but in narrow use the term is reserved for class-based programming (one class *inherits from* another), with the corresponding technique in prototype-based programming being instead called *[delegation](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming))* (one object *delegates to* another).

> NOTE: inheritance and delegation:
>
> 是可以通过delegation来实现inheritance类似的效果的，参见 `Theory\Programming-paradigm\Object-oriented-programming\Assemble\Composition` 章节。

Inheritance should not be confused with [subtyping](https://en.wikipedia.org/wiki/Subtyping).[[3\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-4) In some languages inheritance and subtyping agree,[[a\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-5) whereas in others they differ; in general, subtyping establishes an [is-a](https://en.wikipedia.org/wiki/Is-a) relationship, whereas inheritance only reuses implementation and establishes a **syntactic relationship**, not necessarily a **semantic relationship** (inheritance does not ensure [behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping)). To distinguish these concepts, subtyping is also known as *interface inheritance*, whereas inheritance as defined here is known as *implementation inheritance* or *code inheritance*.[[5\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-Mikhajlov-6) Still, inheritance is a commonly used mechanism for establishing subtype relationships.[[6\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-7)

> NOTE: 最能体现 inheritance 和 subtyping 差异的是 mixin。

Inheritance is contrasted with [object composition](https://en.wikipedia.org/wiki/Object_composition), where one object *contains* another object (or objects of one class contain objects of another class); see [composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance). Composition implements a [has-a](https://en.wikipedia.org/wiki/Has-a) relationship, in contrast to the is-a relationship of subtyping.

> NOTE: 关于 composition over inheritance，参见 `Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Principle\Composition-over-inheritance` 章节。

### Types

## wikipedia Inheritance semantics / [Behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping)


# Software design pattern

“software 的sing pattern”即软件设计模式，是前人总结的解决特定问题的最佳实践（[best practice](https://en.wikipedia.org/wiki/Best_practice)）。我们平时提及design pattern的时候，往往第一反应是[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) design pattern，其实design pattern不仅限于[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) programming，在各个领域中，都能够总结出[domain-specific patterns](https://en.wikipedia.org/wiki/Software_design_pattern#Domain-specific_patterns)。



## 维基百科[Software design pattern](https://en.wikipedia.org/wiki/Software_design_pattern)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), a **software design pattern** is a general, [reusable](https://en.wikipedia.org/wiki/Reusability) solution to a commonly occurring problem within a given context in [software design](https://en.wikipedia.org/wiki/Software_design). It is not a finished design that can be transformed directly into [source](https://en.wikipedia.org/wiki/Source_code) or [machine code](https://en.wikipedia.org/wiki/Machine_code). It is a description or template for how to solve a problem that can be used in many different situations. Design patterns are formalized [best practices](https://en.wikipedia.org/wiki/Best_practice) that the programmer can use to solve common problems when designing an application or system.

[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) design patterns typically show relationships and [interactions](https://en.wikipedia.org/wiki/Interaction) between [classes](https://en.wikipedia.org/wiki/Class_(computer_science)) or [objects](https://en.wikipedia.org/wiki/Object_(computer_science)), without specifying the final application classes or objects that are involved. Patterns that imply mutable（可变的） state may be unsuited for [functional programming](https://en.wikipedia.org/wiki/Functional_programming) languages, some patterns can be rendered unnecessary in languages that have built-in support for solving the problem they are trying to solve, and object-oriented patterns are not necessarily suitable for non-object-oriented languages.



## Practice

Design patterns can speed up the development process by providing tested, proven development paradigms. Effective software design requires considering issues that may not become visible until later in the implementation. Freshly written code can often have hidden subtle issues that take time to be detected, issues that sometimes can cause major problems down the road. Reusing design patterns helps to prevent such subtle issues , and it also improves code readability for coders and architects who are familiar with the patterns.

> NOTE: 使用design pattern的优势。
>
> software design和[城市规划](https://baike.baidu.com/item/%E5%9F%8E%E5%B8%82%E8%A7%84%E5%88%92/491164?fr=aladdin)有点类似，都需要以发展的眼光来进行规划。

In order to achieve flexibility, design patterns usually introduce additional levels of [indirection](https://en.wikipedia.org/wiki/Indirection), which in some cases may complicate the resulting designs and hurt application performance.

> NOTE: 参见文章[分层](https://dengking.github.io/Post/Abstraction/Abstraction-and-architecture-and-layer/)。

By definition, a pattern must be programmed anew（重新，再次） into each application that uses it. Since some authors see this as a step backward from [software reuse](https://en.wikipedia.org/wiki/Software_reuse) as provided by [components](https://en.wikipedia.org/wiki/Software_componentry), researchers have worked to turn patterns into components. Meyer and Arnout were able to provide full or partial componentization of two-thirds of the patterns they attempted. 

Software design techniques are difficult to apply to a broader range of problems. Design patterns provide general solutions, [documented](https://en.wikipedia.org/wiki/Documentation) in a format that does not require specifics tied to a particular problem.



## Classification and list

**Design patterns** were originally grouped into the categories: [creational patterns](https://en.wikipedia.org/wiki/Creational_pattern), [structural patterns](https://en.wikipedia.org/wiki/Structural_pattern), and [behavioral patterns](https://en.wikipedia.org/wiki/Behavioral_pattern), and described using the concepts of [delegation](https://en.wikipedia.org/wiki/Delegation_(programming)), [aggregation](https://en.wikipedia.org/wiki/Aggregation_(object-oriented_programming)), and [consultation](https://en.wikipedia.org/w/index.php?title=Consultation_(object-oriented_programming)&action=edit&redlink=1). For further background on object-oriented design, see [coupling](https://en.wikipedia.org/wiki/Coupling_(computer_science)) and [cohesion](https://en.wikipedia.org/wiki/Cohesion_(computer_science)), [inheritance](https://en.wikipedia.org/wiki/Inheritance_(computer_science)), [interface](https://en.wikipedia.org/wiki/Interface_(object-oriented_programming)), and [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_in_object-oriented_programming). Another classification has also introduced the notion of [architectural design pattern](https://en.wikipedia.org/wiki/Architectural_pattern) that may be applied at the architecture level of the software such as the [Model–View–Controller](https://en.wikipedia.org/wiki/Model–View–Controller) pattern. 

### [Creational patterns](https://en.wikipedia.org/wiki/Creational_pattern)

| Name                                                         | Description                                                  | In *[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns_(book))* | In *[Code Complete](https://en.wikipedia.org/wiki/Code_Complete)* | Other |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ----- |
| [Abstract factory](https://en.wikipedia.org/wiki/Abstract_factory_pattern) | Provide an interface for creating *families* of related or dependent objects without specifying their concrete classes. | Yes                                                          | Yes                                                          | N/A   |
| [Builder](https://en.wikipedia.org/wiki/Builder_pattern)     | Separate the construction of a complex object from its representation, allowing the same construction process to create various representations. | Yes                                                          | No                                                           | N/A   |
| [Dependency Injection](https://en.wikipedia.org/wiki/Dependency_injection) | A class accepts the objects it requires from an injector instead of creating the objects directly. | No                                                           | No                                                           | N/A   |
| [Factory method](https://en.wikipedia.org/wiki/Factory_method_pattern) | Define an interface for creating a *single* object, but let subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses. | Yes                                                          | Yes                                                          | N/A   |
| [Lazy initialization](https://en.wikipedia.org/wiki/Lazy_initialization) | Tactic of delaying the creation of an object, the calculation of a value, or some other expensive process until the first time it is needed. This pattern appears in the GoF catalog as "virtual proxy", an implementation strategy for the [Proxy](https://en.wikipedia.org/wiki/Proxy_pattern) pattern. | No                                                           | No                                                           | PoEAA |
| [Multiton](https://en.wikipedia.org/wiki/Multiton_pattern)   | Ensure a class has only named instances, and provide a global point of access to them. | No                                                           | No                                                           | N/A   |
| [Object pool](https://en.wikipedia.org/wiki/Object_pool_pattern) | Avoid expensive acquisition and release of resources by recycling objects that are no longer in use. Can be considered a generalisation of [connection pool](https://en.wikipedia.org/wiki/Connection_pool) and [thread pool](https://en.wikipedia.org/wiki/Thread_pool) patterns. | No                                                           | No                                                           | N/A   |
| [Prototype](https://en.wikipedia.org/wiki/Prototype_pattern) | Specify the kinds of objects to create using a prototypical instance, and create new objects from the 'skeleton' of an existing object, thus boosting performance and keeping memory footprints to a minimum. | Yes                                                          | No                                                           | N/A   |
| [Resource acquisition is initialization](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) (RAII) | Ensure that resources are properly released by tying them to the lifespan of suitable objects. | No                                                           | No                                                           | N/A   |
| [Singleton](https://en.wikipedia.org/wiki/Singleton_pattern) | Ensure a class has only one instance, and provide a global point of access to it. | Yes                                                          | Yes                                                          | N/A   |





### [Concurrency patterns](https://en.wikipedia.org/wiki/Concurrency_pattern)

| Name                                                         | Description                                                  | In *[POSA2](https://en.wikipedia.org/w/index.php?title=Pattern-Oriented_Software_Architecture&action=edit&redlink=1)* | Other |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ----- |
| [Active Object](https://en.wikipedia.org/wiki/Active_object) | Decouples method execution from method invocation that reside in their own thread of control. The goal is to introduce concurrency, by using [asynchronous method invocation](https://en.wikipedia.org/wiki/Asynchronous_method_invocation) and a [scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)) for handling requests. | Yes                                                          | N/A   |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |
|                                                              |                                                              |                                                              |       |


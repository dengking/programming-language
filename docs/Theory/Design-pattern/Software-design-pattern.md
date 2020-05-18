# Software design pattern

“software 的sing pattern”即软件设计模式，是前人总结的解决特定问题的最佳实践（[best practice](https://en.wikipedia.org/wiki/Best_practice)）。我们平时提及design pattern的时候，往往第一反应是[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) design pattern，其实design pattern不仅限于[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) programming，在各个领域中，都能够总结出[domain-specific patterns](https://en.wikipedia.org/wiki/Software_design_pattern#Domain-specific_patterns)。



## 维基百科[Software design pattern](https://en.wikipedia.org/wiki/Software_design_pattern)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), a **software design pattern** is a general, [reusable](https://en.wikipedia.org/wiki/Reusability) solution to a commonly occurring problem within a given context in [software design](https://en.wikipedia.org/wiki/Software_design). It is not a finished design that can be transformed directly into [source](https://en.wikipedia.org/wiki/Source_code) or [machine code](https://en.wikipedia.org/wiki/Machine_code). It is a description or template for how to solve a problem that can be used in many different situations. Design patterns are formalized [best practices](https://en.wikipedia.org/wiki/Best_practice) that the programmer can use to solve common problems when designing an application or system.

> NOTE: design pattern是formalized [best practices](https://en.wikipedia.org/wiki/Best_practice) 

[Object-oriented](https://en.wikipedia.org/wiki/Object-oriented) design patterns typically show relationships and [interactions](https://en.wikipedia.org/wiki/Interaction) between [classes](https://en.wikipedia.org/wiki/Class_(computer_science)) or [objects](https://en.wikipedia.org/wiki/Object_(computer_science)), without specifying the final application classes or objects that are involved. Patterns that imply mutable（可变的） state may be unsuited for [functional programming](https://en.wikipedia.org/wiki/Functional_programming) languages, some patterns can be rendered unnecessary in languages that have built-in support for solving the problem they are trying to solve, and object-oriented patterns are not necessarily suitable for non-object-oriented languages.

> NOTE: 关于functional programming，参见`Theory\Programming-paradigm\Functional-programming`章节。

### Practice

Design patterns can speed up the development process by providing tested, proven development paradigms. Effective software design requires considering issues that may not become visible until later in the implementation. Freshly written code can often have hidden subtle issues that take time to be detected, issues that sometimes can cause major problems down the road. Reusing design patterns helps to prevent such subtle issues , and it also improves code readability for coders and architects who are familiar with the patterns.

> NOTE: 使用design pattern的优势。
>
> software design和[城市规划](https://baike.baidu.com/item/%E5%9F%8E%E5%B8%82%E8%A7%84%E5%88%92/491164?fr=aladdin)有点类似，都需要以发展的眼光来进行规划。

In order to achieve flexibility, design patterns usually introduce additional levels of [indirection](https://en.wikipedia.org/wiki/Indirection), which in some cases may complicate the resulting designs and hurt application performance.

> NOTE: 参见文章[分层](https://dengking.github.io/Post/Abstraction/Abstraction-and-architecture-and-layer/)、文章`Create-larger-concept.md`。

By definition, a pattern must be programmed anew（重新，再次） into each application that uses it. Since some authors see this as a step backward from [software reuse](https://en.wikipedia.org/wiki/Software_reuse) as provided by [components](https://en.wikipedia.org/wiki/Software_componentry), researchers have worked to turn patterns into components. Meyer and Arnout were able to provide full or partial componentization of two-thirds of the patterns they attempted. 

Software design techniques are difficult to apply to a broader range of problems. Design patterns provide general solutions, [documented](https://en.wikipedia.org/wiki/Documentation) in a format that does not require specifics tied to a particular problem.



## Classification and list

design pattern如此之多，如何对它们进行分类至关重要，这是本节需要讨论的问题，本节的内容参考自：

- [维基百科Software design pattern#Classification and list](https://en.wikipedia.org/wiki/Software_design_pattern#Classification_and_list)
- refactoring [Classification of patterns](https://refactoring.guru/design-patterns/classification)
- 维基百科[*Design Patterns*](https://en.wikipedia.org/wiki/Design_Patterns)

refactoring [Classification of patterns](https://refactoring.guru/design-patterns/classification)：

> Design patterns differ by their complexity, level of detail and scale of applicability to the entire system being designed. I like the analogy to road construction: you can make an intersection（十字路口） safer by either installing some traffic lights or building an entire multi-level interchange with underground passages for pedestrians（行人）.
>
> The most basic and low-level patterns are often called *idioms*. They usually apply only to a single programming language.
>
> The most universal and high-level patterns are *architectural patterns*. Developers can implement these patterns in virtually any language. Unlike other patterns, they can be used to design the architecture of an entire application.
>
> In addition, all patterns can be categorized by their *intent*, or purpose. 

这段总结是非常好的，在c++中，我们总结了[More C++ Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)，显然idiom是language-specific，它是某种具体的programming的pattern。我们平时最最常听说的design pattern，往往指的是的OOP的pattern，显然只要某种programming language支持OOP，那么我们就可以使用OOP design pattern，显然它是programming-paradigm-specific的。最最universal、hight-level的是 *architectural patterns*，显然它不是面向某种具体的programming language、某种具体的programming paradigm的，它是面向application的，比如web  application。



> **Design patterns** were originally grouped into the categories: [creational patterns](https://en.wikipedia.org/wiki/Creational_pattern), [structural patterns](https://en.wikipedia.org/wiki/Structural_pattern), and [behavioral patterns](https://en.wikipedia.org/wiki/Behavioral_pattern), and described using the concepts of [delegation](https://en.wikipedia.org/wiki/Delegation_(programming)), [aggregation](https://en.wikipedia.org/wiki/Aggregation_(object-oriented_programming)), and [consultation](https://en.wikipedia.org/w/index.php?title=Consultation_(object-oriented_programming)&action=edit&redlink=1). For further background on object-oriented design, see [coupling](https://en.wikipedia.org/wiki/Coupling_(computer_science)) and [cohesion](https://en.wikipedia.org/wiki/Cohesion_(computer_science)), [inheritance](https://en.wikipedia.org/wiki/Inheritance_(computer_science)), [interface](https://en.wikipedia.org/wiki/Interface_(object-oriented_programming)), and [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_in_object-oriented_programming). Another classification has also introduced the notion of [architectural design pattern](https://en.wikipedia.org/wiki/Architectural_pattern) that may be applied at the architecture level of the software such as the [Model–View–Controller](https://en.wikipedia.org/wiki/Model–View–Controller) pattern. 



### OOP design pattern

由[***Design Patterns: Elements of Reusable Object-Oriented Software***](https://en.wikipedia.org/wiki/Design_Patterns) 提出的OOP 的design pattern，是design pattern的开山之作。

#### [Creational patterns](https://en.wikipedia.org/wiki/Creational_pattern)

维基百科[Design Patterns#Creational](https://en.wikipedia.org/wiki/Design_Patterns#Creational)：

> [Creational patterns](https://en.wikipedia.org/wiki/Creational_pattern) are ones that create objects, rather than having to instantiate objects directly. This gives the program more flexibility in deciding which objects need to be created for a given case.

#### [Structural patterns](https://en.wikipedia.org/wiki/Structural_pattern)

refactoring [Classification of patterns](https://refactoring.guru/design-patterns/classification)：

> **Structural patterns** explain how to assemble objects and classes into larger structures, while keeping the structures flexible and efficient.

除此看到structural pattern，我的第一想法是[Boost.PropertyTree](https://www.boost.org/doc/libs/1_73_0/doc/html/property_tree.html)。

#### [Behavioral pattern](https://en.wikipedia.org/wiki/Behavioral_pattern)

> **behavioral design patterns** are [design patterns](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) that identify common communication patterns among objects and realize these patterns.

即object之间communicate的pattern。



### [Concurrency patterns](https://en.wikipedia.org/wiki/Concurrency_pattern)





### [Architectural design pattern](https://en.wikipedia.org/wiki/Architectural_pattern) 


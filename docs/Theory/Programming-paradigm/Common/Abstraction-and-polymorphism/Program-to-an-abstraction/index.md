# Program to an abstraction/interface principle

"Program to abstraction"现代programming language中的核心思想。

## Introduction

依赖于抽象而不是具体，即上层依赖于抽象的接口，底层实现具体。上层到底层的映射，依赖于多态，它可以是compiler time完成，也可以是run time完成。比如get_token，实现静态多态，实现了多个具体实现版本，由compiler进行派发，进行选择

> 上述"依赖于抽象而不是具体"，其实就是program to abstraction。



## OOP: program to interface not on implementation 

它是"Program to abstraction principle"在OOP中的体现，参见`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Principle`章节。



1、program to an abstraction，其实本质上是program to an interface，因为interface是 对abstraction的描述；

2、subtyping的本质是interface inheritance，显然它是program to an interface；

3、在wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)中有这样的描述: 

> Composition and interfaces
>
> The C++ examples in this section demonstrate the principle of using composition and interfaces to achieve code reuse and polymorphism.

 显然，无论是subtyping 还是 composition，都是program to an abstraction，所以 最终都是program to an interface。



### Interface in OOP

Interface在OOP中占据着核心地位，关于interface，参见:

1、cs.utah.edu [Interfaces](https://www.cs.utah.edu/~germain/PPS/Topics/interfaces.html) 

2、stackoverflow [What is the definition of “interface” in object oriented programming](https://stackoverflow.com/questions/2866987/what-is-the-definition-of-interface-in-object-oriented-programming)



## wikipedia [Interface (computing) # Programming to the interface](https://en.wikipedia.org/wiki/Interface_(computing)#Programming_to_the_interface)

> NOTE: 这篇文章收录在 `Theory\Interface-and-implementation\Interface\wikipedia-interface` 章节
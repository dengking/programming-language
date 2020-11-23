# Program to an abstraction principle

"Program to abstraction"现代programming language中的核心思想。

下面是一些thought:

> 依赖于抽象而不是具体，即上层依赖于抽象的接口，底层实现具体。上层到底层的映射，依赖于多态，它可以是compiler time完成，也可以是run time完成。比如get_token，实现静态多态，实现了多个具体实现版本，由compiler进行派发，进行选择
>
> 上述"依赖于抽象而不是具体"，其实就是program to abstraction。



## OOP: program to interface not on implementation 

它是"Program to abstraction principle"在OOP中的体现，参见`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Principle`章节。


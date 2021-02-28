# Summary of OOP

## 从面向过程到面向对象

有了[Abstraction](https://dengking.github.io/Post/Abstraction/Abstraction/)和[Language](https://dengking.github.io/Post/Language/Language/)的概念，再来阅读[Thinking-in-java](./Class-based-OOP/Thinking-in-java/Introduction-to-Objects.md)中OO理论的说明就非常容易了。

我们使用语言来描述这个世界，面向过程语言如c，使用一个一个过程。面向对象语言更加expressive，能够更好地描述抽象概念，类的继承关系和概念之间的关系。更加易于理解。

从programming language发展史来看，语言越来越容易理解，越来越expressive。

对问题的描述（使用class来描述concept），对问题的求解（一个一个的object之间的交互）。

将对象看做是一个computer、service provider、一个独立的个体，则整个使用OO实现的就是多个object之间的交互了。

我们使用`class`来描述概念，对象是运行时的概念，运行时才会创建对象。

## Python VS c++ 面向对象的实现

参见[Programming language implementation](https://en.wikipedia.org/wiki/Programming_language_implementation)

Python中一切皆对象，它的class是由对象来进行描述。Python中面向对象特性的实现：attribute lookup，`__dict__`。Python program会被翻译为byte code由interpreter来执行。

Python statement与bytecode之间的对应关系。

c++的面向对象由compile理解，最终都是要翻译为instruction的。


c++中的对象本质上是一片内存区域，在进行programming的时候，可以通过成员方法来操作这片内存区域，this指针指向这片内存区域。

c++并非一切皆对象。

run model都是：user-defined action是subroutine，使用self、this来指向object。

从type system来看，class就是一种type，是一种概念，一种抽象。

我们使用语言来描述世界，OO是一种更加强大的描述方式，但是使用OO编写的program的run model是一致的。



## pcmag [object-oriented programming](https://www.pcmag.com/encyclopedia/term/object-oriented-programming)

| OOP         | Traditional Programming  |
| ----------- | ------------------------ |
| class       | define data + processing |
| object      | data + processing        |
| attribute   | data (a field)           |
| method      | function                 |
| message     | function call            |
| instantiate | allocate a structure     |
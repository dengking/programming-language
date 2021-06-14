# [面向对象](https://leetcode-cn.com/leetbook/read/cpp-interview-highlights/ej8n9m/)

> NOTE: 
>
> 一、主要讨论programming paradigm，参见 `Theory\Programming-paradigm` 章节
>
> 

## 什么是面向对象？面向对象的三大特性

> NOTE: 
>
> 参见 `Theory\Programming-paradigm\OOP` 章节

面向对象：对象是指具体的某一个事物，这些事物的抽象就是类，类中包含数据（成员变量）和动作（成员方法）。

面向对象的三大特性：

1、封装：将具体的实现过程和数据封装成一个函数，只能通过接口进行访问，降低耦合性。

2、继承：子类继承父类的特征和行为，子类有父类的非 private 方法或成员变量，子类可以对父类的方法进行重写，增强了类之间的耦合性，但是当父类中的成员变量、成员函数或者类本身被 final 关键字修饰时，修饰的类不能继承，修饰的成员不能重写或修改。

3、多态：多态就是不同继承类的对象，对同一消息做出不同的响应，基类的指针指向或绑定到派生类的对象，使得基类指针呈现不同的表现方式。



## 重载、重写、隐藏的区别

> NOTE: 
>
> 参见 `Overload-VS-override-VS-name-hiding` 章节





## 如何理解 C++ 是面向对象编程

> NOTE: 
>
> 一、以 `book-Thinking-in-java` 中的思想来谈:
>
> 1、描述concept、abstraction、expressive power、程序员使用programming language来描述世界
>
> 二、是从OOP开始学起的
>
> 三、每种programming paradigm都有着的它的优势、劣势，需要结合多种programming paradigm来充分发挥language的威力
>
> 





## 什么是多态？多态如何实现？


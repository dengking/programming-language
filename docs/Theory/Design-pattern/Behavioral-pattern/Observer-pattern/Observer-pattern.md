# Observer pattern

Observer pattern使用OOP的语言来描述event-driven model。

## 维基百科[Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern)

The **observer pattern** is a [software design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) in which an [object](https://en.wikipedia.org/wiki/Object_(computer_science)#Objects_in_object-oriented_programming), called the **subject**, maintains a list of its dependents, called **observers**, and notifies them automatically of any state changes, usually by calling one of their [methods](https://en.wikipedia.org/wiki/Method_(computer_science)).

> NOTE: subject对应的是event-driven model的monitor的角色，observer对应的是event-driven model的executor角色。

## w3sdesign [Observer design pattern](http://w3sdesign.com/?gr=b07&ugr=proble#gf)

### Intent

The Observer design pattern solves problems like:

- How can a one-to-many dependency between objects be defined without making the objects tightly coupled?
- How can an object **notify** an open-ended-number of other objects?

> NOTE: “open-ended-number of other objects”意味着，在runtime，可以增加observer，也可以删除observer。

The Observer pattern describes how to solve such problems:

- Define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically.
- The key idea in this pattern is to establish a flexible *notification-registration* mechanism that *notifies* all *registered* objects automatically when an event of interest occurs.

> NOTE: *notification-registration* mechanism是observer pattern的核心思想，凭借它，observer pattern解决了前面提出的问题。*notification-registration*也叫做publish-subscribe。



> NOTE: 在原文的intent节给出的sample class diagram和sample sequence diagram其实就已经展示出了observer design pattern的实现了。

### Problem

> NOTE: 在原文的这一节给出了一个反例，我们也应该要注意反例。在原文的motivation章节，给出了更加详细的对比。



### Implementation

> NOTE: 原文的这一节给出的**Push Data**、**Pull Data**实现方式

### Sample code

> NOTE: 原文给出的sample code非常好。



## microsoft [Observer Design Pattern](https://docs.microsoft.com/en-us/dotnet/standard/events/observer-design-pattern)

[Events and routed events overview](https://docs.microsoft.com/zh-cn/previous-versions/windows/apps/hh758286(v=win.10))

[Handling and raising events](https://docs.microsoft.com/en-us/dotnet/standard/events/)



## oodesign [Observer Pattern](https://www.oodesign.com/observer-pattern.html)





## refactoring [Observer](https://refactoring.guru/design-patterns/observer)

> NOTE: 前面给出的示例都没有准确描述对不同类型的时间，执行不同的函数，而本文的示例则展示了这一点。它的代码也是值得阅读的https://refactoring.guru/design-patterns



## cpppatterns [Observer](https://cpppatterns.com/patterns/observer.html) 

## TODO

observer pattern VS publish subscribe pattern

https://hackernoon.com/observer-vs-pub-sub-pattern-50d3b27f838c
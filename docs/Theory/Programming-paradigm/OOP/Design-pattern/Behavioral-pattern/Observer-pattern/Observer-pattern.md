# Observer pattern

Observer pattern使用OOP的语言来描述event-driven model。

## 维基百科[Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern)

The **observer pattern** is a [software design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) in which an [object](https://en.wikipedia.org/wiki/Object_(computer_science)#Objects_in_object-oriented_programming), called the **subject**, maintains a list of its dependents, called **observers**, and notifies them automatically of any state changes, usually by calling one of their [methods](https://en.wikipedia.org/wiki/Method_(computer_science)).

> NOTE: subject对应的是event-driven model的**monitor**的角色，observer对应的是event-driven model的**executor**角色。subject通过调用observer的method来实现message passing。

It is mainly used to implement distributed [event handling](https://en.wikipedia.org/wiki/Event_handling) systems, in "event driven" software. Most modern languages such as C# have built in "event" constructs which implement the observer pattern components.

The observer pattern is also a key part in the familiar [model–view–controller](https://en.wikipedia.org/wiki/Model–view–controller) (MVC) architectural pattern.[[1\]](https://en.wikipedia.org/wiki/Observer_pattern#cite_note-jont-1) The observer pattern is implemented in numerous [programming libraries](https://en.wikipedia.org/wiki/Programming_library) and systems, including almost all [GUI](https://en.wikipedia.org/wiki/GUI)toolkits.

### Strong vs. Weak reference

The observer pattern can cause [memory leaks](https://en.wikipedia.org/wiki/Memory_leak), known as the [lapsed listener problem](https://en.wikipedia.org/wiki/Lapsed_listener_problem), because in basic implementation it requires both explicit registration and explicit deregistration, as in the [dispose pattern](https://en.wikipedia.org/wiki/Dispose_pattern), because the subject holds strong references to the observers, keeping them alive. This can be prevented by the subject holding [weak references](https://en.wikipedia.org/wiki/Weak_reference) to the observers.

> NOTE: 如何来解决呢？

## Coupling and typical pub-sub implementations

Typically the **observer pattern** is implemented with the "**subject**" (which is being "observed") being part of the object, whose state change is being **observed**, to be communicated to the **observers** upon occurrence. This type of implementation is considered "[tightly coupled](https://en.wikipedia.org/wiki/Tightly_coupled_system)", forcing both the **observers** and the **subject** to be aware of each other and have access to their internal parts, creating possible issues of [scalability](https://en.wikipedia.org/wiki/Scalability), speed, message recovery and maintenance (also called event or notification loss), the lack of flexibility in conditional dispersion(分散) and possible hindrance（妨碍） to desired security measures. 

In some ([non-polling](https://en.wikipedia.org/wiki/Polling_(computer_science))) implementations of the [publish-subscribe pattern](https://en.wikipedia.org/wiki/Publish-subscribe_pattern) (also called the **pub-sub pattern**), this is solved by creating a dedicated（专门的） **"message queue" server** and at times an extra **"message handler" object**, as added stages between **the observer** and **the observed object** whose state is being checked, thus "decoupling" the software components. In these cases, the **message queue server** is accessed by the observers with the observer pattern, "subscribing to certain messages" knowing only about the expected message (or not, in some cases), but knowing nothing about the **message sender** itself, and the sender may know nothing about the receivers. Other implementations of the publish-subscribe pattern, which achieve a similar effect of notification and communication to interested parties, do not use the observer pattern altogether.[[4\]](https://en.wikipedia.org/wiki/Observer_pattern#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Observer_pattern#cite_note-5)

> NOTE: 上面这段话对比了MQ和observer pattern。

Still, in early implementations of multi-window operating systems like OS2 and Windows, the terms "publish-subscribe pattern" and "event driven software development" were used as a synonym for the observer pattern.[[6\]](https://en.wikipedia.org/wiki/Observer_pattern#cite_note-6)

The observer pattern, as described in the [GOF book](https://en.wikipedia.org/wiki/Design_Patterns), is a very basic concept and does not deal with observance removal or with any conditional or complex logic handling to be done by the observed "subject" before or after notifying the observers. The pattern also does not deal with recording the "events", the asynchronous passing of the notifications or guaranteeing they are being received. These concerns are typically dealt with in message queueing systems of which the observer pattern is only a small part.

Related patterns: [Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish–subscribe_pattern), [mediator](https://en.wikipedia.org/wiki/Mediator_pattern), [singleton](https://en.wikipedia.org/wiki/Singleton_pattern).



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

> NOTE: 前面给出的示例都没有准确描述对不同类型的事件，执行不同的函数，而本文的示例则展示了这一点。它的代码也是值得阅读的https://refactoring.guru/design-patterns



## cpppatterns [Observer](https://cpppatterns.com/patterns/observer.html) 

## TODO

observer pattern VS publish subscribe pattern

https://hackernoon.com/observer-vs-pub-sub-pattern-50d3b27f838c
# 关于本章

本章描述了c++中关于OOP的idiom。RAII、swap、temporary，很多OOP idiom都是建立在这三者之上。

## Initialization相关

[Runtime Static Initialization Orde Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Runtime_Static_Initialization_Order_Idioms)

| idiom                                                        | example | summary                           |
| ------------------------------------------------------------ | ------- | --------------------------------- |
| [Construct-On-First-Use](./Construct-On-First-Use/Construct-On-First-Use.md) |         | 描述static object的initialization |
| [Nifty-Counter](./Nifty-Counter/Nifty-Counter.md)            |         |                                   |



| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Calling-Virtuals-During-Initialization](./Calling-Virtuals-During-Initialization/Calling-Virtuals-During-Initialization.md) |         | invoke virtual functions of derived classes while a derived object is being initialized |
| [Base-from-Member](./Base-from-Member/Base-from-Member.md)   |         | 基类的初始化需要用到子类的成员变量                           |



## polymorphism相关



| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Virtual-Friend-Function](./Virtual-Friend-Function/Virtual-Friend-Function.md) |         | 添加一层indirection to achieve the desired effect of dynamic binding for friend functions |
| [Virtual-Constructor](./Virtual-Constructor/Virtual-Constructor.md) |         |                                                              |
|                                                              |         |                                                              |





## Idiom about resource

[RAII](./RAII/RAII.md)和[The-rule-of-three-five-zero](./The-rule-of-three-five-zero/The-rule-of-three-five-zero.md)其实都是resource management的一些技巧，它能够保住使用者规避一些错误。

在[What is the copy-and-swap idiom?](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)中有这样的描写：

> In order to use the copy-and-swap idiom, we need three things: a working copy-constructor, a working destructor (both are the basis of any wrapper, so should be complete anyway), and a `swap` function.

copy-and-swap idiom依赖于：

- copy constructor

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)<-[Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)<- [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) 





### 对resource的operation

c++ idiom中有多个关于resource的idiom，可以看到这些idiom都充分利用copy、move、swap、destruct。



### Idiom about pointer

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)->Smart Pointer 

在`Resource-management`章节进行了描述

## Creation of object

- Requiring or Prohibiting Heap-based Objects

## Access privilege

- Attorney-Client





## Idiom concerning temporary/proxy

本节总结涉及到temporary的idiom，而不仅仅局限于oop idiom。

temporary的特性：自动创建、自动销毁。

Idiom concerning temporary/proxy可以分为：

- 使用temporary的idiom
- 消除temporary的idiom

### 使用temporary的idiom

| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) |         | 这个idiom又称为：Create-Temporary-and-Swap，它的名字已经告诉了我们它应用了temporary。 |
| [Clear-and-minimize](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize) |         | 这个idiom又称为： ***swap with temporary idiom***. <br> 显然它是依赖于：temporary、swap，其实它可以看做是将swap idiom用于container上。 |
| [Return Type Resolver](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Return_Type_Resolver) |         |                                                              |
|                                                              |         |                                                              |
|                                                              |         |                                                              |
|                                                              |         |                                                              |
|                                                              |         |                                                              |



proxy 

通过引入proxy class，增加了一层indirection，从而允许在client code和底层的class之间添加一些特殊的operation。

| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Execute-Around Pointer](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Execute-Around_Pointer) |         | 使用一个proxy class来描述aspect，aspect的[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)行为放到proxy class的constructor、destructor中来进行实现。primary class中涉及到[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)的行为，需要由proxy class object来forward到primary class object来实现。 |
| [Temporary Proxy](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Proxy) |         | 通过proxy class来添加一层indirection，在这层direction中，添加上一些特殊的operation。比如通过overload `operator []`和user-defined conversion来识别read、write，从而实现undo |



### 





[Temporary Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Base_Class)



### 

### 消除temporary的idiom

[Computational Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Computational_Constructor)

消除temporary




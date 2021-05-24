# 关于本章

本章描述了C++中关于OOP的idiom。RAII、swap、temporary，很多OOP idiom都是建立在这三者之上。

## Initialization相关

[Runtime Static Initialization Orde Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Runtime_Static_Initialization_Order_Idioms)用于解决*static initialization order problem*

| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Construct-On-First-Use](./Construct-On-First-Use/Construct-On-First-Use.md) |         | 描述static object的initialization                            |
| [Nifty-Counter](./Nifty-Counter/Nifty-Counter.md)            |         | 将reference counting用于对static object的initialization/deinitialization |
| [Meyer's singlton](https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton) |         |                                                              |



| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Attach-by-Initialization](./Attach-by-Initialization/Attach-by-Initialization.md) |         | 充分利用**objects of static storage duration**的“initialized before `main` begins execution”特性，即它的构造函数会在`main`函数开始之前被执行的特性来“Attach a user-defined object to a framework before program execution begins.” |



| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Calling-Virtuals-During-Initialization](./Calling-Virtuals-During-Initialization/Calling-Virtuals-During-Initialization.md) |         | invoke virtual functions of derived classes while a derived object is being initialized |
| [Base-from-Member](./Base-from-Member/Base-from-Member.md)   |         | 基类的初始化需要用到子类的成员变量；充分利用order of initialization即“**base classes** are initialized in the order they are declared”，引入一个class来初始化基类的constructor中需要使用的variable，并且保证这个类的次序在基类之前。 |



## polymorphism相关



| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Virtual-Friend-Function](./Virtual-Friend-Function/Virtual-Friend-Function.md) |         | 添加一层indirection to achieve the desired effect of dynamic binding for friend functions |
| [Virtual-Constructor](./Virtual-Constructor/Virtual-Constructor.md) |         |                                                              |
| [Polymorphic-Value-Types](./Polymorphic-Value-Types/Polymorphic-Value-Types.md) |         |                                                              |
| [Polymorphic-Exception](../Exception/Polymorphic-Exception.md) |         |                                                              |





## Idiom about resource



| idiom                                                        |      | summary                                                      |
| ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| [RAII](./RAII/RAII.md)                                       |      |                                                              |
| [The-rule-of-three-five-zero](./The-rule-of-three-five-zero/The-rule-of-three-five-zero.md) |      |                                                              |
| [Move-Constructor](./Move-Constructor/Move-Constructor.md)   |      | 在c++11中，已经内置支持move semantic了，那在c++11之前，如何实现move semantic呢？这是这个idiom所描述的内容。 |
| [Resource Return](./Resource-Return/Resource-Return.md)      |      | To convey ownership transfer (of a resource) explicitly in the return type of a factory function.<br>convey ownership给caller |
|                                                              |      |                                                              |

[RAII](./RAII/RAII.md)和[The-rule-of-three-five-zero](./The-rule-of-three-five-zero/The-rule-of-three-five-zero.md)其实都是resource management的一些技巧，它能够保住使用者规避一些错误。

在[What is the copy-and-swap idiom?](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)中有这样的描写：

> In order to use the copy-and-swap idiom, we need three things: a working copy-constructor, a working destructor (both are the basis of any wrapper, so should be complete anyway), and a `swap` function.

copy-and-swap idiom依赖于：

- copy constructor

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)<-[Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)<- [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) 



### 基于reference counting的resource management

| idiom                                             |      | summary                                                      |
| ------------------------------------------------- | ---- | ------------------------------------------------------------ |
| [Counted-Body](./Counted-Body/Counted-Body.md)    |      |                                                              |
| [Nifty-Counter](./Nifty-Counter/Nifty-Counter.md) |      | 将reference counting用于对static object的initialization/deinitialization |
|                                                   |      |                                                              |



### 对resource的operation

c++ idiom中有多个关于resource的idiom，可以看到这些idiom都充分利用copy、move、swap、destruct。



### Idiom about pointer

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)->Smart Pointer 

在`Resource-management`章节进行了描述



## Creation of object

[Requiring or Prohibiting Heap-based Objects](./Requiring-or-Prohibiting-Heap-based-Objects/Requiring-or-Prohibiting-Heap-based-Objects.md)

## Access privilege

[Attorney-Client](./Attorney-Client/Attorney-Client.md)



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
|                                                              |


proxy/temporary class

通过引入proxy/temporary class，增加了一层indirection，从而允许在client code和底层的class之间添加一些特殊的operation。

| idiom                                                        | example | summary                                                      |
| ------------------------------------------------------------ | ------- | ------------------------------------------------------------ |
| [Execute-Around Pointer](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Execute-Around_Pointer) |         | 使用一个proxy class来描述aspect，aspect的[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)行为放到proxy class的constructor、destructor中来进行实现。primary class中涉及到[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)的行为，需要由proxy class object来forward到primary class object来实现。 |
| [Temporary Proxy](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Proxy) |         | 通过proxy class来添加一层indirection，在这层direction中，添加上一些特殊的operation。比如通过overload `operator []`和user-defined conversion来识别read、write，从而实现undo |
| [Return Type Resolver](../Template-metaprogramming/Return-Type-Resolver/Return-Type-Resolver.md) |         | 提供**templatized conversion operator function**，充分运用compiler自动地调用**conversion operator function**来讲将**temporary object**转换为目标变量（即等号左侧的变量）从而达到简化代码的目的 |



[Temporary Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Base_Class)



 

### 消除temporary的idiom



[Computational Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Computational_Constructor)

消除temporary



## Type 

### Type conversion

| idiom                                                        | example                                                      | summary                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Coercion-by-Member-Template](./Coercion-by-Member-Template/Coercion-by-Member-Template.md) | [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)、[std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) |                                                              |
| [Return Type Resolver](../Template-metaprogramming/Return-Type-Resolver/Return-Type-Resolver.md) |                                                              | 提供**templatized conversion operator function**，充分运用compiler自动地调用**conversion operator function**来讲将**temporary object**转换为目标变量（即等号左侧的变量）从而达到简化代码的目的 |




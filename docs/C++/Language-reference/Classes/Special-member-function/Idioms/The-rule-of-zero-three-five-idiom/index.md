# The rule of three/five/zero and copyable、movable

一、rule of three/five/zero 与 implicit defined，显然一旦使用了 rule of three/five/zero，则表明不再依赖于implicit defined

二、不仅仅需要遵循rule of three/five/zero，还需要考虑class是否copyable、movable，并进行显式地指定，不要推测、过分依赖于compiler的行为。

我的经历:

1、class RAII，但是没有定义copyable 和 movable，即没有遵循The rule of three/five/zero

2、我推测compiler会执行copy-elision，即不会发生copy，然而在复杂的C++规则面前，programmer可能无法准确的预测compiler的行为，结果发生了copy，而implicit defined copy是shallow copy，并且在定义了destructor的情况下，compiler是不会生成movable的 ，这就导致了"implicit-define-copy-constructor-shallow-copy-double-free"错误。



三、其实对the rule of three/five/zero and copyable、movable的思考，本质上是对ownership的思考，"implicit defined copy constructor shallow copy double free"问题，其实就是典型对ownership描述不清而导致的问题，它到底是unique ownership还是shared ownership？显然raw pointer无法描述清楚，shallow copy对应的更多是shared ownership，而raw pointer无法表达shared ownership，它会导致double free。

因此在使用resource handle的时候，一定要同时阐明清楚它的ownership，最好是使用`unique_ptr`、`shared_ptr`，它们已经明确的阐明清楚了ownership。



## 从resource ownership的角度来分析copy、move

move对应的是move ownership，它需要将resource的ownership transfer走，因此在source object中，需要表明它已经不再own resource了，source object就不再release resource。

> CppCoreGuidelines-C.64 A move operation should move and leave its source in a valid state

copy:

deep copy: 显然是重新acquire resource了

shallow copy: 显然表达的是shared ownership



## Resource wrapper: Single responsibility +  rule of three five zero + RAII 

使用"Single responsibility +  rule of three five zero + RAII"来实现Resource wrapper。
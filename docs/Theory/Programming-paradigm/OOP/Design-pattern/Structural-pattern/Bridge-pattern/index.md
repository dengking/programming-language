# Bridge pattern

1、是在学习 C++ pimpl idiom 的时候，发现的Bridge pattern。

2、它能够实现： information hiding: Separate interface from implementation

3、从C++ pimpl idiom 的角度出发，理解Bridge pattern会相对比较容易

## wikipedia [Bridge pattern](https://en.wikipedia.org/wiki/Bridge_pattern)

The **bridge pattern** is a [design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) used in [software engineering](https://en.wikipedia.org/wiki/Software_engineering) that is meant to *"decouple an [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) from its [implementation](https://en.wikipedia.org/wiki/Implementation) so that the two can vary independently"*, introduced by the [Gang of Four](https://en.wikipedia.org/wiki/Design_Patterns).[[1\]](https://en.wikipedia.org/wiki/Bridge_pattern#cite_note-1) The *bridge* uses [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_science)), [aggregation](https://en.wikipedia.org/wiki/Object_composition#Aggregation), and can use [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) to separate responsibilities into different [classes](https://en.wikipedia.org/wiki/Class_(computer_science)).

When there is only one fixed implementation, this pattern is known as the [Pimpl](https://en.wikipedia.org/wiki/Pimpl) idiom in the [C++](https://en.wikipedia.org/wiki/C%2B%2B) world.
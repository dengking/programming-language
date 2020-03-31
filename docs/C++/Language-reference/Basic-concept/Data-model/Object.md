# Object

需要注意的是，本文中的 object 并不是我们 object-oriented programming 中所指的 object；本文的 object是指“region of storage”。显然，它的含义与object-oriented programming 中所指的 object 完全不同；

此处的 object 是一个 runtime 概念，因为只有当program运行的时候，object才会被创建。

object概念对于理解后面的内容非常重要，后者说，后续的很多概念都是建立在object之上的。

## cppreference [Object](https://en.cppreference.com/w/cpp/language/object)

C++ programs create, destroy, refer to, access, and manipulate *objects*.

> NOTE: 在上一节我们描述了C++ program的组成（是静态的、compile-time的），上面这段话描述了C++ program在runtime所做的事情。



## Lifetime of object

object的initialization。

## Variable and object

两种都是runtime概念，variable是一种object，但是不是所有的object都是variable。
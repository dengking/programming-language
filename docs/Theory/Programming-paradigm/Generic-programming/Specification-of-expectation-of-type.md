# Specification of expectation of type in class-based OOP language

本节标题的含义是：在class-based OOP language中，如何描述对类型的expectation （期望或要求）。这是我在阅读[python doc](https://docs.python.org/3/)和[cppreference](https://en.cppreference.com/w/cpp/named_req)时，发现两者都使用"able"来描述对类型的期望。下面对此进行详细分析：

python和`c++`都是是class-based OOP语言，类可以看做是一种类型，阅读这两种语言的language reference，你就会发现：language reference需要描述类型的**feature**（**特性**），或者说当对某种类型的对象进行操作的时候，期望它具备某种**feature**，以使这种操作可以进行，定义这些特性，能够使对语言的表述非常便利，清晰，易懂。两种语言的normative text of standard中，都使用“able”来这些**feature**，比如callable、iterable、awaitable。c++通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来定义这些特性，python中也有类似的概念，但是貌似python并没有像`c++`这样进行显式地定义。

相同的是，这两种语言都是让user-defined class通过实现**magic function**来为这个类型添加某种**特性**，所以在学习时，需要将**feature**和对应的**magic function**关联起来。



## [C++ Named requirements](https://en.cppreference.com/w/cpp/named_req)



## Python able

[iterable](https://docs.python.org/3/glossary.html#term-iterable)

[asynchronous iterable](https://docs.python.org/3/glossary.html#term-asynchronous-iterable)

[awaitable](https://docs.python.org/3/glossary.html#term-awaitable) 

[hashable](https://docs.python.org/3/glossary.html#term-hashable)

[immutable](https://docs.python.org/3/glossary.html#term-immutable)

[mutable](https://docs.python.org/3/glossary.html#term-mutable)

executable 

callable 

## [Design by contact](https://en.wikipedia.org/wiki/Design_by_contract) and expectation and generic programming

python通过duck type来实现generic programming，duck type是python的核心，python的很多standard library都是建立在这个机制上，python通过`able`来define the expectations of the standard library。

`c++`通过template来实现generic programming，template是c++的核心，c++的很多standard library都是建立在此机制上，c++通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来define the expectations of the standard library。

通过上面的分析，我们已经知道python able和c++ named requirement本质上是相同的东西，

如果从[Design by contact](https://en.wikipedia.org/wiki/Design_by_contract)的角度来看的话，上面所说的expectation就是一种contact，programmer只有遵循了这个contact才能够正确地使用standard library。



python标准库的设计和c++标准库的设计是一种典范，值的借鉴学习。
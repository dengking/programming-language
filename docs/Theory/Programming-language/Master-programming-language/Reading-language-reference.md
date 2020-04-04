# Reading language reference

在上一篇中，我们提出了从语言的设计者的角度来学习programming language，一般这些内容都置于programming language doc的language reference中，所以我们需要reading language reference，下面总结了我在阅读不同语言的language reference时所总结的一些经验。

## Class-based OOP语言描述类型特性

python和c++都是都是class-based OOP语言，类可以看做是一种类型，阅读这两种语言的language reference，你就会发现：language reference需要描述类型的**特性**，或者说当对某种类型的对象进行操作的时候，期望它具备某种特性，以使这种操作可以进行，定义这些特性，能够使对语言的表述非常便利，清晰，易懂。

这些特性往往是“able”，比如callable、iterable、awaitable

c++通过named requirement来定义这些特性，python中也有类似的概念，但是貌似python并没有像c++这样进行显式地定义。

相同的是，这两种语言都是让user-defined class通过实现magic function来为这个类型添加某种特性。c++中，将此称为overload。



### [c++ Named requirements](https://en.cppreference.com/w/cpp/named_req)



### python able

[iterable](https://docs.python.org/3/glossary.html#term-iterable)

[asynchronous iterable](https://docs.python.org/3/glossary.html#term-asynchronous-iterable)

[awaitable](https://docs.python.org/3/glossary.html#term-awaitable) 

[hashable](https://docs.python.org/3/glossary.html#term-hashable)

[immutable](https://docs.python.org/3/glossary.html#term-immutable)

[mutable](https://docs.python.org/3/glossary.html#term-mutable)

executable 

callable 




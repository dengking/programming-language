# Specification of type requirement

本节标题的含义是：描述对类型的期望(expectation)或要求(requirement)。在各种programming paradigm中，都涉及这个问题，本文对此进行总结。



## Specification of type requirement: `***able`

在阅读[python doc](https://docs.python.org/3/)和[cppreference](https://en.cppreference.com/w/cpp/named_req)时，会发现两者都使用"`***able`"来描述对type的期望/要求。在一门语言的language reference中需要详细地说明当对某种type的对象(object)进行操作的时候(比如某个标准库的函数)，期望/要求它(object)具备/满足某些条件(这就是**type requirement**，因为C++、Python都是strong typed language)，因此明确地定义这些**type requirement**，能够使对语言的表述非常便利，清晰，易懂。这些type requirement其实非常能够体现一门语言的核心特性。

两种语言的normative text of standard中，都使用"`***able`"来描述对type的期望/要求，比如callable、iterable、awaitable。`C++`通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来定义这些特性，Python中也有类似的概念，但是貌似python并没有像`C++`这样进行显式地定义。

### Type requirement is more about behavior-based

本节标题的意思是: type requirement往往是**behavior-based**。

**behavior-based**是我在阅读 drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971) 时，其中提出的一个概念，我觉得"**behavior-based**"这个词总结地非常好:

1) 可以对object这些哪些操作(比如call、iterate)

2) object具备/哪些attribute(member data、member function)，这也可以看做是一种behavior

"Type requirement is more about behavior-based"是我的经验之谈，主要是缘于:

1) Python、C++都是让user-defined class通过实现**magic function**来为这个**类型**添加某种**特性**/**behavior**，从而使它满足type requirement，所以在学习时，需要将**feature**/**behavior**和对应的**magic function**关联起来。所谓**magic function**，其实就是实现了特定功能的函数，在上层函数中，这个函数会被调用。比如OOP中的virtual function。

2) 我们往往(TODO)

### Example of `***able`

#### [C++ Named requirements](https://en.cppreference.com/w/cpp/named_req)



#### Python able

[iterable](https://docs.python.org/3/glossary.html#term-iterable)

[asynchronous iterable](https://docs.python.org/3/glossary.html#term-asynchronous-iterable)

[awaitable](https://docs.python.org/3/glossary.html#term-awaitable) 

[hashable](https://docs.python.org/3/glossary.html#term-hashable)

[immutable](https://docs.python.org/3/glossary.html#term-immutable)

[mutable](https://docs.python.org/3/glossary.html#term-mutable)

executable 

callable 



## Implementation

不同的programming language，不同的programming paradigm，对于specification of type requirement的实现方式是不同的，因此无法进行全面的、统一的描述，下面以programming paradigm分类来进行描述。

### Class-based OOP

在class-based OOP语言，**类**可以看做是一种**类型**。Class-based OOP中具备inheritance、implement(Java)关系的class，往往具备相同的behavior，能够满足相同的type requirement。

参见: 

1) wikipedia [Protocol (object-oriented programming)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming))

2) wikipedia [Interface (Java)](https://en.wikipedia.org/wiki/Interface_(Java))



### Generic programming(GP) and duck type

Generic programming中使用concept来specify type requirement，关于此，参见`Theory\Programming-paradigm\Generic-programming`。Duck type可以看做是GP的一种实现。

参见: wikipedia [Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming))

#### [Design by contact](https://en.wikipedia.org/wiki/Design_by_contract) and expectation and generic programming

Python通过duck type来实现generic programming，duck type是python的核心，python的很多standard library都是建立在这个机制上，python通过`able`来define the expectations of the standard library。

`C++`通过template来实现generic programming，template是c++的核心，c++的很多standard library都是建立在此机制上，C++通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来define the expectations of the standard library。

通过上面的分析，我们已经知道Python able和C++ named requirement本质上是相同的东西，

如果从[Design by contact](https://en.wikipedia.org/wiki/Design_by_contract)的角度来看的话，上面所说的expectation就是一种contact，programmer只有遵循了这个contact才能够正确地使用standard library。

Python标准库的设计和c++标准库的设计是一种典范，值的借鉴学习。






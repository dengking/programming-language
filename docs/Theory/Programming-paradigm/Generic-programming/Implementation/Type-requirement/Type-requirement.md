# Type requirement

本节标题的含义是：描述对类型的期望(expectation)或要求(requirement)。在各种programming paradigm，尤其是generic programming，都涉及这个topic，本文对此进行总结。



## Specification of type requirement: `***able`

在阅读

1、[Python doc](https://docs.python.org/3/)

2、[cppreference](https://en.cppreference.com/w/cpp/named_req)

3、Java doc

时，会发现它们都使用"`***able`"来描述对type的期望/要求。在一门语言的language reference中需要详细地说明当对某种type的对象(object)进行操作的时候(比如某个标准库的函数)，期望/要求 它(object) 具备/满足 某些条件(这就是**type requirement**，因为C++、Python都是strong typed language)，因此明确地定义这些**type requirement**，能够使对语言的表述非常便利，清晰，易懂。这些type requirement其实非常能够体现一门语言的核心特性。

### `***able` example

上述语言的normative text of standard中，都使用"`***able`"来描述对type的期望/要求，比如:

1、callable

Python [Callable types](https://docs.python.org/3/reference/datamodel.html)

Java [Interface Callable<V>](https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/Callable.html)

C++ [named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable)

2、iterable

Python [**iterable**](https://docs.python.org/3/glossary.html#term-iterable)

Java [Interface Iterable<T>](https://docs.oracle.com/javase/8/docs/api/java/lang/Iterable.html)

C++？

3、awaitable

Python [Awaitables](https://docs.python.org/3/library/asyncio-task.html#id2)[¶](https://docs.python.org/3/library/asyncio-task.html#awaitables)

Java ?

C++ [Coroutines (C++20)](https://en.cppreference.com/w/cpp/language/coroutines)

4、comparable

Python `object.__lt__`(*self*, *other*)[¶](https://docs.python.org/3/reference/datamodel.html#object.__lt__)

Java [Interface Comparable<T>](https://docs.oracle.com/javase/8/docs/api/java/lang/Comparable.html)

C++ overload operator



#### More

1、C++ cppreference [C++ Named requirements](https://en.cppreference.com/w/cpp/named_req)

2、Python able

[iterable](https://docs.python.org/3/glossary.html#term-iterable)

[asynchronous iterable](https://docs.python.org/3/glossary.html#term-asynchronous-iterable)

[awaitable](https://docs.python.org/3/glossary.html#term-awaitable) 

[hashable](https://docs.python.org/3/glossary.html#term-hashable)

[immutable](https://docs.python.org/3/glossary.html#term-immutable)

[mutable](https://docs.python.org/3/glossary.html#term-mutable)

executable 

callable 

`C++`通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来定义这些特性，Python中也有类似的概念，但是貌似python并没有像`C++`这样进行显式地定义。



## Generic programming

Python通过duck type来实现generic programming，duck type是python的核心，python的很多standard library都是建立在这个机制上，python通过`able`来define the expectations of type。

`C++`通过template来实现generic programming，template是c++的核心，C++的很多standard library都是建立在此机制上，C++通过[named requirement](https://en.cppreference.com/w/cpp/named_req)来define the expectations of type。

Java通过interface、generics来实现generic programming，Java的很多standard library都是建立在此机制上，Java通过generics来define the expectations of type。

通过上面的分析，我们已经知道Python able、C++ named requirement、Java generics本质上是相同的东西: specification of type requirement。

Python standard library、C++ standard library的设计是一种典范，值的借鉴学习。

## Type requirement is more about behavior-based

本节标题的意思是: type requirement往往是**behavior-based**。在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`中，提出了"Polymorphism is more about behavior-based"，本节标题的含义与此类似；这是我的经验之谈，主要源于:

1、Python、C++、Java都是让user-defined class通过实现**magic function**来为这个**类型**添加某种**特性**/**behavior**，从而使它满足type requirement，所以在学习时，需要将**feature**/**behavior**和对应的**magic function**关联起来。所谓**magic function**，其实就是实现了特定功能的函数，在上层函数中，这个函数会被调用。比如OOP中的virtual function。



## Implementation

不同的programming language，不同的programming paradigm，对于specification of type requirement的实现方式是不同的，因此无法进行全面的、统一的描述，下面以programming paradigm分类来进行描述。

### Class-based OOP

在class-based OOP语言，**类**可以看做是一种**类型**。Class-based OOP中具备inheritance、implement(Java)关系的class，往往具备相同的behavior，能够满足相同的type requirement。

参见: 

1) wikipedia [Protocol (object-oriented programming)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming))

2) wikipedia [Interface (Java)](https://en.wikipedia.org/wiki/Interface_(Java))

下面是一些example:

1) wikipedia [Bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification)

```java
interface Comparable<T> {
  public int compareTo(T other);
}

class Integer implements Comparable<Integer> {
  @Override
  public int compareTo(Integer other) {
    //...
  }
}

class String implements Comparable<String> {
  @Override
  public int compareTo(String other) {
    //...
  }
}

class Test {
  public static void main(String[] args) {
    Comparable<String> a = min("cat", "dog");
    Comparable<Integer> b = min(new Integer(10), new Integer(3));
    String str = Fmin("cat", "dog");
    Integer i = Fmin(new Integer(10), new Integer(3));
  }
  public static <S extends Comparable> S min(S a, S b) {
    if (a.compareTo(b) <= 0)
      return a;
    else
      return b;
  }
  public static <T extends Comparable<T>> T Fmin(T a, T b) {
    if (a.compareTo(b) <= 0)
      return a;
    else
      return b;
  }
}
```



### Generic programming(GP) 

Generic programming中使用concept来specify type requirement，关于此，参见`Theory\Programming-paradigm\Generic-programming`。

参见: wikipedia [Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming))

### Python duck type

Duck type可以看做是GP的一种实现。

### Generics

通过[Bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification)来实现，参见`Theory\Programming-paradigm\Generic-programming\Generics`章节。



## [Design by contact](https://en.wikipedia.org/wiki/Design_by_contract) and type requirement



如果从[Design by contact](https://en.wikipedia.org/wiki/Design_by_contract)的角度来看的话，上面所说的expectation就是一种contact，programmer只有遵循了这个contact才能够正确地使用standard library。








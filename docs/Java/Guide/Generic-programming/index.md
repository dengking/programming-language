# Java generic programming

在`Theory\Programming-paradigm\Generic-programming`章节中阐述了generic programming的核心思想，其中非常重要的一点是实现concept，并且基于concept来设计algorithm。后来无意中，阅读了Java的"interface as method parameter"相关的文章，对Java generic programming有了一个新的认知，下面首先描述这两篇文章，然后对Java generic programming进行总结。



## Java generic programming 

### Java interface

我是通过C++ template来学习generic programming的，在阅读了"Interface as method parameter"章节的example后，我的想法是: 

1、如果使用C++ template来实现上述"Interface as method parameter"章节的example，会发现Java interface相当于C++ template parameter，如果我们使用类比思维来进行分析的话，可以发现: 上述example也是符合generic programming的，因此: 

a、Java interface特性让programmer能够explicit、formal描述interface

b、Java通过interface来进行generic programming，java是interface是完全behavior-based的；

c、Java interface能够让programmer来实现concept。

d、相比于C++ template behavior based的implicit，java interface是explicit。

关于我的这个观点，在 wikipedia [Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming)) 中有证据:

> As generics in [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) have some similarities to C++'s [templates](https://en.wikipedia.org/wiki/Template_(C%2B%2B)), the role of concepts there is played by [interfaces](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)).
>
> > NOTE: 上面这段话中的concept指的是[Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming)) 。

关于Java interface，参见:

1、wikipedia [Interface (Java)](https://en.wikipedia.org/wiki/Interface_(Java))

### Java Generics

仅仅使用Java interface还不足以完全地实现generic programming，Java还有generics特性，借助这个特性，Java能够更加完整的实现generic programming。

关于Java generics，参见:

1. wikipedia [Generics in Java](https://en.wikipedia.org/wiki/Generics_in_Java)

2. [Java tutorial Generics](https://docs.oracle.com/javase/tutorial/java/generics/index.html) 



## C++ VS Java: generic programming

1、Java的generic programming可以使用Java interface，Java interface是对behavior的formal description，Java interface是完全behavior-based的；C++ template也是behavior based的，但是它的interface是implicit，java interface是explicit。C++ concept相比之下，不似JAVA那般直观、容易理解




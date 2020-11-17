# Concept (generic programming)



## What is concept in generic programming

在前文中(wikipedia [Generic programming](https://en.wikipedia.org/wiki/Generic_programming))中，我们已经知道:

> **fundamental requirements on types** are **abstracted** from across concrete examples of algorithms and data structures and formalized as [concepts](https://en.wikipedia.org/wiki/Concept_(generic_programming)), with [generic functions](https://en.wikipedia.org/wiki/Generic_function) implemented in terms of these concepts

显然，concept就是对requirement on type的formal description。



在[C++ Type Traits](https://www.drdobbs.com/cpp/c-type-traits/184404270)中有这样的描述：

> Generic programming -- that is, writing code that works with any data type meeting a set of requirements -- has become the method of choice for delivering reusable code. 

这段话引发了我思考一个问题：在generic programming中，如何来描述requirement呢？在维基百科[Generic programming](https://en.wikipedia.org/wiki/Generic_programming)的第三段中就给出了这个问题的答案：

> fundamental requirements on types are abstracted from across concrete examples of algorithms and data structures and formalized as [concepts](https://en.wikipedia.org/wiki/Concept_(generic_programming))

即[concepts](https://en.wikipedia.org/wiki/Concept_(generic_programming))。

concept是generic programming的重要组成部分

### Generic programming and design by contact

Requirement其实就是一种contact。



## wikipedia [Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming))

In [generic programming](https://en.wikipedia.org/wiki/Generic_programming), a **concept** is a **description of supported operations** on a type, including syntax and semantics. In this way, concepts are related to [abstract types](https://en.wikipedia.org/wiki/Abstract_type) but concepts do not require a **subtype relationship**.

> NOTE: "**description of supported operations** "表明concept是behavior-based，关于behavior-based，参见`./Templates-and-Duck-Typing`。

### Language use

#### C++

In the [C++](https://en.wikipedia.org/wiki/C%2B%2B) 1998 standard, the *Concept* term was introduced to name just a simple description of the requirements for particular type, usually being a template parameter. It was not encoded in the language explicitly – the concept was expressed only by what operations are attempted on objects of that type and what is expected to work (that is, to compile correctly). There was a proposal to add [concepts](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)) as an explicit language feature in [C++11](https://en.wikipedia.org/wiki/C%2B%2B11), though it was rejected as "not ready".

#### Java and C`#`

As generics in [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) have some similarities to C++'s [templates](https://en.wikipedia.org/wiki/Template_(C%2B%2B)), the role of concepts there is played by [interfaces](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)).
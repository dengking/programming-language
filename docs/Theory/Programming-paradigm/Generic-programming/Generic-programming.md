# Generic programming

如何使代码更加地generic，即更加地通用？这是所有的programming language都需要考虑的一个问题，`c++`中通过template来实现generic，python中的duck type也能够使程序更加地generic，需要注意的是，严格说来，这里所说的generic并不是本文接下来要讨论的generic programming。我是思考“如何使代码更加地generic”而想到generic programming的。[Generic programming](https://en.wikipedia.org/wiki/Generic_programming)的思想是抽象出类型而是程序更加地generic，更多细节将再下面这篇文章中介绍。

## 维基百科[Generic programming](https://en.wikipedia.org/wiki/Generic_programming)

**Generic programming** is a style of [computer programming](https://en.wikipedia.org/wiki/Computer_programming) in which [algorithms](https://en.wikipedia.org/wiki/Algorithm) are written in terms of [types](https://en.wikipedia.org/wiki/Data_type) *to-be-specified-later* that are then *instantiated* when needed for specific types provided as [parameters](https://en.wikipedia.org/wiki/Parameter_(computer_programming)). 

> NOTE: c++ 的template就是典型的案例

This approach, pioneered by [ML](https://en.wikipedia.org/wiki/ML_(programming_language)) in 1973,[[1\]](https://en.wikipedia.org/wiki/Generic_programming#cite_note-Lee2008-1)[[2\]](https://en.wikipedia.org/wiki/Generic_programming#cite_note-2) permits writing common [functions](https://en.wikipedia.org/wiki/Function_(computer_science)) or [types](https://en.wikipedia.org/wiki/Type_(computer_science)) that differ only in the set of types on which they operate when used, thus reducing [duplication](https://en.wikipedia.org/wiki/Duplicate_code). 

Such software entities are known as *generics* in [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), [Ada](https://en.wikipedia.org/wiki/Ada_(programming_language)), [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), [Delphi](https://en.wikipedia.org/wiki/Delphi_(programming_language)), [Eiffel](https://en.wikipedia.org/wiki/Eiffel_(programming_language)), [F#](https://en.wikipedia.org/wiki/F_Sharp_(programming_language)), [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), [Rust](https://en.wikipedia.org/wiki/Rust_(programming_language)), [Swift](https://en.wikipedia.org/wiki/Swift_(programming_language)), [TypeScript](https://en.wikipedia.org/wiki/TypeScript) and [Visual Basic .NET](https://en.wikipedia.org/wiki/Visual_Basic_.NET). 

They are known as *[parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)* in [ML](https://en.wikipedia.org/wiki/ML_(programming_language)), [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)), [Julia](https://en.wikipedia.org/wiki/Julia_(programming_language)), and [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)) (the Haskell community also uses the term "generic" for a related but somewhat different concept); *[templates](https://en.wikipedia.org/wiki/Template_(C%2B%2B))* in [C++](https://en.wikipedia.org/wiki/C%2B%2B) and [D](https://en.wikipedia.org/wiki/D_(programming_language)); 

and *parameterized types* in the influential 1994 book *[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns_(book))*.[[3\]](https://en.wikipedia.org/wiki/Generic_programming#cite_note-GoF-3)

> NOTE: 通过上面四段话，我们可以看到，同一个概念在不同的领域有着不同的名称，其实它们表达了相同的内涵。



The term "generic programming" was originally coined by [David Musser](https://en.wikipedia.org/wiki/David_Musser) and [Alexander Stepanov](https://en.wikipedia.org/wiki/Alexander_Stepanov)[[4\]](https://en.wikipedia.org/wiki/Generic_programming#cite_note-FOOTNOTEMusserStepanov1989-4) in a more specific sense than the above, to describe a programming paradigm whereby fundamental requirements on types are abstracted from across concrete examples of algorithms and data structures and formalized as [concepts](https://en.wikipedia.org/wiki/Concept_(generic_programming)), with [generic functions](https://en.wikipedia.org/wiki/Generic_function) implemented in terms of these concepts, typically using language genericity mechanisms as described above.

> NOTE: 

## Generic programming and polymorphism



Generic 的实现是依赖于polymorphism的



## Generic programming and reusable code 

在[C++ Type Traits](https://www.drdobbs.com/cpp/c-type-traits/184404270)中有这样的描述：

> Generic programming -- that is, writing code that works with any data type meeting a set of requirements -- has become the method of choice for delivering reusable code. 

这段话提示我们：generic programming是实现reusable code的一种方法

## Generic programming and requirements 

在[C++ Type Traits](https://www.drdobbs.com/cpp/c-type-traits/184404270)中有这样的描述：

> Generic programming -- that is, writing code that works with any data type meeting a set of requirements -- has become the method of choice for delivering reusable code. 

这段话引发了我思考一个问题：在generic programming中，如何来描述requirement呢？不同的programming language有着不同的实现。

### Generic programming and design by contact

requirement其实就是一种contact。



## Example of generic programming

### C++

#### [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library)



#### [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/)
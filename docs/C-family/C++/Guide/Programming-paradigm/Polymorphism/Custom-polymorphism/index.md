# Custom polymorphism、multiple dispatch

起因: 在阅读 stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 时，它的comment中，有这样的描述:

> Side note: the pattern used i.a. by `Boost.Any` is called the external polymorphism pattern – [wmamrak](https://stackoverflow.com/users/469659/wmamrak) [Dec 4](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment18828402_5450159)

其中提及了external polymorphism，于是我检索了一下，检索结果中包含 wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism) ，这篇文章总结地不错

1、C++语言原生并不不支持multiple dispatch，有的时候，需要进行multiple dispatch，因此一个思路就是: custom polymorphism；但是，对于通过custom polymorphism来实现multiple dispatch的方式，有的人是反对的，参见 `C++\Pattern\Visitor-pattern` 章节，其中对此进行了讨论。

2、在一些特殊的情况下，需要custom polymorphism来实现某些特殊目的

3、本章的目的是并不是讨论custom polymorphism的好坏，而是讨论如何实现custom polymorphism。





## 术语: External Polymorphism and InternalPolymorphism

Google了一下


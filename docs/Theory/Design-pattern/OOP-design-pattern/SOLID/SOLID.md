# SOLID



## 维基百科[SOLID](https://en.wikipedia.org/wiki/SOLID)

### Concepts

#### [Single responsibility principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)

> 单一职能原则

a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) should have only a single **responsibility** (i.e. changes to only one part of the software's specification should be able to affect the specification of the class).

> NOTE: [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy)中有一条为:
> Make each program do one thing well. To do a new job, build afresh rather than complicate old programs by adding new "features".
> Mike Gancarz所著的《Linux and the Unix Philosophy》也对[该哲学](#http://read.pudn.com/downloads63/ebook/222048/Linux%20and%20the%20Unix%20Philosophy.pdf)进行了解释。
>
> 现在对比来看，其实这两种思想是契合的。

#### [Open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle) 

> 闭开原则

"software entities … should be open for extension, but closed for modification."

#### [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle)

> 里氏替换原则

"objects in a program should be replaceable with instances of their subtypes without altering the correctness of that program." See also [design by contract](https://en.wikipedia.org/wiki/Design_by_contract).

#### [Interface segregation principle](https://en.wikipedia.org/wiki/Interface_segregation_principle) 

"many client-specific interfaces are better than one general-purpose interface." 

#### [Dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle) 

one should "depend upon abstractions, [not] concretions." 

### 深入理解

上述罗列了五大设计原则，但是并没有深入，下面这些是原文的**References**中给出的列表中，我发现的一些比较有价值的内容：

 [Robert C. Martin](https://en.wikipedia.org/wiki/Robert_C._Martin). ["Principles Of OOD"](http://butunclebob.com/ArticleS.UncleBob.PrinciplesOfOod). *butunclebob.com*. Retrieved 2014-07-17.. (Note the reference to “the first five principles”, though the acronym is not used in this article.) Dates back to at least 2003.

## butunclebob [The Principles of OOD](http://butunclebob.com/ArticleS.UncleBob.PrinciplesOfOod)





## 两个 inversion

在面向对象编程的SOID原则中有[Dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)，即依赖反转，今天在阅读[Rabit](https://github.com/dmlc/rabit)的时候，看到了**inversion of control principle**，即控制理论的反转。所以对这个两个**inversion**有些好奇，不知它们是否是基于同一个原则而提出的呢？

### [Inversion of control](https://en.wikipedia.org/wiki/Inversion_of_control) 

### [Dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) 

inject Dependency 注入依赖

replace dependency 替换依赖
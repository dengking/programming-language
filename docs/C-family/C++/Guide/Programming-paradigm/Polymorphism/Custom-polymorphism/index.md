# Custom polymorphism、multiple dispatch

## 起因

在阅读 stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 时，它的comment中，有这样的描述:

> Side note: the pattern used i.a. by `Boost.Any` is called the external polymorphism pattern – [wmamrak](https://stackoverflow.com/users/469659/wmamrak) [Dec 4](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment18828402_5450159)

其中提及了"external polymorphism"，于是我Google了一下，检索结果中包含 wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism) ，这篇文章总结地不错，通过其中的内容，我发现我之前很多的做法其实都可以归为 external polymorphism，显然我们使用 external polymorphism 的目的是为了实现 custom polymorphism，即自定义polymorphism。我又联想到了之前总结的custom virtual table，显然它也是为了实现 custom polymorphism；我又联想到了之前阅读的visitor pattern中实现multiple dispatch，其中是明确反对custom polymorphism、external polymorphism的，因此这就是导致了我编写本文。

## Custom polymorphism

1、C++语言原生并不不支持multiple dispatch，有的时候，需要进行multiple dispatch，因此一个思路就是: custom polymorphism；但是，对于通过custom polymorphism来实现multiple dispatch的方式，有的人是反对的，参见 `C++\Pattern\Visitor-pattern` 章节，其中对此进行了讨论。

2、在一些特殊的情况下，需要custom polymorphism来实现某些特殊目的，正如在  stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 中说明的:

> Side note: the pattern used i.a. by `Boost.Any` is called the external polymorphism pattern – [wmamrak](https://stackoverflow.com/users/469659/wmamrak) [Dec 4](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment18828402_5450159)

3、本章的目的是并不是讨论custom polymorphism的好坏，而是讨论如何实现custom polymorphism。





## 术语: External Polymorphism and InternalPolymorphism

Google了一下，发现下面这篇文章的介绍是比较好的。



### agnihotris [External Polymorphism (Adapter Design pattern) vs InternalPolymorphism](https://agnihotris.wordpress.com/2013/09/22/external-polymorphism-design-pattern-vs-internalpolymorphism/)

**Internal Polymorphism :-** When you call a function based on its dynamic type of objects by using virtual table or vtable . The only methods declared as virtual are polymorphic in c++.

In internal polymorphism function bindings are part of class definition of the object .

> NOTE: internal polymorphism是指C++ programming language内置支持的

**External polymorphism :-** Here functions and their bindings are  defined independently of a class.

This allows classes which are not related by inheritance and/or have no virtual methods to be treated polymorphically. Thus unrelated classes can be treated in common manner by s/w that uses them.

> NOTE: external polymorphism其实是custom polymorphism，是由programmer自己创建的

This pattern capitalizes on both c++ features and on other basic patterns (Adapter or Decorator) to give appearance of polymorphic behavior of otherwise unrelated class.

 
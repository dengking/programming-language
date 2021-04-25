# Custom polymorphism



1、指的是基于if-else的dispatch方式来实现polymorphism，一般这种方式是不推荐的。

2、对于通过custom polymorphism来实现multiple dispatch的方式，有的人是反对的，参见 `C++\Pattern\Visitor-pattern` 章节，其中对此进行了讨论。

3、在一些特殊的情况下，需要custom polymorphism来实现某些特殊目的，正如在  stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 中说明的:

> Side note: the pattern used i.a. by `Boost.Any` is called the external polymorphism pattern – [wmamrak](https://stackoverflow.com/users/469659/wmamrak) [Dec 4](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment18828402_5450159)

4、本章的目的是除了讨论custom polymorphism的好坏，而是讨论如何实现custom polymorphism。



## Custom polymorphism的方式总结



### Concept-based-polymorphism

参见 `Polymorphic-value-and-concept-based-polymorphism` 章节。



### Signature-Based-polymorphism

参见 `Signature-Based-polymorphism` 章节。



### Custom static polymorphism

参见 `Custom-static-polymorphism` 章节。



### Custom virtual table

参见 `TODO-custom-virtual-table` 章节。


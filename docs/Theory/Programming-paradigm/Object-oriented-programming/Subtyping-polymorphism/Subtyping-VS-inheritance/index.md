# Subtyping VS inheritance



## wikipedia [Subtyping](https://en.wikipedia.org/wiki/Subtyping) 

Subtyping should not be confused with the notion of (class or object) [inheritance](https://en.wikipedia.org/wiki/Inheritance_(computer_science)) from object-oriented languages;[[1\]](https://en.wikipedia.org/wiki/Subtyping#cite_note-FOOTNOTECookHillCanning1990-1) subtyping is a relation between types (interfaces in object-oriented parlance) whereas inheritance is a relation between implementations stemming from a language feature that allows new objects to be created from existing ones. In a number of object-oriented languages, subtyping is called **interface inheritance**, with inheritance referred to as *implementation inheritance*.

### Relationship with inheritance

Subtyping and inheritance are independent (orthogonal) relationships. They may coincide(碰巧一致), but none is a special case of the other. In other words, between two types *S* and *T*, all combinations of subtyping and inheritance are possible:

1. *S* is neither a subtype nor a derived type of *T*
2. *S* is a subtype but is not a derived type of *T*
3. *S* is not a subtype but is a derived type of *T*
4. *S* is both a subtype and a derived type of *T*

> NOTE: 上面用到了两个词语: 
>
> subtype: 对应的是subtyping
>
> derived type: 对应的是inheritance

The first case is illustrated by independent types, such as `Boolean` and `Float`.

The second case can be illustrated by the relationship between `Int32` and `Int64`. In most object oriented programming languages, `Int64` are unrelated by inheritance to `Int32`. However `Int32` can be considered a subtype of `Int64` since any 32 bit integer value can be promoted into a 64 bit integer value.

The third case is a consequence of [function subtyping input contravariance](https://en.wikipedia.org/wiki/Subtyping_of_functions). Assume a super class of type *T* having a method *m* returning an object of the same type (*i.e.* the type of *m* is *T → T*, also note that the first argument of *m* is this/self) and a derived class type *S* from *T*. By inheritance, the type of *m* in *S* is *S → S*. In order for *S* to be a subtype of *T* the type of *m* in *S* must be a subtype of the type of *m* in *T*, in other words: *S → S ≤: T → T*. By bottom-up application of the function subtyping rule, this means: *S ≤: T* and *T ≤: S*, which is only possible if *S* and *T* are the same. Since inheritance is an irreflexive relation, *S* can't be a subtype of *T*.

Subtyping and inheritance are compatible when all inherited fields and methods of the derived type have types which are subtypes of the corresponding fields and methods from the inherited type [[1\]](https://en.wikipedia.org/wiki/Subtyping#cite_note-FOOTNOTECookHillCanning1990-1).

## wikipedia [Inheritance (object-oriented programming)](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming))

Inheritance should not be confused with [subtyping](https://en.wikipedia.org/wiki/Subtyping).[[3\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-4) In some languages inheritance and subtyping agree,[[a\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-5) whereas in others they differ; in general, **subtyping** establishes an [is-a](https://en.wikipedia.org/wiki/Is-a) relationship, whereas inheritance only reuses implementation and establishes a **syntactic(语法) relationship**, not necessarily a **semantic(语义) relationship** (inheritance does not ensure [behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping)). To distinguish these concepts, subtyping is also known as *interface inheritance*, whereas inheritance as defined here is known as *implementation inheritance* or *code inheritance*.[[5\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-Mikhajlov-6) Still, inheritance is a commonly used mechanism for establishing subtype relationships.[[6\]](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)#cite_note-7)



## Summary 

需要注意的是，programming language并不是严谨的、严格的学术作品，所以不会严格的区分两者，而是会考虑programmer的学习成本、提供了非常灵活的，多种可能性的实现方式等多重因素来提供实现；因此，在现代programming language中，两者是存在一定的重叠的，但是作为programmer，我们应该要知道两者之间本质的区别。

### 如何区分?

#### Syntactic(语法) relationship VS semantic(语义) relationship

1) 通过inheritance而关联的两个class，建立了**syntactic(语法) relationship**，两者之间可能并没有语义上的subtyping关系；或者说:

2) "inheritance"是很多现代programming language用于实现subtyping的mechanism:

wikipedia [Inheritance (object-oriented programming)](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) 中有这样的描述: "inheritance is a commonly used mechanism for establishing subtype relationships"，这可能是导致很多programmer混淆两者的原因

3) 相比于 inheritance，subtyping更加强调implement interface语义。

#### Implementation

下面从 implementation 的角度来进行分析

> NOTE: 这段分析，是我在阅读 wikipedia [*Modern C++ Design*#Policy-based design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#Policy-based_design) 时，想到的。

Subtyping **override** interface，implementation是由subtype(下)实现/提供，它的主要目的是: subtyping polymorphism。

Inheritance **inherit** implementation，implementation是由derived type(上)实现提供，它的主要目的是code reuse；最能体现此的是Mixin、Policy-based design，在下面进行了介绍。



### 现代programming language的实现

现代programming language是如何来处理subtyping VS inheritance？或者说: 是否区分interface inheritance和implementation/code inheritance: 

区分: Java，Java中提供了inheritance 和 implement 语义，但是C++中没有

未区分: C++，Python



## 体现 inheritance 和 subtyping 差异的example

Mixin和policy-based design都使用了inheritance，但是它们并不是 subtyping ，它们都符合前面"Summary 3)"中描述的内容

### Mixin

参见 `Theory\Programming-paradigm\Object-oriented-programming\Assemble\Mixin` 章节。

### Policy-based design

参见 `C++\Idiom\Generic-programming\Policy-based-design` 章节。






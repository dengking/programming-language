# Subtyping polymorphism 



## wikipedia [Subtyping](https://en.wikipedia.org/wiki/Subtyping)

> NOTE: subtype是事物之间固有的、天然的relation的 [is-a](https://en.wikipedia.org/wiki/Is-a) relation，就像下面的duck和bird之间的relation，作为programmer，我们往往是基于事物的这种固有的、天然的relation来进行model。

[![img](https://upload.wikimedia.org/wikipedia/en/thumb/d/d1/Inheritance.svg/350px-Inheritance.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance.svg)

> Example of subtypes: where bird is the supertype and all others are subtypes as denoted by the arrow in [UML](https://en.wikipedia.org/wiki/Unified_Modeling_Language) notation

In [programming language theory](https://en.wikipedia.org/wiki/Programming_language_theory), **subtyping** (also **subtype polymorphism** or **inclusion polymorphism**) is a form of [type polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) in which a **subtype** is a [datatype](https://en.wikipedia.org/wiki/Datatype) that is related to another datatype (the **supertype**) by some notion of [substitutability](https://en.wikipedia.org/wiki/Liskov_substitution_principle), meaning that program elements, typically [subroutines](https://en.wikipedia.org/wiki/Subroutines) or functions, written to operate on elements of the supertype can also operate on elements of the subtype. If S is a subtype of T, the subtyping [relation](https://en.wikipedia.org/wiki/Binary_relation) is often written S <: T, to mean that any term of type S can be *safely used in a context where* a term of type T is expected. The precise semantics of subtyping crucially(至关重要的) depends on the particulars(细节) of what "safely used in a context where" means in a given [programming language](https://en.wikipedia.org/wiki/Programming_language). The [type system](https://en.wikipedia.org/wiki/Type_system) of a programming language essentially defines its own subtyping relation, which may well be [trivial](https://en.wikipedia.org/wiki/Identity_relation) should the language support no (or very little) conversion mechanisms.

> NOTE: 上述  "[substitutability](https://en.wikipedia.org/wiki/Liskov_substitution_principle)" 所链接的是Liskov substitution principle。

Due to the subtyping relation, a term may belong to more than one type. Subtyping is therefore a form of type polymorphism. In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) the term 'polymorphism' is commonly used to refer solely to this *subtype polymorphism*, while the techniques of [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) would be considered *[generic programming](https://en.wikipedia.org/wiki/Generic_programming)*.

> NOTE: programming paradigm and polymorphism

The concept of subtyping is related to the linguistic notions of [hyponymy](https://en.wikipedia.org/wiki/Hyponymy) and [holonymy](https://en.wikipedia.org/wiki/Holonymy). It is also related to the concept of [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification) in mathematical logic. 

### Subsumption

> NOTE: 意思是: "包含"

### Relationship with inheritance

> NOTE: 放到了`./Subtyping-VS-inheritance`中

## wikipedia [Behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping)



## How to implementation subtyping in OOP?

现代programming language，往往使用inheritance + override 来供programmer使用subtyping polymorphism。

在programming language实现层，不同的programming language使用的实现方式是不同的。

## Subtyping polymorphism is based on behavior

本节标题的是含义是: Subtyping polymorphism是基于behavior的，意思是: Subtyping polymorphism中能够polymorphism的只有behavior，data member是无法polymorphism；关于behavior，参见`Theory\Programming-paradigm\Object-oriented-programming\Behaivor.md`。关于这个观点，下面是例证:

| 文章                                                         |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `C++\Language-reference\Classes\Subtype-polymorphism\index.md` | 在C++，dynamic polymorphism是behavior（function），只有virtual method（virtual table），而不是data/value的 |
| wikipedia [Behavioral subtyping](https://en.wikipedia.org/wiki/Behavioral_subtyping) | TODO                                                         |
|                                                              |                                                              |




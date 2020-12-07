# Bounded quantification

"bounded"的含义是"有界限的"，"quantification"的含义是"量化"。

"bounded quantification"所描述的是在generic programming中，对parameter进行constrain，从下面的介绍可以看出，它是: generic programming + OOP subtyping。

## wikipedia [Bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification)

In [type theory](https://en.wikipedia.org/wiki/Type_theory), **bounded quantification** (also **bounded polymorphism** or **constrained genericity**) refers to [universal](https://en.wikipedia.org/wiki/Universally_quantified) or [existential quantifiers](https://en.wikipedia.org/wiki/Existential_type) which are restricted ("bounded") to range only over the subtypes of a particular type. Bounded quantification is an interaction of [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) with [subtyping](https://en.wikipedia.org/wiki/Subtyping). Bounded quantification has traditionally been studied in the [functional](https://en.wikipedia.org/wiki/Functional_programming) setting of [System F<:](https://en.wikipedia.org/wiki/System_F-sub), but is available in modern [object-oriented languages](https://en.wikipedia.org/wiki/Object-oriented_language) supporting [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) ([generics](https://en.wikipedia.org/wiki/Generic_programming)) such as [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) and [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)).

> NOTE: 相比之下，**constrained genericity** 是更加容易理解的，它的字面意思是: 受限的genericity，它能够更加直接地传递出 bounded quantification的含义；
>
> 从上面的描述能够看出: 
>
> bounded quantification是"[parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) with [subtyping](https://en.wikipedia.org/wiki/Subtyping)"。

### Overview


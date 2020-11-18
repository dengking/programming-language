# Type constructor

在阅读wikipedia [Covariance and contravariance (computer science)](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)) 时，其中有很多关于 type constructor的描述，比如:

> Depending on the **variance** of the [type constructor](https://en.wikipedia.org/wiki/Type_constructor), the subtyping relation of the simple types may be either preserved, reversed, or ignored for the respective complex types.

> A programming language designer will consider variance when devising(设计) typing rules for language features such as arrays, inheritance, and [generic datatypes](https://en.wikipedia.org/wiki/Generic_datatype). By making **type constructors** **covariant** or **contravariant** instead of **invariant**, more programs will be accepted as well-typed. 

通过 wikipedia [Covariance and contravariance (computer science)](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)) 中的内容，我对 Type constructor 的直观认知是: 

1) 它让programmer基于现有 type 来构造新的 type

2) 通过type constructor，programmer可以构建新的type，从而扩展programming language的现有的type。

现代programming language普遍支持各种各样的type constructor，这是programmer需要了解的，并且与type constructor的variance属性是非常重要的内容，需要programmer进行了解。本章将对type constructor以及它的variance属性进行深入分析。

## Wikipedia [Type constructor](https://infogalactic.com/info/Type_constructor)

In the area of [mathematical logic](https://infogalactic.com/info/Mathematical_logic) and [computer science](https://infogalactic.com/info/Computer_science) known as [type theory](https://infogalactic.com/info/Type_theory), a **type constructor** is a feature of a typed [formal language](https://infogalactic.com/info/Formal_language) that builds new types from old ones. Typical type constructors encountered are [product types](https://infogalactic.com/info/Product_type), [function types](https://infogalactic.com/info/Function_type), power types and [list types](https://infogalactic.com/info/List_type). [Basic types](https://infogalactic.com/info/Basic_type) are considered [nullary](https://infogalactic.com/info/Nullary) (0元)type constructors. New types can be defined by recursively composing type constructors.

### See also

- [Algebraic data type](https://infogalactic.com/info/Algebraic_data_type)
- [Recursive data type](https://infogalactic.com/info/Recursive_data_type)
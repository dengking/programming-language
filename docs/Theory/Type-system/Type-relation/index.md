# Type relation

Type relation是programming language的type system的一个非常重要的内容，programming language designer需要进行精准定义；

## Subtype relation

subtype是最最典型的type relation，它是OOP中的核心话题，OOP type之间的relation是相对简单的，inheritance则对应了subtype/subclass。与subtype relation相关的词语有:

1) generic and specific

在wikipedia [Covariance and contravariance (computer science)](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science))中:

> - *covariant* if it preserves the [ordering of types (≤)](https://en.wikipedia.org/wiki/Subtype), which orders types from more specific to more generic;

2) wide and narrow

在wikipedia [Covariant return type](https://en.wikipedia.org/wiki/Covariant_return_type)中:

> More specifically, covariant (wide to narrower) or contravariant (narrow to wider) return type......

关于由type constructor构造的complex type之间的subtype relation规则，在`./Covariance-and-contravariance`中进行了描述。

## Type ordering

可以基于type relation对type进行ordering，现代programming language的很多特性的实现都是建立在type ordering的基础之上的:

1) polymorphism dispatch

现代programming language的dispatch是建立在type ordering的基础上的。

于此相关的有:

- C++ overload resolution，参见`C++\Language-reference\Functions\Function-overload\Overload-resolution`章节
- 

> NOTE: 只有ordering之后，才能够基于type进行computation，参见`Relation-structure-computation\Make-it-computational`。



## Type hierarchy

基于subtype relation可以构建type hierarchy。
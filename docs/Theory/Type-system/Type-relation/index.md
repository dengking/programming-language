# Type relation

Type relation是programming language的type system的一个非常重要的内容，programming language designer需要进行精准定义；

## Subtype relation

subtype是最最典型的type relation，它是OOP中的核心话题，OOP type之间的relation是相对简单的，inheritance则对应了subtype/subclass。与subtype relation相关的词语为generic、specific。

关于由type constructor构造的complex type之间的subtype relation规则，在`./Covariance-and-contravariance`中进行了描述。

## Type ordering

可以基于type relation对type进行ordering，现代programming language的很多特性的实现都是建立在type ordering的基础之上的:

1) polymorphism dispatch

现代programming language的dispatch是建立在type ordering的基础上的。

于此相关的有:

- C++ overload resolution
- 





## Type hierarchy

基于subtype relation可以构建type hierarchy。
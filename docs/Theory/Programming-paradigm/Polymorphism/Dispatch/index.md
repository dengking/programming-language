## dispatch的含义

在polymorphism中，dispatch是一个出现频率非常高的词语；那dispatch的含义是什么呢？在polymorphism中，另外一个出现频率比较高的词语是substitution，以substitution为对照，理解dispatch就比较容易一些:

dispatch描述的是method dispatch、method selection，在wikipedia [Static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) 中有如下描述:

> It is a form of *method dispatch,* which describes how a language or environment will select which implementation of a method or function to use.

"一种抽象多种实现"，dispatch是指在这些implementations中选择一个。

## substitution的含义

一个抽象符号，可以被多种复合条件的具体符号替换。



不同的语言、不同的polymorphism，实现dispatch的方式是不同的，dispatch的对象也不同:

[**Ad hoc polymorphism**](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) 仅仅涉及 method selection。

[**Subtyping**](https://en.wikipedia.org/wiki/Subtyping) 涉及 substitution（里氏替换法则） 和 method selection(dispatch)。

[**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) 涉及 substitution 和 method selection。



dynamic dispatch所关注的是method selection










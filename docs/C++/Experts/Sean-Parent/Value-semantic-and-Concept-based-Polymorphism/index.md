# 关于本章

本章以 Sean Parent [Value Semantics and Concept-based Polymorphism](https://sean-parent.stlab.cc/papers-and-presentations/#value-semantics-and-concept-based-polymorphism) 的论文为主，学习其提出的technique，然后进行一些拓展、补充；

本章首先 阅读 Sean Parent  [Value Semantics and Concept-based Polymorphism](https://sean-parent.stlab.cc/papers-and-presentations/#value-semantics-and-concept-based-polymorphism) 的论文，然后 学习它的 implementation，然后进行一些拓展、补充。

后续为了描述便利，将这种technique称为"Polymorphic Value Types"。

## Polymorphic Value Types and type erasure

1、Polymorphic Value Types 的实现应用了 type erasure

2、Polymorphic Value Types 站在了更高的角度
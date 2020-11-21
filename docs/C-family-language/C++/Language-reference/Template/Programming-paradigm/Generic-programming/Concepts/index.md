# 关于本章

在[generic programming](https://en.wikipedia.org/wiki/Generic_programming)中，就有以及提出了“[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))”的概念，但是c++知道c++20版本才显式地支持它了，在之前的版本，用户只能够通过`SFINAE`和`enable_if`来间接地表达它。

## 为什么添加concept特性？

在`Theory\Programming-languageDesign-of-programming-language.md#What's new in the programming language`中，提出了“首先搞清楚设计者引入这些新特性的意图、这些新特性所能够解决的问题，然后再去了解它们的grammar”，所以在学习concept特性的grammar之前，我们实现要搞清楚c++的designer引入这个feature的原因。关于这个问题，我首先从generic programming的角度来进行分析，我觉得这个角度是最高的、最本质的，后面会结合具体的问题来进行分析。

### Complete support for generic programming

阅读维基百科[generic programming](https://en.wikipedia.org/wiki/Generic_programming)，我们会发现，[generic programming](https://en.wikipedia.org/wiki/Generic_programming)中已经提出了[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))概念，[concept](https://en.wikipedia.org/wiki/Concept_(generic_programming))是完整地实现generic programming所必须的，关于这一点，在[Generic Programming in ConceptC++](http://www.generic-programming.org/languages/conceptcpp.html)中进行了非常详细地说明，下面截取了其中一部分：

> ConceptC++ is an extension to the C++ Programming Language that introduces first-class support for Generic Programming. It is the first attempt to introduce complete support for Generic Programming into a mainstream programming language. ConceptC++ is a strong candidate for inclusion into the upcoming ANSI/ISO C++ Standard, dubbed C++0x. 



c++20之前，这个语言并没有显式定义concept的语法，c++20引入了这个feature，允许用户显式地定义concept。
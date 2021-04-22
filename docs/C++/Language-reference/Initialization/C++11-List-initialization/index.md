# Uniform initialization/List initialization (since C++11)



## What's in c++: Uniform initialization/List initialization (since C++11)

按照 `C++\Language-reference\Expressions\Constant-expressions` 中总结的经验:

> 应该是先阅读what's new in c++（一般是在这个特性的提案中描述这部分内容， 但是提案往往非常漫长，最快捷的方式是阅读维基百科中所总结的），先搞清楚引入这些feature的意图、application，然后再去了解这些feature的grammar等详情。

首先讨论了为什么C++11引入uniform initialization特性。

“uniform initialization”正如其名称所揭示的：它让用户可以以非常uniform（统一的）的grammar来描述各种各样的initialization，这种uniform，是符合 `Uniform-consistent-interface-API-syntax`章节章节的思想的，它带来的价值是：

| 价值                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| 简单易用、消除了syntax ambiguity、进行了simplify             |                                                              |
| 统一了syntax，促进generic programming，强化了对template meta-programming的支持 | 在akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)中对此进行了深入说明 |

> uniform initialization其实和“一致性接口”背后的思想是相同的。



### akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)

1、这篇文章非常好，它基本上讲清楚了为什么需要uniform initialization。

2、遵循 CppCoreGuidelines [ES.20: Always initialize an object](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-always)，阅读完成后，我的一个想法是: 我们总是需要value initialization，C++11 的 uniform initialization `{}` 让我们以uniform、consistent syntax来实现 value initialization。

### `Syntax-ambiguity-in-initialization-with-round-parenthese`

Uniform initialization/List initialization (since C++11) 就是为了解决 syntax ambiguity。

### wikipedia [C++11#Uniform initialization](https://en.wikipedia.org/wiki/C%2B%2B11#Uniform_initialization)



### CppCoreGuidelines [T.68: Use `{}` rather than `()` within templates to avoid ambiguities](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t68-use--rather-than--within-templates-to-avoid-ambiguities)





### TODO

stackexchange [Is C++11 Uniform Initialization a replacement for the old style syntax?](https://softwareengineering.stackexchange.com/questions/133688/is-c11-uniform-initialization-a-replacement-for-the-old-style-syntax)

geeksforgeeks [Uniform Initialization in C++](https://www.geeksforgeeks.org/uniform-initialization-in-c/)

维基百科[C++11#Uniform initialization](https://en.wikipedia.org/wiki/C++11#Uniform_initialization)

isocpp Uniform initialization syntax and semantics [¶](https://isocpp.org/wiki/faq/cpp11-language#uniform-init) [Δ](https://isocpp.org/wiki/faq/cpp11-language#)

stackoverflow [Uniform initialization of references](https://stackoverflow.com/questions/6546470/uniform-initialization-of-references)

microsoft Microsoft [Brace initialization](https://docs.microsoft.com/en-us/cpp/cpp/initializing-classes-and-structs-without-constructors-cpp?view=vs-2019)


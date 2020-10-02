# Uniform initialization

按照 `C++\Language-reference\Expressions\Constant-expressions` 中总结的经验: 应该是先阅读what's new in c++（一般是在这个特性的提案中描述这部分内容， 但是提案往往非常漫长，最快捷的方式是阅读维基百科中所总结的），先搞清楚引入这些feature的意图、application，然后再去了解这些feature的grammar等详情。

本节奇怪的是，在cppreference [C++11](https://en.cppreference.com/w/cpp/11)中并没有找到Uniform initialization的介绍，不确定 "brace-or-equal initializers" 是否就是uniform initialization。

## TODO: What's in c++: Uniform initialization



### wikipedia [C++11#Uniform initialization](https://en.wikipedia.org/wiki/C%2B%2B11#Uniform_initialization)



### akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)

> NOTE: `C++\Language-reference\Initialization\Value-initialization.md`中收录了这篇文章。

### Summary

“uniform initialization”正如其名称所揭示的：它让用户可以以非常uniform（统一的）的grammar来描述各种各样的initialization，这种uniform，带来的价值是：

| 价值                                    | 说明                                                         |
| --------------------------------------- | ------------------------------------------------------------ |
| 简单易用                                |                                                              |
| 强化了对template meta-programming的支持 | 在akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)中对此进行了深入说明 |



uniform initialization其实和“一致性接口”背后的思想是相同的。


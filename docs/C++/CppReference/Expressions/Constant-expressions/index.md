# 关于本章

本章描述的是c++中的constant expression。本章首先描述constant expression概念，然后描述C++11中引入的`constexpr`，对这两者的介绍都是基于cppreference的，cppreference的对两者的讲解中会涉及到彼此，所以需要两者一起阅读。

传统的constant expression（比如`const int i = 0`）是非常容易理解的，但是从C++11开始，对这个概念进行了generalize（推广），引入了[`constexp`](https://en.cppreference.com/w/cpp/language/constexpr) 、[`consteval`](https://en.cppreference.com/w/cpp/language/consteval) ，显然越推广就越抽象，越难以理解，所以我第一次直接阅读cppreference的时候，并没有抓住引入这些新feature的意图所在，而是直接阅读cppreference中介绍这个语言特性的各种详细细节，花费了很多时间，但是最终还是没有完全搞清楚这个特性所解决的问题，这种做法是非常愚蠢的。应该是先阅读what's new in C++（一般是在这个特性的提案中描述这部分内容， 但是提案往往非常漫长，最快捷的方式是阅读维基百科中所总结的），先搞清楚引入这些feature的意图、application，然后再去了解这些feature的grammar等详情。



### SUMMARY

C++11 `constexpr`和C++20 `consteval`的引入，是为了强化C++的[Compile time function execution](https://en.wikipedia.org/wiki/Compile_time_function_execution)，关于C++中的compile-time context、compile time function execution，参见

1) `Compile-time&&run-time\Compile-time-computation`




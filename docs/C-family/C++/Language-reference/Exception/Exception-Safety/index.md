# Exception safety

本章讨论Exception safety，会对各种等级的exception safety分章节进行介绍，同时还会讨论如何实现各种等级的exception safety。

## 为什么我们需要考虑exception safety?

1、resource management

避免leakage2

2、predictable、computational

关于这个观点，在下面文章中，有介绍:

a、boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)

## Guide

关于exception safety的一些非常好的文章，这些文章深入浅出，非常适合阅读。

1、boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)

2、wikipedia [Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

3、microsoft [How to: Design for exception safety](https://docs.microsoft.com/en-us/cpp/cpp/how-to-design-for-exception-safety?view=vs-2019)



## How to improve exception safety? 

我们需要考虑的是如何来improve exception safety。

### Predo

#### Preallocate

##### Copy and swap idiom

Copy and swap idiom能够提供strong exception safety:

> Get the new data ready before we replace the old

即在修改正确，先将需要的resource准备好。

参见 `C++\Idiom\OOP\Copy-and-swap` 章节。

##### Placement new



#### Temporary

将file copy到temporary，参见 `C++\Idiom\OOP\Scope-Guard\Declarative-Control-Flow` 章节。



### Stack unwind

参见`C++\Idiom\OOP\Scope-Guard`章节 。


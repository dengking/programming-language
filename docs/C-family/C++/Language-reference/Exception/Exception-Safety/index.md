# Exception safety

本章讨论Exception safety，会对各种等级的exception safety分章节进行介绍，同时还会讨论如何实现各种等级的exception safety。

## 为什么我们需要考虑exception safety?

1、resource management

避免leakage

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



## Implementation of strong guarantee may depend on stronger guarantee

本节标题的含义是: 实现 "strong guarantee" 可能依赖于 "stronger guarantee"，这是我在阅读 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get? "时，根据 "First, the guarantee specified for the composite container actually depends on stronger guarantees from its components (the *no-throw* guarantees in line 11)" 总结而来的，后来经过仔细思考，发现这是一个普遍的规律，它提醒了我:

1、高层是需要底层的exception safety来实现exception safety的

2、no throw guarantee的重要价值

它是可靠的、它是最最强的guarantee、可以依靠它来实现strong exception safety

### Examples

1、copy-and-swap: swap是no throw的，这个idiom依赖于此，实现了strong exception safety

2、"boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get? ": 其中的`SearchableStack<T>::push(const T& t)`例子，能够非常好的说明这个规律




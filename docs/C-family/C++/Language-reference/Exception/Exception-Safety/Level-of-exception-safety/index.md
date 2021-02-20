# Level of exception safety

C++ 的 exception safety 是典型的分为多个等级的，关于level，参见文章 《Level-in-computer-science》。

## Natural exception safety/天花板

这是在 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)" 中给出的一个概念。

1、我们总是期望更高级别的exception safety，但是需要清楚的是: 一些operation的exception safety可能无法提高，它是natural的，典型的例子是:

a、很多operation都是无法实现最高级别的exception safety: no throw guarantee

b、在 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) #  5 What level of exception-safety should a component specify?" 中给出了这样的说明 

## Performance and exception safety tradeoff

这是在阅读 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 5 What level of exception-safety should a component specify?" 时悟出的。在下面文章中，也讨论这个话题:

1、"boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get?"

> Faster but less-safe implementations could always be provided as extensions to the standard components

## Implementation of strong guarantee may depend on stronger guarantee

本节标题的含义是: 实现 "strong guarantee" 可能依赖于 "stronger guarantee"，这是我在阅读 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get? "时，根据 "First, the guarantee specified for the composite container actually depends on stronger guarantees from its components (the *no-throw* guarantees in line 11)" 总结而来的，后来经过仔细思考，发现这是一个普遍的规律，它提醒了我:

1、高层是需要底层的exception safety来实现exception safety的

2、no throw guarantee的重要价值

它是可靠的、它是最最强的guarantee、可以依靠它来实现strong exception safety

### Examples

下面是沿着本节观点的例子: 

1、copy-and-swap: swap是no throw的，这个idiom依赖于此，实现了strong exception safety

2、"boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get? ": 其中的`SearchableStack<T>::push(const T& t)`例子，能够非常好的说明这个规律




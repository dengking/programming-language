# Delegation 

“delegation ”即“委托、授权”。

## 维基百科[Delegation (computing)](https://en.wikipedia.org/wiki/Delegation_(computing))

> NOTE: 这篇对delegation的总结是比较全面的

[Delegation (object-oriented programming)](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)), evaluating a member of one object (the receiver) in the context of another, original object (the sender).

- [Delegation pattern](https://en.wikipedia.org/wiki/Delegation_pattern), a design pattern implementing this feature.
- [Forwarding (object-oriented programming)](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming))
- [Object aggregation](https://en.wikipedia.org/wiki/Object_aggregation)



## wikipedia [Delegation (object-oriented programming)](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming))

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), **delegation** refers to evaluating a member (property or method) of one object (the receiver) in the context of another original object (the **sender**). **Delegation** can be done explicitly, by passing the **sending object** to the **receiving object**, which can be done in any [object-oriented language](https://en.wikipedia.org/wiki/Object-oriented_language); or implicitly, by the **member lookup rules** of the language, which requires language support for the feature. **Implicit delegation** is the fundamental method for **behavior reuse** in [prototype-based programming](https://en.wikipedia.org/wiki/Prototype-based_programming), corresponding to [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) in [class-based programming](https://en.wikipedia.org/wiki/Class-based_programming). The best-known languages that support delegation at the language level are [Self](https://en.wikipedia.org/wiki/Self_(programming_language)), which incorporates（合并） the notion of **delegation** through its notion of **mutable parent slots** that are used upon method lookup on self calls, and [JavaScript](https://en.wikipedia.org/wiki/JavaScript); see [JavaScript delegation](https://en.wikipedia.org/wiki/JavaScript#Delegative).

> NOTE: 上面是使用类比方法来描述delegation的:
>
> |                    | [prototype-based programming](https://en.wikipedia.org/wiki/Prototype-based_programming) | [class-based programming](https://en.wikipedia.org/wiki/Class-based_programming) |
> | ------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | **behavior reuse** | delegation                                                   | inheritance                                                  |
> |                    | sender、receiver                                             | parent、child                                                |
> |                    |                                                              |                                                              |
>
> 在[class-based programming](https://en.wikipedia.org/wiki/Class-based_programming)中，可以通过delegation pattern](https://en.wikipedia.org/wiki/Delegation_pattern) 来实现delegation；

The term *delegation* is also used loosely for various other relationships between objects; see [delegation (programming)](https://en.wikipedia.org/wiki/Delegation_(programming)) for more. Frequently confused concepts are simply using another object, more precisely referred to as *consultation* or *aggregation*; and evaluating a member on one object by evaluating the corresponding member on another object, notably in the context of the receiving object, which is more precisely referred to as *forwarding* (when a wrapper object doesn't pass itself to the wrapped object).[[1\]](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)#cite_note-FOOTNOTEGammaHelmJohnsonVlissides1995%22Delegation%22,_pp._20%E2%80%9321-1)[[2\]](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)#cite_note-FOOTNOTEBeck1997%22Delegation%22,_pp._64%E2%80%9369-2)[[a\]](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)#cite_note-3) The [delegation pattern](https://en.wikipedia.org/wiki/Delegation_pattern) is a [software design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) for implementing delegation, though this term is also used loosely for consultation or forwarding.

> NOTE: 关于delegation pattern，参见`Theory\Design-pattern\OOP-design-pattern\Structural-pattern\Delegation-pattern`。



## Delegation VS inheritance

这其实就是composition VS inheritance，composition over inheritance；
# Open–closed principle

## Application

[Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) 是典型的遵循 open-close principle的；

## 为什么需要遵循 open-closed  principle?

在我阅读 refactoring.guru [Visitor](https://refactoring.guru/design-patterns/visitor) 时，其中的一段描述，我觉得能够回答这个问题:

> Unfortunately, the system architect refused to allow you to alter existing node classes. He said that the code was already in production and he didn’t want to risk breaking it because of a potential bug in your changes.
>
> There was another reason for the refusal. It was highly likely that after this feature was implemented, someone from the marketing department would ask you to provide the ability to export into a different **format**, or request some other weird stuff. This would force you to change those precious and fragile classes again.

对于code in production，一般是close to change、open to extend的。



### N * M code reuse

一定程度上能够实现N * M code reuse，因为它允许不断地扩充新的behavior。

## wikipedia [Open–closed principle](https://en.wikipedia.org/wiki/Open%E2%80%93closed_principle)


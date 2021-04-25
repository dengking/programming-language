# 关于本章

Function as first class citizen是一个非常有用的特性，但是C++并不直接支持这个特性，本章介绍C++中如何实现类似于function as first class citizen效果。

## 两种实现方式

在 stackoverflow [What is ScopeGuard in C++?](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c) # [A](https://stackoverflow.com/a/31365171) 中，提出了两种实现思路:

> The rôle of the `function` here is to avoid templating so that `Scope_guard` instances can be declared as such, and passed around. An alternative, slightly more complex and with slightly constrained usage, but possibly marginally more efficient, is to have a class templated on a functor type, and use C++11 `auto` for declarations, with the scope guard instance created by a factory function. 

下面是总结: 

|                                | 优势                    | 劣势   |
| ------------------------------ | ----------------------- | ------ |
| `std::function`                | 1、can be passed around | 性能？ |
| function as template parameter |                         |        |
| function object                |                         |        |



### Function as template parameter

参见 `../Generic-programming/Function-as-template-parameter` 章节。



### `std::function`

`std::function` 是非常好的选择，因为它是polymorphic value type，它融合了OOP和GP的优势。

> NOTE: 
>
> 1、tag-std-function-polymorphic value type-融合OOP-和-GP



参见 `std-function` 章节。

### Function object

参见 `Function-objects` 章节。

## Application

1、AOP

一个非常重要的application就是AOP。

## TO RAED

### github [cwrap](https://github.com/corelight/cwrap)

Auto wrap C and C++ functions with instrumentation
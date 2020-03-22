# Polymorphism

“polymorphism”即“多态”，初次见到这个词语，我的第一印象是“多种形态”。

## [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

In [programming languages](https://en.wikipedia.org/wiki/Programming_language) and [type theory](https://en.wikipedia.org/wiki/Type_theory), **polymorphism** is the provision of a single [interface](https://en.wikipedia.org/wiki/Interface_(computing)) to entities of different [types](https://en.wikipedia.org/wiki/Data_type) or the use of a single symbol to represent multiple different types.

> NOTE: 这段话的意思是：在编程语言和类型理论中，多态性是向不同类型的实体提供单一接口，或使用单一符号表示多个不同类型。
>
> 显然，polymorphism中蕴含是：single和multiple，所以它和dispatch是密切相关的。
>
> 从上面这段话中，我们可以看到，**polymorphism** 和 [type](https://en.wikipedia.org/wiki/Data_type) ，或者更加具体来说 和  [type systems](https://en.wikipedia.org/wiki/Type_system) 密切相关，参见原文的[History](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#History)章节。

The most commonly recognized major classes of polymorphism are:

- *[Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)*
- *[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)* 
- *[Subtyping](https://en.wikipedia.org/wiki/Subtyping)*  (also called *subtype polymorphism* or *inclusion polymorphism*)

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| a single [interface](https://en.wikipedia.org/wiki/Interface_(computing)) to entities of different [types](https://en.wikipedia.org/wiki/Data_type) | [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) |
| a single symbol to represent multiple different types        | [Subtyping polymorphism](https://en.wikipedia.org/wiki/Subtyping) |

[Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)

- [Function overloading](https://en.wikipedia.org/wiki/Function_overloading)
- [Operator overloading](https://en.wikipedia.org/wiki/Operator_overloading)

[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)

- [Generic function](https://en.wikipedia.org/wiki/Generic_function)
- [Generic programming](https://en.wikipedia.org/wiki/Generic_programming)

[Subtyping](https://en.wikipedia.org/wiki/Subtyping)

- [Virtual function](https://en.wikipedia.org/wiki/Virtual_function)
- [Single and dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch)
- [Double dispatch](https://en.wikipedia.org/wiki/Double_dispatch)
- [Multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)



## [Types](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Types)

### Ad hoc polymorphism

参见[Ad hoc polymorphism](./Ad-hoc-polymorphism.md)



### Parametric polymorphism

参见[Parametric polymorphism](./Parametric-polymorphism.md)



### Subtyping

*Main article:* [Subtyping](https://en.wikipedia.org/wiki/Subtyping)



## Implementation aspects

### Static and dynamic polymorphism

|                   Static(at compile time)                    |                     Dynamic(at run time)                     |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| [static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) | [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) |
|        [Static/early binding](Static/early binding )         | [Late/dynamic binding](https://en.wikipedia.org/wiki/Late_binding) |

[Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)和[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)都属于[Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)，而[Subtyping](https://en.wikipedia.org/wiki/Subtyping)则属于[dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)。



## Polymorphism的本质

我觉得Polymorphism的本质是描述了一种one-to-many关系。






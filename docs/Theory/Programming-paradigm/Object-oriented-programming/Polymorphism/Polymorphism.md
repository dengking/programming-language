# Polymorphism

“polymorphism”即“多态”，初次见到这个词语，我的第一印象是“多种形态”。

## 维基百科[Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

In [programming languages](https://en.wikipedia.org/wiki/Programming_language) and [type theory](https://en.wikipedia.org/wiki/Type_theory), **polymorphism** is the provision of a single [interface](https://en.wikipedia.org/wiki/Interface_(computing)) to entities of different [types](https://en.wikipedia.org/wiki/Data_type) or the use of a single symbol to represent multiple different types.

> NOTE: 这段话的意思是：在编程语言和类型理论中，多态性是向不同类型的实体提供单一接口，或使用单一符号表示多个不同类型。
>
> 显然，polymorphism中蕴含是：single和multiple，所以它和dispatch是密切相关的，关于此，参见下面的《Polymorphism的本质》段
>
> 从上面这段话中，我们可以看到，**polymorphism** 和 [type](https://en.wikipedia.org/wiki/Data_type) ，或者更加具体来说 和  [type systems](https://en.wikipedia.org/wiki/Type_system) 密切相关，参见原文的[History](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#History)章节。

The most commonly recognized major classes of polymorphism are:

- *[Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)*
- *[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)* 
- *[Subtyping](https://en.wikipedia.org/wiki/Subtyping)*  (also called *subtype polymorphism* or *inclusion polymorphism*)

> NOTE: 上述三者是major classes，其实还有一些其他类型的polymorphism，比如在原文See also中提及的：
>
> [Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types
>
> 在原文的[Types](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#Types)段，描述了更多类型的polymorphism。

|                                                              | 类型                                                         |
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



### [Types](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Types)

#### Ad hoc polymorphism

参见[Ad hoc polymorphism](./Ad-hoc-polymorphism.md)



#### Parametric polymorphism

参见[Parametric polymorphism](./Parametric-polymorphism.md)

#### Subtyping

*Main article:* [Subtyping](https://en.wikipedia.org/wiki/Subtyping)

#### Row polymorphism

*Main article:* [Row polymorphism](https://en.wikipedia.org/wiki/Row_polymorphism)

> NOTE: “row”即“行”

#### [Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types

> NOTE: 这是原文放在[See also](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#See_also)段中的一段话，我将它移到了这里，因为duck typing也是一种polymorphsim，并且python就是采用的duck typing。

### Implementation aspects

> NOTE: 原文关于实现的解释仅仅涉及了static和dynamic，其实不同的programming language的polymorphisim的实现是截然不同的，关于这些实现，将在不同的programming language中进行具体的展开。

#### Static and dynamic polymorphism

|                   Static(at compile time)                    |                     Dynamic(at run time)                     |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| [static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) | [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) |
|        [Static/early binding](Static/early binding )         | [Late/dynamic binding](https://en.wikipedia.org/wiki/Late_binding) |



> [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)和[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)都属于[Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)，而[Subtyping](https://en.wikipedia.org/wiki/Subtyping)则属于[dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)。

| Static/dynamic                                               |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)、[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) |
| [Dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [Subtyping](https://en.wikipedia.org/wiki/Subtyping)         |

However, it is possible to achieve static polymorphism with subtyping through more sophisticated use of [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming), namely the [curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).

> NOTE: 这是c++中的一个idiom，在`C++\Idiom\Curiously-recurring-template-pattern`中对此有描述。

## Polymorphism的本质

### Polymorphism is a mapping

本节标题的含义是：多态是一种映射	

我觉得Polymorphism的本质是描述了一种one-to-many关系，实现了一种**映射**，这种**映射**是由programming language实现来提供的，往更加宽泛地说，**映射**就是**dispatch**，显然，Polymorphism让我们不用写一堆的if-else判断。



### Polymorphism is a generic trait

本节标题的含义是：多态性是一种通用特性，它源自于[Is duck typing a subset of polymorphism](https://softwareengineering.stackexchange.com/questions/121778/is-duck-typing-a-subset-of-polymorphism)的[回答](https://softwareengineering.stackexchange.com/a/121818)：

> I say that polymorphism is a generic trait, that can be implemented several ways:
>
> - class based inheritance.
> - prototype based objects (with or without inheritance)
> - duck typing
> - interface compliance (as done by Go's interfaces and implicitly on **C++ templates**)
>
> each of them allows the programmer to use a single code with different types, so all embody the concept of polymorphism.

## Polymorphism的价值

- 使代码更加地generic（参见[generic programming](http://en.wikipedia.org/wiki/Generic_programming)），从而带来更好的扩展性
- 降低编写成本，使我们无需写一堆的`if-else`



## Template and polymorphism

Template属于“single symbol to represent multiple different types”。

## Duck type and polymorphism

Duck type属于“single symbol to represent multiple different types”。

在维基百科[Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))中已经提及了，下面是一些补充。

https://stackoverflow.com/questions/11502433/what-is-the-difference-between-polymorphism-and-duck-typing



https://softwareengineering.stackexchange.com/questions/121778/is-duck-typing-a-subset-of-polymorphism



https://dev.to/middlebrooks314/duck-typing-1gnn



https://subscription.packtpub.com/book/application_development/9781788293181/8/08lvl1sec92/polymorphism-and-duck-typing



https://en.wikipedia.org/wiki/Duck_typing#Templates_or_generic_types
# Implementation of polymorphism

本文讨论implementation of polymorphism。



## wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) # [Implementation aspects](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#Implementation_aspects)

### Static and dynamic polymorphism

*Main articles:* [Static polymorphism](https://en.wikipedia.org/wiki/Static_polymorphism)*,* [Late binding](https://en.wikipedia.org/wiki/Late_binding)*, and* [Dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch)

Polymorphism can be distinguished by when the **implementation** is selected: statically (at compile time) or dynamically (at run time, typically via a [virtual function](https://en.wikipedia.org/wiki/Virtual_function)). This is known respectively as *[static dispatch](https://en.wikipedia.org/wiki/Static_dispatch)* and *[dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch),* and the corresponding forms of polymorphism are accordingly called *static polymorphism* and *dynamic polymorphism*.

|                   Static(at compile time)                    |                     Dynamic(at run time)                     |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| [static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) | [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) |
|        [Static/early binding](Static/early binding )         | [Late/dynamic binding](https://en.wikipedia.org/wiki/Late_binding) |



> NOTE: [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)和[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)都属于[Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)，而[Subtyping](https://en.wikipedia.org/wiki/Subtyping)则属于[dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism)。

| Static/dynamic                                               | Example                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)、[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) |
| [Dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [Subtyping](https://en.wikipedia.org/wiki/Subtyping)         |



However, it is possible to achieve static polymorphism with subtyping through more sophisticated use of [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming), namely the [curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).

> NOTE: 这是c++中的一个idiom，在`C++\Idiom\Curiously-recurring-template-pattern`中对此有描述。



> NOTE: 原文关于实现的解释仅仅涉及了static和dynamic，其实不同的programming language的polymorphisim的实现是截然不同的，关于这些实现，将在不同的programming language中进行具体的展开，下面是对此的一些总结：
>
> |      | static                                 | dynamic                                       |
> | ---- | -------------------------------------- | --------------------------------------------- |
> | 实现 | compiler实现，无需在内存中保存映射关系 | 需要在内存中保存所有的candidate之间的映射关系 |
> | 例子 | C++ overload、template specialization  | C++ virtual method、python attribute find     |
>
> 关于C++ implementation of polymorphism，参加文章`C-family-language\C++\Guide\Implementation-of-polymorphism\index.md`



## Dispatch 和 substitute的含义

在polymorphism中，dispatch 和 substitute是出现频率非常高的词语；我们需要考虑在polymorphism中它们的含义是什么。

Dispatch描述的是**method/implementation dispatch**、**method/implementation selection**，在wikipedia [Static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) 中有如下描述:

> It is a form of *method dispatch,* which describes how a language or environment will select which implementation of a method or function to use.

"一个抽象，可能有多个实现"，dispatch是指在这些implementations的method中选择一个。

### Substitution的含义

一个抽象符号，可以被多种符合条件的具体符号替换。

依然符合"一种抽象多种实现"

## Polymorphism总结

不同的语言、不同的polymorphism，实现dispatch的方式是不同的，dispatch的对象也不同:

|                                                              | substitution      | dispatch/method selection |
| ------------------------------------------------------------ | ----------------- | ------------------------- |
| [**Ad hoc polymorphism**](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) | no                | yes                       |
| [**Subtyping**](https://en.wikipedia.org/wiki/Subtyping)     | yes(里氏替换法则) | yes                       |
| [**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) | yes               | no                        |

上面所总结的仅仅是表面的内容，各种具体的programming language的实现，比这个要复杂，具体到programming language，又存在着千差万别，上述总结是可能存在错误的。





### Behavior-based and type-base and inheritance-based



[**Ad hoc polymorphism**](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) 是 type-based 

[**Subtyping**](https://en.wikipedia.org/wiki/Subtyping) 是 inheritance-based

[**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) 不同的programming language使用的实现方式不同，具体参见`Programming-paradigm\Generic-programming\Implementation`章节。



在下面文章这描述了Behavior-based、inheritance-based

1) drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971)

收录在`Theory\Programming-paradigm\Generic-programming\Implementation`中

2) wikipedia [Generic programming](https://en.wikipedia.org/wiki/Generic_programming) # [Templates in C++](https://en.wikipedia.org/wiki/Generic_programming#Templates_in_C++)

> This works whether the arguments `x` and `y` are integers, strings, or any other type for which the expression `x < y` is sensible, or more specifically, for any type for which operator`<` is defined. Common inheritance is not needed for the set of types that can be used, and so it is very similar to [duck typing](https://en.wikipedia.org/wiki/Duck_typing#Templates_or_generic_types). 





## draft



### 如何实现[Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

各种Polymorphism 的实现机制是不同的，实现机制在[Application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface)中进行了说明

Ad hoc polymorphism的实现机制是[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)

Subtyping的实现方式是[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)

[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types


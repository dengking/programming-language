# Static and dynamic polymorphism

本文对static和dynamic polymorphism进行总结。

Static和dynamic是符合在工程 compile-principle的`Chapter-1-Introduction\1.6-Programming-Language-Basics`章节中static和dynamic含义的。

关于本文中early bind和late bind，在工程compiler-principle的`Guide\Bind`章节中进行了描述。

## wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) # Static and dynamic polymorphism

*Main articles:* [Static polymorphism](https://en.wikipedia.org/wiki/Static_polymorphism)*,* [Late binding](https://en.wikipedia.org/wiki/Late_binding)*, and* [Dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch)

Polymorphism can be distinguished by when the **implementation** is selected: statically (at compile time) or dynamically (at run time, typically via a [virtual function](https://en.wikipedia.org/wiki/Virtual_function)). This is known respectively as *[static dispatch](https://en.wikipedia.org/wiki/Static_dispatch)* and *[dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch),* and the corresponding forms of polymorphism are accordingly called *static polymorphism* and *dynamic polymorphism*.

|                   Static(at compile time)                    |                     Dynamic(at run time)                     |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| [static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) | [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) |
| [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) | [dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) |
|        [Static/early binding](Static/early binding )         | [Late/dynamic binding](https://en.wikipedia.org/wiki/Late_binding) |



> NOTE: 
>
> 现代programming language的 [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) 应该属于 [Static polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) ;
>
> 现代programming language的 [Subtyping polymorphism](https://en.wikipedia.org/wiki/Subtyping) 应该属于 [dynamic polymorphism](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Static_and_dynamic_polymorphism) ;
>
> 不同programming language实现 [Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism) 的方式不同，参见`Theory\Programming-paradigm\Generic-programming\Implementation`章节。



However, it is possible to achieve static polymorphism with subtyping through more sophisticated use of [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming), namely the [curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).

> NOTE: 这是C++中的一个idiom，在`C++\Idiom\Curiously-recurring-template-pattern`中对此有描述。

## wikipedia [Static dispatch](https://en.wikipedia.org/wiki/Static_dispatch)

Examples are [templates in C++](https://en.wikipedia.org/wiki/Template_(C%2B%2B)), and [generic programming](https://en.wikipedia.org/wiki/Generic_programming) in other languages, in conjunction with [function overloading](https://en.wikipedia.org/wiki/Function_overloading) (including [operator overloading](https://en.wikipedia.org/wiki/Operator_overloading)). 

## wikipedia [Dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **dynamic dispatch** is the process of selecting which implementation of a [polymorphic](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) operation ([method](https://en.wikipedia.org/wiki/Method_(computer_programming)) or function) to call at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). It is commonly employed in, and considered a prime characteristic of, [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) (OOP) languages and systems.[[1\]](https://en.wikipedia.org/wiki/Dynamic_dispatch#cite_note-1)

## Implementation

对于static polymorphism，concrete/implementation is selected statically (at [compile time](https://en.wikipedia.org/wiki/Compile_time)), compiler采用early bind，compile主要根据static type info来进行dispatch；

对于dynamic polymorphism，concrete/implementation is selected dynamically (at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)))，compiler采用late bind，compile主要根据dynamic type info来进行dispatch；

前面讨论了early bind 和 late bind，那如何来进行实现呢？

对于early bind，compiler在compile time就将函数调用转换为函数调用过程的指令，即bind到了指定的function；

对于late bind，compiler无法根据static type来决定到底bind到哪个function，而是需要将它转换为能够让program在runtime时根据dynamic type info查询到其准确的函数地址，然后进行调用的机制，对于这套机制，不同的programming language有不同的实现，无法进行统一的描述。

### Dynamic polymorphism

Dynamic polymorphism的dispatch发生于runtime，显然是late binding；因此，如果采用这种实现方式，则它需要存储**abstraction**和所有的**candidate**、**concrete**、**implementation**之间的**映射关系**。

由于dispatch发生于runtime，则必然存在overhead。

**Example**:

1) Python

2) virtual and abstraction

### Static polymorphism

Static polymorphism的dispatch发生于compile time，显然是early binding，由compiler进行dispatch；因此，如果采用这种实现方式，无需在内存中保存**映射关系**。

由于dispatch发生于compile time，没有任何overhead。

### Example

|      | static                                | dynamic                                       |
| ---- | ------------------------------------- | --------------------------------------------- |
| 实现 | compiler实现，                        | 需要在内存中保存所有的candidate之间的映射关系 |
| 例子 | C++ overload、template specialization | C++ virtual method、python attribute find     |

关于C++ implementation of polymorphism，参加文章`C-family-language\C++\Guide\Implementation-of-polymorphism\index.md`



## Static polymorphism VS Dynamic polymorphism

在 wikipedia [Static dispatch](https://en.wikipedia.org/wiki/Static_dispatch) 中有这样的描述: 

> Static dispatch is typically faster than dynamic dispatch which by nature has higher overhead.



上面内容其实已经对Static polymorphism 和 Dynamic polymorphism进行了比较，下面是一些补充内容: 

https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c/

https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/
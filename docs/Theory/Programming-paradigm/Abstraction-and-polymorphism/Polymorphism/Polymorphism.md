# Polymorphism

“polymorphism”即“多态”，初次见到这个词语，我的第一印象是“多种形态”。



## wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

In [programming languages](https://en.wikipedia.org/wiki/Programming_language) and [type theory](https://en.wikipedia.org/wiki/Type_theory), **polymorphism** is the provision(提供) of a single [interface](https://en.wikipedia.org/wiki/Interface_(computing)) to entities of different [types](https://en.wikipedia.org/wiki/Data_type) or the use of a single symbol to represent multiple different types.

> NOTE: 这段话的意思是：在编程语言和类型理论中，多态性是向不同type的entity提供单一interface，或使用单一符号表示多个不同type。这段话总结了polymorphism的本质，在后面的《Polymorphism的本质》段会进行详细分析。
>
> 从上面这段话中，我们可以看到，**polymorphism** 和 [type](https://en.wikipedia.org/wiki/Data_type) ，或者更加具体来说 和  [type systems](https://en.wikipedia.org/wiki/Type_system) 密切相关，参见原文的 [History](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#History) 章节。

The most commonly recognized major classes of polymorphism are:

|                                                              | 说明                                                         | 实现方式                                                     | paradigm |                |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | -------- | -------------- |
| [**Ad hoc polymorphism**](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) | defines a common **interface** for an arbitrary set of individually specified types. | - [Function overloading](https://en.wikipedia.org/wiki/Function_overloading) <br>- [Operator overloading](https://en.wikipedia.org/wiki/Operator_overloading) |          |                |
| [**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) | when one or more types are not specified by name but by **abstract symbols** that can represent any type. | - [Generic function](https://en.wikipedia.org/wiki/Generic_function) <br>- [Generic programming](https://en.wikipedia.org/wiki/Generic_programming) | GP、FP   | behavior-based |
| [**Subtyping**](https://en.wikipedia.org/wiki/Subtyping)     | (also called *subtype polymorphism* or *inclusion polymorphism*) <br>when a name denotes instances of many different classes related by some common superclass.[[3\]](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#cite_note-gbooch-3) | - [Virtual function](https://en.wikipedia.org/wiki/Virtual_function) <br>- [Single and dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) <br>- [Double dispatch](https://en.wikipedia.org/wiki/Double_dispatch) <br>- [Multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch) | OOP      | inheritance    |

> NOTE:
>
> "ad hoc"的中文意思"特别的"。

> NOTE: 上述三者是major classes，其实还有一些其他类型的polymorphism，比如在原文See also中提及的：
>
> [Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) **types**，在后面的"[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types"章节进行了具体介绍。
>
> 在原文的[Types](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#Types)段，描述了更多类型的polymorphism。



### [Types](https://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29#Types)

> NOTE: 本段描述各种类型的polymorphism。

#### Ad hoc polymorphism

> NOTE: 特别多态

*Main article:* [Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism)

The term "[ad hoc](https://en.wikipedia.org/wiki/Ad_hoc)" in this context is not intended to be pejorative(轻蔑词); it refers simply to the fact that this type of polymorphism is not a fundamental feature of the type system. 



[Implicit type conversion](https://en.wikipedia.org/wiki/Implicit_type_conversion) has also been defined as a form of polymorphism, referred to as "coercion(强制) polymorphism".[[2\]](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#cite_note-Luca-2)[[6\]](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#cite_note-Tucker2004-6)



#### Parametric polymorphism

> NOTE: 参数多态性

*Main article:* [Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)

*Parametric polymorphism* allows a function or a data type to be written **generically**, so that it can handle values *uniformly* without depending on their type.[[7\]](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#cite_note-bjpierce-7) Parametric polymorphism is a way to make a language more expressive while still maintaining full static [type-safety](https://en.wikipedia.org/wiki/Type-safety).

> NOTE: 在[Generic programming](https://en.wikipedia.org/wiki/Generic_programming)、Functional programming中，都涉及parametric polymorphism。



The concept of **parametric polymorphism** applies to both [data types](https://en.wikipedia.org/wiki/Data_type) and [functions](https://en.wikipedia.org/wiki/Function_(programming)). 

1) A function that can evaluate to or be applied to values of different types is known as a *polymorphic function.* 

2) A data type that can appear to be of a generalized type (e.g. a [list](https://en.wikipedia.org/wiki/List_(computing)) with elements of arbitrary type) is designated *polymorphic data type* like the generalized type from which such specializations are made.

Parametric polymorphism is ubiquitous(普遍存在) in **functional programming**, where it is often simply referred to as "**polymorphism**". The following example in [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)) shows a parameterized list data type and two parametrically polymorphic functions on them:



#### Subtyping

*Main article:* [Subtyping](https://en.wikipedia.org/wiki/Subtyping)

> NOTE: 在OOP中，普遍存在

[Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle)

[Object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language) offer subtype polymorphism using *[subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science))* (also known as *[inheritance](https://en.wikipedia.org/wiki/Inheritance_in_object-oriented_programming)*). 

> NOTE:  *[subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science))* 和  *[inheritance](https://en.wikipedia.org/wiki/Inheritance_in_object-oriented_programming)* 可以看做是同义词



In typical implementations, each class contains what is called a *[virtual table](https://en.wikipedia.org/wiki/Virtual_table)*—a table of functions that implement the polymorphic part of the class interface—and each object contains a pointer to the "vtable" of its class, which is then consulted whenever a polymorphic method is called. This mechanism is an example of:

- *[late binding](https://en.wikipedia.org/wiki/Late_binding)*, because virtual function calls are not bound until the time of invocation;
- *[single dispatch](https://en.wikipedia.org/wiki/Single_dispatch)* (i.e. **single-argument polymorphism**), because virtual function calls are bound simply by looking through the vtable provided by the first argument (the `this` object), so the runtime types of the other arguments are completely irrelevant.

> NOTE: 
>
> 这完全是以C++来描述的。
>
> 上面从实现层面描述了仅仅支持single dispatch的原因，是非常具备启发性的。

The same goes for most other popular object systems. 

> NOTE: 这段话的意思是: 大多数programming language都是按照这个思路来实现的。下面列举了一些反例: 

Some, however, such as [Common Lisp Object System](https://en.wikipedia.org/wiki/Common_Lisp_Object_System), provide *[multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)*, under which method calls are polymorphic in *all* arguments.

> NOTE: 从上面的描述，可以看出single dispatch、multiple dispatch之间的差异。

The interaction(相互作用，相互影响) between **parametric polymorphism** and **subtyping** leads to the concepts of [variance](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)) and [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification).

> NOTE: "variance"即"可变性"，在`Theory\Type-system\Type-constructor`章节进行了描述。
>
>  [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification)在`Programming-paradigm\Generic-programming\Generics`章节进行了描述。

#### Row polymorphism

> NOTE: “row”即“行”

Main article: [Row polymorphism](https://en.wikipedia.org/wiki/Row_polymorphism)

See also: [Duck typing](https://en.wikipedia.org/wiki/Duck_typing)



#### [Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types

> NOTE: 这是原文放在[See also](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#See_also)段中的一段话，我将它移到了这里，因为duck typing也是一种polymorphsim，并且python就是采用的duck typing。[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism 更加类似于 [**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism)，都是:
>
> "when one or more types are not specified by name but by **abstract symbols** that can represent any type."
>
> 下面是关于duck typing和polymorphism的一些补充。
>
> https://stackoverflow.com/questions/11502433/what-is-the-difference-between-polymorphism-and-duck-typing
>
> https://softwareengineering.stackexchange.com/questions/121778/is-duck-typing-a-subset-of-polymorphism
>
> https://dev.to/middlebrooks314/duck-typing-1gnn
>
> https://subscription.packtpub.com/book/application_development/9781788293181/8/08lvl1sec92/polymorphism-and-duck-typing
>
> 

### Implementation aspects

> NOTE: 参见`Theory\Programming-paradigm\Polymorphism\Implementation`章节



## Polymorphism的本质



### Polymorphism is a map/dispatch

本节标题的含义是：多态是一种映射/派发；下面是我polymorphism的本质的思考:

1) 描述了一种one-to-many关系:

polymorphism中的one-to-many关系的含义是多重的: 

- **single**(**one**) [interface](https://en.wikipedia.org/wiki/Interface_(computing)) to **entities**(**many**) of different [types](https://en.wikipedia.org/wiki/Data_type) 
- a **single**(**one**) symbol to represent multiple(**many**) different types
- "一个抽象(**one**)，可能有多个(**many**)实现"(参见文章《Abstraction》)

2) 按照one-to-many关系, 自动实现**map/dispatch**:

这种**map/dispatch**是由**programming language的实现**来提供的(或者说: 这种**map/dispatch**是programming language内置的dispatch机制): 

**programming language的实现**按照one-to-many关系来实现**map/dispatch**，这就是标题中的**自动**的含义。显然，Polymorphism让我们不用写一堆的**if-else**判断。



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

1) 让programmer写出更加abstract的code，能够增强code reuse

2) 使代码更加地generic（参见[generic programming](http://en.wikipedia.org/wiki/Generic_programming)），从而带来更好的扩展性

3) 降低编写成本，使我们无需写一堆的`if-else`

`if-else`其实也是描述的one-to-many，使用polymorphism能够让我们运用programming language内置的dispatch机制，避免显式地使用`if-else`。

相比于显式地使用`if-else`，polymorphism有如下优势:

- 更加高效
- 代码维护性更高、复杂度更低(可以使用**圈复杂度**来进行量化)



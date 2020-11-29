# Implementation of polymorphism

需要注意的是: 本文所描述的implementation并不局限于某种特定的programing language，而是对implementation of polymorphism中可能涉及的问题、解决方案的总结。现代programming language，往往支持多种programming paradigm，并且再加上它们的实现方式的特殊，导致了它们的polymorphism的implementation是存在巨大差异的，因此很难进行统一的、简单的描述。

## Common issue 

本节讨论在implementation of polymorphism中的一些common issue:

### 1) dispatch发生的时间

1 static: compile-time

2 dynamic: runtime

在下面的"Static and dynamic polymorphism"章节进行了总结。

### 2) relationship between abstract and concrete

描述abstract 和 concrete之间的关系，在下面的"Relationship between abstract and concrete"章节进行了总结。



### 3) dispatch的搜索和排序

参见下面的"dispatch的搜索和排序"章节。



## Relationship between abstract and concrete

Abstraction 和 polymorphism是现代programming language的核心，因此无论使用何种programming language，当我们按照"program to abstraction"原则进行开发的时候，都需要描述(one)abstract和(multiple)concrete/implementation，因此都涉及描述abstract和concrete/implementation之间的relationship(关系)，或者说: 如何将multiple concrete/implementation 聚集在一起。不同的programming language、不同的polymorphism采用不同的方式，本节对此进行总结。

触发我思考这个问题的是下面这篇文章，其中描述了behavior-based、inheritance-based，我基于其中提出的behavior-based、inheritance-based，综合考虑了一些其他类型的polymorphism，进一步进行了整理。



### drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971)

> NOTE: 这篇文章收录在`C++\Language-reference\Template\Programming-paradigm\Generic-programming\Template-is-behavior-based`中

When we wish to select from a set of classes at **runtime**, C++ requires that those classes be related by **inheritance**. When we wish to select from a set of types at **compile time**, the relationship between those types is more subtle(难以捉摸的、微妙的). The types need be related only indirectly, and only by their **behavior**.

> NOTE: 在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Polymorphism.md`中，我们已经知道了“[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) **types**”。上面这一段中描述的使用template实现的polymorphism和使用duck type实现的polymorphism非常类似。

The C++ community does not have a generally accepted term for this kind of **behavior-based relationship** between types. Accordingly, people first learning about C++ generic programming are tempted to think that **inheritance** is involved somehow, just as it is for **object-oriented programming**. For example, on several occasions we have seen questions such as "Why isn't a **bidirectional iterator** derived from a **forward iterator**?" A student who asks that question has probably already formed a significant misconception about how templates deal with types.

> NOTE: 上面这段话的意思是：当人们刚开始学习C++ generic programming 的时候，会和object-oriented programming混淆，OOP是基于inheritance的，即要求“classes be related by **inheritance**”；而generic programming是基于duck type的，即“types need be related only indirectly, and only by their **behavior**”



2) wikipedia [Generic programming](https://en.wikipedia.org/wiki/Generic_programming) # [Templates in C++](https://en.wikipedia.org/wiki/Generic_programming#Templates_in_C++)

> This works whether the arguments `x` and `y` are integers, strings, or any other type for which the expression `x < y` is sensible, or more specifically, for any type for which operator`<` is defined. Common inheritance is not needed for the set of types that can be used, and so it is very similar to [duck typing](https://en.wikipedia.org/wiki/Duck_typing#Templates_or_generic_types). 



### Type-based

#### Inheritance-based

[Subtyping polymorphism](https://en.wikipedia.org/wiki/Subtyping) 

Inheritance-based其实也能够实现behavior-based，但是它是更加constrained，因为它强制要求了type inheritance关系；

#### Specialization-based

C++ template specialization。之所以将它归为"type-based"，是因为C++要求primary template、specialization属于同一个。

### Behavior-based



#### Duck typing

Duck typing是天生的behavior-based:

> if it quacks like a duck, and it fly like a duck, it is a duck

#### C++ Template

C++ Template也是behavior-based，参见:

1) drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971)

2) `C++\Language-reference\Template\Programming-paradigm\Generic-programming\Template-is-behavior-based`章节

### Name-based

需要有相同的name，下面是一些例子: 

[Ad hoc polymorphism](https://en.wikipedia.org/wiki/Ad_hoc_polymorphism) 



[**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism) 不同的programming language使用的实现方式不同，具体参见`Programming-paradigm\Generic-programming\Implementation`章节。



## Dispatch example

在文章`Abstract-and-concrete`中，对dispatch的定义如下: 

> 将从abstract到concrete/implementation的过程称为**dispatch**

本节对各种programming language的polymorphism的dispatch进行总结，下面是一些例子: 

| programming language | polymorphism              | dispatch                       |
| -------------------- | ------------------------- | ------------------------------ |
| **C++**              | subtyping                 | (virtual)method                |
|                      | ad hoc(function overload) | function                       |
|                      | Parametric                | template specialization        |
| **Python**           | subtyping/duck typing     | attribute(method、member data) |



## Substitution的含义

一个抽象符号，可以被多种符合条件的具体符号替换。

依然符合"一种抽象多种实现"



## Static and dynamic polymorphism

本节对static 和 dynamic polymorphism进行总结。

### wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) # [Implementation aspects](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)#Implementation_aspects) # Static and dynamic polymorphism



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

### Implementation

#### Dynamic polymorphism

Dynamic polymorphism的dispatch发生于runtime，显然是late binding；因此，如果采用这种实现方式，则它需要存储**abstraction**和所有的**candidate**、**concrete**、**implementation**之间的**映射关系**。

由于dispatch发生于runtime，则必然存在overhead。

#### Static polymorphism

Static polymorphism的dispatch发生于compile time，显然是early binding，由compiler进行dispatch；因此，如果采用这种实现方式，无需在内存中保存**映射关系**。

由于dispatch发生于compile time，没有任何overhead。

#### Example

|      | static                                | dynamic                                       |
| ---- | ------------------------------------- | --------------------------------------------- |
| 实现 | compiler实现，                        | 需要在内存中保存所有的candidate之间的映射关系 |
| 例子 | C++ overload、template specialization | C++ virtual method、python attribute find     |

关于C++ implementation of polymorphism，参加文章`C-family-language\C++\Guide\Implementation-of-polymorphism\index.md`



### Static polymorphism VS Dynamic polymorphism

上面内容其实已经对Static polymorphism 和 Dynamic polymorphism进行了比较，下面是一些补充内容: 

https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c/
https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/

##  Dispatch的搜索和排序

> NOTE: dispatch的过程可以看做是"搜索和排序"。

无论是哪种polymorphism，在实现的时候都需要考虑one-to-many的问题，即需要考虑从set  of candidates(concrete/implementation)中的选择哪一个来作为最终的实现。

显然，dispatch的过程**可能**涉及到了如下步骤:

> NOTE: 上面用到了**可能**这个词语，这是因为programming language实现polymorphism的差异性，并且有的implementation，将下面的两个步骤合并在一起了，比如OOP subtyping polymorphism，它的搜索和排序都是沿着class hierarchy自底向上进行的，由于class hierarchy已经是有序的了，因此，它在**搜索**过程中已经进行了**排序**。

### 1) 搜索

> 如何找到所有的candidates(concrete/implementation)

搜索到所有的candidates。一般，programming language的实现，往往需要搜索所有的candidates，搜索过程往往遵循: "**try my best原则**"，即:

即使在**当前搜索节点**没有找到符合条件的candidate，并不会终止搜索进程，而是会继续在**下一个搜索节点**中寻找，直至完成了所有的搜索节点。

综合来看，下列polymorphism的实现都遵循了这个原则: 

1 OOP subtyping polymorphism: 它的搜索是沿着class hierarchy自底向上进行的，逐个节点进行搜索，直到root节点；典型的例子就是Python attribute find algorithm。

2 C++ SFINAE

### 2) 排序

> 基于什么标准来对multiple concrete进行排序/比较，选择最最concrete/合适的实现。

对candidates进行**比较**/**排序**，在文章`Abstract-and-concrete`、`Abstraction-and-polymorphism`(`Theory\Programming-paradigm\Abstraction-and-polymorphism`)中，都是采用的informal的描述: 选择**最合适**的那一个。

到底哪个**最合适**的呢？不同的polymorphism有不同的标准，采用不同的比较算法，当考虑实现的时候，需要进行准确、formal定义。

> NOTE: 显然这是涉及ordering theory的

Dispatch所采用的比较算法是会考虑relationship between abstract and concrete的。

不同的polymorphism采用的比较算法是不同的，下面对此进行总结:

Subtyping polymorphism: 基于class hierarchy；

C++ template specialization、overload: 基于type odering；

Python polymorphism: 基于class hierarchy；

具体参见各programming language中的描述。

## draft



### 如何实现[Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

各种Polymorphism 的实现机制是不同的，实现机制在[Application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface)中进行了说明

Ad hoc polymorphism的实现机制是[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)

Subtyping的实现方式是[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)

[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) types


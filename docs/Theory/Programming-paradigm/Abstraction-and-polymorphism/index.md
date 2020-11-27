# Abstraction and polymorphism

本章讨论abstraction 和 polymorphism，它们是现代programming language的核心所在。



## Abstraction and polymorphism概述

> NOTE: 描述思路如下:
>
> conceptual model->使用programming language描述->implementation conceptual model->一个抽象可以有多个concrete/implementation->polymorphism: dispatch to concrete/implementation automatically
>
> 在下面文章中，其实已经对abstraction and polymorphism进行了描述
>
> 1) 文章Abstraction
>
> 2) 文章Abstraction-and-model
>
> 3) 文章Abstract-and-concrete

Abstraction and polymorphism是现代programming language的核心所在，本节对它们进行概述。

1) 作为software engineer，我们需要解决现实世界中的各种问题，这个过程其实是: **abstraction**(抽象) 并 **model**(建模)，然后使用**programming language**来实现/描述conceptual model，最后将此model应用于实际，从而使实际问题得解;

**如何来描述concept？**

这在文章Abstraction中进行了讨论。在programming language中，我们往往使用OOP class来进行描述。

**如何实现conceptual model？**

在文章[Abstraction-principle](./Abstraction-principle.md)中，提出了如下原则: 

- Program to abstraction

- Design to abstraction

2) 一个abstraction可以有多个concrete/implementation;

Concrete的一些例子: 

- Is-a: concept之间的关系: 从更加abstract的concept到更加concrete的concept
- 对某种具体的情况进行说明: 比如 C++ template specialization、OOP subclass(参见对应章节)

Implementation: 实现抽象的概念

3) Dispatch: 从abstract到concrete/implementation的过程

4) Polymorphism: dispatch to concrete/implementation **automatically**

Polymorphism是现代programming language内置了dispatch机制，能够自动进行dispatch，从而选择最最**具体**(**concrete**)的/**合适**的**实现**(implementation)，从而大大增加了研发效率;

> NOTE: 在`./Polymorphism/Implementation`中会对此进行说明

### 总结

下面是我之前想到的一些观点: 

1) **Program to an abstraction and polymorphism**

这是我们遵循"Program to an abstraction"的原因，在`./Program to an abstraction` 章节中会对此进行详细说明。

2) Polymorphism是实现conceptual model/abstraction model的基础



## Abstraction in programming language history

> NOTE: 本节从programming language的发展史来思考abstraction。

参见`Theory\Programming-language`的"Abstraction in programming language history"章节。





## Abstraction in programming paradigm

许多programming paradigm都遵循abstraction principle，本节对它们进行总结。

### OOP VS AOP

|         | 抽象内容                                                  | 方向         |
| ------- | --------------------------------------------------------- | ------------ |
| **AOP** | 抽象出 *cross-cutting concerns* 即 横切关注点 或者 aspect | 是**横向**的 |
| **OOP** | 抽象出 base class、object、class                          | 是**纵向**的 |

两者都能够让我们**抽象**出公共部分，实现code reuse。

#### AOP VS Mixin

AOP是一种编程思想，它告诉我们使用aspect来进行抽象；

Mixin是OOP中的一种技巧，目的是促进code reuse，参见`Theory\Programming-paradigm\Object-oriented-programming\Assemble\Mixin`;

两者都能够让我们实现code reusage。

Mixin and AOP：https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class，在`C++\Idiom\TMP\Parameterized-Base-Class\Parameterized-Base-Class`中收录了这篇文章。



#### Good article

1) 在 [C++11实现一个轻量级的AOP框架](https://www.cnblogs.com/qicosmos/p/4772389.html) 中进行了介绍:

> AOP（Aspect-Oriented Programming，面向方面编程），可以解决面向对象编程中的一些问题，是OOP的一种有益补充。面向对象编程中的继承是一种从上而下的关系，不适合定义从左到右的横向关系，如果继承体系中的很多无关联的对象都有一些公共行为，这些公共行为可能分散在不同的组件、不同的对象之中，通过继承方式提取这些公共行为就不太合适了。使用AOP还有一种情况是为了提高程序的可维护性，AOP将程序的非核心逻辑都“横切”出来，将非核心逻辑和核心逻辑分离，使我们能集中精力在核心逻辑上，例如图1所示的这种情况。

2) [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918):



### AOP VS Meta-programming

1) [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918):

> To me AOP is some kind of "meta-programming".

2) Python使用metaclass来实现AOP



### TODO: OOP VS GP

> TODO 重点比较OOP和duck type




### Abstraction in [object-oriented programming](http://en.wikipedia.org/wiki/Object-oriented_programming)

> NOTE: 当今，OOP paradigm普遍流行，这背后有着深刻的原因，其中非常重要的一点是: OOP对concept的描述。

思考这样的一个问题：如何使用面向对象方式来描述上述kernel control path概念和task概念？

使用`class`来描述[concept](https://en.wikipedia.org/wiki/Concept)，这几个[concept](https://en.wikipedia.org/wiki/Concept)之间是[Is-a](https://en.wikipedia.org/wiki/Is-a)关系，那么可以使用inheritance，下面是一个`c++`实现的demo：

```c++
class KernelControlPath
{
    
}
	

class SystemCallKernelControlPath public: KernelControlPath
{
    
}

class InterruptKernelControlPath public: KernelControlPath
{
    
}

```

kernel control path的一个**特征**就是它能够被suspend、resume，使用面向对象的方式来进行描述就是：

```c++
void suspend(KernelControlPath& kernel_control_path)
{
    
}
	

void resume(KernelControlPath& kernel_control_path)
{
    
}
```

面向对象的[Polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))（多态性）表明`suspend`和`resume`的入参可以为`SystemCallKernelControlPath`、`InterruptKernelControlPath`类型的对象，这其实就体现了：

> 当一个表述中使用kernel control path的时候，我们就知道它可以是system call kernel control path、也可以是interrupt handler kernel control path



```python
class Task:
	pass

class ProcessTask(Task):
	pass

class ThreadTask(Task):
	pass
```



在计算机科学中存在与上述抽象概念类似的做法：

父类是子类的抽象

[Virtual function](https://en.wikipedia.org/wiki/Virtual_function)

参见：维基百科[Abstract type](https://en.wikipedia.org/wiki/Abstract_type)

参见：工程[programming-language](https://dengking.github.io/programming-language)的文章[Introduction to Objects](https://dengking.github.io/programming-language/Theory/Programming-paradigm/Object-oriented-programming/Thinking-in-java/Introduction-to-objects)












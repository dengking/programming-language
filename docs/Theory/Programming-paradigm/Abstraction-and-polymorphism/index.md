# Abstraction and polymorphism

本章讨论abstraction 和 polymorphism，它们是现代programming language的核心所在。



## Abstraction and polymorphism概述

> NOTE: 描述思路如下:
>
> conceptual model->使用programming language描述->implementation conceptual model->一个抽象可以有多个concrete/implementation->polymorphism: dispatch to concrete/implementation automatically
>
> 参考:
>
> 1) 文章Abstraction
>
> 2) 文章Abstraction-and-model
>
> 3) 文章Abstract-and-concrete

Abstraction and polymorphism是现代programming language的核心所在，本节对它们进行概述。

1) 作为software engineer，我们需要解决现实世界中的各种问题，这个过程其实是: **abstraction**(抽象) 并 **model**(建模)，然后使用**programming language**来实现/描述conceptual model，最后将此model应用于实际，从而使实际问题得解;

如何来描述concept？这在文章Abstraction中进行了讨论。

实现conceptual model: 

- Program to abstraction

- Design to abstraction

2) 一个abstraction可以有多个concrete/implementation;

Concrete: 

- Is-a: concept之间的关系: 从更加abstract的concept到更加concrete的concept
- 对某种具体的情况进行说明: specialization

Implementation: 实现抽象的概念

3) Dispatch: 从abstract到concrete/implementation

4) Polymorphism: dispatch to concrete/implementation **automatically**

Polymorphism是现代programming language内置了dispatch机制，能够自动进行dispatch，从而选择最最**具体**(**concrete**)的/**合适**的**实现**(implementation)，从而大大增加了研发效率;

> NOTE: 在`./Polymorphism/Implementation`中会对此进行说明

### 总结

下面是我之前想到的一些观点: 

1) Program to abstraction and then dispatch

2) Polymorphism是实现abstraction的基础



## Abstraction in programming language history

> NOTE: 本节从programming language的发展史来思考abstraction。

纵观 [programming language的发展史](https://en.wikipedia.org/wiki/Programming_language#History)，我们发现：语言越来越高级，越来越抽象，越来越易用。最底层的语言：[machine language](https://en.wikipedia.org/wiki/Machine_code) 晦涩难懂，计算机科学的先驱在[machine language](https://en.wikipedia.org/wiki/Machine_code) 的基础上抽象出了 [function](https://en.wikipedia.org/wiki/Subroutine) 概念，又抽象出了 [class](https://en.wikipedia.org/wiki/Class-based_programming) 的概念，创建了非常多 [高级的语言](https://en.wikipedia.org/wiki/High-level_programming_language)，比如C。不管多么高级的语言最终都需要被翻译（compiler或interpreter）为[machine language](https://en.wikipedia.org/wiki/Machine_code)才能够被机器执行，显然这个翻译的过程是与前面的抽象的过程相逆的。

与machine language相比，我们发现高级语言往往伴随着非常多的概念，而且越高级的语言貌似概念越多（这是我的直觉，可能并非如此），当然也越容易使用。与此类似的是，各种library为了简化某个领域的问题， 也创造了各种直观易懂的概念，使得software engineer在解决这类问题时，非常轻松容易，因此获得了software engineer的喜爱。

> TODO: 底层的programming language，比如C，由于它们是面向hardware，无法描述抽象的概念，这就限制了language的express power

从这个发展史我们能够看到抽象所带来的巨大优势：

- 隐藏了底层的细节
- 越抽象，越易用，概念也就越多
- machine-independent，通用性更加，开发效率更高



参见: [Abstraction(computer science)#Rationale](https://en.wikipedia.org/wiki/Abstraction_(computer_science)#Rationale)

> A central form of abstraction in computing is language abstraction: new artificial languages are developed to express specific aspects of a system. *[Modeling languages](https://en.wikipedia.org/wiki/Modeling_languages)* help in planning. *[Computer languages](https://en.wikipedia.org/wiki/Computer_language)* can be processed with a computer. An example of this abstraction process is the generational development of [programming languages](https://en.wikipedia.org/wiki/Programming_language) from the [machine language](https://en.wikipedia.org/wiki/First-generation_programming_language) to the [assembly language](https://en.wikipedia.org/wiki/Second-generation_programming_language) and the [high-level language](https://en.wikipedia.org/wiki/Third-generation_programming_language). Each stage can be used as a stepping stone for the next stage. The language abstraction continues for example in [scripting languages](https://en.wikipedia.org/wiki/Scripting_language) and [domain-specific programming languages](https://en.wikipedia.org/wiki/Domain-specific_programming_language).

参见：龙书 [Chapter 7 Run-Time Environments](https://dengking.github.io/compiler-principle/Chapter-7-Run-Time-Environments/)。



### TODO

关于programming language的发展史，需要将thinking in Java的内容添加到其中。





## Program to abstraction

"Program to abstraction"现代programming language中的核心思想。

下面是一些thought:

> 依赖于抽象而不是具体，即上层依赖于抽象的接口，底层实现具体。上层到底层的映射，依赖于多态，它可以是compiler time完成，也可以是run time完成。比如get_token，实现静态多态，实现了多个具体实现版本，由compiler进行派发，进行选择
>
> 上述"依赖于抽象而不是具体"，其实就是program to abstraction。



## Abstraction in [object-oriented programming](http://en.wikipedia.org/wiki/Object-oriented_programming)

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









## Abstraction in programming paradigm

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









## Definition of abstraction

“abstraction”在软件工程中随处可见，那如何来定义抽象呢？不同的programming paradigm有着不同的描述方式：

- OOP class
- generic programming：duck type、concept

> NOTE: 在文章`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`中对于两者进行了比较。

在文章[Abstraction](./Abstraction.md)的“抽象与实现”章节中，我们已经总结了

> 一种抽象，可能有多种实现。

再来看上述两种programming paradigm中的描述方式，可以发现它们都满足“一种抽象，可能有多种实现”，这就是polymorphism。




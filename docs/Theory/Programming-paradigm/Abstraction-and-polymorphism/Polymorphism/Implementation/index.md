# Implementation of polymorphism

需要注意的是: 本文所描述的implementation并不局限于某种特定的programing language，而是对implementation of polymorphism中可能涉及的问题、解决方案的总结。现代programming language，往往支持多种programming paradigm，并且再加上它们的实现方式的特殊，导致了它们的polymorphism的implementation是存在巨大差异的，因此很难进行统一的、简单的描述。

## Common issue 

本节讨论在implementation of polymorphism中的一些common issue:

### Dispatch发生的时间

1 static: compile-time

2 dynamic: runtime

在`./Static-and-dynamic-polymorphism`中进行了总结。

### Relationship between abstract and concrete

描述abstract 和 concrete之间的关系，在下面的"Relationship between abstract and concrete"章节进行了总结。

### Dispatch考虑的因素/参数

不同的dispatch考虑的因素/参数的个数是不同的，不同的实现会带来一些差异，在下面的"Dispatch考虑的因素/参数"章节进行了介绍。

### Dispatch的搜索和排序

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

**behavior-based**是我在阅读 drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971) 时，其中提出的一个概念，我觉得"**behavior-based**"这个词总结地非常好:

1) 可以对object这些哪些操作(比如call、iterate)

2) object具备/哪些attribute(member data、member function)，这也可以看做是一种behavior



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



### [Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)  

对于Parametric polymorphism，不同的programming language使用的实现方式不同，具体参见`Programming-paradigm\Generic-programming\Implementation`章节。

### Polymorphism is more about behavior-based

本节标题的意思是: Polymorphism往往是**behavior-based**。这是我的经验之谈，主要源于:

1) polymorphism的dispatch是对function/method的dispatch

2) virtual function而没有virtual data

于此类似的是: 在 generic programming中，有"type requirement is more about behavior-based";

### Type determine everything

无论是behavior-based还是inheritance-based，它们本质上都是type-based: 

1) 因为type determine everything，因此behavior是type决定的 

2) inheritance更是不用说，也是如此

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

依然符合"一种抽象多种实现"。





## Dispatch考虑的因素/参数

不同的dispatch考虑的因素/参数的个数是不同的，根据个数，其实可以分为:

### Single dispatch

最终的concrete/implementation由一个 因素/参数 决定; 当concrete/implementation仅由单个 因素/参数 决定时，需要使用single dispatch。

能够表示 one-to-many 关系: 当仅仅涉及一个abstraction时，一个abstraction有多个concrete/implementation(one-to-many)，single dispatch能够实现abstraction到concrete/implementation的dispatch；



### Multiple dispatch

最终的concrete/implementation由多个 因素/参数 决定; 当concrete/implementation由多个 因素/参数 决定时，需要使用multiple dispatch。

能够表示 many-to-many 关系: 当涉及多个abstraction(遵循 program to abstraction principle )，每个abstraction都有多个concrete/implementation，因此就出现了非常多的可能**组合**(many-to-many,参见下面的**组合分析**)，multiple dispatch能够实现根据这多个abstraction找到它们对应concrete/implementation组合所对应的最终concrete/implementation。关于此的例子:

1 thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) :

`Intersect`的实现依赖于它的两个parameter，按照program to abstraction principle，`Intersect`的两个parameter应该是pointer to interface，每个interface都有多个concrete/implementation，显然这是many-to-many关系，显然对于每一种可能的组合都有对应的implementation，因此需要使用multiple dispatch才能够实现。

2 有多类listener、多类event，每类listener对不同的event的的处理是不同的，显然这就涉及multiple dispatch。

#### 组合分析

在thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) 中提及了**组合分析**:

> A telling sign that **multiple dispatch** may be in order is when you have some operation that involves more than one class and there is no single obvious class where this operation belongs. Think of simulating a sound when a drumstick（鸡腿） hits a drum. There are many kinds of drumsticks, and many kinds of drums; their **combinations** produce different sounds. Say we want to write a function (or family of functions) that determines which sound is produced. Should this function be a method of the `Drum` class or the `DrumStick` class? Forcing this decision is one of the follies（罪恶） of classical OOP, and **multiple dispatch** helps us solve it naturally without adding a kludg（组装）e into our design.

 

#### See also

下面是讲述Multiple dispatch非常好的文章:

1) thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)

收录在 `C++\Pattern\Visitor-pattern` 章节。

### Emulating(模拟) multiple dispatch using multiple single dispatch

本节标题的含义是: 使用使用多个single dispatch来实现/模拟multiple dispatch相同的效果。显然这种实现方式势必会引入两个method、两次method invocation。

关于此的一个典型的例子是 thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) 中提出的方法。

### Examples

下面是一些典型的例子:

1 C++ function overload 

考虑所有的function argument，可以看做是multiple dispatch。关于此的一个典型的例子是 thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) 中给出的样例程序。

2 Subtyping polymorphism

对于支持OOP的programming language，在实现subtyping polymorphism的时候，需要考虑是采用single dispatch还是采用multiple dispatch，参见 `Theory\Programming-paradigm\Object-oriented-programming\Subtyping-polymorphism\Dispatch` 章节。

3 C++ template specialization

考虑所有的template parameter，可以看做是multiple dispatch。





##  Dispatch的搜索和排序

> NOTE: dispatch的过程可以看做是"搜索和排序"。

无论是哪种polymorphism，在实现的时候都需要考虑one-to-many的问题，即需要考虑从set  of candidates(concrete/implementation)中的选择哪一个来作为最终的实现。

显然，dispatch的过程**可能**涉及到了如下步骤:

> NOTE: 上面用到了**可能**这个词语，这是因为programming language实现polymorphism的差异性，并且有的implementation，将下面的两个步骤合并在一起了，比如OOP subtyping polymorphism，它的搜索和排序都是沿着class hierarchy自底向上进行的，由于class hierarchy已经是有序的了，因此，它在**搜索**过程中已经进行了**排序**。

### 1) 搜索

> 如何找到所有的candidates(concrete/implementation)

搜索到所有的candidates。一般，programming language的实现，往往需要搜索所有的candidates，搜索过程往往遵循: "**try my best principle**"，即:

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


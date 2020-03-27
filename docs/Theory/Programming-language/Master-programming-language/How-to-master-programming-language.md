# How to master programming language

你是否思考过：how many programming languages are there？Google了一下，第一个回答是[there are about **700 programming languages**](https://careerkarma.com/blog/how-many-coding-languages-are-there/)，如此 之多。维基百科的[List of programming languages](https://en.wikipedia.org/wiki/List_of_programming_languages)中，收录了大部分programming language，从[Timeline of programming languages](https://en.wikipedia.org/wiki/Timeline_of_programming_languages)中，我们大致可以看到 programming language的变迁与发展。面对如此之多、不断发展、层出不穷的programming language，我不禁思考：how to master programming language？本文就对这个问题来进行探讨。



## 从语言的设计者的角度出发来学习一门语言

让我产生这种想法的是在对c和`c++`中都有的[Storage duration和Linkage](https://en.cppreference.com/w/c/language/storage_duration)概念的思考：

在c和`c++`中都有[Storage duration和Linkage](https://en.cppreference.com/w/c/language/storage_duration)的概念，在对这两个概念的理解过程中，我发现这两个概念是和program的编译、运行有着非常密切的关联；后来想想，这其实是必然的，c++和c是system language，它是允许programmer操作memory的，如果我们站在c和c++语言的设计者的角度来思考，他们在设计这两门语言的时候，肯定也需要考虑程序在编译过程、运行过程中可能涉及到的一些细节，所以这门语言需要提供各种各样的specifier来供programmer对这两个过程进行控制（或者说对这两个过程进行描述）；

从根本上来说，我们使用programming language来描述计算（在文章[Language](https://dengking.github.io/Post/Language/Language/)中有对此的分析），越是底层的语言，需要考虑的计算细节越多，所以语言的设计者需要提供specifier来对这些内容进行描述，进行控制。

如何设计一门programming language？或者说：语言的设计者在设计这一门语言的时候，需要考虑到哪些问题？这个问题我无法给出准确的答案，只能够给出基于使用python、c++和c的经验给出一些经验之谈，当我们阅读这些语言的official doc的时候，会发现programming language主要涉及两大块：language reference和standard library，其实，这些就是这门语言的standard，下面以此为主要线索来进行详细说明。





## Language reference

（基本上）每种programming language都会有它的Language reference，programming language虽然多，但是其实它们的Language reference会涉及很多相同的主题，所以如果我们对这些common主题有一定的了解，那么掌握一门programming language会变得相对容易。

### Syntax 

programming language属于[formal language](https://en.wikipedia.org/wiki/Formal_language)，所以都会定义严格的syntax，“syntax"即语法。正如维基百科[Syntax (programming languages)](https://en.wikipedia.org/wiki/Syntax_(programming_languages))的[Levels of syntax](https://en.wikipedia.org/wiki/Syntax_(programming_languages)#Levels_of_syntax)段所总结的：

> Computer language syntax is generally distinguished into three levels:
>
> - Words – the lexical level, determining how characters form tokens;
> - Phrases – the grammar level, narrowly speaking, determining how tokens form phrases;
> - Context – determining what objects or variables names refer to, if types are valid, etc.

上述“Words”即词法，“Phrases”即语法，关于syntax，可以参考工程[compiler-principle](https://dengking.github.io/compiler-principle/)。

expression、statement等都是属于此范轴。

### Semantics 

参见维基百科[Semantics (computer science)](https://en.wikipedia.org/wiki/Semantics_(computer_science))。



### Type system

语言的设计者，需要考虑这门语言的type system，参见[Type system](../../Type-system/Type-system.md)。

语言的设计者还会向开发者提供对type system进行操作的接口，比如c++提供了`typeid`，`dynamic_cast`，python提供了`isinstance`，Java提供了`isinstanceof`。

### Runtime model

前面都是language的静态时，还需要对run time进行说明，比如data model、程序的运行模型等。

#### Data model

语言的设计者需要为 这门语言定义统一的data model，比如The Python Language Reference[¶](https://docs.python.org/3/reference/index.html#the-python-language-reference)中就有专门描述Data model[¶](https://docs.python.org/3/reference/datamodel.html#data-model)的 章节，与此类似的是，在cppreference的[Object](https://en.cppreference.com/w/cpp/language/object)中，对c++语言的data model进行了总结。



#### Run model

语言的设计者会假定该使用该语言所编写的程序运行与一个abstract machine上以便对run model进行描述，比如在cppreference的[Memory model](https://en.cppreference.com/w/cpp/language/memory_model)中有这样的描述：

> Defines the semantics of computer memory storage for the purpose of the C++ abstract machine.



在The Python Language Reference[¶](https://docs.python.org/3/reference/index.html#the-python-language-reference)的Execution model[¶](https://docs.python.org/3/reference/executionmodel.html#execution-model)中对python程序的run model进行了描述。

需要对程序的run model进行分析。

##### Abstract machines

[Stack machine](https://en.wikipedia.org/wiki/Stack_machine)





## Library

各种programming language都提供了大量的library，所以在学习一门programming language的时候，尤其需要注意其standard library。

一般standard library都会涉及到如下内容：

### Container

container指各种常见的数据结构，一般programming language的standard library都会包含这部分内容。



### Language support library

这是我在阅读[cppreference](https://en.cppreference.com/w/cpp/)的时候发现的一个概念，其中给出的[Language support library](https://en.cppreference.com/w/cpp/utility#Language_support)解释如下：

> Language support libraries provide classes and functions that interact closely with language features and support common language idioms.

依据此，python标准库中的很多library都可以归入此范轴：

- [Python Language Services](https://docs.python.org/3/library/language.html)
- [Python Runtime Services](https://docs.python.org/3/library/python.html)
- built-in



#### Run time info

比如python的标准库提供了Python Runtime Services[¶](https://docs.python.org/3/library/python.html#python-runtime-services)来供用户进行run time。



## Philosophy

programming language的设计者往往是遵循着一定的philosophy来设计这门语言的，作为使用者，了解这门语言的philosophy，也有助于我们对它的掌握。



## Programming paradigm

有很多语言都号称是支持“multiple [programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigms)”，我们有必要了解programming paradigm的知识，参见[Programming paradigm](../../Programming-paradigm/index.md)。



## [Programming language implementation](https://en.wikipedia.org/wiki/Programming_language_implementation)

在programming language中，也涉及标准与实现的问题，一种语言标准，可能有多种实现。

### Compiler and interpreter

目前主流的programming language都会涉及compile过程，所以它们的实现都会涉及compiler，一些语言（如python、java）运行于interpreter，关于compiler和 interpreter，可以参见工程[compiler-principle](https://dengking.github.io/compiler-principle/)。

#### Compile-time and run time

在学习一门语言的时候，从compile time和run time来分析各种概念是非常重要的。



## Common in development

下面罗列了在进行development中会涉及到的一系列问题：

### Design pattern

design pattern是前人所总结的最佳实践，不管是哪种programming  language，都可以运用它，关于design pattern，参见[Design-pattern](../../Design-pattern/index.md)。



### Resource-management

不管使用哪种programming language，都会涉及都resource management的问题，参见[Resource-management](../../Resource-management/Resource-management.md)。

### 注释

如何进行注释？

### 测试

如何进行测试？

### 代码覆盖率

如何统计代码覆盖率？

### Profile

如何进行profile？

### Linter

如何进行lint？



## 衍生问题：specifier

python中基本上没有什么specifier，但是在`c++`和`c`中存在着大量的specifier，为什么呢？

因为c++和c是system language，它们需要对process运行的底层细节进行描述、进行控制，这些specifier就是出于此目的的。
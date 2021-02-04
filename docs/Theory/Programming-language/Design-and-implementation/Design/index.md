# Design of programming language

本文接着上一篇进行描述。programming language虽然多，但是其实在设计一门programming language的时候，会涉及很多相同主题的内容，这也是我们在阅读各种programming language的Language reference时，会发现它们涉及了很多相同的主题。所以如果我们对这些common主题有一定的了解，那么掌握一门programming language会变得相对容易。

一般一门programming language的设计，可以分为两大块：

- language 
- standard library

## Language 

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





## Standard library

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





## Specification of expectation of type in class-based OOP language

这部分内容移到了`Theory\Programming-paradigm\Generic-programming\Specification-of-expectation-of-type.md`中。



## Design of specifier/keyword

语言为了简便，key word要尽可能地少，能够表达多种含义，因此一个key word能够用于多种entity，有多种用途，典型的例子有:

- [C++ `static` key word](https://en.cppreference.com/w/cpp/keyword/static)，它既可以用于object，也可以用于function的
- [C++ `extern`](https://en.cppreference.com/w/cpp/keyword/extern)
- [C++ `using`](https://en.cppreference.com/w/cpp/keyword/using)

## Design to an abstraction

本节标题的含义是: 对于general purpose programming language，designer在设计programming language的时候，是遵循如下原则的:

**Design to an abstraction, not an implementation**

它的意思是，designer在设计programming language的时候，是基于**abstract machine**来进行设计的，而不是specific to an concrete platform（physical machine、OS），然后由各个platform的开发者来完成abstraction到implementation的dispatch/map(映射)，即在该platform上实现这门语言。这是很多programming language实现cross platform的关键所在。

> NOTE: 
>
> "一个抽象(abstract)可以有多个实现(implementation)"，参见文章Abstraction。
>
> 这是我仿照OOP design principle中的: "Program to an 'interface', not an 'implementation'"而总结的，其实两者背后的思想是相同的，关于OOP design principle，参见: `Theory\Design-pattern\OOP-design-pattern\Principle`。
>
> 

### Example

下面以三种主流programming language为例来进行说明：

| language | implementation                                               |
| -------- | ------------------------------------------------------------ |
| Java     | 见下                                                         |
| Python   | python、java的跨平台是依赖于VM，这些VM也是由实现提供，比如VM for windows，VM for linux，由这些VM实现来完成abstract machine 到 physical machine 的映射。 |
| C++      | 跨平台由具体实现（compiler等）完成，MCVC for windows、gcc for linux，由这些compiler来完成abstract machine 到 physical machine的映射。 |



### Abstract machine

出于语言设计的目标，programming language designer对abstract machine的定义是不同的: 

Python都没有向programmer提供memory model，所以programmer无法直接操作memory；

Java、C++和C都提供了memory model，在C++和C中，programmer可以直接操作memory。



关于abstract machine，参见工程[automata-and-formal-language](https://dengking.github.io/automata-and-formal-language)。



### 相关文章

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)
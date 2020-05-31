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



##### Abstract machines

一般在设计一门programming language的时候，都是使用abstract machine来进行描述的。在工程[automata-and-formal-language](https://dengking.github.io/automata-and-formal-language)中对这方面内容进行了描述。

[Stack machine](https://en.wikipedia.org/wiki/Stack_machine)





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



## What's new in the programming language

> “Living languages must change,
> must adapt,
> must grow.”
> – Edward Finegan

上面这段话是Bjarne Stroustrup的[C++ in 2005](http://www.stroustrup.com/DnE2005.pdf)的开篇词，他的这篇文章描述了c++后续的发展方向。

Programming language是在不断演进的，各种programming language都会有自己的版本计划，在每个版本中，设计者会引入新的特性，这就是本节标题的What's new in the programming language的含义，即在这个版本中，引入了哪些新的特性。

作为开发者，在学习programming language的时候，可以从这些新特性，即what is new入手来进行学习，首先搞清楚设计者引入这些新特性的意图、这些新特性所能够解决的问题，然后再去了解它们的grammar。这种学习方式是更加高效的。

## Philosophy

programming language的设计者往往是遵循着一定的philosophy来设计这门语言的，作为使用者，了解这门语言的philosophy，也有助于我们对它的掌握。



[Philosophy of python](https://www.python.org/dev/peps/pep-0020/)

在维基百科[C++#Philosophy](https://en.wikipedia.org/wiki/C%2B%2B#Philosophy)中，描述了Philosophy of C++（在https://en.wikipedia.org/wiki/C++中对此有描述）



## Specification of expectation of type in class-based OOP language

这部分内容移到了`Theory\Programming-paradigm\Generic-programming\Specification-of-expectation-of-type.md`中。
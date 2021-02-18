# Interface

我们一直都在接触“接口”这个词语，在之前的描述中，我把“接口”定义为：对抽象的描述。有必要看看，它总结地比较好。



## Guide

1、wikipedia [Interface (computing)](https://en.wikipedia.org/wiki/Interface_(computing))

2、文章the interface principle

阅读完这篇文章，我的一个感受是: 一致性接口的价值。

这是[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)的文章[What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)中提出的一个观点，虽然它描述的是C++世界，但是我觉得它对interface的阐释是比较好的，这篇文章收录在了`gotw-What's-In-a-Class-The-Interface-Principle` 章节。



## 不同level的interface

从level角度来分析interface: 

| level                                                        |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Hardware interfaces](https://en.wikipedia.org/wiki/Interface_(computing)#Hardware_interfaces) |                                                              |
| [Software interfaces](https://en.wikipedia.org/wiki/Interface_(computing)#Software_interfaces) | 1、[Application programming interface](https://en.wikipedia.org/wiki/Application_programming_interface) aka API <br>2、[Application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface) aka ABI |

在`Software-design\Design-pattern\Architecture-pattern\Multilayered-architecture`的以层次思想来思考节会对两者进行区分。



## Compatibility

"Compatibility"即"兼容性"，在 `./Compatibility` 章节进行了描述。



## Interface-is-contract

本节标题的含义是: interface是协议、契约。



## Explicit 和 implicit interface

于此相关的内容有:

1、`Java\Guide\Generic-programming`

其中讨论了C++ VS Java generic programming，其实是比较C++ template和Java interface:

a、C++ template: implicit interface

b、Java interface: explicit interface

2、`Theory\Programming-paradigm\Object-oriented-programming\Subtyping-polymorphism\Subtyping-VS-inheritance`

其中也对这个topic进行了讨论。



## 与interface相关得到词语

下面几个词在OOP中是经常出现的，我觉得它们的有着相似的内涵，且它们都与behavior、interface有关:

|          | 简介                                                   | 链接                                                         |
| -------- | ------------------------------------------------------ | ------------------------------------------------------------ |
| protocol | 约定好protocol，其实就是约定好interface                | [Protocol (object-oriented programming)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)) |
| contract | design by contract，其实很大一部分内容是约定好protocol | wikipedia [Design by contract](https://en.wikipedia.org/wiki/Design_by_contract) <br> |



## Interface是对abstraction、behavior的描述/实现

1、"Interface is a description of behavior"

参见 `Theory\Programming-paradigm\Common\Behavior-based-and-interface` 章节

2、Interface is a description of abstraction

关于interface is a description of abstraction的一些素材:

a、wikipedia [Kernel_(operating_system)](https://en.wikipedia.org/wiki/Kernel_(operating_system)) 中有这样的描述:

> The kernel's [interface](https://en.wikipedia.org/wiki/Application_programming_interface) is a [low-level](https://en.wikipedia.org/wiki/High-_and_low-level) [abstraction layer](https://en.wikipedia.org/wiki/Abstraction_layer). When a [process](https://en.wikipedia.org/wiki/Process_(computing)) requests a service to the kernel, it must invoke a [system call](https://en.wikipedia.org/wiki/System_call)

b、Abstraction and architecture，参见工程software-engineering的`Software-design\Principle\Abstraction`章节



## "program to interface"  principle

参见: 

1、`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Principle`

2、`Theory\Programming-paradigm\Common\Abstraction-and-polymorphism\Program-to-an-abstraction`



## Uniform/consistent interface

统一的描述方式，其实和一致性接口类似，最最典型的例子就是: unified initialization

#### C++ trait is an interface

trait 是一种interface/abstraction，关于此参见: `C++\Idiom\Templates-and-generic-programming\SFINAE-trait-enable-if\Trait`




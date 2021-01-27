# Reference counting

Reference counting的优势:

1、相对简单、容易实现

## Draft: 无处不在的counting

技术，并且当count归0的时候，就执行某个操作，这在computer science中有着非常广泛的application:



### Automatic resource management

需要注意的是，本节标题使用的是"resource"而不是"memory"，它的含义是: Reference counting不仅仅可以用于管理memory，还可以用于管理更加广泛的resource。下面是一些例子:

1、`shared_ptr`

当reference个数为0的时候，则析构object；

2、reader write lock

当reader个数为0的时候，则唤醒writer；

当writer个数为0的时候，则唤醒reader；

3、Memory reclamation

在lockfree programming中，它有着非常广泛的应用；



## wikipedia [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **reference counting** is a programming technique of storing the number of [references](https://en.wikipedia.org/wiki/Reference_(computer_science)), [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)), or [handles](https://en.wikipedia.org/wiki/Handle_(computing)) to a resource, such as an object, a block of memory, disk space, and others.

### Advantages and disadvantages

> NOTE: 下面的这一段，着重描述的是:
>
> 1、Reference counting VS [tracing garbage collection](https://en.wikipedia.org/wiki/Tracing_garbage_collection)

#### Objects are reclaimed *as soon as* they can no longer be referenced

The main advantage of the reference counting over [tracing garbage collection](https://en.wikipedia.org/wiki/Tracing_garbage_collection) is that objects are reclaimed *as soon as* they can no longer be referenced, and in an incremental fashion, without long pauses for collection cycles and with clearly defined lifetime of every object. 



#### Simplest forms of memory management

Reference counting is also among the simplest forms of memory management to implement. 

#### Multiuse

> NOTE: 多用途

It also allows for effective management of non-memory resources such as operating system objects, which are often much scarcer than memory (tracing garbage collection systems use [finalizers](https://en.wikipedia.org/wiki/Finalizer) for this[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*], but the delayed reclamation may cause problems). [Weighted reference counts](https://en.wikipedia.org/wiki/Reference_counting#Weighted_reference_counting) are a good solution for garbage collecting a distributed system.

## wikipedia [Dealing with reference cycles](https://en.wikipedia.org/wiki/Reference_counting#Dealing_with_reference_cycles)

> NOTE: 第一次阅读的时候，并没有搞清楚reference cycle的含义，原文中的这样的一段话：
>
> the [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework, for instance, recommends using "strong" references for parent-to-child relationships and "weak" references for child-to-parent relationships.
>
> 当parent-to-child和child-to-parent都使用strong reference的时候，则两者相互引用，则两者都不可能的reference count都不可能成为0，则两者最终都无法被析构。



## stackexchange [Why are reference-counting smart pointers so popular?](https://softwareengineering.stackexchange.com/questions/208182/why-are-reference-counting-smart-pointers-so-popular)

> NOTE: 主要对比reference counting 和 garbage collection

[A](https://softwareengineering.stackexchange.com/a/208183)

> NOTE: 这个回答着重强调的是reference counting的优势

Some advantages of reference counting over garbage collection:

1、Low overhead.

2、More predictable behavior. 

3、Simpler.

4、Standard.

## Implementation

### spdlog

spdlog使用`std::shared_ptr`来实现reference counting。

### 那如何使用`std::atomic`来实现呢？

TODO



## TODO



mortoray [What is reference counting?](https://mortoray.com/2012/01/08/what-is-reference-counting/)



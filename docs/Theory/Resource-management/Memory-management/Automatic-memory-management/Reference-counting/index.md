# Reference counting

Reference counting的优势:

1、相对简单、容易实现

## 无处不在的counting

计数，并且当count归0的时候，就执行某个操作，这在computer science中有着非常广泛的application:

### Automatic resource management

本节标题的含义是: 自动资源管理。

需要注意的是，本节标题使用的是"resource"而不是"memory"，它的含义是: Reference counting不仅仅可以用于管理memory，还可以用于管理更加广泛的resource，在 wikipedia [Reference counting](https://en.wikipedia.org/wiki/Reference_counting) 的"Advantages and disadvantages"章节也提出了相同的观点。



下面是一些例子:

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

> NOTE: 
>
> 一、多用途
>
> 这段话的意思是reference counting可以用于实现通用的"Automatic resource management"
>
> 二、"scarcer"的意思是"稀少的"

It also allows for effective management of non-memory resources such as operating system objects, which are often much scarcer(稀少) than memory (tracing garbage collection systems use [finalizers](https://en.wikipedia.org/wiki/Finalizer) for this, but the delayed reclamation may cause problems). [Weighted reference counts](https://en.wikipedia.org/wiki/Reference_counting#Weighted_reference_counting) are a good solution for garbage collecting a distributed system.

### Graph interpretation

When dealing with garbage collection schemes, it is often helpful to think of the **reference graph**, which is a [directed graph](https://en.wikipedia.org/wiki/Directed_graph) where the [vertices](https://en.wikipedia.org/wiki/Vertex_(graph_theory)) are objects and there is an edge from an object A to an object B if A holds a reference to B. 



### Dealing with reference cycles

> NOTE: 
>
> 一、第一次阅读的时候，并没有搞清楚reference cycle的含义，原文中的这样的一段话：
>
> "the [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework, for instance, recommends using "strong" references for parent-to-child relationships and "weak" references for child-to-parent relationships."
>
> 当parent-to-child和child-to-parent都使用strong reference的时候，则两者相互引用，则两者都不可能的reference count都不可能成为0，则两者最终都无法被析构。
>
> 二、上述 "[Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework" 的例子就是非常典型的"break cycle"。

Perhaps the most obvious way to handle reference cycles is to design the system to avoid creating them. A system may explicitly forbid reference cycles; file systems with [hard links](https://en.wikipedia.org/wiki/Hard_link) often do this. Judicious use of ["weak" (non-counted) references](https://en.wikipedia.org/wiki/Weak_reference) may also help avoid retain cycles; the [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework, for instance, recommends using "strong" references for parent-to-child relationships and "weak" references for child-to-parent relationships.

Computer scientists have also discovered ways to [detect](https://en.wikipedia.org/wiki/Cycle_detection_(graph_theory)) and collect reference cycles automatically, without requiring changes in the data structure design. One simple solution is to periodically use a [tracing garbage collector](https://en.wikipedia.org/wiki/Tracing_garbage_collection) to reclaim cycles; since cycles typically constitute a relatively small amount of reclaimed space, the collector can be run much less often than with an ordinary tracing garbage collector.



## wikipedia [Weak reference](https://en.wikipedia.org/wiki/Weak_reference)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **weak reference** is a [reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) that does not protect the referenced [object](https://en.wikipedia.org/wiki/Object_(computer_science)) from collection by a [garbage collector](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)), unlike a strong reference. An object referenced *only* by weak references – meaning "every chain of references that reaches the object includes at least one weak reference as a link" – is considered *[weakly reachable](https://en.wikipedia.org/wiki/Weakly_reachable),* and can be treated as [unreachable](https://en.wikipedia.org/wiki/Unreachable_memory) and so may be collected at any time. 

> NOTE:
>
> 一、上面这段话的意思是:  "*[weakly reachable](https://en.wikipedia.org/wiki/Weakly_reachable)*" 相当于 "[unreachable](https://en.wikipedia.org/wiki/Unreachable_memory)"，因此它是能够被回收的。

### Uses

> NOTE:
>
> 一、本段的总结非常好

Weak references have a number of common uses. 

1、When using [reference counting](https://en.wikipedia.org/wiki/Reference_counting) garbage collection, weak references can break [reference cycles](https://en.wikipedia.org/wiki/Reference_cycle), by using a weak reference for a link in the cycle. 

> NOTE:
>
> 一、这一段非常好的总结了break cycle: "using a weak reference for a link in the cycle"

2、When one has an [associative array](https://en.wikipedia.org/wiki/Associative_array) (mapping, hash map) whose keys are (references to) objects, for example to hold auxiliary data about objects, using weak references for the keys avoids keeping the objects alive just because of their use as a key. 

> NOTE:
>
> 一、使用weak reference来避免"keeping the objects alive"

3、When one has an object where other objects are registered, such as in the [observer pattern](https://en.wikipedia.org/wiki/Observer_pattern) (particularly in [event handling](https://en.wikipedia.org/wiki/Event_handling)), if a strong reference is kept, objects must be explicitly unregistered, otherwise a memory leak occurs (the [lapsed listener problem](https://en.wikipedia.org/wiki/Lapsed_listener_problem)), while a weak reference removes the need to unregister. 

> NOTE:
>
> 一、关于此，参见:
>
> 1、observer pattern章节，其中进行了讨论
>
> 2、nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 

4、When holding cached data that can be recreated if necessary, weak references allow the cache to be reclaimed, effectively producing discardable memory. This last case (a cache) is distinct from others, as it is preferable that the objects only be garbage collected if necessary, and there is thus a need for finer distinctions within weak references, here a stronger form of a weak reference. 

> NOTE:
>
> 一、上面这段话的翻译如下:
>
> "当保存可以在必要时重新创建的缓存数据时，弱引用允许回收缓存，从而有效地生成可丢弃内存。最后一种情况（缓存）与其他情况不同，因为最好只在必要时对对象进行垃圾收集，因此需要在弱引用中进行更精细的区分，这里是弱引用的一种更强大的形式。"

In many cases weak references do not need to be directly used, instead simply using a weak array or other [container](https://en.wikipedia.org/wiki/Container_(abstract_data_type)) whose keys or values are weak references.

### Garbage collection



A very common case of such strong vs. weak reference distinctions is in tree structures, such as the [Document Object Model](https://en.wikipedia.org/wiki/Document_Object_Model) (DOM), where parent-to-child references are strong, but child-to-parent references are weak. For example, Apple's [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework recommends this approach.



## Implementation

### spdlog

spdlog使用`std::shared_ptr`来实现reference counting。

### 那如何使用`std::atomic`来实现呢？

参见 `Impl-use-std-atomic` 章节。



## 素材

mortoray [What is reference counting?](https://mortoray.com/2012/01/08/what-is-reference-counting/)


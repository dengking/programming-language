[TOC]

# use `shared_ptr` to control object lifetime to avoid dangling pointer

这是一个非常重要的主题，在control theory中已经涉及这个topic了，这是没有提供具体的实现案例，在下面的文章中，给出了非常好的例子:

1、《shared_ptr-draft》

2、spdlog `async_logger`

使用reference counting所实现的control，是一种隐式的control、自动的control，这种control方式是更加高级的，在很多情况下，我们都是 需要这种更加高级的control方式的，因为它使得用户不用关注、调用很多的接口。

在 nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 中，对此进行了讨论；

> One of the biggest concerns dealing with the raw pointers is that sometimes it is hard to ensure that a raw pointer is not dangling or valid. **Automatic memory management** by *shared_ptr* leads to a safer and easier to maintain code. As long as a component or function holds a *shared_ptr*, the object managed by the *shared_ptr* stays in memory.

# dangling pointer and segment fault
access 一个 dangling pointer，就会导致segmentation fault

# dangling pointer的情况
1、multithread: 在multithread中，是非常容易出现dangling pointer的，比如:

event-driven/network library中，传入一个object来作为callback，下面会对这种情况进行详细分析；这种情况的最最典型的例子是: amust api。

# access outside of object lifetime

将一个object作为callback传入到另外一个event driven library中的做法是非常不好的，这种情况下，一般涉及multithread，这个object同时被多个thread access，这样的做法有如下劣势:

## 1

1、如果在这个object的constructor中，使用`this`传入到另外一个library中，则这就导致了access outside of object lifetime；

在这个object的constructor的执行过程中，可能library中的thread就开始access它了，那么这就发生了access outside of object lifetime，这个问题，我之前碰到过，就是在是使用研发中心的仲裁插件的时候。

相关问题: Google: pass this in constructor c++: 

stackoverflow [Passing “this” to a function from within a constructor?](https://stackoverflow.com/questions/445127/passing-this-to-a-function-from-within-a-constructor)

https://isocpp.org/wiki/faq/ctors#using-this-in-ctors

"call-virtual-during-During construction and destruction"

## 2


2、dangling reference: 这个object可能已经被析构了，但是在library中，依然尝试去access它，这也是access outside of object lifetime

# calling virtual during construction and destruction




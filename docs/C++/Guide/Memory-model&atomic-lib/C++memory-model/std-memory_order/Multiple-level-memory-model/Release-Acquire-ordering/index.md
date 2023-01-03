# Release-Acquire ordering

首先介绍 Acquire-release semantic，然后阅读 cppreference 中的说明。

## Acquire-release semantic



### zhihu [现代C++的内存模型](https://zhuanlan.zhihu.com/p/382372072)

> NOTE:
>
> 一、关于acquire-release-semantic的最好的解释就是这篇文章中给出的，并且基于其中的模型，我们是能够很快地掌握acquire- release-semantic的

多线程编程，临界区是一个很重要的概念。我们对此再做进一步的认识。

对于临界区区内的语句，显而易见，我们不能将其移出临界区，如图5所示，编译器和CPU都不会做这种移出临界区的优化：

![img](https://pic4.zhimg.com/80/v2-b98871fbc8c78e36c725ef1613d32cfb_1440w.webp)

图5. 不能移出临界区区内代码

但我们可以将临界区区外的代码移进来，如图6所示

![img](https://pic3.zhimg.com/80/v2-b4e18614982ee5c9f9f3222675c967a6_1440w.webp)

图6. 临界区区外代码可以移入临界区

从图5和图6我们可以看出，lock和unlock可以看作两个单方向的屏障，lock对应的屏障，只允许代码往下方向移动，而unlock则只允许上方向移动。

C++内存模型借鉴lock/unlock，引入了两个等效的概念，Acquire（类似lock）和Release（类似unlock），这两个都是单方向的屏障（One-way Barriers: acquire barrier, release barrier）。

借助于Acquire & Release语义，在比互斥锁更底层的层面，我们也能实现线程间同步。先来看图7中的例子，p是我们后面会介绍的原子变量（std::atomic<X*>），当线程2看到p为非空时，后续的r1 = A语句可以看到线程1对A的写操作结果。这是因为线程1对p的store操作Synchronizes-with线程2对p的load操作，这样加之Happens-before关系，我们实现了线程间同步。值得一提的是，线程1中，”A=B+1” Happens-before “B = 1”，但”B = 1”可能先执行，但线程1对A的写操作的结果，依旧能正确地被线程2中”int r1 = A”语句读取。

有人也许会问，用非原子变量难道不能实现Synchronizes-with关系？首先，非原子变量，显然不能被多个线程同时读写，再者，其无法提供内存屏障，不要忘了前文重排的例子，试想，若线程1中A和B的写入操作被重排到了p.store的后面，且即使替换p.store的非原子变量操作在多线程下可以正确执行，但此时"int r1 = A"能读取到正确的值吗？

![img](https://pic1.zhimg.com/80/v2-10fa317d8cca122cd63dbb8400488470_1440w.webp)

图7. 使用内存顺序为Acquire/Release模型进行同步

值得说明的是，线程2需要看到线程1对原子变量p的写结果，才能认为线程1 synchronizes-with 线程2，所以线程2对p的读取是在while中进行的。相比于mutex，两个线程都不再需要lock/unlock操作，而仅靠对原子变量的写/读(release/acquire)便能进行同步。

上述线程1和线程2中用到的memory_order_release和memory_order_acquire，其实是比SC-DRF更为宽松的模型，C++默认的SC-DRF对应为memory_order_seq_cst，使用该默认模型的操作，上移下移都不被允许，相当于双向屏障，可以用图8稍作总结。

![img](https://pic4.zhimg.com/80/v2-4f469e7c62c43a4ccd3acc31af5f0def_1440w.webp)





### stackoverflow [How to understand acquire and release semantics?](https://stackoverflow.com/questions/24565540/how-to-understand-acquire-and-release-semantics)



[A](https://stackoverflow.com/a/24565699)

> NOTE:这个回答是非常好的，结合了具体的例子，通俗易懂

I'll take a stab at that.

> NOTE: 我来试一试。

Something to remember is that the compiler, or the CPU itself, may reorder memory reads and writes if they appear to not deal with each other.

This is useful, for instance, if you have some code that, maybe is updating a structure:

```c++
if ( playerMoved ) {
  playerPos.X += dx;
  playerPos.Y += dy; 

  // Keep the player above the world's surface.
  if ( playerPos.Z + dz > 0 ) {
     playerPos.Z += dz;
  }
  else {
     playerPos.Z = 0;
  }
}
```

Most of above statements may be reordered because there's no **data dependency** between them, in fact, a superscalar(超标量) CPU may execute most of those statements simultaneously, or maybe would start working on the `Z` section sooner, since it doesn't affect `X` or `Y`, but might take longer.

> NOTE: 
>
> 1、没有dependency，则compiler/CPU是可以free to reorder的
>
> 2、上述 `Z` section 指的是:
>
> ```C++
>   // Keep the player above the world's surface.
>   if ( playerPos.Z + dz > 0 ) {
>      playerPos.Z += dz;
>   }
>   else {
>      playerPos.Z = 0;
>   }
> ```
>
> 

Here's the problem with that - lets say that you're attempting **lock-free programming**. You want to perform a whole bunch of memory writes to, maybe, fill in a shared queue. You signal that you're done by finally writing to a flag.

> NOTE: 
>
> 1、需要注意，这段话的前提是**lock-free programming**；如果使用lock的话，则就不存在这个问题了；
>
> 2、作者此处所列举的例子就是: write-flag-notify-read-model，解决方法就是: read-acquire-write-release

Well, since that flag appears to have nothing to do with the rest of the work being done, the compiler and the CPU may reorder those instructions, and now you may set your '`done`' flag before you've actually committed the rest of the structure to memory, and now your "lock-free" queue doesn't work.

This is where Acquire(获得) and Release(释放) ordering semantics come into play:

I set(设置) that I'm doing work by setting a flag or so with an **Acquire semantic**, and the CPU guarantees that any memory games I play after that instruction stay actually below that instruction. 

I set(设置) that I'm done by setting a flag or so with a **Release semantic**, and the CPU guarantees that any memory games I had done just before the release actually stay before the release.

> NOTE: 画一条线，它能够区分两个方向: before、after；因此需要两个instruction来分别标识这两个方向: 
>
> 1、**Acquire semantic**: after; 它之前的还是可以reorder到它的后面
>
> 2、**Release semantic**: before; 它之后的还是可以reorder到它的前面
>
> 其实这非常类似于`tf.control_dependency`。

Normally, one would do this using explicit locks - mutexes, semaphores, etc, in which the CPU already knows it has to pay attention to memory ordering. The point of attempting to create 'lock free' data structures is to provide data structures that are thread safe (for some meaning of thread safe), that don't use explicit locks (because they are very slow).

Creating lock-free data structures is possible on a CPU or compiler that doesn't support acquire/release ordering semantics, but it usually means that some slower memory ordering semantic is used. For instance, you could issue a full **memory barrier** - everything that came before this instruction has to actually be committed before this instruction, and everything that came after this instruction has to be committed actually after this instruction. But that might mean that I wait for a bunch of actually irrelevant memory writes from earlier in the instruction stream (perhaps function call prologue) that has nothing to do with the memory safety I'm trying to implement.

Acquire says "only worry about stuff after me". Release says "only worry about stuff before me". Combining those both is a **full memory barrier**.

> NOTE: 前面这两段话说明了: **Full memory barrier**、**Acquire semantic**、**Release semantic** 三者之间的关联。





### microsoft [Acquire and release sound like bass fishing terms, but they also apply to memory models](https://devblogs.microsoft.com/oldnewthing/20081003-00/?p=20663)

Many of the normal interlocked operations come with variants called `InterlockedXxxAcquire` and `InterlockedXxxRelease`. What do the terms `Acquire` and `Release` mean here?

They have to do with the **memory model** and how aggressively the CPU can reorder operations around it.

An operation with *acquire* semantics is one which does not permit subsequent memory operations to be advanced before it. Conversely, an operation with *release* semantics is one which does not permit preceding memory operations to be delayed past it. (This is pretty much the same thing that [MSDN says on the subject of Acquire and Release Semantics](http://msdn.microsoft.com/en-us/library/aa490209.aspx).)

> NOTE: 
>
> 1、作者此处所列举的例子就是: write-flag-notify-read-model，解决方法就是: read-acquire-write-release

Consider the following code fragment:

```c++
int adjustment = CalculateAdjustment();
while (InterlockedCompareExchangeAcquire(&lock, 1, 0) != 0)
  { /* spin lock */ }
for (Node *node = ListHead; node; node = node->Next)
   node->value += adjustment;
InterlockedExchangeRelease(&lock, 0);
```

Applying *Acquire* semantics to the first operation operation ensures that the operations on the linked list are performed only after the `lock` variable has been updated. This is obviously desired here, since the purpose of the updating the `lock` variable is ensure that no other threads are updating the list while we’re walking it. Only after we have successfully set the lock to 1 is it safe to read from `ListHead`. On the other hand, the *Acquire* operation imposes no constraints upon when the store to the `adjustment` variable can be completed to memory. (Of course, there may very well be other constraints on the `adjustment` variable, but the Acquire does not add any new constraints.)

Conversely, *Release* semantics for an interlocked operation prevent pending(未决的) memory operations from being delayed past the operation. In our example, this means that the stores to `node->value` must all complete before the interlocked variable’s value changes back to zero(release保证之前的操作不能够移到后面). This is also desired, because the purpose of the lock is to control access to the linked list. If we had completed the stores after the lock was released, then somebody else could have snuck in, taken the lock, and, say, deleted an entry from the linked list. And then when our pending writes completed, they would end up writing to memory that has been freed. Oops.

> NOTE: 这段话没有读懂，其实就是release没有读懂

The easy way to remember the difference between *Acquire* and *Release* is that *Acquire* is typically used when you are acquiring a resource (in this case, taking a lock), whereas *Release* is typically used when you are releasing the resource.

> NOTE: 
>
> 1、前面的code fragment其实是一个典型的lock critical section，它是通过Acquire、Release实现的；
>
> 2、结合上述code example，它的resource就是lock，其实就是一个flag，显然，它就非常符合我们平时所说的read-acquire-write-release；

As [the MSDN article on acquire and release semantics](http://msdn.microsoft.com/en-us/library/aa490209.aspx) already notes, the plain versions of the interlocked functions impose both acquire and release semantics.

**Bonus reading**: [Kang Su](http://blogs.msdn.com/kangsu/) discusses [how VC2005 converts volatile memory accesses into acquires and releases](http://blogs.msdn.com/kangsu/archive/2007/07/16/volatile-acquire-release-memory-fences-and-vc2005.aspx).

[Raymond is currently away; this message was pre-recorded.]



## Idiom 

1、在介绍acquire-release的时候，都以这样的一个例子: 只有当flag被write thread置位后，read thread才能够执行操作，这其实就是"synchronizes-with"，按照在 "cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description" 中总结的理论: "synchronizes-with"-> "Inter-thread happens-before"->"Happens-before"，"Happens-before"能够保证 "Visible side-effects"，我们将它的实现方式简记为: write-release-flag-notify-read-acquire-model，可以看到，其中的实现方式都是: read-acquire-write-release；

2、需要ready之后才能够去读，需要禁止的操作: 在未ready之前就去read，即禁止CPU/compiler将acquire之后的read、write reorder到它之前；

3、acquire and release是memory ordering semantic

在 microsoft [Interlocked Variable Access](https://docs.microsoft.com/en-us/windows/win32/sync/interlocked-variable-access?redirectedfrom=MSDN) 中有如下描述: 

>  acquire and release memory ordering semantics 

4、preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/)



## cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Release-Acquire ordering

If an atomic store in thread A is tagged `memory_order_release` and an atomic load in thread B from the same variable is tagged `memory_order_acquire`, all memory writes (non-atomic and relaxed atomic) that *happened-before* the atomic store from the point of view of thread A, become *visible side-effects* in thread B. That is, once the atomic load is completed, thread B is guaranteed to see everything thread A wrote to memory.

> NOTE: 
>
> 1、关于这段话的翻译、理解，参见 "Acquire-release semantic" 段的`1`，那段话是我按照上面这段话编写的。
>
> 

### 使用modification order来进行描述

The synchronization is established only between the threads *releasing* and *acquiring* the same atomic variable. Other threads can see different order of memory accesses than either or both of the synchronized threads.

> NOTE: 
>
> 1、这段话总结地非常好，它是使用modification order来描述的: 
>
> 在参与"Release-Acquire synchronization"(实现inter-thread happens before)的thread之间建立modification order，因此这些参与的thread会看到相同的modification order，而未参与的thread则无法保证看到这个modification order。
>
> 需要注意的是，release semantic 和 acquire semantic仅仅是关于memory reordering的，它们需要结合其它的控制语句(典型的是while)才能够实现"inter-thread happens before"。
>
> 2、"order of write to shared data may be different among different threads"

### Implementation

On strongly-ordered systems — x86, SPARC TSO, IBM mainframe, etc. — release-acquire ordering is automatic for the majority of operations. No additional CPU instructions are issued for this synchronization mode; only certain compiler optimizations are affected (e.g., the compiler is prohibited from moving non-atomic stores past the atomic store-release or performing non-atomic loads earlier than the atomic load-acquire). 

On weakly-ordered systems (ARM, Itanium, PowerPC), special CPU load or memory fence instructions are used.

> NOTE: 
>
> 1、参见: 
>
> a、preshing [Weak vs. Strong Memory Models](https://preshing.com/20120930/weak-vs-strong-memory-models/)
>
> b、preshing [This Is Why They Call It a Weakly-Ordered CPU](https://preshing.com/20121019/this-is-why-they-call-it-a-weakly-ordered-cpu/)

### Example of release-acquire synchronization

Mutual exclusion locks, such as [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) or [atomic spinlock](https://en.cppreference.com/w/cpp/atomic/atomic_flag), are an example of release-acquire synchronization: when the lock is released by thread A and acquired by thread B, everything that took place in the critical section (before the release) in the context of thread A has to be visible to thread B (after the acquire) which is executing the same critical section.

> NOTE: 
> 1、lock、mutex  implementation
>
> 2、inter-thread happens before

### Example: producer-consumer

```C++
#include <thread>
#include <atomic>
#include <cassert>
#include <string>

std::atomic<std::string*> ptr;
int data;

void producer()
{
    std::string* p  = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)))
        ;
    assert(*p2 == "Hello"); // never fires
    assert(data == 42); // never fires
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}

// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```

### Example: transitive release-acquire ordering across three threads

The following example demonstrates transitive release-acquire ordering across three threads

```C++
#include <thread>
#include <atomic>
#include <cassert>
#include <vector>

std::vector<int> data;
std::atomic<int> flag = {0};

void thread_1()
{
    data.push_back(42);
    flag.store(1, std::memory_order_release);
}

void thread_2()
{
    int expected=1;
    while (!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
        expected = 1;
    }
}

void thread_3()
{
    while (flag.load(std::memory_order_acquire) < 2)
        ;
    assert(data.at(0) == 42); // will never fire
}

int main()
{
    std::thread a(thread_1);
    std::thread b(thread_2);
    std::thread c(thread_3);
    a.join(); b.join(); c.join();
}

```

## Application

典型的例子、并且是比较难以理解的是:

1、double checked locking

2、reference counting
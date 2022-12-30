# zhihu [现代C++的内存模型](https://zhuanlan.zhihu.com/p/382372072) 

本文将从编译器和CPU的优化，引出内存模型，最后再介绍内存模型的使用。

## 1、编译器和CPU的优化



我们可以看出，在速度上，CPU和内存之间有两个数量级的差异。试想，如果没有Cache，CPU每执行一条指令，都要去内存取下一条，而执行一条指令也就几个时钟周期（几ns），而取指令却要上百个时钟周期，这将导致CPU大部分时间都在等待状态，进而导致执行效率低下。

> NOTE:
>
> 一、本段介绍的其实就是"Von-Neumann-bottleneck"，显然cache是解决这个问题的手段



引入了Cache，解决了CPU等待的问题，但却产生了很多新的问题，例如[Cache的一致性](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Cache_coherence)，[Cache的缺失](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/CPU_cache%23Cache_miss)，[Cache的乒乓效应](https://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/30684974/are-cache-line-ping-pong-and-false-sharing-the-same%23%3A~%3Atext%3DCache%20line%20ping-ponging%20is%20the%20effect%20where%20a%2Cbe%20cause%20by%20either%20false%20or%20true%20sharing)等等，为了解决这些问题，各CPU平台（X86/IA64/ARM/Power…）都有自己的解决方案，软件层面（编译器）也会有对应的优化。这导致了CPU执行的程序，并不是你写的那个版本，只是从结果上看不出差别而已。如果你曾经单步调试过一个release版本的程序，你会发现运行过程很怪异，居然没有沿着你的代码顺序执行。因为，要是严格按照你的代码顺序执行，编译器和CPU都会抱怨，说执行效率很低速度很慢。

### [As-if rule](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/As-if_rule)

为什么它们执行了一个不一样的程序，我们竟然不知情？其实，主要原因是编译器和CPU优化都遵循了一个同样的原则（[As-if rule](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/As-if_rule)），即优化后的代码，[若是单线程执行，要与原有代码行为保持一致](https://link.zhihu.com/?target=https%3A//preshing.com/20120625/memory-ordering-at-compile-time/)。再加之多线程环境我们使用的互斥锁，其对编译器和CPU优化做了很多限制，可以让我们对线程间的执行顺序进行同步，进而保证了即使被优化成了另一个程序，仍然有相同的执行结果。

### Memory model is an abstraction、contract

若我们从比互斥锁更为底层地去了解多线程间的同步机制，我们势必会看到CPU平台（X86/IA64/ARMv7/ARMv8/Power…）和编译器（gcc/VC/clang…）的差异，进而可以知道优化的存在。现代C++的内存模型，便是为了屏蔽这些差异，而让你可以不用去了解特定平台特定编译器，也不用依赖互斥锁，就可以完成线程间的同步。C++11开始提供的`std::atomic<>`类模板，便可以作为更为底层的同步工具，这也是内存模型起作用的地方。



要了解内存模型，我觉得还是要先谈谈一些重要的优化，包括编译器和CPU/Cache方面。优化主要包括Reorder/Invent/Remove，正如图4所言，优化发生在哪一层并不重要。但有哪些类型的优化还是值得一提。

![img](https://pic3.zhimg.com/80/v2-7e5fd70c1c30243c2a2d67ed49f6ff0e_1440w.webp)

图4. 优化（Herb Sutter: atomic Weapons）

### Compiler reordering

对于重排（Reordering），编译器和CPU都会有重排，主要就是为了 [优化性能](https://link.zhihu.com/?target=https%3A//preshing.com/20120625/memory-ordering-at-compile-time/) 。既然发生在哪一层不重要，我们这里就从编译器方面来看，以下面代码为例，

```cpp
//reordering 重排示例代码
int A = 0, B = 0;
void foo()
{
    A = B + 1;  //(1)
    B = 1;      //(2) 
}
// g++ -std=c++11 -O2 -S test.cpp
// 编译器重排后的代码
// 注意第一句汇编，已经将B最初的值存到了
// 寄存器eax，而后将该eax的值加1，再赋给A
movl  B(%rip), %eax
movl  $1, B(%rip)          // B = 1
addl  $1, %eax             // A = B + 1
movl  %eax, A(%rip)
```

可以看到，`B=1`居然先于`A=B+1`被执行，不过，最终的结果仍然与重排前一致。对于单线程来说，这没什么问题。但是，对于多线程，若另一个线程依赖于未重排的假设，当B为1时，它认为`A=B+1`已经执行了。这样，就会发生非预期的行为。重排有什么好处呢？我们前文谈到了Cache，其实Cache的读写单位是**Cache line**，即一块内存区域，大小一般为64字节。若需要读写该区域中的某个变量，则CPU会将该区域整体进行读写。对于重排的例子，假设A和B在不同的Cache line，且假定没有重排优化，也就是第二句汇编放于最后，那我们设想一种情形，当CPU将B读入Cache之后，Cache已经满了，那么，若要接着读入A，这时候只能移出某些Cache line，且假设刚好B所在的Cache line被移出，这样导致最后一句（原先的第二句汇编）给B赋值时，又要再次读取B到Cache。要知道，读取变量到Cache是需要读写主存的，还记得图3中主存的访问速度吗？重排只需读取主存一次，而未重排则需要两次读取。

> NOTE:
>
> 一、"对于多线程，若另一个线程依赖于未重排的假设"
>
> 依赖于一个不成立的假设
>
> 二、越集中则越有利于优化



### Compiler invention

再来说Invention，其实这主要涉及CPU的[Speculative execution](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Speculative_execution)，我们看如下代码，

```cpp
// Invention示例代码
// 原始代码
if( cond ) x = 42;

// 优化后代码
r1 = x;// read what's there
x = 42;// oops: optimistic write is not conditional
if( !cond)// check if we guessed wrong
    x = r1;// oops: back-out write is not SC
```

我们看到，相比于最初的代码，多了一次对x的写入。同样，对于单线程来说，没有什么问题，但是对于多线程，显然是有问题的，另一个线程在cond为false的情况下，依然可能看到x为42.

当然，Invention还有很多其它情况，这里就不细表了。

### Compiler removal

最后说说removal，一般是编译器在优化时会移除一些不影响行为的代码，比如，未使用的变量、if(false)代码块的语句等。本文在讨论内存模型时，不会涉及此优化，所以就此略过。



从上面的几类优化可以看出，即使编译器和CPU遵循As-if原则，优化仍然可能会影响多线程程序的行为。当然，在多线程编程中，我们一般借助于互斥锁去同步相关线程，即使在优化存在的条件下仍然能得到预期的程序行为。但是，互斥锁是较为高层的同步工具。C++11引入的现代C++内存模型，可以使我们在不借助互斥锁时，从更为底层的层面，让我们写出符合预期行为的多线程程序。

## 2、现代C++的内存模型

谈内存模型之前，先谈谈如下几个概念，以便于后续分析多线程的同步过程。

### （1）顺序一致（Sequential consistency）

编译器和CPU需要严格按照代码顺序进行生成和执行。对于多线程程序，由图1架构可知，每个线程的指令执行顺序由各自的CPU来控制，因而都有独立的执行顺序，但因为它们共享同一个内存，而内存修改的顺序是全局的（global memory order），我们可以以该全局顺序为参照，对所有线程进行同步。其实，我们用互斥锁来推导线程间同步顺序时，也是基于类似这种**顺序一致**的前提。但**顺序一致**更为严格，对于内存读写来说，读写顺序需要严格按照代码顺序，即要求如下（符号”<p”表示程序代码顺序，符号”<m”表示内存的读写顺序）：

```cpp
// 顺序一致的要求
/* Load→Load */
/*若按代码顺序，a变量的读取先于b变量，
则内存顺序也需要先读a再读b
后面的规则同理。*/
if L(a) <p L(b) ⇒ L(a) <m L(b) 

/* Load→Store */
if L(a) <p S(b) ⇒ L(a) <m S(b) 

/* Store→Store */	
if S(a) <p S(b) ⇒ S(a) <m S(b) 

 /* Store→Load */
if S(a) <p L(b) ⇒ S(a) <m L(b)	
```

既然顺序一致这么严格，其显然会限制编译器和CPU的优化，所以业界提出了很多宽松的模型，例如在X86中使用的[TSO](https://link.zhihu.com/?target=https%3A//www.cis.upenn.edu/~devietti/classes/cis601-spring2016/sc_tso.pdf)（Total Store Order）便允许某些条件下的重排。

从程序员（a programmer-centric approach）的角度，抛开编译器和硬件，也即从语言层面，我们也可以提出合适的模型。现代C++（包括Java）都是使用了SC-DRF(Sequential consistency for data race free)。在SC-DRF模型下，程序员只要不写出[Race Condition](https://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/34510/what-is-a-race-condition)的代码，编译器和CPU便能保证程序的执行结果与顺序一致相同。**因而，内存模型就如同程序员与编译器/CPU之间的契约，需要彼此遵守承诺。C++的内存模型默认为SC-DRF，此外还支持更宽松的非SC-DRF的模型。**



### （2）Happens-before

按代码顺序，若语句A在语句B的前面，则语句A Happens Before 语句B。对同一个线程来说，前面的代码Happens Before后面的代码。线程之间的语句也有Happens Before的关系，这需要借助我们后面讲的synchronizes-with来确定。值得说明的是，[Happens Before](https://link.zhihu.com/?target=https%3A//preshing.com/20130702/the-happens-before-relation/)并不表明语句A一定会在语句B之前执行，例如前面重排（Reordering）的例子。不过这并不影响此概念的引入和使用，后面在具体使用此概念时会看到原因。



### （3）Synchronizes-with

用于线程间的同步，通常借助原子变量的操作来实现。线程1对原子变量x进行写操作，若线程2对原子变量x进行读操作且读到了线程1的写结果，则线程1的写操作Synchronizes-with线程2的读操作。

假设线程1的原子操作A Synchronizes-with 线程2的原子操作B，则线程1中，所有Happens Before 操作A的其它所有写操作结果（包括原子和非原子），都对线程2中操作B之后的操作可见（也就是被操作B Happens Before的那些操作）。基于Synchronizes-with，我们可以推导出跨线程的Happens Before关系。

> NOTE:
>
> 一、inter-thread-happens-before=happens-before+synchronizes-with



### Acquire and Release Semantics

多线程编程，临界区是一个很重要的概念。我们对此再做进一步的认识。

对于临界区区内的语句，显而易见，我们不能将其移出临界区，如图5所示，编译器和CPU都不会做这种移出临界区的优化：

![img](https://pic4.zhimg.com/80/v2-b98871fbc8c78e36c725ef1613d32cfb_1440w.webp)

但我们可以将临界区区外的代码移进来，如图6所示

![img](https://pic3.zhimg.com/80/v2-b4e18614982ee5c9f9f3222675c967a6_1440w.webp)

图6. 临界区区外代码可以移入临界区

从图5和图6我们可以看出，lock和unlock可以看作两个单方向的屏障，lock对应的屏障，只允许代码往下方向移动，而unlock则只允许上方向移动。

C++内存模型借鉴lock/unlock，引入了两个等效的概念，Acquire（类似lock）和Release（类似unlock），这两个都是单方向的屏障（One-way Barriers: acquire barrier, release barrier）。

借助于Acquire & Release语义，在比互斥锁更底层的层面，我们也能实现线程间同步。先来看图7中的例子，`p`是我们后面会介绍的原子变量（`std::atomic<X*>`），当线程2看到 `p` 为非空时，后续的`r1 = A`语句可以看到线程1对 `A` 的写操作结果。这是因为线程1对 `p` 的store操作Synchronizes-with线程2对 `p` 的load操作，这样加之Happens-before关系，我们实现了线程间同步。值得一提的是，线程1中，”`A=B+1`” Happens-before “`B = 1`”，但”`B = 1`”可能先执行，但线程1对A的写操作的结果，依旧能正确地被线程2中”`int r1 = A`”语句读取。

![img](https://pic1.zhimg.com/80/v2-10fa317d8cca122cd63dbb8400488470_1440w.webp)

图7. 使用内存顺序为Acquire/Release模型进行同步



有人也许会问，用非原子变量难道不能实现Synchronizes-with关系？首先，非原子变量，显然不能被多个线程同时读写，再者，其无法提供内存屏障，不要忘了前文重排的例子，试想，若线程1中 `A` 和 `B` 的写入操作被重排到了 `p.store` 的后面，且即使替换 `p.store` 的非原子变量操作在多线程下可以正确执行，但此时 "`int r1 = A`" 能读取到正确的值吗？

值得说明的是，线程2需要看到线程1对原子变量p的写结果，才能认为线程1 synchronizes-with 线程2，所以线程2对p的读取是在while中进行的。相比于mutex，两个线程都不再需要lock/unlock操作，而仅靠对原子变量的写/读(release/acquire)便能进行同步。



### Acquire-release VS Sequential-consistency

上述线程1和线程2中用到的 `memory_order_release` 和 `memory_order_acquire`，其实是比SC-DRF更为宽松的模型，C++默认的SC-DRF对应为 `memory_order_seq_cst`，使用该默认模型的操作，上移下移都不被允许，相当于双向屏障，可以用图8稍作总结。

![img](https://pic4.zhimg.com/80/v2-4f469e7c62c43a4ccd3acc31af5f0def_1440w.webp)

图8. 三种屏障 （可下移、可上移、不能跨越）

### Memory model

现代C++的内存模型，其实主要表现在对原子变量进行操作时，通过指定memory order（内存顺序）的参数，来控制线程间同步。图7的示例便是其中一种内存顺序模型（获取发布模型：Acquire-Release Ordering）。memory order是一个枚举值，定义如下：

```cpp
// 内存模型枚举值
typedef enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
} memory_order;
```

以上6个枚举值可以细分为三种内存顺序模型，见下表1

| 内存顺序模型                                 | Memory order枚举值                                           |
| -------------------------------------------- | ------------------------------------------------------------ |
| 顺序一致（sequentially consistent ordering） | memory_order_seq_cst，只有该值满足SC-DRF，其它都是比SC-DRF更宽松的模型，原子操作默认使用该值。 |
| 获取发布 （acquire-release ordering）        | memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel |
| 松散（relaxed ordering ）                    | memory_order_relaxed                                         |



## 3、`std::atomic<>`与内存模型 



### （1）顺序一致模型（sequentially consistent ordering）

对于顺序一致模型，因为所有线程都参考全局的内存修改顺序。因此，我们可认为所有变量的读写都直接从内存进行，从而完全不用考虑Cache，Store Buffer这些因素，而以图1的理想架构就能完成线程同步的推导。



值得指出的是，C++此处的顺序一致是指SC-DRF，而并不是真正的SC。如表2最后一条所言，前后语句都不能跨越使用该模型（memory_order_seq_cst）的原子操作，且有全局的内存顺序，满足此要求下的重排是允许的。例如，在不跨越该原子操作的前提下，该原子操作前面的几条语句之间的重排是允许的，当然，若还有其它原子操作，则也要满足表2对应的要求。

> NOTE:
>
> 一、即使sequential-consistency也是允许一定程度的重拍的



### （2）获取发布模型（acquire-release ordering）





### （3）松散模型（relaxed ordering）

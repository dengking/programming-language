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

### Memory model is an abstraction

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





从程序员（a programmer-centric approach）的角度，抛开编译器和硬件，也即从语言层面，我们也可以提出合适的模型。现代C++（包括Java）都是使用了SC-DRF(Sequential consistency for data race free)。在SC-DRF模型下，程序员只要不写出[Race Condition](https://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/34510/what-is-a-race-condition)的代码，编译器和CPU便能保证程序的执行结果与顺序一致相同。**因而，内存模型就如同程序员与编译器/CPU之间的契约，需要彼此遵守承诺。C++的内存模型默认为SC-DRF，此外还支持更宽松的非SC-DRF的模型。**

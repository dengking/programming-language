# 关于本章

在**指令层**压根就没有函数、函数参数等概念，这些概念都是high level program language建立起来的，它们的实现都是建立在**指令层**之上，连接函数调用和指令层的正是所谓的calling convention，它由compiler来实现的。

本章所将探索各种在high level program language中建立的概念是如何通过指令来进行实现的。

## 如何实现控制流

### 所谓的[Control flow](https://en.wikipedia.org/wiki/Control_flow)其实就是[`program counter`](https://en.wikipedia.org/wiki/Program_counter) 

我们常常听到Control flow，维基百科的[Control flow](https://en.wikipedia.org/wiki/Control_flow)对它的总结是非常全面的，从high-level programming language级别（在high-level programming language中有control flow statement，比如return、goto等），到[machine language](https://en.wikipedia.org/wiki/Machine_language)级别（这是最底层了；以x86 为例，[JMP](https://en.wikipedia.org/wiki/JMP_(x86_instruction)) 指令，更多参见[X86 Assembly/Control Flow](https://en.wikibooks.org/wiki/X86_Assembly/Control_Flow)）。在本文中，我们重点关注的是[machine language](https://en.wikipedia.org/wiki/Machine_language)级别，正如其所总结的：

> At the level of [machine language](https://en.wikipedia.org/wiki/Machine_language) or [assembly language](https://en.wikipedia.org/wiki/Assembly_language), control flow instructions usually work by altering the [program counter](https://en.wikipedia.org/wiki/Program_counter). For some [central processing units](https://en.wikipedia.org/wiki/Central_processing_unit) (CPUs), the only control flow instructions available are conditional or unconditional [branch](https://en.wikipedia.org/wiki/Branch_(computer_science)) instructions, also termed jumps.

CPU的[program counter](https://en.wikipedia.org/wiki/Program_counter)默认行为是：自加1的，所以程序默认是顺序执行即可（编译器编译生成的machine language program其实是顺序的），通过control flow instruction，可用改变这种默认行为，从而实现各种执行flow。

一个例子是在OS书的4.1. The Role of Interrupt Signals

> As the name suggests, interrupt signals provide a way to divert the processor to code outside the
> normal** flow of control**. When an interrupt signal arrives, the CPU must stop what it's currently doing and switch to a new activity; it does this by saving the current value of the program counter (i.e., the content of the  eip and  cs registers) in the Kernel Mode stack and by placing an address related to the interrupt type into the program counter.

正在不同的层次来看待本质上相同的事情，在program language层，我们把它叫做flow of control，在指令层，我们它其实是program counter。



## 如何实现函数调用
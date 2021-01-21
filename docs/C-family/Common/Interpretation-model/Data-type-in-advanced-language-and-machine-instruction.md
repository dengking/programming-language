# Data type in advanced language and machine instruction
## 前言

 [ISA](https://en.wikipedia.org/wiki/Instruction_set_architecture) serves as the [interface](https://en.wikipedia.org/wiki/Interface_(computing)) between [software](https://en.wikipedia.org/wiki/Software) and [hardware](https://en.wikipedia.org/wiki/Computer_hardware). 在advanced programming language中有着严格的type system，那么advanced programming language中的type和机器指令之间的对应关系是什么呢？



## [一直有个疑问是关于数据类型的](http://bbs.chinaunix.net/thread-3996911-1-1.html) 

高级语言中有**数据类型**，高级语言翻译成**机器指令**没有**数据类型**之说。比如说一条指令是访问一个内存地址也就是高级语言中的一个变量，那`cpu`怎么知道这个地址以后的几个字节都是属于这个变量的呢？也就是**数据类型**在**机器语言**级别是如何实现的？是不是有额外的表来记录程序每个需要用到的地址是几个字节，这个问题一直困扰着我，还请高人能解答。



***SUMMARY*** : 编译器根据对于不同的数据类型选择不同的机器指令；

### A

每条指令都包含着到底要运算、搬移具体到多少个字节的信息

### A

可以这么说，但也不全是，大家支持的很微妙。简单说，大多数RISC处理器在数据load的时候，可以通过**指令**区分load位宽和符号（0扩展或者符号扩展），但对于**数据处理指令**则并不一定区分，典型的`add指令`就不会区分**数据类型**。

**类型信息**并不会被显示存储，而是编译器根据高级语言中**数据类型信息**，来编排**汇编指令**，比如：

对`char`变量，编译器会在`load`的时候，通过专用的**字节load指令**并进行符号扩展，完成**内存**到**CPU内部寄存器**的载入；
再比如`unsigned short`变量，编译器也会在数据读取的时候安排一个0扩展的半字load；

而对于寄存器向外部存储器写入，则只控制位宽，不控制符号（你可以想想为什么）。

以上主要针对RISC，类似x86，则更为灵活，你可以深入研究研究。  

## [Type system](https://en.wikipedia.org/wiki/Type_system)

这篇文章中有这样的内容：

> A [compiler](https://en.wikipedia.org/wiki/Compiler) may also use the static type of a value to optimize the storage it needs and the choice of algorithms for operations on the value. In many [C](https://en.wikipedia.org/wiki/C_(programming_language)) compilers the *float* [data type](https://en.wikipedia.org/wiki/Data_type), for example, is represented in 32 [bits](https://en.wikipedia.org/wiki/Bit), in accord with(与什么一致) the [IEEE specification for single-precision floating point numbers](https://en.wikipedia.org/wiki/IEEE_754-2008). They will thus use floating-point-specific [microprocessor operations](https://en.wikipedia.org/wiki/Instruction_set) on those values (floating-point addition, multiplication, etc.)(它们将对这些值使用浮点特定的微处理器操作（浮点加法，乘法等），这是一种优化).

总结：这段胡以float类型的数据为例来说明：编译器在汇编阶段可以根据value的static type来优化对该value的存储以及选择处理该value的指令。
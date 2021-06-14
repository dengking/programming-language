# Valgrind



## wikipedia [Valgrind](https://en.wikipedia.org/wiki/Valgrind)

**Valgrind** ([/ˈvælɡrɪnd/](https://en.wikipedia.org/wiki/Help:IPA/English)) is a [programming tool](https://en.wikipedia.org/wiki/Programming_tool) for [memory debugging](https://en.wikipedia.org/wiki/Memory_debugger), [memory leak](https://en.wikipedia.org/wiki/Memory_leak) detection, and [profiling](https://en.wikipedia.org/wiki/Profiling_(computer_programming)).

> NOTE: 集成了多种功能

Valgrind was originally designed to be a [free](https://en.wikipedia.org/wiki/Free_software) [memory debugging](https://en.wikipedia.org/wiki/Memory_debugging) tool for [Linux](https://en.wikipedia.org/wiki/Linux) on [x86](https://en.wikipedia.org/wiki/X86), but has since evolved to become a generic framework for creating dynamic analysis tools such as checkers and profilers.

> NOTE: 关于上述"generic framework"，需要结合"wikipedia [Valgrind](https://en.wikipedia.org/wiki/Valgrind) # Overview"章节内容和 官网 [Valgrind](https://www.valgrind.org/) 中"Valgrind is an instrumentation framework for building dynamic analysis tools"中的内容来进行理解。

### Overview

Valgrind is in essence a [virtual machine](https://en.wikipedia.org/wiki/Virtual_machine) using [just-in-time](https://en.wikipedia.org/wiki/Just-in-time_compilation) (JIT) compilation techniques, including [dynamic recompilation](https://en.wikipedia.org/wiki/Dynamic_recompilation). Nothing from the original program ever gets run directly on the host [processor](https://en.wikipedia.org/wiki/Central_processing_unit). Instead, Valgrind first translates the program into a temporary, simpler form called **Intermediate Representation (IR)**, which is a processor-neutral, [SSA](https://en.wikipedia.org/wiki/Static_single_assignment_form)-based form. 

After the conversion, a *tool* (see below) is free to do whatever transformations it would like on the IR, before Valgrind translates the IR back into machine code and lets the host processor run it. 

Valgrind recompiles [binary code](https://en.wikipedia.org/wiki/Binary_code) to run on host and target (or simulated) CPUs of the same architecture. 

> NOTE: 各种IR的转换

It also includes a [GDB](https://en.wikipedia.org/wiki/GDB) stub to allow debugging of the target program as it runs in Valgrind, with "monitor commands" that allow querying the Valgrind tool for various information.

A considerable amount of performance is lost in these transformations (and usually, the code the tool inserts); usually, code run with Valgrind and the "none" tool (which does nothing to the IR) runs at 20% to 25% of the speed of the normal program.[[4\]](https://en.wikipedia.org/wiki/Valgrind#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Valgrind#cite_note-5)

### Tools

*See also:* [Memory debugger](https://en.wikipedia.org/wiki/Memory_debugger) *and* [Profiling (computer programming)](https://en.wikipedia.org/wiki/Profiling_(computer_programming))

#### Memcheck



## 官网 [Valgrind](https://www.valgrind.org/) 

Valgrind is an instrumentation framework for building dynamic analysis tools





## 简介

valgrind 是一套 Linux 下，开放源代码（GPL V2）的仿真调试工具的集合，包括以下工具：

![](./LeetCode-valgrind-1612687346-kyLXKn-image.png)



1、`Memcheck`：内存检查器（valgrind 应用最广泛的工具），能够发现开发中绝大多数内存错误的使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。

2、`Callgrind`：检查程序中函数调用过程中出现的问题。

3、`Cachegrind`：检查程序中缓存使用出现的问题。

4、`Helgrind`：检查多线程程序中出现的竞争问题。

5、`Massif`：检查程序中堆栈使用中出现的问题。

6、`Extension`：可以利用 core 提供的功能，自己编写特定的内存调试工具。

Memcheck 能够检测出内存问题，关键在于其建立了两个全局表：

一、Valid-Value 表：对于进程的整个地址空间中的每一个字节（byte），都有与之对应的 8 个 bits ；对于 CPU 的每个寄存器，也有一个与之对应的 bit 向量。这些 bits 负责记录该字节或者寄存器值是否具有有效的、已初始化的值。

二、Valid-Address 表：对于进程整个地址空间中的每一个字节（byte），还有与之对应的 1 个 bit，负责记录该地址是否能够被读写。

> NOTE: 
>
> 是否属于这个process

### 检测原理

当要读写内存中某个字节时，首先检查这个字节对应的 Valid-Address 表中对应的 bit。如果该 bit 显示该位置是**无效位置**，Memcheck 则报告读写错误。

内核（core）类似于一个虚拟的 CPU 环境，这样当内存中的某个字节被加载到真实的 CPU 中时，该字节在 Valid-Value 表对应的 bits 也被加载到虚拟的 CPU 环境中。一旦寄存器中的值，被用来产生内存地址，或者该值能够影响程序输出，则 Memcheck 会检查 Valid-Value 表对应的 bits，如果该值尚未初始化，则会报告使用未初始化内存错误。


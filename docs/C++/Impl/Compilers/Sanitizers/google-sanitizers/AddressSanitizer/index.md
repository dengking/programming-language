# AddressSanitizerAlgorithm



## wikipedia [AddressSanitizer](https://en.wikipedia.org/wiki/AddressSanitizer)

**AddressSanitizer** (or **ASan**) is an open source programming tool that detects [memory corruption](https://en.wikipedia.org/wiki/Memory_corruption) [bugs](https://en.wikipedia.org/wiki/Software_bug) such as [buffer overflows](https://en.wikipedia.org/wiki/Buffer_overflow) or accesses to a [dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer) (use-after-free). AddressSanitizer is based on [compiler](https://en.wikipedia.org/wiki/Compiler) [instrumentation](https://en.wikipedia.org/wiki/Instrumentation_(computer_programming)) and directly mapped [shadow memory](https://en.wikipedia.org/wiki/Shadow_memory). 

> NOTE:
>
> " directly mapped [shadow memory](https://en.wikipedia.org/wiki/Shadow_memory)"

## What is shallow shallow memory？

shallow即影子，它其实是关于"main application memory"的一些信息（metadata、shallow value），shadow memory 就是存储metadata、shallow value的memory，这在github google  [AddressSanitizerAlgorithm](https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm) 中，有着非常好的描述：

> Shadow memory (`Shadow`): this memory contains the shadow values (or metadata). There is a correspondence between the shadow and the main application memory. **Poisoning** a byte in the main memory means writing some special value into the corresponding shadow memory.

简而言之：

1、每个 "main application memor" 都有对应的 shallow、shallow memory。

2、通过shallow memory来获知"main application memory"的信息，显然如果"main application memory"已经被free了，那么再次access它，就是dangling，就是典型的memory access error。



## wikipedia [Shadow memory](https://en.wikipedia.org/wiki/Shadow_memory)

In [computing](https://en.wikipedia.org/wiki/Computing), **shadow memory** is a technique used to track and store information on [computer memory](https://en.wikipedia.org/wiki/Computer_memory) used by a [program](https://en.wikipedia.org/wiki/Computer_program) during its execution. Shadow memory consists of shadow bytes that map to individual bits or one or more bytes in main memory. These shadow bytes are typically invisible to the original program and are used to record information about the original piece of data.

### Technique

The technique is utilized by **memory-error checkers** that can store information on which parts of memory have been allocated to the program being checked. This shadow memory is then used for detecting and reporting incorrect accesses of memory, even though the program may not be crashing due to a [segmentation fault](https://en.wikipedia.org/wiki/Segmentation_fault) or similar. 

> NOTE:
>
> "该技术被内存错误检查器使用，它可以存储有关哪些内存部分已分配给正在检查的程序的信息。然后，此影子内存用于检测和报告内存的不正确访问，即使程序可能不会由于分段错误或类似情况而崩溃。"



AddressSanitizer on the other hand is created on compile-time and inserts error-checking code inline into a program during compilation. Its shadow-memory implementation uses a huge reservation of virtual memory for its shadow memory, giving very different performance characteristics.

> NOTE:
>
> 这段话总结地非常好。





## github google [AddressSanitizerAlgorithm](https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm)

### Short version

The run-time library replaces the `malloc` and `free` functions. The memory around malloc-ed regions (red zones) is poisoned. The `free`-ed memory is placed in quarantine and also poisoned. Every memory access in the program is transformed by the compiler in the following way:

> NOTE:
>
> 1、"in quarantine" 的意思是 "隔离"
>
> 2、从下面的demo code来看，一旦 `IsPoisoned(address)`，则表示错误，需要报错。
>
> 3、原文提及了 red zone的概念

Before:

```c
*address = ...;  // or: ... = *address;
```

After:

```c
if (IsPoisoned(address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
*address = ...;  // or: ... = *address;
```

The tricky part is how to implement `IsPoisoned` very fast and `ReportError` very compact. Also, instrumenting some of the accesses may be [proven redundant](https://github.com/google/sanitizers/wiki/AddressSanitizerCompileTimeOptimizations).

> NOTE:
>
> 一、"棘手的部分是如何非常快速地实现 IsPoisoned 和非常紧凑的 ReportError。此外，检测某些访问可能被证明是多余的。"
>
> 二、最后一段话的含义是：并不是所有的memory access都需要进行检查，一些memory access的检查是多余的

### Memory mapping and Instrumentation

The virtual address space is divided into 2 disjoint classes:

1、Main application memory (`Mem`): this memory is used by the regular application code.

2、Shadow memory (`Shadow`): this memory contains the shadow values (or metadata). There is a correspondence between the shadow and the main application memory. **Poisoning** a byte in the main memory means writing some special value into the corresponding shadow memory.

These 2 classes of memory should be organized in such a way that computing the shadow memory (`MemToShadow`) is fast.

> NOTE:
>
> 因为shallow memory所存储的是关于application memory的metadata，因此对于application memory的instrumentation需要shallow memory，因此这就要求 "computing the shadow memory (`MemToShadow`) is fast"。

The instrumentation performed by the compiler:

```c++
shadow_address = MemToShadow(address);
if (ShadowIsPoisoned(shadow_address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
```

> NOTE:
>
> 上面这段话其实已经描述了address sanitizer的核心算法了

### Mapping

[AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer) maps 8 bytes of the application memory into 1 byte of the shadow memory.

There are only 9 different values for any aligned 8 bytes of the application memory:

1、All 8 bytes in qword are unpoisoned (i.e. addressable). The shadow value is 0.

2、All 8 bytes in qword are poisoned (i.e. not addressable). The shadow value is negative.

3、First `k` bytes are unpoisoned, the rest `8-k` are poisoned. The shadow value is `k`. This is guaranteed by the fact that `malloc` returns 8-byte aligned chunks of memory. The only case where different bytes of an aligned qword have different state is the tail of a malloc-ed region. For example, if we call `malloc(13)`, we will have one full unpoisoned qword and one qword where 5 first bytes are unpoisoned.
# Memory management

本文所描述的memory management是指process级别的memory management，而不是OS kernel级别的memory management，关于OS kernel的memory management，参见工程[Linux-OS](https://dengking.github.io/Linux-OS/)。process级别的memory management主要由programming language的实现决定，目前有如下两种方式：

- [Automatic memory management](./Automatic-memory-management/Automatic-memory-management.md)
- [Manual memory management](./Manual-memory-management/Manual-memory-management.md)

## wikipedia [Memory management](https://en.wikipedia.org/wiki/Memory_management)



**Memory management** is a form of [resource management](https://en.wikipedia.org/wiki/Resource_management_(computing)) applied to [computer memory](https://en.wikipedia.org/wiki/Computer_memory). The essential requirement of memory management is to provide ways to dynamically allocate portions of memory to programs at their request, and free it for reuse when no longer needed. This is critical to any advanced computer system where more than a single [process](https://en.wikipedia.org/wiki/Process_(computing)) might be underway（运行中） at any time. 

Several methods have been devised that increase the effectiveness of memory management. [Virtual memory](https://en.wikipedia.org/wiki/Virtual_memory) systems separate the memory addresses used by a process from actual physical addresses, allowing separation of processes and increasing the size of the [virtual address space](https://en.wikipedia.org/wiki/Virtual_address_space) beyond the available amount of [RAM](https://en.wikipedia.org/wiki/Random-access_memory) using [paging](https://en.wikipedia.org/wiki/Paging) or swapping to [secondary storage](https://en.wikipedia.org/wiki/Secondary_storage). The quality of the virtual memory manager can have an extensive effect on overall system performance.



### Details

Memory management within an address space is generally categorized as either automatic memory management, usually involving [garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)), or [manual memory management](https://en.wikipedia.org/wiki/Manual_memory_management).

> NOTE: 
>
> automatic memory management和[manual memory management](https://en.wikipedia.org/wiki/Manual_memory_management)是当前programming language implementation所采用的两种主流方式，后面会对此进行展开。

#### Dynamic memory allocation

*See also:* [C dynamic memory allocation](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation)

##### Implementations

###### Fixed-size blocks allocation

*Main article:* [Memory pool](https://en.wikipedia.org/wiki/Memory_pool)

###### Buddy blocks

*Further information:* [Buddy memory allocation](https://en.wikipedia.org/wiki/Buddy_memory_allocation)

###### Slab allocation

*Main article:* [Slab allocation](https://en.wikipedia.org/wiki/Slab_allocation)

###### Stack allocation

*Main article:* [Stack-based memory allocation](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)

#### Automatic variables

*Main article:* [Automatic variable](https://en.wikipedia.org/wiki/Automatic_variable)

In many programming language implementations, all variables declared within a procedure (subroutine, or function) are local to that function; the runtime environment for the program automatically allocates memory for these variables on program execution entry to the procedure, and automatically releases that memory when the procedure is exited. Special declarations may allow local variables to retain values between invocations of the procedure, or may allow local variables to be accessed by other procedures. The automatic allocation of local variables makes [recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science)) possible, to a depth limited by available memory.

#### Garbage collection

*Main article:* [Garbage collection (computer science)](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))




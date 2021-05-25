# wikipedia [Memory safety](https://en.wikipedia.org/wiki/Memory_safety)

**Memory safety** is the state of being protected from various [software bugs](https://en.wikipedia.org/wiki/Software_bugs) and [security vulnerabilities](https://en.wikipedia.org/wiki/Vulnerability_(computing)) when dealing with [memory](https://en.wikipedia.org/wiki/Random-access_memory) access, such as [buffer overflows](https://en.wikipedia.org/wiki/Buffer_overflow) and [dangling pointers](https://en.wikipedia.org/wiki/Dangling_pointer).[[1\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-Adve-1) For example, [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) is said to be memory-safe because its [runtime error detection](https://en.wikipedia.org/wiki/Runtime_error_detection) checks array bounds and pointer dereferences.[[1\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-Adve-1) In contrast, [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B) allow arbitrary [pointer arithmetic](https://en.wikipedia.org/wiki/Pointer_arithmetic) with pointers implemented as direct memory addresses with no provision for [bounds checking](https://en.wikipedia.org/wiki/Bounds_checking),[[2\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-2) and thus are termed **memory-unsafe**.[[3\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-3)

## History

Memory errors were first considered in the context of [resource management](https://en.wikipedia.org/wiki/Resource_management) and [time-sharing](https://en.wikipedia.org/wiki/Time-sharing) systems, in an effort to avoid problems such as [fork bombs](https://en.wikipedia.org/wiki/Fork_bomb).[[4\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-4)Developments were mostly theoretical until the [Morris worm](https://en.wikipedia.org/wiki/Morris_worm), which exploited(利用) a buffer overflow in [fingerd](https://en.wikipedia.org/wiki/Fingerd) (unix的一条指令).[[5\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-Veen-5) The field of [computer security](https://en.wikipedia.org/wiki/Computer_security) developed quickly thereafter, escalating with multitudes of new [attacks](https://en.wikipedia.org/wiki/Attack_(computing)) such as the [return-to-libc attack](https://en.wikipedia.org/wiki/Return-to-libc_attack) and defense techniques such as the [non-executable stack](https://en.wikipedia.org/wiki/Executable_space_protection)[[6\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-6) and [address space layout randomization](https://en.wikipedia.org/wiki/Address_space_layout_randomization). Randomization prevents most [buffer overflow](https://en.wikipedia.org/wiki/Buffer_overflow) attacks and requires the attacker to use [heap spraying](https://en.wikipedia.org/wiki/Heap_spraying) or other application-dependent methods to obtain addresses, although its adoption has been slow.[[5\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-Veen-5) However, deployments of the technology are typically limited to randomizing libraries and the location of the stack.



## Approaches

DieHard,[[7\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-7) its redesign DieHarder,[[8\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-8) and the [Allinea Distributed Debugging Tool](https://en.wikipedia.org/wiki/Allinea_Distributed_Debugging_Tool) are special heap allocators that allocate objects in their own random virtual memory page, allowing invalid reads and writes to be stopped and debugged at the exact instruction that causes them. Protection relies upon hardware memory protection and thus overhead is typically not substantial, although it can grow significantly if the program makes heavy use of allocation.[[9\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-9) Randomization provides only probabilistic protection against memory errors, but can often be easily implemented in existing software by relinking the binary.

The memcheck tool of [Valgrind](https://en.wikipedia.org/wiki/Valgrind) uses an [instruction set simulator](https://en.wikipedia.org/wiki/Instruction_set_simulator) and runs the compiled program in a memory-checking virtual machine, providing guaranteed detection of a subset of runtime memory errors. However, it typically slows the program down by a factor of 40,[[10\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-10) and furthermore must be explicitly informed of custom memory allocators.[[11\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-11)[[12\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-12)

With access to the source code, libraries exist that collect and track legitimate values for pointers ("metadata") and check each pointer access against the metadata for validity, such as the [Boehm garbage collector](https://en.wikipedia.org/wiki/Boehm_garbage_collector).[[13\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-13) In general, memory safety can be safely assured using [tracing garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) and the insertion of runtime checks on every memory access; this approach has overhead, but less than that of Valgrind. All garbage-collected languages take this approach.[[1\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-Adve-1) For C and C++, many tools exist that perform a compile-time transformation of the code to do memory safety checks at runtime, such as CheckPointer[[14\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-14) and [AddressSanitizer](https://en.wikipedia.org/wiki/AddressSanitizer) which imposes an average slowdown factor of 2[[15\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-15).

Another approach uses [static program analysis](https://en.wikipedia.org/wiki/Static_program_analysis) and [automated theorem proving](https://en.wikipedia.org/wiki/Automated_theorem_proving) to ensure that the program is free of memory errors. For example, the [Rust programming language](https://en.wikipedia.org/wiki/Rust_(programming_language)) implements a borrow checker to ensure memory safety.[[16\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-16) Tools such as [Coverity](https://en.wikipedia.org/wiki/Coverity) offer static memory analysis for C.[[17\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-17) C++'s [smart pointers](https://en.wikipedia.org/wiki/Smart_pointers)are a limited form of this approach.



***TODO*** ： 上段未阅



## Types of memory errors

> NOTE: 
> 1、总结地非常好

Many different types of memory errors can occur:[[18\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-18)[[19\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-19)



### Access errors: invalid read/write of a pointer

- **Buffer overflow** - out-of-bound writes can corrupt the content of adjacent objects, or internal data (like bookkeeping information for the [heap](https://en.wikipedia.org/wiki/Memory_management#Dynamic_memory_allocation)) or [return](https://en.wikipedia.org/wiki/Return_statement)addresses.
- **Buffer over-read** - out-of-bound reads can reveal sensitive data or help attackers bypass [address space layout randomization](https://en.wikipedia.org/wiki/Address_space_layout_randomization).
- **Race condition** - concurrent reads/writes to shared memory
- **Invalid page fault** - accessing a pointer outside the virtual memory space. A null pointer dereference will often cause an exception or program termination in most environments, but can cause corruption in operating system [kernels](https://en.wikipedia.org/wiki/Kernel_(computing)) or systems without [memory protection](https://en.wikipedia.org/wiki/Memory_protection), or when use of the null pointer involves a large or negative offset.
- **Use after free** - dereferencing a [dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer) storing the address of an object that has been deleted.

### [Uninitialized variables](https://en.wikipedia.org/wiki/Uninitialized_variable)  

a variable that has not been assigned a value is used. It may contain an undesired or, in some languages, a corrupt value.

- **Null pointer dereference** - dereferencing an invalid pointer or a pointer to memory that has not been allocated
- **Wild pointers** arise when a pointer is used prior to initialization to some known state. They show the same erratic behaviour as dangling pointers, though they are less likely to stay undetected.

### [Memory leak](https://en.wikipedia.org/wiki/Memory_leak) 

when memory usage is not tracked or tracked incorrectly

- **Stack exhaustion** - occurs when a program runs out of stack space, typically because of too deep [recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science)). A [guard page](https://en.wikipedia.org/wiki/Memory_protection) typically halts the program, preventing memory corruption, but functions with large [stack frames](https://en.wikipedia.org/wiki/Stack_frame) may bypass the page.
- **Heap exhaustion** - the program tries to [allocate](https://en.wikipedia.org/wiki/Memory_allocation) more memory than the amount available. In some languages, this condition must be checked for manually after each allocation.
- **Double free** - repeated calls to [free](https://en.wikipedia.org/wiki/Malloc) may prematurely free a new object at the same address. If the exact address has not been reused, other corruption may occur, especially in allocators that use [free lists](https://en.wikipedia.org/wiki/Free_list).
- **Invalid free** - passing an invalid address to [free](https://en.wikipedia.org/wiki/Malloc) can corrupt the [heap](https://en.wikipedia.org/wiki/Memory_management#Dynamic_memory_allocation).
- **Mismatched free** - when multiple allocators are in use, attempting to free memory with a deallocation function of a different allocator[[20\]](https://en.wikipedia.org/wiki/Memory_safety#cite_note-20)
- **Unwanted aliasing** - when the same memory location is allocated and modified twice for unrelated purposes.


# Memory model (programming)

对于支持multiple thread的programming language，designer可能需要为其设计memory model，典型的案例就是C++、C、Java。

## Wikipedia [Memory model (programming)](https://infogalactic.com/info/Memory_model_(programming))

*This article is about a concept in multi-thread programming. For details of memory addressing, see* [Memory address § Memory models](https://infogalactic.com/info/Memory_address#Memory_models)*.*

In computing, a **memory model** describes the interactions of [threads](https://infogalactic.com/info/Thread_(computer_science)) through [memory](https://infogalactic.com/info/Memory_(computing)) and their shared use of the [data](https://infogalactic.com/info/Data_(computing)).

### History and significance

A **memory model** allows a **compiler** to perform many important **optimizations**. Even simple [compiler optimizations](https://infogalactic.com/info/Compiler_optimization) like [loop fusion](https://infogalactic.com/info/Loop_fusion) move statements in the program, which can influence the **order** of **read** and **write** operations of potentially shared [variables](https://infogalactic.com/info/Variable_(programming)). Changes in the ordering of reads and writes can cause [race conditions](https://infogalactic.com/info/Race_condition). Without a memory model, a compiler is not allowed to apply such optimizations to multi-threaded programs in general, or only in special cases.

> NOTE: 上面所描述的内容属于Compile-time memory ordering，在Wikipedia [Memory ordering](https://infogalactic.com/info/Memory_ordering) 的 [Compile-time memory ordering](https://infogalactic.com/info/Memory_ordering#Compile-time_memory_ordering) 章节中进行了具体描述。

Modern programming languages like [Java](https://infogalactic.com/info/Java_(programming_language)) therefore implement a **memory model**. The **memory model** specifies [synchronization barriers](https://infogalactic.com/info/Synchronization_barrier) that are established via special, well-defined synchronization operations such as acquiring a lock by entering a **synchronized** block or method. The memory model stipulates(规定) that changes to the values of **shared variables** only need to be made visible to other threads when such a synchronization barrier is reached. Moreover, the entire notion of a [race condition](https://infogalactic.com/info/Race_condition) is defined over the order of operations with respect to these memory barriers.[[1\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-1)

> NOTE: 没有读懂

These semantics then give optimizing compilers a higher degree of freedom when applying optimizations: the compiler needs to make sure **only** that the values of (potentially shared) variables at **synchronization barriers** are guaranteed to be the same in both the optimized and unoptimized code. In particular, **reordering statements** in a block of code that contains no synchronization barrier is assumed to be safe by the compiler.

Most research in the area of memory models revolves around:

- Designing a **memory model** that allows a maximal degree of freedom for compiler optimizations while still giving sufficient guarantees about race-free and (perhaps more importantly) race-containing programs.
- Proving program optimizations that are correct with respect to such a memory model.



The [Java Memory Model](https://infogalactic.com/info/Java_Memory_Model) was the first attempt to provide a comprehensive threading memory model for a popular programming language.[[2\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-2) After it was established that threads could not be implemented safely as a [library](https://infogalactic.com/info/Library_(computing)) without placing certain restrictions on the implementation and, in particular, that the [C](https://infogalactic.com/info/C_(programming_language)) and [C++](https://infogalactic.com/info/C%2B%2B) standards ([C99](https://infogalactic.com/info/C99) and [C++03](https://infogalactic.com/info/C%2B%2B03)) lacked necessary restrictions,[[3\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-3)[[4\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-4) the C++ threading subcommittee(小组委员会) set to work on suitable **memory model**; in 2005, they submitted C working document n1131[[5\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-5) to get the C Committee on board with their efforts. The final revision of the proposed memory model, C++ n2429,[[6\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-6) was accepted into the C++ draft standard at the October 2007 meeting in Kona.[[7\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-7) The memory model was then included in the next C++ and C standards, [C++11](https://infogalactic.com/info/C%2B%2B11) and [C11](https://infogalactic.com/info/C11_(C_standard_revision)).[[8\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-8)[[9\]](https://infogalactic.com/info/Memory_model_(programming)#cite_note-9)
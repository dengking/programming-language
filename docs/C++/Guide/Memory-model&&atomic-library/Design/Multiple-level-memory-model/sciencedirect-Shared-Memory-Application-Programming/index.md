# sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

链接为: https://www.sciencedirect.com/topics/computer-science/sequential-consistency



## 8.4.1 Memory Models and Memory Ordering Options

**Three** memory models proposed by C++11, which, together with the **memory ordering options** that can be used in each one of them, are described next. **Memory order options** are **symbolic constants** passed as second argument to the **atomic class member functions**. As stated before, the same **memory models** are implemented in TBB and Windows, with minor implementation differences.

> NOTE: TBB指的是[Threading Building Blocks](https://en.wikipedia.org/wiki/Threading_Building_Blocks):
>
> > **Threading Building Blocks** (**TBB**) is a [C++](https://en.wikipedia.org/wiki/C%2B%2B) [template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)) [library](https://en.wikipedia.org/wiki/Library_(computer_science)) developed by [Intel](https://en.wikipedia.org/wiki/Intel) for [parallel programming](https://en.wikipedia.org/wiki/Parallel_programming) on [multi-core processors](https://en.wikipedia.org/wiki/Multi-core_processor). Using TBB, a computation is broken down into [tasks](https://en.wikipedia.org/wiki/Task_parallelism) that can run in parallel. The library manages and [schedules](https://en.wikipedia.org/wiki/Scheduling_(computing)) [threads](https://en.wikipedia.org/wiki/Thread_(computing)) to execute these tasks.

### 1 Sequential consistency

This memory model imposes the strongest **memory order constraints**. In fact, there is more to **sequential consistency** than the **happens before relation** exhibited in Listing 8.2. Broader global synchronizations are implemented, forcing all threads to see all the sequential consistent synchronizations in the program happening in the same order. There is, in a given program, a unique global order of sequentially consistent atomic operations, seen by all the threads in the process.

> NOTE: global synchronization是什么含义？下面也有global order的描述。

#### `memory_order_seq_cst`

This ordering enforces sequential consistency, preventing preceding (or succeeding) memory operations to be reordered beyond (or before) this point. But there are also subsidiary(附属的) global synchronizations as discussed above. An example will be given next. *This is the default value for the second argument in member functions*.

### 2 Acquire-release

In this model, stores and loads still synchronize as described above, but there is no **global order**. Different threads may see different synchronizations happening in different orders. Memory order options are:

#### `memory_order_release` 

Prevents preceding memory operations from being reordered past this point.

#### `memory_order_acquire` 

Prevents succeeding memory operations from being reordered before this point.

#### `memory_order_consume` 

A weaker form of the acquire option: memory order constraints only apply to succeeding operations that are computationally dependent on the value retrieved (loaded) by the atomic variable.

#### `memory_order_acq_rel`

Combines both release and acquire memory order constraints: a full **memory fence** is in operation.

### 3 Relaxed

In this model, the basic store-load synchronization described above does not occur, and no “**happens before**” relations across threads are established.

#### `memory_order_relaxed`

There is no ordering constraint. Following operations may be reordered before, and preceding operations may be reordered after the atomic operation.

### Summary

Finally, it is important to observe(注意) that not all **memory ordering options** are adapted(适合) to all member functions. Atomic operations are of three kinds: 

1、*load, store*—the load() and store() member functions—and 

2、*read_modify_write*—

3、all the other member functions. 

> NOTE: 上面将atomic operation的分类是较好的

Table 8.2 lists the memory ordering options available to each type of atomic operation.

> NOTE: 这段话的意思是: atomic operation 和 memory ordering option的组合是有条件的，不允许随意的组合；

Table 8.2. Memory Order Options for Atomic Operations

|                   | Options for Atomic Operations |
| :---------------- | :---------------------------- |
| **Operation**     | **Memory Options**            |
| Store             | `memory_order_seq_cst`        |
|                   | `memory_order_release`        |
|                   | `memory_order_relaxed`        |
| Load              | `memory_order_seq_cst`        |
|                   | `memory_order_acquire`        |
|                   | `memory_order_consume`        |
|                   | `memory_order_relaxed`        |
| Read-modify-write | all available options         |

> NOTE: 
>
> 一、store对应的是write，load对应的是read，上述table也是符合write-release-read-acquire的，从上面的table可以看出:
>
> 1、Store不能够跟`memory_order_acquire`、`memory_order_consume`组合
>
> 2、Load不能够跟`memory_order_release`组合



The rest of this section provides a very qualitative(高质量的) discussion of the differences between the different memory ordering options, so that readers can feel more or less at ease when dealing with references on this subject. A couple of examples will also be examined. There are, in this subject, a substantial number of subtleties that are beyond the scope of this book. Readers willing to go deeper into this subject should consult the exhaustive discussion presented in [14].




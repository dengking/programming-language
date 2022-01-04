# Mechanized Verification of a Fine-Grained Concurrent Queue from Meta’s Folly Library

> NOTE: 
>
> 

We use the **ReLoC relational logic** and the **Iris program logic** to carry out the proof and to mechanize it in the **Coq proof assistant**. The **MPMC queue** is implemented using three modules, and our proof is similarly modular. By using ReLoC and Iris’s support for modular reasoning we verify each module in isolation and compose these together.

> NOTE: 
>
> 一、"The **MPMC queue** is implemented using three modules, and our proof is similarly modular"
>
> 三个模块

A key challenge of the MPMC queue is that it has a so-called **external linearization point**, which ReLoC has no support for reasoning about. Thus we extend ReLoC, both on paper and in Coq, with novel support for reasoning about external linearization points.

> NOTE: 
>
> 一、 "**external linearization point**"中"**linearization point**"的概念在 stackexchange [What is a linearization point?](https://cs.stackexchange.com/questions/7132/what-is-a-linearization-point) # [A](https://cs.stackexchange.com/a/7133) 中有着非常好的描述。

## 1 Introduction

One of the key ideas used in the algorithm is to improve **scalability** by decreasing the **contention** found in other lock-free algorithms, such as the Michael-Scott queue, by striping the queue across 𝑞 “smaller” sub-queues. To avoid the overhead of maintaining 𝑞 subqueues, the striping is taken to the extreme by letting each sub-queue store only a single element. These single-element queues can then be simpler and faster. In fact, they are implemented merely as a reference to a value and a so-called **turn sequencer**. The latter is a synchronization mechanism used by the single-element queue to guard access to its value. The enqueue and dequeue operations on the MPMC queue are delegated to one of the single-element queues by taking a ticket from one of two ticket dispensers using an atomic increment (FAA). After receiving a ticket, up to 𝑞 separate enqueue or dequeue operations can proceed in parallel, completely independent of each other as they operate on different single-element queues. The FAA instruction thus becomes the main point of contention, but since an FAA instruction (unlike CAS) always succeeds, this design, in the words of Bronson, “makes contention count” as its cost always pays off in significant progress being made in the algorithm [4]. Altogether, this makes the queue scalable to hundreds of thousands of producer and consumer threads. 

> NOTE: 
>
> 一、"this design, in the words of Bronson, “makes contention count” as its cost always pays off in significant progress being made in the algorithm "
>
> "用 Bronson 的话来说，这种设计“使竞争变得重要”，因为它的成本总是在算法取得重大进展时得到回报"

More concretely, in this paper we present an implementation of the MPMC queue and all its components in an ML-like language with concurrency primitives. The implementation captures the essence and the key verification challenges of the algorithm while eliding some of the low-level details of
the original C++ implementation. We prove that the MPMC queue contextually refines a coarse-grained concurrent queue. The coarse-grained queue uses a lock to ensure that only one thread at a time access the queue. We take this simple queue to be the specification of a queue and the MPMC
queue to be an implementation of the specification. Informally, the contextual refinement property then means that in any program we may replace uses of the “obviously correct” coarse-grained concurrent queue with the more efficient, but also more complicated, MPMC queue, without changing
the observable behavior of the program. More precisely, an expression 𝑒 contextually refines another expression 𝑒′, if for all contexts 𝐶 of a ground type, if 𝐶[𝑒] terminates with a value, then there exists an execution of𝐶[𝑒′] that terminates with the same value.
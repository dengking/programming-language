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

## 2 The Folly MPMC Queue

We now describe the three data structures, starting with the turn sequencer and proceeding bottom-up.

### 2.1 Turn Sequencer

A **turn sequencer** is a data structure that implements mutual exclusion by ***sequentializing*** access to a critical section among threads ordered by a monotonically increasing turn. The **turn sequencer** implementation is shown in Figure 1a.

The **turn sequencer** provides two operations: **wait** and **complete**. These are similar to the acquire and release operations on a lock, but they take an additional natural number as an argument. The natural number specifies which ***turn*** to wait for or to complete.

> NOTE: 
>
> wait for turn
>
> complete turn

The **turn sequencer** guarantees that if a thread waits for the **𝑛th turn**, then it will only proceed once all the preceding turns have been completed. For this to hold, the **turn sequencer** assumes that its clients never wait for the same turn several times. As such, it is the responsibility of clients to manage the turns, i.e., which natural numbers they wait for. Compared to a lock, this places a greater demand on the client, but in return the client is given precise control over the order in which threads run their critical sections.

> NOTE: 
>
> 是否需要保证turn是连续的？

We implement the turn sequencer as a pointer `ts` to a number, which represents the **current turn**. The function wait `ts 𝑛` simply spins on that pointer until its value is equal to `𝑛`. The implementation of complete ends the current turn by incrementing `ts`.

### 2.2 Single-Element Queue

A single-element queue (SEQ) is a queue with a capacity of one. Our implementation is shown in Figure 1a. It is a blocking queue: if it is **empty** (**full**) then any subsequent **dequeue** (**enqueue**) is blocked until the queue becomes **nonempty** (**non-full**).

Similarly to the **turn sequencer**, the SEQ’s operations take a **turn** as an argument, however the turns are separate for **enqueue** and **dequeue**. The turn argument specifies the order of the operations: an enqueue or dequeue operation is carried out only after all operations with a lower number have been
carried out. For an enqueue and a dequeue operation with the same turns, the enqueue is carried out first. This ordering ensures that when an enqueue operation is carried out, the queue is always empty, and when dequeue is run the queue is non-empty.

### 2.3 MPMC Queue

> NOTE: 
>
> ticket 和 turn 之间的对应关系

In addition to the array, the queue contains two **ticket dispensers** (references to natural numbers): `pushTicket` and `popTicket`. The first keep track of tickets for the **enqueue** operation, and the second does the same for the **dequeue** operation.

The enqueue operation first takes a ticket by incrementing the value of `pushTicket` with FAA, which atomically increments the ticket and leaves enqueue with a ticket `𝑡` . From this ticket, we calculate an index (`𝑡 mod 𝑞`) in the array for a SEQ. Then, enqueue writes an element into the SEQ by using the turn `⌊𝑡/𝑞⌋`. The dequeue operation proceeds in a similar way. It atomically increments `popTicket` and calculates an index and a turn in the same way. It dequeues a value from the SEQ and returns this value.

### (a) Turn sequencer and single-element queue.

```javascript
newTS () = ref(0) // 这是在定义一个新类型；ref 表示reference、pointer； "="后面的是它的函数体

complete ts turn = ts ← turn + 1; () // complete 是一个函数，它有两个入参: ts、turn； "="后面的是它的函数体

wait ts turn =
    let turn′ = ! ts in // 这是一个赋值语句， `!ts`  表示读取 ts 所指向的memory的值，并将该值赋值给 turn′
    if (𝑡𝑢𝑟𝑛′ = 𝑡𝑢𝑟𝑛) then () // 函数返回
    else wait ts turn // 这里使用递归来实现spin
    
queueSEQ () = (newTS (), ref(None)) // 这是在定义一个新类型queueSEQ；它使用的是functional language中的product type

enqueueSEQ (ts, 𝑟 ) enqTurn 𝑣 = // 定义一个函数enqueueSEQ，(ts, 𝑟 )是这个函数的第一个入参，它其实就是 queueSEQ 类型的: ts 对应 queueSEQ 类型的第一个成员，r对应的是第二个成员；enqTurn 是第二个入参；v是第三个入参
	let turn = enqTurn ∗ 2 in // 定义一个变量
    wait ts turn; // 等待 turn
    𝑟 ← Some(𝑣); // 赋值
    complete ts turn // 完成 turn
    
dequeueSEQ (ts, 𝑟 ) deqTurn =
    let turn = deqTurn ∗ 2 + 1 in
    wait ts turn;
    let 𝑣 = match ! 𝑟 with // 这是使用FP的pattern match特性，它本质上做的是将 r 赋值给 v；v是函数的返回值
    | Some(𝑥) ⇒𝑥
    | None ⇒ assert(false)
    in complete ts turn; 𝑣 // v是函数的返回值
```



```JavaScript
queueMPMC 𝑞 = Λ.
    let slots = arrayInit 𝑞 queueSEQ in // 定义成员变量 slots；queueSEQ引用的是在(a)中定义的类型
    let pushTicket = ref(0) in // 定义成员变量 pushTicket
    let popTicket = ref(0) in // 定义成员变量 popTicket
    (𝜆𝑣. enqueue slots 𝑞 pushTicket 𝑣, // 定义成员函数 enqueue，它的入参有四个
     𝜆𝑥. dequeue slots 𝑞 popTicket) // 定义成员函数 dequeue
enqueue slots 𝑞 pushTicket 𝑣 = // 定义函数，入参有四个
    let 𝑡 = FAA(pushTicket, 1) in
    let idx = 𝑡 mod 𝑞 in
    let ticket = 𝑡/𝑞 in
    enqueueSEQ (slots[idx]) ticket 𝑣
dequeue slots 𝑞 popTicket =
    let 𝑡 = FAA(popTicket, 1) in
    let idx = 𝑡 mod 𝑞 in
    let ticket = 𝑡/𝑞 in
    dequeueSEQ (slots[idx]) ticket 𝑣
```



Figure 1. Implementation of the various data structures.
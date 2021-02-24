# cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)



`std::memory_order` specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered around an atomic operation. Absent any constraints on a **multi-core system**, when multiple threads simultaneously read and write to several variables, **one thread can observe the values change in an order different from the order another thread wrote them**. Indeed, the apparent order of changes can even differ among multiple reader threads. Some similar effects can occur even on uniprocessor systems due to compiler transformations allowed by the memory model.

> NOTE: 
>
> 1、make it computational ordering
>
> 2、"**one thread can observe the values change in an order different from the order another thread wrote them**"如何理解？
>
> TODO 这需要结合具体的例子来进行说明，使用 "order of write to shared data may be different among different threads " 可以检索到 相关的例子。

The default behavior of all atomic operations in the library provides for *sequentially consistent ordering* (see discussion below). That default can hurt performance, but the library's atomic operations can be given an additional `std::memory_order` argument to specify the exact constraints, beyond atomicity, that the compiler and processor must enforce for that operation.

## Formal description

**Inter-thread synchronization** and **memory ordering** determine how *evaluations* and *side effects* of expressions are ordered between different threads of execution. 

> NOTE: 
>
> ### 手段 和 目标
>
> 需要搞清楚**主被**、**因果**关系
>
> programmer使用 **inter-thread synchronization** 和 **memory ordering** 来对 "how *evaluations* and *side effects* of expressions are ordered between different threads of execution" 进行控制(control)。
>
> 从control theory的角度来看: **Inter-thread synchronization** and **memory ordering** 是**手段**，下面的这些是**目标**；
>
> ### Inter-thread synchronization 
>
> 在下面的章节中，有总结。
>
> ### 如何理解side effect？
>
> 放到multicore中来理解: 一个thread运行于core1，它对memory的write，能够被运行于其他core的thread看到、

They are defined in the following terms:

### Sequenced-before

> NOTE: 描述的是同一个thread内的order

Within the same thread, evaluation A may be *sequenced-before* evaluation B, as described in [evaluation order](https://en.cppreference.com/w/cpp/language/eval_order).

### Carries dependency

> NOTE: 描述的是同一个thread内的order

Within the same thread, evaluation A that is *sequenced-before* evaluation B may also carry a dependency into B (that is, B depends on A), if any of the following is true

1) The value of A is used as an operand of B, **except**

a) if B is a call to [std::kill_dependency](https://en.cppreference.com/w/cpp/atomic/kill_dependency)

> NOTE: 打破dependency

b) if A is the left operand of the built-in `&&`, `||`, `?:`, or `,` operators.

> NOTE: 为什么这些operator不形成dependency关系？

2) A writes to a scalar object M, B reads from M

3) A carries dependency into another evaluation X, and X carries dependency into B

### Modification order

All modifications to any particular atomic variable occur in a total order that is specific to this one atomic variable.

> NOTE: 
>
> 上面这段话如何理解？
>
> 表明意思:" 对任何特定原子变量的所有修改都按照特定于这个原子变量的总顺序进行。"
>
> 含义解释: 
>
> 1、从全局的角度对atomic variable的read、write order进行限制，它是基于happen-before关系而建立的，关于 *happens-before* ，在下面章节会进行介绍。
>
> 

The following four requirements are guaranteed for all atomic operations:

1) **Write-write coherence**: If evaluation A that modifies some atomic M (a write) *happens-before* evaluation B that modifies M, then A appears earlier than B in the *modification order* of M

2) **Read-read coherence**: if a value computation A of some atomic M (a read) *happens-before* a value computation B on M, and if the value of A comes from a write X on M, then the value of B is either the value stored by X, or the value stored by a side effect Y on M that appears later than X in the *modification order* of M.

3) **Read-write coherence**: if a value computation A of some atomic M (a read) *happens-before* an operation B on M (a write), then the value of A comes from a side-effect (a write) X that appears earlier than B in the *modification order* of M

4) **Write-read coherence**: if a side effect (a write) X on an atomic object M *happens-before* a value computation (a read) B of M, then the evaluation B shall take its value from X or from a side effect Y that follows X in the modification order of M

### Inter-thread happens-before

> NOTE: 描述的是线程之间的order。

Between threads, evaluation A *inter-thread happens before* evaluation B if any of the following is true

> NOTE: 
>
> 1、如何理解 *synchronizes-with* ？参见:
>
> a、preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 
>
> 2、如何理解*dependency-ordered before*？参见:
>
> a、preshing [The Purpose of memory_order_consume in C++11](https://preshing.com/20140709/the-purpose-of-memory_order_consume-in-cpp11)

1) A *synchronizes-with* B

2) A is *dependency-ordered before* B

3) A *synchronizes-with* some evaluation X, and X is *sequenced-before* B

4) A is *sequenced-before* some evaluation X, and X *inter-thread happens-before* B

5) A *inter-thread happens-before* some evaluation X, and X *inter-thread happens-before* B

### Happens-before

> NOTE:综合 *sequenced-before* 和  *inter-thread happens before* 

Regardless of threads, evaluation A *happens-before* evaluation B if any of the following is true:

1) A is *sequenced-before* B

2) A *inter-thread happens before* B

> NOTE: 
>
> ### Happens-before VS Inter-thread happens-before VS Sequenced-before
>
> stackoverflow [Why is there a distinct “inter-thread happens before” relation defined in ISO/IEC 14882:2011?](https://stackoverflow.com/questions/17768718/why-is-there-a-distinct-inter-thread-happens-before-relation-defined-in-iso-ie) 
>
> [A](https://stackoverflow.com/a/17769015)
>
> "Inter-thread happens before" is defined purely for exposition(说明、展示). Note how "happens before" is defined as "sequenced before or inter-thread happens before", and the notion of "inter-thread happens before" is not used for anything else. 
>
> "Sequenced before" describes evaluations happening on the same thread, 
>
> "inter-thread happens before" applies to evaluations happening on different threads; 
>
> "happens before" combines the two.
>
> > NOTE: 这个回答总结的非常好



#### Acyclic

The implementation is required to ensure that the *happens-before* relation is acyclic, by introducing additional synchronization if necessary (it can only be necessary if a consume operation is involved, see [Batty et al](http://www.cl.cam.ac.uk/~pes20/cpp/popl085ap-sewell.pdf))

#### Data race and happen-before

If one evaluation modifies a memory location, and the other reads or modifies the same memory location, and if at least one of the evaluations is not an atomic operation, the behavior of the program is undefined (the program has a [data race](https://en.cppreference.com/w/cpp/language/memory_model)) unless there exists a *happens-before* relationship between these two evaluations.

### Visible side-effects

The side-effect A on a scalar M (a write) is *visible* with respect to value computation B on M (a read) if both of the following are true:

1) A *happens-before* B

2) There is no other side effect X to M where A *happens-before* X and X *happens-before* B

> NOTE: 两种之间没有发生任何事情，这非常类似于ABA problem

If side-effect A is visible with respect to the value computation B, then the longest contiguous subset of the side-effects to M, in *modification order*, where B does not *happen-before* it is known as the *visible sequence of side-effects*. (the value of M, determined by B, will be the value stored by one of these side effects)

#### Inter-thread synchronization 

Note: inter-thread synchronization **boils down to**(归结为) preventing data races (by establishing **happens-before relationships**) and defining which **side effects** become visible under what conditions

> NOTE: 这段对 inter-thread synchronization 进行了总结

### Consume operation

Atomic load with `memory_order_consume` or stronger is a consume operation. Note that [std::atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence) imposes stronger synchronization requirements than a consume operation.

### Acquire operation

Atomic load with `memory_order_acquire` or stronger is an acquire operation. The `lock()` operation on a [*Mutex*](https://en.cppreference.com/w/cpp/named_req/Mutex) is also an acquire operation. Note that [std::atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence) imposes stronger synchronization requirements than an acquire operation.

### Release operation

Atomic store with `memory_order_release` or stronger is a release operation. The `unlock()` operation on a [*Mutex*](https://en.cppreference.com/w/cpp/named_req/Mutex) is also a release operation. Note that [std::atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence) imposes stronger synchronization requirements than a release operation.

> NOTE: 组合
>
> |       | ordering               |
> | ----- | ---------------------- |
> | load  | `memory_order_consume` |
> |       | `memory_order_acquire` |
> | store | `memory_order_release` |



## Explanation

### Relaxed ordering

Atomic operations tagged memory_order_relaxed are not synchronization operations; they do not impose an order among concurrent memory accesses. They only guarantee atomicity and modification order consistency.

> NOTE: 
>
> 1、上面这段话中的: synchronization operation如何来理解？
>
> 2、上面这段话中的: "modification order consistency"如何理解？

For example, with `x` and `y` initially zero,

```C++
// Thread 1:
r1 = y.load(std::memory_order_relaxed); // A
x.store(r1, std::memory_order_relaxed); // B
// Thread 2:
r2 = x.load(std::memory_order_relaxed); // C 
y.store(42, std::memory_order_relaxed); // D
```



In particular, this may occur if D is completed before C in thread 2, either due to compiler reordering or at runtime.

> NOTE: 这就是reordering。



#### Use case

Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the **shared_ptr** counters requires **acquire-release synchronization** with the destructor)

> NOTE: 
>
> 1、这段话中的"acquire-release synchronization with destructor"如何理解？
>
> 防止memory reordering发生在decrementing the **shared_ptr** counters 和 destructor 之间(这让我想起来很多  **acquire-release synchronization** 的例子)，显然只有当counter为0的时候，才能够调用destructor，这是典型的可以使用 **acquire-release synchronization** 的场景；


### Release-Acquire ordering



### Release-Consume ordering



### Sequentially-consistent ordering
# cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)



## since C++11 until C++20

```C++
typedef enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
} memory_order;
```

## since C++20

```c++
enum class memory_order : /*unspecified*/ {
    relaxed, consume, acquire, release, acq_rel, seq_cst
};
inline constexpr memory_order memory_order_relaxed = memory_order::relaxed;
inline constexpr memory_order memory_order_consume = memory_order::consume;
inline constexpr memory_order memory_order_acquire = memory_order::acquire;
inline constexpr memory_order memory_order_release = memory_order::release;
inline constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
inline constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;
```



`std::memory_order` specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered around an atomic operation. Absent any constraints on a **multi-core system**, when multiple threads simultaneously read and write to several variables, **one thread can observe the values change in an order different from the order another thread wrote them. Indeed, the apparent order of changes can even differ among multiple reader threads. Some similar effects can occur even on uniprocessor systems due to compiler transformations allowed by the memory model**.

> NOTE: 
>
> 1、make it computational ordering
>
> 2、"**one thread can observe the values change in an order different from the order another thread wrote them**"如何理解？
>
> TODO 这需要结合具体的例子来进行说明，使用 "order of write to shared data may be different among different threads " 可以检索到 相关的例子。
>
> 3、"**one thread can observe the values change in an order different from the order another thread wrote them. Indeed, the apparent order of changes can even differ among multiple reader threads. Some similar effects can occur even on uniprocessor systems due to compiler transformations allowed by the memory model**"，这段话总结地非常好，它所描述的是memory reordering。

The default behavior of all atomic operations in the library provides for *sequentially consistent ordering* (see discussion below). That default can hurt performance, but the library's atomic operations can be given an additional `std::memory_order` argument to specify the exact constraints, beyond atomicity, that the compiler and processor must enforce for that operation.

## Constants

> NOTE: 
>
> 一、将原文中的这部分内容放到了 `Multiple-level-memory-model` 章节中 

## Formal description

> NOTE: 
>
> 1、参见 `Formal-description` 章节
>
> 2、重要描述理论，掌握理论后，运用起来就非常方便了

## Explanation

> NOTE: 
>
> 1、参见 `Explanation` 章节。
>
> 2、重要描述如何使用。





## 总结

C++还提供了对memory order的控制。

关于C++ memory order的设计，我主要是参考的如下两篇文章:

1、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492)

2、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description

### shared data是同步点

本节的内容主要源自 zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) 。

1、在使用C++ atomic library进行lockless programming的时候，一般使用 atomic variable 来作为shared data。

### `std::memory_order` "控制的是同一个线程内指令的执行顺序"

本节的内容主要源自 zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) ，标题中的"控制的是同一个线程内指令的执行顺序"也是源自其中。

1、`std::memory_order` 是控制single thread的memory order

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) : "限制编译器以及CPU对单线程当中的指令执行顺序进行重排的程度"



### 可以使用memory order进行控制的对象

1、`std::atomic`

2、`std::atomic_thread_fence`

为什么要这样设计呢？它们是同步点。

### Modification order

本节的内容主要源自 cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description

1、Modification order是从全局角度基于"happens-before"关系对所有thread对同一个**atomic variable**的read、write进行order而形成的sequence，基于它我们能够对C++ multi thread program的行为进行formal description、formal analysis。

2、"happens-before"关系需要由programmer来进行建立，C++为programmer提供了实现"happens-before"关系的方法(后面会进行介绍)，可以肯定的是，它的实现是会使用到 `std::memory_order` 的。



### Multi level: 从"控制范围"来进行对比

简单概括来说:

1、sequential consistent ordering: 所有的thread看到的modification order是一致的

2、使用release-acquire、release-acquire实现的happens-before而构建起的modification order，仅限于参与的thread看到的modification order是一致的

下面总结对memory order的控制范围

| Ordering                | 范围                                                     |
| ----------------------- | -------------------------------------------------------- |
| Relaxed                 | 不控制，因此范围为0                                      |
| Release-consume         | 对存在data dependency的shared data的memory order进行控制 |
| Release-acquire         | 不考虑data dependency关系，直接画两条线，不允许越界      |
| Sequentially consistent | 全局                                                     |



### Multi level: 从implementation来进行对比

都会涉及strong、weak ordered memory CPU。



### Multi level: acquire-release VS sequential-consistency

素材: 

1、stackoverflow [What's are practical example where acquire release memory order differs from sequential consistency?](https://stackoverflow.com/questions/41858540/whats-are-practical-example-where-acquire-release-memory-order-differs-from-seq)

2、herbsutter [Reader Q&A: Acquire/release and sequential consistency](https://herbsutter.com/2013/10/28/reader-qa-acquirerelease-and-sequential-consistency/)



#### 基于producer-consumer来进行分析

multiple-producer-multiple-consumer需要sequential consistency

acquire release 能否实现multiple-producer-multiple-consumer？









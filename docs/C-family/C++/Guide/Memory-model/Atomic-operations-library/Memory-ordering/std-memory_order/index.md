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

## Formal description

参见 `Formal-description` 章节。

## Explanation

参见 `Explanation` 章节。
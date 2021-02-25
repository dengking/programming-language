# cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation

> NOTE: 
>
> 1、Relaxed ordering
>
> 2、Release-Acquire ordering
>
> 3、Release-Acquire ordering
>
> 4、Sequentially-consistent ordering
>
> 

## Relaxed ordering

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



### Use case

Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the **shared_ptr** counters requires **acquire-release synchronization** with the destructor)

> NOTE: 
>
> 1、这段话中的"acquire-release synchronization with destructor"如何理解？
>
> 防止memory reordering发生在decrementing the **shared_ptr** counters 和 destructor 之间(这让我想起来很多  **acquire-release synchronization** 的例子)，显然只有当counter为0的时候，才能够调用destructor，这是典型的可以使用 **acquire-release synchronization** 的场景；


## Release-Acquire ordering



## Release-Consume ordering



## Sequentially-consistent ordering



## Relationship with `volatile`


# 7 Designing lock-free concurrent data structures

In this chapter we’ll look at how the memory-ordering properties of the atomic operations introduced in chapter 5 can be used to build lock-free data structures. You need to take extreme care when designing such data structures, because they’re hard to get right, and the conditions that cause the design to fail may occur very rarely. We’ll start by looking at what it means for data structures to be lock-free; then we’ll move on to the reasons for using them before working through some examples and drawing out some general guidelines.

# *7.1* *Definitions and consequences*

Algorithms and data structures that use mutexes, condition variables, and futures to synchronize the data are called ***blocking*** data structures and algorithms. The application calls library functions that will suspend the execution of a thread until another thread performs an action. Such library calls are termed ***blocking*** calls because the thread can’t progress past this point until the **block** is removed. Typically, the OS will suspend a blocked thread completely (and allocate its time slices to another thread) until it’s ***unblocked*** by the appropriate action of another thread, whether that’s unlocking a mutex, notifying a condition variable, or making a future *ready*.

Data structures and algorithms that don’t use blocking library functions are said to be *nonblocking*. Not all such data structures are *lock-free,* though, so let’s look at the various types of nonblocking data structures.

> NOTE:
>
> 一、lock-free 和 nonblocking 之间的关系

## *7.1.1* *Types of nonblocking data structures*

Back in chapter 5, we implemented a basic mutex using `std::atomic_flag` as a spin lock. The code is reproduced in the following listing.

Listing 7.1 Implementation of a spin-lock mutex using **`std::atomic_flag`**

```c++
class spinlock_mutex
{
 std::atomic_flag flag;
public:
 spinlock_mutex():
 flag(ATOMIC_FLAG_INIT)
 {}
 void lock()
 {
 while(flag.test_and_set(std::memory_order_acquire));
 }
 void unlock()
 {
 flag.clear(std::memory_order_release);
 }
};
```

Anyway, there are no blocking calls, so any code that uses this mutex to protect shared data is consequently ***nonblocking***. It’s not *lock-free,* though. It’s still a mutex and can still be locked by only one thread at a time. Let’s look at the definition of ***lock-free*** so you can see what kinds of data structures *are* covered.



## *7.1.2* *Lock-free data structures*

For a data structure to qualify as lock-free, more than one thread must be able to access the data structure concurrently. They don’t have to be able to do the same operations; a lock-free queue might allow one thread to push and one to pop but break if two threads try to push new items at the same time. Not only that, but if one of the threads accessing the data structure is suspended by the scheduler midway through its operation, the other threads must still be able to complete their operations without waiting for the suspended thread.

> NOTE:
>
> 一、最后一句是核心所在

Algorithms that use compare/exchange operations on the data structure often have loops in them. The reason for using a compare/exchange operation is that another thread might have modified the data in the meantime, in which case the code will need to redo part of its operation before trying the compare/exchange again. Such code can still be lock-free if the compare/exchange would eventually succeed if the other threads were suspended. If it wouldn’t, you’d essentially have a spin lock,which is nonblocking but not lock-free.

Lock-free algorithms with such loops can result in one thread being subject to ***starvation***. If another thread performs operations with the “wrong” timing, the other thread might make progress while the first thread continually has to retry its operation. Data structures that avoid this problem are wait-free as well as lock-free.

## *7.1.3* *Wait-free data structures*

A wait-free data structure is a lock-free data structure with the additional property that every thread accessing the data structure can complete its operation within a bounded number of steps, regardless of the behavior of other threads. Algorithms that can involve an unbounded number of retries because of clashes with other threads are thus not wait-free.

Writing wait-free data structures correctly is extremely hard. In order to ensure that every thread can complete its operations within a bounded number of steps, you have to ensure that each operation can be performed in a single pass and that the steps performed by one thread don’t cause an operation on another thread to fail. This can make the overall algorithms for the various operations considerably more complex.

Given how hard it is to get a lock-free or wait-free data structure right, you need some pretty good reasons to write one; you need to be sure that the benefit outweighs the cost. Let’s therefore examine the points that affect the balance.



## *7.1.4* *The pros and cons of lock-free data structures*

### pros: enable maximum concurrency

When it comes down to it, the primary reason for using lock-free data structures is to enable maximum concurrency. With lock-based containers, there’s always the potential for one thread to have to block and wait for another to complete its operation before the first thread can proceed; preventing concurrency through mutual exclusion is the entire purpose of a mutex lock. 

With a lock-free data structure, some thread makes progress with every step. 

With a wait-free data structure, every thread can make forward progress, regardless of what the other threads are doing; there’s no need for
waiting. 

This is a desirable property to have but hard to achieve. It’s all too easy to end up writing what’s essentially a spin lock.

> NOTE:
>
> 一、"This is a desirable property to have but hard to achieve" 翻译如下:
>
> "希望得到，但是很难得到"
>
> 二、"It’s all too easy to end up writing what’s essentially a spin lock"翻译如下:
>
> "最终写出本质上是自旋锁的东西太容易了。"

### pros: robustness

A second reason to use lock-free data structures is robustness. If a thread dies while holding a lock, that data structure is broken forever. But if a thread dies partway through an operation on a lock-free data structure, nothing is lost except that thread’s data; other threads can proceed normally.

### cons: 

The flip side here is that if you can’t exclude threads from accessing the data structure, then you must be careful to ensure that the invariants are upheld or choose alternative invariants that can be upheld. Also, you must pay attention to the ordering constraints you impose on the operations. To avoid the undefined behavior associated with a data race, you must use atomic operations for the modifications. But that alone isn’t enough; you must ensure that changes become visible to other threads in the correct order. All this means that writing thread-safe data structures without using locks is
considerably harder than writing them with locks.
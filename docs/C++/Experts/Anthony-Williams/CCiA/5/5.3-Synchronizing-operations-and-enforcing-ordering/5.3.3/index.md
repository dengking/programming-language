# 5.3.3 Memory ordering for atomic operations



## SEQUENTIALLY CONSISTENT ORDERING

The default ordering is named **sequentially consistent** because it implies that the behavior of the program is consistent with a simple sequential view of the world. If all operations on instances of atomic types are **sequentially consistent**, the behavior of a multithreaded program is as if all these operations were performed in some particular sequence by a single thread.

> NOTE:
>
> 一、其实加入了synchronization的multithread程序应该都是如此的

This is by far the easiest **memory ordering** to understand, which is why it’s the default: all threads must see the same order of operations. This makes it easy to reason about the behavior of code written with **atomic variables**. You can write down all the possible sequences of operations by different
threads, eliminate those that are **inconsistent**, and verify that your code behaves as expected in the others. It also means that operations can’t be reordered; if your code has one operation before another in one thread, that ordering must be seen by all other threads.

> NOTE:
>
> 一、上面从侧面描述了"reordered"的含义

From the point of view of **synchronization**, a sequentially consistent store **synchronizes-with** a sequentially consistent load of the same variable that reads the value stored. This provides one ordering constraint on the operation of two (or more) threads, but **sequential consistency** is more powerful than that. Any sequentially consistent atomic operations done after that load must also appear after the store to other threads in the system using **sequentially consistent** atomic operations. The example in listing 5.4 demonstrates this ordering constraint in action. This constraint doesn’t carry forward to threads that use atomic operations with relaxed memory orderings; they can still see the operations in a different order, so you must use **sequentially consistent** operations on all your threads in order to get the benefit.

> NOTE:
>
> 一、先store然后load

This ease of understanding can come at a price, though. On a **weakly ordered machine** with many processors, it can impose a noticeable performance penalty, because the overall sequence of operations must be kept consistent between the processors, possibly requiring extensive (and expensive!) synchronization operations between the processors. That said, some processor architectures (such as the common x86 and x86-64 architectures) offer **sequential consistency** relatively cheaply, so if you’re concerned about the performance implications of using **sequentially consistent ordering**, check the documentation for your target processor architectures.	

The following listing shows **sequential consistency** in action. The loads and stores to `x` and `y` are explicitly tagged with `memory_order_seq_cst`, although this tag could be omitted in this case because it’s the default.

### Listing 5.4 Sequential consistency implies a total ordering

```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst)) // x 为 true，退出循环
        ;
    if (y.load(std::memory_order_seq_cst)) 
        ++z; // y为true则执行
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst)) // y 为 true，退出循环
        ;
    if (x.load(std::memory_order_seq_cst))
        ++z; // x为true则执行
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0);
}

```

The `assert` can never fire, because either the store to `x` or the store to `y` must happen first, even though it’s not specified which. If the load of `y` in `read_x_then_y` returns `false`, the store to `x` must occur before the store to `y`, in which case the load of `x` in `read_y_then_x` must return `true`, because the while loop ensures that the `y` is true at this point. Because the semantics of `memory_order_seq_cst` require a **single total ordering** over all operations tagged `memory_order_seq_cst`, there’s an implied ordering relationship between a load of `y` that returns `false` and the store to `y` . For there to be a **single total order**, if one thread sees `x==true` and then subsequently sees `y==false`, this implies that the store to `x` occurs before the store to `y` in this **total order**.

Of course, because everything is symmetrical, it could also happen the other way around, with the load of `x` returning false, forcing the load of `y` to return true. In both cases, `z` is equal to 1. Both loads can return true, leading to `z` being 2, but under no circumstances can `z` be zero.

> NOTE:
>
> 一、可以肯定: 上述四个thread执行顺序是未指定的
>
> 二、"The `assert` can never fire, because either the store to `x` or the store to `y` must happen first, even though it’s not specified which" 的理解如下:
>
> 因为在 `read_x_then_y()`、`read_y_then_x()` 都会首先执行一个while loop，显然如果要退出while loop就需要 "either the store to `x` or the store to `y` must happen first"，这样它们的值才能够为true。需要注意的是:  无法确定 `write_x()`、`write_y()` 执行的先后顺序是没有指定的，系统地来说，有如下两种可能性:
>
> 1、`write_x()`、`write_y()` 
>
> 这种情况下，`read_x_then_y()` 会先退出while循环
>
> 2、`write_y()`、`write_x()` 
>
> 这种情况下，`read_y_then_x()` 会先退出while循环
>
> 
>
> `read_x_then_y()` 和 `read_y_then_x()` 是相反的，其实这就是第二段中的"symmetrical"的含义。



The operations and **happens-before** relationships for the case that `read_x_then_y` sees `x` as true and `y` as `false` are shown in figure 5.3. The dashed line from the load of `y` in `read_x_then_y` to the store to `y` in `write_y` shows the implied ordering relationship required in order to maintain sequential consistency: the load must occur before the store in the global order of `memory_order_seq_cst` operations in order to achieve the outcomes given here.



![](./Figure-5.3-Sequential-consistency-and-happens-before.jpg)

Sequential consistency is the most straightforward and intuitive ordering, but it’s also the most expensive memory ordering because it requires global synchronization between all threads. On a multiprocessor system this may require quite extensive and time-consuming communication between processors.

In order to avoid this synchronization cost, you need to step outside the world of sequential consistency and consider using other memory orderings.




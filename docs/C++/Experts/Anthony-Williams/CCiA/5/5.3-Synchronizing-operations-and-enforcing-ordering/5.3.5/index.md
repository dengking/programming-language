# 5.3.5 Fences

An atomic operations library wouldn’t be complete without a set of fences. These are operations that enforce memory-ordering constraints without modifying any data and are typically combined with atomic operations that use the `memory_order_relaxed` ordering constraints.

Fences are **global operations** and affect the ordering of other atomic operations in the thread that executed the fence. **Fences** are also commonly
called ***memory barriers***, and they get their name because they put a line in the code that certain operations can’t cross. As you may recall from section 5.3.3, relaxed operations on separate variables can usually be freely reordered by the compiler or the hardware. Fences restrict this freedom and introduce **happens-before** and **synchronizes-with** relationships that weren’t present before.

Let’s start by adding a **fence** between the two atomic operations on each thread in listing 5.5, as shown in the following listing.

## Listing 5.12 Relaxed operations can be ordered with fences



```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ;
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x.load(std::memory_order_relaxed))
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);
}
```

The **release fence** 2 **synchronizes-with** the **acquire fence** 5 because the load from `y` at 4 reads the value stored at 3. This means that the store to `x` at 1 **happens-before** the load from `x` at 6, so the value read must be true and the `assert` at 7 won’t fire. This is in contrast to the original case without the fences where the store to and load from `x` weren’t ordered, and so the `assert` could fire. Note that both fences are necessary: you need a release in one thread and an acquire in another to get a **synchronizes-with** relationship.

In this case, the **release fence** 2 has the same effect as if the store to `y` 3 was tagged with `memory_order_release` rather than `memory_order_relaxed`. Likewise, the **acquire fence** 5 makes it as if the load from `y` 4 was tagged with `memory_order_acquire`. This is the general idea with fences: 

if an **acquire operation** sees the result of a store that takes place after a **release fence**, the fence **synchronizes-with** that **acquire operation**(上述例子所展示的就是这种情况); 

if a load that takes place before an **acquire fence** sees the result of a **release operation**, the release operation **synchronizes-with** the **acquire fence**. 

Of course, you can have fences on both sides, as in the example here, in which case if a load that takes place before the **acquire fence** sees a value written by a store that takes place after the **release fence**, the **release fence** synchronizes-with the **acquire fence**.

> NOTE:
>
> 一、上面这段话是非常拗口的，下面是剖析:
>
> "as in the example here, in which case if a load that takes place before the **acquire fence** sees a value written by a store that takes place after the **release fence**, the **release fence** synchronizes-with the **acquire fence**."
>
> 上述例子所展示的就是 "the **release fence** synchronizes-with the **acquire fence**"

Although the **fence synchronization** depends on the values read or written by operations before or after the fence, it’s important to note that the **synchronization point** is the fence itself. If you take `write_x_then_y` from listing 5.12 and move the write to x after the fence as follows, the condition in the `assert` is no longer guaranteed to be true, even though the write to x comes before the write to y:





These two operations are no longer separated by the fence and so are no longer ordered. It’s only when the fence comes between the store to `x` and the store to `y` that it imposes an ordering. Of course, the presence or absence of a fence doesn’t affect any enforced orderings on happens-before relations that exist because of other atomic operations.



## 承上启下

This example, and almost every other example so far in this chapter, is built entirely from variables with an atomic type. However, the real benefit to using atomic operations to enforce an ordering is that they can enforce an ordering on nonatomic operations and thus avoid the undefined behavior of a data race, as you saw back in listing 5.2.
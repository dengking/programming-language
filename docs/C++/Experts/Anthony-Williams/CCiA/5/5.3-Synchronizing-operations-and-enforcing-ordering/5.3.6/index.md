# 5.3.6 Ordering nonatomic operations with atomics

If you replace `x` from listing 5.12 with an ordinary nonatomic bool (as in the following listing), the behavior is guaranteed to be the same.

```c++
#include <atomic>
#include <thread>
#include <assert.h>

bool x = false;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y()
{
    x = true;
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ;
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x)
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



The fences still provide an enforced ordering of the store to `x` 1 and the store to `y` 2 and the load from `y` 3 and the load from `x` 4, and there’s still a **happens-before** relationship between the store to `x` and the load from `x`, so the `assert` 5 still won’t fire. The store to 2 and load from 4 `y` still have to be atomic; otherwise, there would be a data race on y, but the fences enforce an ordering on the operations on x, once the reading thread has seen the stored value of y. This enforced ordering means that there’s no data race on x, even though it’s modified by one thread and read by another.
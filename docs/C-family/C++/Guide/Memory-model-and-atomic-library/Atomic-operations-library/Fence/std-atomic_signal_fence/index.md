# `std::atomic_signal_fence`



在阅读 [cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[**inmemorylogger.h**](https://github.com/preshing/cpp11-on-multicore/blob/master/common/inmemorylogger.h) 时，发现了其中有这样的用法:

```c++
    void log(const char* msg, size_t param = 0)
    {
        std::atomic_signal_fence(std::memory_order_seq_cst);    // Compiler barrier
        // On weak CPUs and current C++ compilers, memory_order_consume costs the same as acquire. :(
        // (If you don't like that, you can probably demote this load to relaxed and get away with it.
        // Technically, you'd be violating the spec, but in practice it will likely work. Just
        // inspect the assembly and make sure there is a data dependency between m_tail.load and
        // both subsequent uses of page, and you're golden. The only way I can imagine the dependency
        // chain being broken is if the compiler knows the addresses that will be allocated
        // in allocateEventFromNewPage at runtime, which is a huuuuuuuuuge leap of the imagination.)
        // http://preshing.com/20140709/the-purpose-of-memory_order_consume-in-cpp11
        Page* page = m_tail.load(std::memory_order_consume);
        Event* evt;
        int index = page->index.fetch_add(1, std::memory_order_relaxed);
        if (index < EVENTS_PER_PAGE)
            evt = &page->events[index];
        else
            evt = allocateEventFromNewPage();   // Double-checked locking is performed inside here.
        evt->tid = std::this_thread::get_id();
        evt->msg = msg;
        evt->param = param;
        std::atomic_signal_fence(std::memory_order_seq_cst);    // Compiler barrier
    }
```



## cppreference [std::atomic_signal_fence](https://en.cppreference.com/w/cpp/atomic/atomic_signal_fence)

```C++
extern "C" void atomic_signal_fence( std::memory_order order ) noexcept;
```



Establishes memory synchronization ordering of non-atomic and relaxed atomic accesses, as instructed by `order`, between a thread and a signal handler executed on the same thread. 

> NOTE: 这段话的意思是: 按照参数 `order` 所指示的，在 "a thread" 和 "a signal handler executed on the same thread" 之间建立"memory synchronization ordering of non-atomic and relaxed atomic accesses"

This is equivalent to [std::atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence), except no CPU instructions for memory ordering are issued. Only reordering of the instructions by the compiler is suppressed(抑制) as `order` instructs. For example, a fence with release semantics prevents reads or writes from being moved past subsequent writes and a fence with acquire semantics prevents reads or writes from being moved ahead of preceding reads.

> NOTE: 显然，[std::atomic_signal_fence](https://en.cppreference.com/w/cpp/atomic/atomic_signal_fence) 是作为 compiler memory fence/barrier的



## stackoverflow [How to correctly use std::atomic_signal_fence()?](https://stackoverflow.com/questions/14581090/how-to-correctly-use-stdatomic-signal-fence)

cppreference.com documents this function as "fence between a thread and a signal handler executed in the same thread". But I found no example on the Internet.

I wonder whether or not the following psuedo-code correctly illustrates the function of `std::atomic_signal_fence()`:

```cpp
int n = 0;
SignalObject s;

void thread_1()
{
    s.wait();
    std::atomic_signal_fence(std::memory_order_acquire);
    assert(1 == n); // never fires ???
}

void thread_2()
{
    n = 1;
    s.signal();
}

int main()
{
    std::thread t1(thread_1);
    std::thread t2(thread_2);

    t1.join(); t2.join();
}
```

### **comments**

A `signal_fence` is a ["compiler" barrier](http://preshing.com/20120625/memory-ordering-at-compile-time/): it prevents compile-time reordering/combining/hoisting of memory operations, but will never emit a hardware memory barrier instructions. Jeff Preshing's blog is excellent at explaining this stuff, definitely a must-read if you're unsure about memory-ordering stuff. – [Peter Cordes](https://stackoverflow.com/users/224132/peter-cordes) [Feb](https://stackoverflow.com/questions/14581090/how-to-correctly-use-stdatomic-signal-fence#comment58532051_14581090)



### [A](https://stackoverflow.com/a/14594071)

No, your code does not demonstrate correct usage of `atomic_signal_fence`. As you quote cppreference.com, `atomic_signal_fence` only perform synchronization between a signal handler and other code running on the same thread. That means that it *does not* perform synchronization between two *different* threads. Your example code shows two different threads.

The C++ spec contains the following notes about this function:

> *Note:* compiler optimizations and reorderings of loads and stores are inhibited in the same way as with `atomic_thread_fence`, but the hardware fence instructions that atomic_thread_fence would have inserted are not emitted.
>
> *Note:* `atomic_signal_fence` can be used to specify the order in which actions performed by the thread become visible to the signal handler.

Here's an example of correct, if not motivating, usage:

```cpp
#include <atomic>
#include <cassert> // asserta
#include <csignal> // std::signal
#include <cstdlib>//std::exit
static_assert(2 == ATOMIC_INT_LOCK_FREE, "this implementation does not guarantee that std::atomic<int> is always lock free.");

std::atomic<int> a { 0 };
std::atomic<int> b { 0 };

extern "C" void handler(int)
{
	if (1 == a.load(std::memory_order_relaxed))
	{
		std::atomic_signal_fence(std::memory_order_acquire);
		assert(1 == b.load(std::memory_order_relaxed));
	}

	std::exit(0);
}

int main()
{
	std::signal(SIGTERM, &handler);

	b.store(1, std::memory_order_relaxed);
	std::atomic_signal_fence(std::memory_order_release);
	a.store(1, std::memory_order_relaxed);
}
// g++ --std=c++11 test.cpp

```

The assertion, if encountered, is guaranteed to hold true.
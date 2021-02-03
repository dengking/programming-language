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
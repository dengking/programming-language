# Implement reference count use std::atomic

在阅读 cppreference [std::memory_order # Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 时，其中有关于实现counter的描述:

> Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the shared_ptr counters requires acquire-release synchronization with the destructor)

下面是参考实现:

1、`shared_ptr`，参见相关章节。

2、[amin-jabri](https://github.com/amin-jabri)/**[atomic_shared_ptr](https://github.com/amin-jabri/atomic_shared_ptr)** 参见相关章节


# Memory synchronization ordering



|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)(C++11) | defines memory ordering constraints for the given atomic operation (enum) |
| [kill_dependency](https://en.cppreference.com/w/cpp/atomic/kill_dependency)(C++11) | removes the specified object from the [std::memory_order_consume](https://en.cppreference.com/w/cpp/atomic/memory_order) dependency tree (function template) |
| [atomic_thread_fence](https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence)(C++11) | generic memory order-dependent fence synchronization primitive (function) |
| [atomic_signal_fence](https://en.cppreference.com/w/cpp/atomic/atomic_signal_fence)(C++11) | fence between a thread and a signal handler executed in the same thread (function) |
# Thread

本章记录C++ thread support library的用法，对thread以及其相关问题的探讨，在如下工程中也会涉及：

- 工程Linux-OS的`Programming\Process\Thread`章节
- 工程Parallel-computing的`Concurrent-computing`章节

## cppreference [Thread support library](https://en.cppreference.com/w/cpp/thread)





## cnblogs [C++雾中风景12:聊聊C++中的Mutex，以及拯救生产力的Boost](https://www.cnblogs.com/happenlee/p/9747743.html)

> NOTE: 这篇文章中，对C++ mutex、lock进行了一些总结

### mutex

| 互斥量                | 版本      | 作用                       |
| --------------------- | --------- | -------------------------- |
| mutex                 | C++11     | 最基本的互斥量             |
| timed_mutex           | C++11     | 有超时机制的互斥量         |
| recursive_mutex       | C++11     | 可重入的互斥量             |
| recursive_timed_mutex | C++11     | 结合 2,3 特点的互斥量      |
| shared_timed_mutex    | **C++14** | 具有超时机制的可共享互斥量 |
| shared_mutex          | **C++17** | 共享的互斥量               |

### lock

| 互斥量管理  | 版本      | 作用                   |
| ----------- | --------- | ---------------------- |
| lock_graud  | C++11     | 基于作用域的互斥量管理 |
| unique_lock | C++11     | 更加灵活的互斥量管理   |
| shared_lock | **C++14** | 共享互斥量的管理       |
| scope_lock  | **C++17** | 多互斥量避免死锁的管理 |

简单来说：

- **shared_lock是读锁**。被锁后仍允许其他线程执行同样被shared_lock的代码
- **unique_lock是写锁**。被锁后不允许其他线程执行被shared_lock或unique_lock的代码。它可以同时限制unique_lock与share_lock

不得不说，C++11没有将共享互斥量集成进来，在很多读多写少的应用场合之中，标准库本身提供的锁机制显得很鸡肋，也从而导致了笔者最终只能求助与boost的解决方案。(**其实也可以通过标准库的mutex来实现一个读写锁，这也是面试笔试之中常常问到的问题。不过太麻烦了，还得考虑和互斥量管理类兼容什么的，果断放弃啊**)





## thispointer [C++11 Multi-threading Tutorials](https://thispointer.com/category/c/c-11/c11-threads/)


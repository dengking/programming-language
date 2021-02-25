# Atomic operations library design and implementation



## Design of C++ atomic library

在工程parallel-computing的`Concurrent-computing\Multithread\Thread-safety\What-cause-unsafety`章节总结了导致非线程安全的两个原因，C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

更多详细的信息，参见工程programming language的 `C++\Guide\Memory-model\Atomic-operations-library\Design-and-implementation` 章节。



### `std::atomic` is neither copyable nor movable

为什么这样设计呢？

#### `std::atomic<bool> as static member`

1、stackoverflow [Initialize static atomic member variable](https://stackoverflow.com/questions/20453054/initialize-static-atomic-member-variable)

[A](https://stackoverflow.com/a/20453089)

```C++
std::atomic<int> order::c(0);   // or {0} for a more C++11 experience
```



## TODO

https://www.decadent.org.uk/pipermail/cpp-threads/2008-December/001933.html

stackoverflow [Where is the lock for a std::atomic?](https://stackoverflow.com/questions/50298358/where-is-the-lock-for-a-stdatomic)
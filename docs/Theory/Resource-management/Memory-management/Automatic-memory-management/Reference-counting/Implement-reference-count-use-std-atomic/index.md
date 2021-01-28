# Implement reference count use std::atomic

在阅读 cppreference [std::memory_order # Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 时，其中有关于实现counter的描述:

> Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the `shared_ptr` counters requires acquire-release synchronization with the destructor)

阅读了这段话，我的疑问如下:

1、如何实现呢? 

2、why？为什么increment的时候，可以使用relaxed memory ordering？为什么decrement的时候，需要使用acquire-release synchronization ？



于是我Google: c++ atomic reference counting，下面是一些非常好的资源:

## stackoverflow [How can memory_order_relaxed work for incrementing atomic reference counts in smart pointers?](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm)

Consider the following code snippet taken from Herb Sutter's talk on atomics:

The `smart_ptr` class contains a pimpl object called `control_block_ptr` containing the reference count **refs**.

```cpp
// Thread A:
// smart_ptr copy ctor
smart_ptr(const smart_ptr& other) {
  ...
  control_block_ptr = other->control_block_ptr;
  control_block_ptr->refs.fetch_add(1, memory_order_relaxed);
  ...
}

// Thread D:
// smart_ptr destructor
~smart_ptr() {
  if (control_block_ptr->refs.fetch_sub(1, memory_order_acq_rel) == 1) {
    delete control_block_ptr;
  }
}
```

> NOTE: 
>
> 1、`control_block_ptr->refs.fetch_sub(1, memory_order_acq_rel)`是atomic operation，它能够原子的decrement `refs`并返回它的原值；显然当原值为1的时候，则表示需要delete了
>
> 2、`~smart_ptr()`的实现能够保证只有一个thread 在执行判断`== 1`的时候，条件满足，然后执行`delete`，因此保证了thread safe，不可能发生double delete

Herb Sutter says the increment of **refs** in Thread A can use `memory_order_relaxed` because "nobody does anything based on the action". Now as I understand `memory_order_relaxed`, if **refs** equals N at some point and two threads A and B execute the following code:

> NOTE: "nobody does anything based on the action"意味这不需要synchronization

```cpp
control_block_ptr->refs.fetch_add(1, memory_order_relaxed);
```

then it may happen that both threads see the value of **refs** to be N and both write N+1 back to it. That will clearly not work and `memory_order_acq_rel` should be used just as with the destructor. Where am I going wrong?

> NOTE: 典型的错误

EDIT1: Consider the following code.

```cpp
atomic_int refs = N; // at time t0. 

// [Thread 1]
refs.fetch_add(1, memory_order_relaxed); // at time t1. 

// [Thread 2]
n = refs.load(memory_order_relaxed);   // starting at time t2 > t1
refs.fetch_add(1, memory_order_relaxed);
n = refs.load(memory_order_relaxed);
```

What is the value of **refs** observed by Thread 2 before the call to `fetch_add`? Could it be either N or N+1? What is the value of refs observed by Thread 2 after the call to fetch_add? Must it be at least N+2?

[Talk URL: C++ & Beyond 2012 - http://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-2-of-2 (@ 1:20:00)]



[A](https://stackoverflow.com/a/27716387)

Boost.Atomic library that emulates `std::atomic` provides [similar reference counting example and explanation](http://www.boost.org/doc/libs/1_57_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters), and it may help your understanding.

> Increasing the reference counter can always be done with `memory_order_relaxed`: New references to an object can only be formed from an existing reference, and passing an existing reference from one thread to another must already provide any required synchronization.
>
> It is important to enforce any possible access to the object in one thread (through an existing reference) to happen before deleting the object in a different thread. This is achieved by a "release" operation after dropping a reference (any access to the object through this reference must obviously happened before), and an "acquire" operation before deleting the object.
>
> It would be possible to use `memory_order_acq_rel` for the fetch_sub operation, but this results in unneeded "acquire" operations when the reference counter does not yet reach zero and may impose a performance penalty.



## medium [C++ atomic 的例子：為何 reference count -1 的時候要用 memory_order_acq_rel](https://medium.com/@fcamel/c-atomic-%E6%93%8D%E4%BD%9C%E7%9A%84%E4%BE%8B%E5%AD%90-c85295b08af4)

> NOTE: 原文是使用繁体中文写的

這是 Herb Sutter 在 《C++ and Beyond 2012: atomic<> Weapons》介紹的例子：

不懂為何在 -1 的時候要用 `memory_order_acq_rel`。

> NOTE: 

[在這裡查到說明](http://stackoverflow.com/a/27716387/278456)：

> NOTE: 上述链接到的如下内容:
>
> stackoverflow [How can memory_order_relaxed work for incrementing atomic reference counts in smart pointers?](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm)



在 -1 的時候分兩個情況討論：

1、n > 1: 確保釋放object前的更新能被其它 thread 看到，需要 release。

2、n = 1: 確保有看到其它 thread 的更新，需要 acquire。

除了演講[投影片中提到的例子](https://onedrive.live.com/?authkey=!AMtj_EflYn2507c&cid=4E86B0CF20EF15AD&id=4E86B0CF20EF15AD!24884&parId=4E86B0CF20EF15AD!180&o=OneUp) (影片中沒有細講)，Bootstrap 也有提供一些 [atomic 的使用例子](http://www.boost.org/doc/libs/1_57_0/doc/html/atomic/usage_examples.html)。要用的時候，還是多看些例子，看看能否直接拿來用 (C++11 應該和 Bootstrap 的 API 差不多吧，有需要再來確認，還有查其它 C++11 的例子)，或是藉此確認觀念正確再來用 atomic。



下面是参考实现:

1、`shared_ptr`，参见相关章节。

2、[amin-jabri](https://github.com/amin-jabri)/**[atomic_shared_ptr](https://github.com/amin-jabri/atomic_shared_ptr)** 参见相关章节


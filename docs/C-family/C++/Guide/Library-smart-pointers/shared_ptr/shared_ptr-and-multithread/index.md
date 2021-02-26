# `std::shared_ptr` and multithread

`std::shared_ptr` 具备shared semantic，因此它就需要考虑 multithread 的经典问题: thread safety。

## `std::shared_ptr` thread_safety

综合stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)、cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 、[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)中的描述可知：

1、Standard guarantees that reference counting is handled thread safe and it's platform independent

2、standard guarantees that only one thread (holding last reference) will call delete on shared object

3、`shared_ptr` does not guarantee any thread safety for object stored in it

> NOTE: 上述内容源自 stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained) 

## cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

> All member functions (including copy constructor and copy assignment) can be called by multiple threads on different instances of `shared_ptr` without additional synchronization even if these instances are copies and share ownership of the same object. If multiple threads of execution access the same `shared_ptr` without synchronization and any of those accesses uses a non-const member function of `shared_ptr` then a **data race** will occur; the [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) can be used to prevent the data race.

上面这段话其实描述了两种情况：

1、多个thread同时使用同一个`shared_ptr` instance，此时是需要synchronization 

2、多个thread，每个thread有一个自己的`shared_ptr` instance，这些instance执行同一个object

## stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)

I'm reading http://gcc.gnu.org/onlinedocs/libstdc++/manual/shared_ptr.html and some thread safety issues are still not clear for me:

1、Standard guarantees that reference counting is handled thread safe and it's platform independent, right?

2、Similar issue - standard guarantees that only one thread (holding last reference) will call delete on shared object, right?
3、`shared_ptr` does not guarantee any thread safety for object stored in it?

Pseudo code:

```cpp
// Thread I
shared_ptr<A> a (new A (1));

// Thread II
shared_ptr<A> b (a);

// Thread III
shared_ptr<A> c (a);

// Thread IV
shared_ptr<A> d (a);

d.reset (new A (10));
```

[A](https://stackoverflow.com/a/13639645)

As others have pointed out, you've got it figured out correctly regarding your original 3 questions.

But the ending part of your edit

> Calling reset() in thread IV will delete previous instance of A class created in first thread and replace it with new instance? Moreover after calling reset() in IV thread other threads will see only newly created object?

is incorrect. Only `d` will point to the new `A(10)`, and `a`, `b`, and `c` will continue to point to the original `A(1)`. This can be seen clearly in the following short example.

[A](https://stackoverflow.com/a/9133225)

Correct, `shared_ptr`s use atomic increments/decrements of a reference count value.

## stackoverflow [Atomic Reference Counting](https://stackoverflow.com/questions/31254767/atomic-reference-counting) # [A](https://stackoverflow.com/a/31255038)

To repeat it once more: Accessing a **single** `shared_ptr` **instance** from **multiple threads** where one of those accesses is a write to the pointer **is still a race condition**.

If you want to e.g. copy a `std::shared_ptr`in a threadsafe manner, you have to ensure that all loads and stores happen via [`std::atomic_...`](http://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) operations which are specialized for `shared_ptr`.
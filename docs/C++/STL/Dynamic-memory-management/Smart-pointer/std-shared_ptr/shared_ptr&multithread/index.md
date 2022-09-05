# `std::shared_ptr` and multithread

`std::shared_ptr` 具备shared semantic，因此它就需要考虑 multithread 的经典问题: thread safety。

## `std::shared_ptr` thread_safety

一、对于`std::shared_ptr`，它的对于它的data race的考虑

1、control block(包括reference counting)的data race

2、同一个`std::shared_ptr` object的data race

同一个`std::shared_ptr` object可能被多个thread同时access

3、target object的thread safe

综合stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)、cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 、[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)中的描述可知：

1、Standard guarantees that reference counting is handled thread safe and it's platform independent

2、standard guarantees that only one thread (holding last reference) will call delete on shared object

3、`shared_ptr` does not guarantee any thread safety for object stored in it

> NOTE: 上述内容源自 stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained) 

### cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

All member functions (including copy constructor and copy assignment) can be called by multiple threads on different instances of `shared_ptr` without additional synchronization even if these instances are copies and share ownership of the same object. 

> NOTE: 
>
> 一、对于具备shared ownership的所有`shared_ptr`，它们是共享同一个control block；通过上面这段话可知: `shared_ptr`的实现能够保证: **不同**的thread同时使用**不同**的`shared_ptr` object，control block是thread safe的；
>
> 这是可以使用many-to-one-model进行解释的



If multiple threads of execution access the same `shared_ptr` without synchronization and any of those accesses uses a non-const member function of `shared_ptr` then a data race will occur; the [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) can be used to prevent the data race.

> NOTE: 
>
> 一、上面这段话所描述的是: 多个thread同时使用同一个`shared_ptr` object;
>
> 通过上面这段话的描述可以看出，同一个`std::shared_ptr`如果同时被multiple thread使用， 是存在data race的，此时是需要synchronization 



> NOTE: 
>
> 上面两段话其实描述了两种情况：
>
> 1、多个thread同时使用同一个`shared_ptr` instance
>
> 2、多个thread，每个thread有一个自己的`shared_ptr` instance，这些instance指向同一个object

### stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)

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



### stackoverflow [std::shared_ptr thread safety](https://stackoverflow.com/questions/14482830/stdshared-ptr-thread-safety)

I've read that

> "Multiple threads can simultaneously read and write different `shared_ptr` objects, even when the objects are copies that share ownership." ([MSDN: Thread Safety in the Standard C++ Library](http://msdn.microsoft.com/en-us/library/c9ceah3b(v=vs.100).aspx))

Does that mean that changing `shared_ptr` object is safe ? For an instance, is the next code considered safe:

```c++
shared_ptr<myClass> global = make_shared<myClass>();
...

//In thread 1
shared_ptr<myClass> private = global;
...

//In thread 2
global = make_shared<myClass>();
...
```

> NOTE:
>
> 一、上述是典型的data race: read-write-to-shared-data

Can I be sure in that case that thread 1 `private` will have the original value of `global` or the new value which thread 2 assigned but either way it will have a valid `shared_ptr` to `myClass`?

==EDIT==

Just to explain my motivation. I want to have a shared pointer to hold my configuration and I have a thread pool to handle requests.

so `global` is the global configuration.

`thread 1` is taking the current configuration as it start to handle a request.

`thread 2` is updating the configuration. (only apply to future requests)

If it's work, I can update the configuration that way without breaking it in the middle of a request handling.



#### [A](https://stackoverflow.com/a/14485302)

What you're reading isn't meaning what you think it means. First of all, try the msdn page for [shared_ptr](https://docs.microsoft.com/en-us/cpp/standard-library/shared-ptr-class) itself.

Scroll down into the "Remarks" section and you'll get to the meat of the issue. Basically, a `shared_ptr<>` points to a "control block" which is how it keeps track of how many `shared_ptr<>` objects are actually pointing to the "Real" object. So when you do this:

```cpp
shared_ptr<int> ptr1 = make_shared<int>();
```

While there is only 1 call to allocate memory here via `make_shared`, there are two "logical" blocks that you should not treat the same. One is the `int` which stores the actual value, and the other is the control block, which stores all the `shared_ptr<>` "magic" that makes it work.

**It is only the control block itself which is thread-safe.**

I put that on its own line for emphasis. The *contents* of the `shared_ptr` are not thread-safe, nor is writing to the same `shared_ptr` instance. Here's something to demonstrate what I mean:

##### Example 1: safe

```C++
// In main()
shared_ptr<myClass> global_instance = make_shared<myClass>();
// (launch all other threads AFTER global_instance is fully constructed)

//In thread 1
shared_ptr<myClass> local_instance = global_instance;
```

> NOTE:
>
> 一、上述例子是典型的只有一个pointee
>
> 二、"The snippet you pulled from msdn basically means "access to the control block is thread-safe" so other `shared_ptr<>` instances can be created and destroyed on different threads as much as necessary."
>
> 这段话总结得非常好

This is fine, in fact you can do this in all threads as much as you want. And then when `local_instance` is destructed (by going out of scope), it is also thread-safe. Somebody can be accessing `global_instance` and it won't make a difference. The snippet you pulled from msdn basically means "access to the control block is thread-safe" so other `shared_ptr<>` instances can be created and destroyed on different threads as much as necessary.

##### Example 2: safe

```c++
//In thread 1
local_instance = make_shared<myClass>();
```

> NOTE:
>
> 一、这个例子是 `shared_ptr` assignment

This is fine. It *will* affect the `global_instance` object, but only indirectly. The control block it points to will be decremented, but done in a thread-safe way. `local_instance` will no longer point to the same object (or control block) as `global_instance` does.

##### Example 3: unsafe

> NOTE:
>
> 一、多个thread同时read-write同一个`shared_ptr`
>
> ```c++
> shared_ptr<myClass> global = make_shared<myClass>();
> ...
> 
> //In thread 1
> shared_ptr<myClass> local = global;
> ...
> 
> //In thread 2
> global = make_shared<myClass>();
> ...
> ```
>
> 

```c++
//In thread 2
global_instance = make_shared<myClass>();
```

This is almost certainly not fine if `global_instance` is accessed from any other threads (which you say you're doing). It needs a lock if you're doing this because you're writing to wherever `global_instance` lives, not just reading from it. So writing to an object from multiple threads is bad unless it's you have guarded it through a lock. So you can read from `global_instance` the object by assigning new `shared_ptr<>` objects from it but you can't write to it.

```c++
// In thread 3
*global_instance = 3;
int a = *global_instance;

// In thread 4
*global_instance = 7;
```

The value of `a` is undefined. It might be 7, or it might be 3, or it might be anything else as well. The thread-safety of the `shared_ptr<>` instances only applies to managing `shared_ptr<>` instances which were initialized from each other, not what they're pointing to.

To emphasize what I mean, look at this:

```c++
#include <memory>
#include <atomic>
#include <thread>

std::shared_ptr<int> global_instance = std::make_shared<int>(0);

void thread_fcn()
{
    // This is thread-safe and will work fine, though it's useless.  Many
    // short-lived pointers will be created and destroyed.
    for (int i = 0; i < 10000; i++)
    {
        std::shared_ptr<int> temp = global_instance;
    }

    // This is not thread-safe.  While all the threads are the same, the
    // "final" value of this is almost certainly NOT going to be
    // number_of_threads*10000 = 100,000.  It'll be something else.
    for (int i = 0; i < 10000; i++)
    {
        *global_instance = *global_instance + 1;
    }
}

int main(int argc, char **argv)
{
    std::thread thread1(thread_fcn);
    std::thread thread2(thread_fcn);

    std::thread thread10(thread_fcn);

    std::chrono::milliseconds duration(10000);
    std::this_thread::sleep_for(duration);

    return 0;
}

```

A `shared_ptr<>` is a mechanism to ensure that multiple object *owners* ensure an object is destructed, not a mechanism to ensure multiple *threads* can access an object correctly. You still need a separate synchronization mechanism to use it safely in multiple threads (like [std::mutex](http://en.cppreference.com/w/cpp/thread/mutex)).

The best way to think about it IMO is that `shared_ptr<>` makes sure that multiple copies pointing to the same memory don't have synchronization issues for *itself*, but doesn't do anything for the object pointed to. Treat it like that.





### stackoverflow [Atomic Reference Counting](https://stackoverflow.com/questions/31254767/atomic-reference-counting) # [A](https://stackoverflow.com/a/31255038)

To repeat it once more: Accessing a **single** `shared_ptr` **instance** from **multiple threads** where one of those accesses is a write to the pointer **is still a race condition**.

If you want to e.g. copy a `std::shared_ptr`in a threadsafe manner, you have to ensure that all loads and stores happen via [`std::atomic_...`](http://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) operations which are specialized for `shared_ptr`.



## 解决方案



一、wrap the `shared_ptr` with a lock

这种方式是最易想到的，在下面文章中有提及:

1、justsoftwaresolutions [Why do we need atomic_shared_ptr?](https://www.justsoftwaresolutions.co.uk/threading/why-do-we-need-atomic_shared_ptr.html) 

二、 [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) since C++11

这是在 cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 中提及的。

cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) :

> If multiple threads of execution access the same `shared_ptr` without synchronization and any of those accesses uses a non-const member function of `shared_ptr` then a data race will occur; the [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) can be used to prevent the data race.

cppreference [std::atomic_...std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) :

> These functions were deprecated in favor of the specializations of the [`std::atomic`](https://en.cppreference.com/w/cpp/atomic/atomic) template: [`std::atomic`](http://en.cppreference.com/w/cpp/atomic/atomic) < [`std::shared_ptr`](http://en.cppreference.com/w/cpp/memory/shared_ptr) > and [`std::atomic`](http://en.cppreference.com/w/cpp/atomic/atomic)< [`std::weak_ptr`](http://en.cppreference.com/w/cpp/memory/weak_ptr) >.

cppreference [`std::atomic_shared_ptr`](https://en.cppreference.com/w/cpp/experimental/atomic_shared_ptr)

> The class template `atomic_shared_ptr` provides thread-safe atomic pointer operations over a [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr). It provides a better alternative to [the non-member `shared_ptr` atomic access functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic).



3、[`std::atomic_shared_ptr`](https://en.cppreference.com/w/cpp/experimental/atomic_shared_ptr)



4、[`std::atomic<std::shared_ptr>`](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic2) since C++20



### stackoverflow [What is the difference between std::shared_ptr and std::experimental::atomic_shared_ptr?](https://stackoverflow.com/questions/40223599/what-is-the-difference-between-stdshared-ptr-and-stdexperimentalatomic-sha)

```c++
#include <memory>
#include <atomic>

template <class T>
class lock_free_stack
{
public:
    void push(const T &data)
    {
        const std::shared_ptr<node> new_node = std::make_shared<node>(data);
        new_node->next = std::atomic_load(&head_);
        while (!std::atomic_compare_exchange_weak(&head_, &new_node->next, new_node))
            ;
    }

    std::shared_ptr<T> pop()
    {
        std::shared_ptr<node> old_head = std::atomic_load(&head_);
        while (old_head &&
               !std::atomic_compare_exchange_weak(&head_, &old_head, old_head->next))
            ;
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        std::shared_ptr<node> next;

        node(const T &data_) : data(std::make_shared<T>(data_)) {}
    };

private:
    std::shared_ptr<node> head_;
};

```



[A](https://stackoverflow.com/a/40227410)

The atomic "thing" in `shared_ptr` is not the shared pointer itself, but the control block it points to. meaning that as long as you don't mutate the `shared_ptr` across multiple threads, you are ok. do note that *copying* a `shared_ptr` only mutates(改变) the control block, and not the `shared_ptr` itself.

```cpp
std::shared_ptr<int> ptr = std::make_shared<int>(4);
for (auto i =0;i<10;i++){
   std::thread([ptr]{ auto copy = ptr; }).detach(); //ok, only mutates the control block 
}
```

Mutating the shared pointer itself, such as assigning it different values from multiple threads, is a data race, for example:

```cpp
std::shared_ptr<int> ptr = std::make_shared<int>(4);
std::thread threadA([&ptr]{
   ptr = std::make_shared<int>(10);
});
std::thread threadB([&ptr]{
   ptr = std::make_shared<int>(20);
});    
```

Here, we are mutating the control block (which is ok) but also the shared pointer itself, by making it point to a different values from multiple threads. This is not ok.

A solution to that problem is to wrap the `shared_ptr` with a lock, but this solution is not so scalable under some contention, and in a sense, loses the automatic feeling of the standard shared pointer.

Another solution is to use the standard functions you quoted, such as `std::atomic_compare_exchange_weak`. This makes the work of synchronizing shared pointers a manual one, which we don't like.

This is where atomic shared pointer comes to play. You can mutate the shared pointer from multiple threads without fearing a data race and without using any locks. The standalone functions will be members ones, and their use will be much more natural for the user. This kind of pointer is extremely useful for lock-free data structures.

> NOTE:
>
> 1、tag-performance-lock VS lockless-scalability


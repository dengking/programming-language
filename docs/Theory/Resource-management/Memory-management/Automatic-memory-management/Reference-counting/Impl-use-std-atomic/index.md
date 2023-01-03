# Implement reference count use `std::atomic`

Basic requirement: 

a、atomic increment、decrement

b、在`refs`为0的时候，释放资源

Sample code: 

```c++
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

上述code源自 stackoverflow [How can memory_order_relaxed work for incrementing atomic reference counts in smart pointers?](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm) 

理解的难点:

1、为什么constructor increment可以使用 `memory_order_relaxed`:

 ```c++
 control_block_ptr->refs.fetch_add(1, memory_order_relaxed)
 ```

2、为什么destructor decrement使用 `memory_order_acq_rel`:

```c++
if (control_block_ptr->refs.fetch_sub(1, memory_order_acq_rel) == 1) {
    delete control_block_ptr;
}
```



## 素材

### stackoverflow [How can memory_order_relaxed work for incrementing atomic reference counts in smart pointers?](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm) 

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



Herb Sutter says the increment of **refs** in Thread A can use `memory_order_relaxed` because "nobody does anything based on the action". Now as I understand `memory_order_relaxed`, if **refs** equals N at some point and two threads A and B execute the following code:

> NOTE:
>
> 一、"nobody does anything based on the action"意味这不需要synchronization，这是比较符合 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 的观点的，其实这段话是从一个非常高的角度来解释为什么使用 `memory_order_relaxed`

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

### COMMENTS

So the only reason we need `memory_order_acq_rel` in the destructor is so that when the following is run: `delete control_block_ptr`; it does not run before `refs` is set to 0? – [CppNoob](https://stackoverflow.com/users/422131/cppnoob) [Dec 24 '14 at 3:50](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm/27716387#comment43682821_27631173) 



#### [A](https://stackoverflow.com/a/27716387)

Boost.Atomic library that emulates `std::atomic` provides [similar reference counting example and explanation](http://www.boost.org/doc/libs/1_57_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters), and it may help your understanding.

>NOTE: 
>
>一、原文引用的内容，下面收录了

#### [A](https://stackoverflow.com/a/44867508)

From C++ reference on [`std::memory_order`](http://en.cppreference.com/w/cpp/atomic/memory_order):

> `memory_order_relaxed`: Relaxed operation: there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed

There is also an example [below on that page](http://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering).

So basically, `std::atomic::fetch_add()` is still atomic, even when with `std::memory_order_relaxed`, therefore concurrent `refs.fetch_add(1, std::memory_order_relaxed)` from 2 different threads will always increment `refs` by 2. The point of the memory order is how other non-atomic or `std::memory_order_relaxed` atomic operations can be reordered around the current atomic operation with memory order specified.



### [Boost.Atomic](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic.html) # [Usage examples](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html) # [Reference counting](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters)

The purpose of a *reference counter* is to count the number of pointers to an object. The object can be destroyed as soon as the **reference counter** reaches zero.

#### [Implementation](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.implementation)

```C++
#include <boost/intrusive_ptr.hpp>
#include <boost/atomic.hpp>

class X {
public:
  typedef boost::intrusive_ptr<X> pointer;
  X() : refcount_(0) {}

private:
  mutable boost::atomic<int> refcount_;
  friend void intrusive_ptr_add_ref(const X * x)
  {
    x->refcount_.fetch_add(1, boost::memory_order_relaxed);
  }
  friend void intrusive_ptr_release(const X * x)
  {
    if (x->refcount_.fetch_sub(1, boost::memory_order_release) == 1) {
      boost::atomic_thread_fence(boost::memory_order_acquire);
      delete x;
    }
  }
};
```

> NOTE: 
>
> 一、intrusive的意思是: 侵入的

#### [Usage](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.usage)

```C++
X::pointer x = new X;
```

#### [Discussion](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.discussion)

Increasing the reference counter can always be done with `memory_order_relaxed`: New references to an object can only be formed from an existing reference, and passing an existing reference from one thread to another must already provide any required synchronization.

> NOTE: 
>
> 上面这段的后一句是对`class X`的用法的介绍，下面是字面的翻译: 
>
> "对象的新引用只能从现有引用形成，并且将现有引用从一个线程传递到另一个线程必须已经提供了所需的同步"
>
> 从字面意思，我们可以推测知道:
>
> 1、`class X`的用法和`std::shared_ptr`类似的: "New references to an object can only be formed from an existing reference"，即通过copy constructor。
>
> 2、`class X`的copy constructor不提供thread safety的保证，因此 "passing an existing reference from one thread to another must already provide any required synchronization"，那这是否意味着依赖于外部的"synchronization"来实现将`x->refcount_.fetch_add`的结果同步到了其它的thread。
>
> 最后一句话是误区: 现代CPU是能够保证更改被其他的thread看到的，不需要programmer进行额外的同步，所不能保证的是，所有的thread看到的是相同的order。

It is important to enforce(强制) any possible access to the object in one thread (through an **existing reference**) to *happen before* deleting the object in a different thread. This is achieved by a "release" operation after dropping a reference (any access to the object through this reference must obviously happened before), and an "acquire" operation before deleting the object.

> NOTE:
>
> 一、上面这段话是需要结合 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 中总结的模型来进行理解的，reference counting 类比 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 中给出的 guard-variable-playload 模型: 
>
> guard-variable: reference count
>
> playload: object
>
> "any possible access to the object in one thread (through an existing reference) to *happen before* deleting the object in a different thread"
>
> 使用write-release-read-acquire来实现happen-before relation

It would be possible to use `memory_order_acq_rel` for the `fetch_sub` operation, but this results in unneeded "acquire" operations when the reference counter does not yet reach zero and may impose a performance penalty.

> NOTE:
>
> 一、上面这段话中描述的点对于reference counting而言是不可避免的，因为每次更新了reference counting后它都需要检查

### cppreference [std::memory_order # Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 

Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the `shared_ptr` counters requires acquire-release synchronization with the destructor)

## 解释

`refs` 是shared data。是否会存在不同thread对shared data的修改没有同步过来而导致看到了旧数据？这是一个容易陷入的误区，事实是: 不同thread对shared data的修改，其他的thread是能够看到的，不同thread可能看到的不同的是: memory order的不同，正是由于order的不同，而导致了在lock free情况下会出现问题。因此需要使用memory order进行控制。始终谨记: memory order只能够控制single thread的ordering。





### construtor increment `memory_order_relaxed`？

increment使用的是`control_block_ptr->refs.fetch_add(1, memory_order_relaxed)`，显然这是atomic operation，因此是free of data race的，需要注意的是: 

1、`memory_order_relaxed`是指能够out of order execution，但是它的结果还是可以准确的更新到**share variable**即`refs`上的，即另外一个thread能够看到更新后的value，因此它还是能够保证reference count的运行正常

2、reordering发生于一个函数内，在 `increment` 中，它的上下文没有依赖它的，因此它允许reorder的方式，因此可以使用relaxed



### destructor decrement decrement `memory_order_acq_rel`

destructor是可以使用 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 中总结的模型来进行理解:

```c++
void SendTestMessage(void* param)
{
    // Copy to shared memory using non-atomic stores.
    g_payload.tick  = clock();
    g_payload.str   = "TestMessage";
    g_payload.param = param;
    
    // Perform an atomic write-release to indicate that the message is ready.
    g_guard.store(1, std::memory_order_release);
}

bool TryReceiveMessage(Message& result)
{
    // Perform an atomic read-acquire to check whether the message is ready.
    int ready = g_guard.load(std::memory_order_acquire);
    
    if (ready != 0)
    {
        // Yes. Copy from shared memory using non-atomic loads.
        result.tick  = g_payload.tick;
        result.str   = g_payload.str;
        result.param = g_payload.param;
        
        return true;
    }
    
    // No.
    return false;
}
```

a、`refs`是 **guard variable** 

b、`control_block_ptr`是**payload**



| preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) | reference counting                      |                   |
| ------------------------------------------------------------ | --------------------------------------- | ----------------- |
| `SendMessage`                                                | --reference count                       | **write-release** |
| `ReceiveMessage`                                             | if reference count == 0: delete object; | **read-acquire**  |



reference counting的特殊之处:

1、多个thread执行同一个函数，而不是像 preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/) 所给的例子各个thread执行不同的函数，这样可以将acquire-release拆分开来，另外一个不同点是: 上述使用的是read-modify-write操作。

2、正是由于多个thread执行同一个函数，因此它的**write-release-read-acquire**放到了同一个函数中进行实现



1、`control_block_ptr->refs.fetch_sub(1, memory_order_acq_rel)`是atomic operation，它能够原子的decrement `refs`并返回它的原值；显然当原值为1的时候，则表示需要delete了

2、`~smart_ptr()`的实现能够保证只有一个thread 在执行判断`== 1`的时候，条件满足，然后执行`delete`，因此保证了thread safe，不可能发生double delete；

3、`refs`是shared variable，因此会有多个thread同时对它进行read、write；



## Implementation

下面是参考实现:

1、`shared_ptr`，参见相关章节。

2、[amin-jabri](https://github.com/amin-jabri)/**[atomic_shared_ptr](https://github.com/amin-jabri/atomic_shared_ptr)** 参见相关章节

3、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[atomic_ref_count.h](https://github.com/chromium/chromium/blob/master/base/atomic_ref_count.h)**



### spdlog

spdlog使用`std::shared_ptr`来实现reference counting。


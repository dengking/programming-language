# Implement reference count use `std::atomic`

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

> NOTE: 上述是一个比较分散的实现

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

### COMMENTS

So the only reason we need memory_order_acq_rel in the destructor is so that when the following is run: `delete control_block_ptr`; it does not run before `refs` is set to 0? – [CppNoob](https://stackoverflow.com/users/422131/cppnoob) [Dec 24 '14 at 3:50](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm/27716387#comment43682821_27631173) 



### [A](https://stackoverflow.com/a/27716387)

Boost.Atomic library that emulates `std::atomic` provides [similar reference counting example and explanation](http://www.boost.org/doc/libs/1_57_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters), and it may help your understanding.

>NOTE: 原文引用的内容，下面收录了，因此这里copy。

### [A](https://stackoverflow.com/a/44867508)

From C++ reference on [`std::memory_order`](http://en.cppreference.com/w/cpp/atomic/memory_order):

> memory_order_relaxed: Relaxed operation: there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed

There is also an example [below on that page](http://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering).

So basically, `std::atomic::fetch_add()` is still atomic, even when with `std::memory_order_relaxed`, therefore concurrent `refs.fetch_add(1, std::memory_order_relaxed)` from 2 different threads will always increment `refs` by 2. The point of the memory order is how other non-atomic or `std::memory_order_relaxed` atomic operations can be reordered around the current atomic operation with memory order specified.

> NOTE: 始终谨记: memory order只能怪控制single thread的ordering

## [Boost.Atomic](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic.html) # [Usage examples](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html) # [Reference counting](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters)

The purpose of a *reference counter* is to count the number of pointers to an object. The object can be destroyed as soon as the **reference counter** reaches zero.

### [Implementation](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.implementation)

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

> NOTE: intrusive的意思是: 侵入的

### [Usage](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.usage)

```C++
X::pointer x = new X;
```

### [Discussion](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters.discussion)

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

It would be possible to use `memory_order_acq_rel` for the `fetch_sub` operation, but this results in unneeded "acquire" operations when the reference counter does not yet reach zero and may impose a performance penalty.

> NOTE: 阅读了上面这段话，我的想法有:
>
> 1、"any possible access to the object in one thread (through an existing reference) to *happen before* deleting the object in a different thread"
>
> 使用write-release-read-acquire来实现happen-before relation,显然，这是为了保证正确的运行
>
> 2、"This is achieved by a "release" operation after dropping a reference"
>
> 此处"release"是非常好理解的，具体可以参加 "aristeia-C++and-the-Perils-of-Double-Checked-Locking"，其中有非常好的解释
>
> 3、"an "acquire" operation before deleting the object"
>
> "acquire"要如何理解呢？
>
> 

## medium [C++ atomic 的例子：為何 reference count -1 的時候要用 memory_order_acq_rel](https://medium.com/@fcamel/c-atomic-%E6%93%8D%E4%BD%9C%E7%9A%84%E4%BE%8B%E5%AD%90-c85295b08af4)

> NOTE: 原文是使用繁体中文写的

這是 Herb Sutter 在 《C++ and Beyond 2012: atomic<> Weapons》介紹的例子：

不懂為何在 -1 的時候要用 `memory_order_acq_rel`。

> NOTE: 

[在這裡查到說明](http://stackoverflow.com/a/27716387/278456)：

> NOTE: 上述链接到的如下内容:
>
> stackoverflow [How can memory_order_relaxed work for incrementing atomic reference counts in smart pointers?](https://stackoverflow.com/questions/27631173/how-can-memory-order-relaxed-work-for-incrementing-atomic-reference-counts-in-sm)
>
> 前面已经包含了，此处不再重复



在 -1 的时候分两个情況讨论：

1、n > 1: 确保释放object前的更新能被其它 thread 看到，需要 release。

2、n = 1: 确保有看到其它 thread 的更新，需要 acquire。

> NOTE: 上述解释是有误的，上述实现中对acquire、release的使用，并没有上面描述的修改，上述实现中对acquire、release，在 [Boost.Atomic](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic.html) # [Usage examples](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html) # [Reference counting](https://www.boost.org/doc/libs/1_75_0/doc/html/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters) 中已经说得很清楚了。

除了演讲[投影片中提到的例子](https://onedrive.live.com/?authkey=!AMtj_EflYn2507c&cid=4E86B0CF20EF15AD&id=4E86B0CF20EF15AD!24884&parId=4E86B0CF20EF15AD!180&o=OneUp) (影片中沒有细讲)，Boost  也有提供一些 [atomic 的使用例子](http://www.boost.org/doc/libs/1_57_0/doc/html/atomic/usage_examples.html)。要用的時候，还是多看些例子，看看能否直接拿來用 (C++11 应该和 Boost 的 API 差不多吧，有需要再来确认，還有查其它 C++11 的例子)，或是借此确认观念正确再來用 atomic。



## SUMMARY

1、`refs` 是shared data

2、是否会存在不同thread对shared data的修改没有同步过来而导致看到了旧数据？这是一个容易陷入的误区，事实是: 不同thread对shared data的修改，其他的thread是能够看到的，不同thread可能看到的不同的是: memory order的不同，正是由于order的不同，而导致了在lock free情况下会出现问题。因此需要使用memory order进行控制。

> NOTE: tag-order of write to shared data may be different among different threads

### 为什么increment的时候，可以使用`memory_order_relaxed`？

increment使用的是`control_block_ptr->refs.fetch_add(1, memory_order_relaxed)`，显然这是atomic operation，因此是free of data race的，需要注意的是: 

1、`memory_order_relaxed`是指能够out of order execution，但是它的结果还是可以准确的更新到**share variable**即`refs`上的，即另外一个thread能够看到更新后的value，因此它还是能够保证reference count的运行正常

2、reordering发生于一个函数内，在 `increment` 中，它的上下文没有依赖它的，因此它可以使用relaxed、

### decrement的实现



1、`control_block_ptr->refs.fetch_sub(1, memory_order_acq_rel)`是atomic operation，它能够原子的decrement `refs`并返回它的原值；显然当原值为1的时候，则表示需要delete了

2、`~smart_ptr()`的实现能够保证只有一个thread 在执行判断`== 1`的时候，条件满足，然后执行`delete`，因此保证了thread safe，不可能发生double delete；

3、`refs`是shared variable，因此会有多个thread同时对它进行read、write；

由于`delete control_block_ptr`是依赖于`refs`的，因此按照preshing [The Synchronizes-With Relation](https://preshing.com/20130823/the-synchronizes-with-relation/)中的说法:

a、`refs`是 **guard variable** 

b、`control_block_ptr`是**payload**

由于`delete control_block_ptr`的执行，是依赖于`refs`的，因此对它的decrement(write)是需要release operation的；

那为什么在`delete control_block_ptr`之前还需要acquire operation呢？是需要建立如下所述的happen before relation: 

> It is important to enforce any possible access to the object in one thread (through an existing reference) to **happen before** deleting the object in a different thread. This is achieved by a "release" operation after dropping a reference (any access to the object through this reference must obviously happened before), and an "acquire" operation before deleting the object.

让"reference count == 0 " happens-before "deleting the object"



## Implementation

下面是参考实现:

1、`shared_ptr`，参见相关章节。

2、[amin-jabri](https://github.com/amin-jabri)/**[atomic_shared_ptr](https://github.com/amin-jabri/atomic_shared_ptr)** 参见相关章节

3、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[atomic_ref_count.h](https://github.com/chromium/chromium/blob/master/base/atomic_ref_count.h)**


# modernescpp [Acquire-Release Fences](https://www.modernescpp.com/index.php/acquire-release-fences)

Acquire and release fences guarantees **similar** [synchronisation and ordering constraints](https://www.modernescpp.com/index.php/synchronization-and-ordering-constraints) as atomics with [acquire-release semantic](https://www.modernescpp.com/index.php/acquire-release-semantic). Similar, because the differences are in the details.

> NOTE: 
>
> Acquire and release fences
>
> atomic with acquire-similar semantic
>
> 注意，上面这段话中使用的是"similar"，而不是"same"，也就是说还是有点差异的，并且"differences are in the details"，即差异是在细微之处

The most obvious difference between **acquire and release memory barriers (fences)** and **atomics with acquire-release semantic** is that memory barriers need no operations on atomics. But there is a more subtle difference. The acquire and release memory barriers are more heavyweight.

## Atomic operations versus memory barriers

To make my job of writing simpler, I will now simply speak of **acquire operations**, if I use memory barriers or atomic operations with acquire semantic. The same will hold for release operations.

The key idea of an **acquire and a release operation** is, that it establishes synchronisations and ordering constraints between thread. This will also hold for atomic operations with relaxed semantic or non-atomic operations. So you see, **the acquire and release operations come in pairs**. In addition, for the operations on atomic variables with acquire-release semantic must hold that these act on the same atomic variable. Said that I will in the first step look at these operations in isolation.

> NOTE: **acquire and a release operation** 如何建立 synchronization？

I start with the acquire operation.

### Acquire operation

A read operation on an atomic variable attached with `std::memory_order_acquire` is an **acquire operation**.

 

![acquireOperation](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/acquireOperation.png) 

In opposite to that there is the `std::atomic_thread_fence` with **acquire semantic**.

![acquireFence](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/acquireFence.png)

This comparison emphasise two points.

1、A memory barrier with acquire semantic establishes stronger ordering constraints. Although the acquire operation on an atomic and on a memory barrier requires, that no read or write operation can be moved before the acquire operation, there is an additional guarantee with the acquire memory barrier. No read operation can be moved after the acquire memory barrier.

2、The relaxed semantic is sufficient for the reading of the atomic variable var. The `std::atomc_thread_fence(std::memory_order_acquire)` ensures that this operation can not be moved after the acquire fence.

The similar statement holds for the release memory barrier.

### Release operation

The write operation on an atomic variable attached with the memory model std::memory_order_release is a release operation.

 ![releaseOperation](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/releaseOperation.png)

And further the release memory barrier.

![releaseFence](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/releaseFence.png)

In addition to the release operation on an atomic variable `var`, the release barrier guarantees two points:

1、Store operations can't be moved before the memory barrier.

2、It's sufficient for the variable var to have relaxed semantic.

In case you want a simple overview of memory barriers, please read the last post in this blog. But now, I want to go one step further and build a program out of the presented components.

## Synchronisation with atomic operations versus memory barriers

I implement as starting point for my comparison a typical consumer-producer workflow with acquire-release semantic. I will do this job with atomics and memory barriers.

Let's start with atomics because the most of us are comfortable with them. That will not hold for memory barriers. They are almost complete ignored in the literature to the C++ memory model.



```C++
// acquireRelease.cpp

#include <atomic>
#include <thread>
#include <iostream>
#include <string>
 
std::atomic<std::string*> ptr;
int data;
std::atomic<int> atoData;
 
void producer(){
    std::string* p  = new std::string("C++11");
    data = 2011;
    atoData.store(2014,std::memory_order_relaxed);
    ptr.store(p, std::memory_order_release);
}
 
void consumer(){
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)));
    std::cout << "*p2: " << *p2 << std::endl;
    std::cout << "data: " << data << std::endl;
    std::cout << "atoData: " << atoData.load(std::memory_order_relaxed) << std::endl;
}
 
int main(){
    
    std::cout << std::endl;
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    delete ptr;
    
    std::cout << std::endl;
    
}
// g++ --std=c++11 test.cpp -lpthread

```

I hope, this program looks familiar to you. That my classic that I used in the post to[ memory_order_consume](https://www.modernescpp.com/index.php/tag/memory-order-consume). The graphic goes directly to the point, why the consumer thread t2 sees all values from the producer thread t1.

![acquireRelease](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/acquireRelease.png)

The program is well defined, because the *happens-before* relation is transitive. I have only to combine the three happens-before relations:

1、Line 13 - 15 *happens-before* line 16 (`ptr.store(p,std::memory_order_release`).

2、Line 21 `while(!(p2= ptrl.load(std::memory_order_acquire)))` *happens-before* the lines 22 - 24.

3、Line 16 *synchronizes-with* line 21. **=>** Line 16 *happens-before* line 21.

But now the story get's more thrilling. How can I adjust the workflow to memory barriers?

### Memory barriers

It's straightforward to port the program to memory barriers.

```C++
// acquireReleaseFences.cpp

#include <atomic>
#include <thread>
#include <iostream>
#include <string>
 
std::atomic<std::string*> ptr;
int data;
std::atomic<int> atoData;
 
void producer(){
    std::string* p  = new std::string("C++11");
    data = 2011;
    atoData.store(2014,std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    ptr.store(p, std::memory_order_relaxed);
}
 
void consumer(){
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_relaxed)));
    std::atomic_thread_fence(std::memory_order_acquire);
    std::cout << "*p2: " << *p2 << std::endl;
    std::cout << "data: " << data << std::endl;
    std::cout << "atoData: " << atoData.load(std::memory_order_relaxed) << std::endl;
}
 
int main(){
    
    std::cout << std::endl;
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    delete ptr;
    
    std::cout << std::endl;
    
}
```

The first step is to insert just in place of the operations with acquire and release semantic the corresponding memory barriers with acquire and release semantic (line 16 and 23). In the next step, I change the atomic operations with acquire or release semantic to relaxed semantic (line 17 and 22). That was already mechanically. Of course, I can only replace one acquire or release operation with the corresponding memory barrier. The key point is, that the release operation establishes with the acquire operation a *synchronize-with* relation and therefore a *happens-before* relation.

For the more visual reader, the whole description in a picture.

![acquireReleaseFences](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/acquireReleaseFences.png)

The key question is. Why do the operations after the acquire memory barrier sees the effects of the operations before the release memory barrier? Because data is a non-atomic variable and atoData is used with relaxed semantic, both can be reordered. But that's not possible. The std::atomic_thread_fence(std::memory_order_release) as a release operation in combination with the std::atomic_thread_fence(std::memory_order_acquire) forbid the partial reordering. To follow my reasoning in detail, read the analysis of the memory barriers at the beginning of the post.

For clarity, the whole reasoning to the point.

1. The acquire and release memory barriers prevents the reordering of the atomic and non-atomic operations across the memory barriers.
2. The consumer thread t2 is waiting in the while (!(p2= ptr.load(std::memory_order_relaxed))) loop, until the pointer ptr.stor(p,std::memory_order_relaxed) is set in the producer thread t1.
3. The release memory barrier *synchronizes-with* the acquire memory barrier.

Finally, the output of the programs.

![acquireReleaseAcquireReleaseFences](https://www.modernescpp.com/images/blog/Speichermodell/AcquireUndReleaseFences/acquireReleaseAcquireReleaseFences.png)
# Release-Consume ordering

1、data dependency，参见 `Data-dependency` 章节

## cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Release-Consume ordering

If an atomic store in thread A is tagged `memory_order_release` and an atomic load in thread B from the same variable that read the stored value is tagged `memory_order_consume`, all memory writes (non-atomic and relaxed atomic) that *happened-before* the atomic store from the point of view of thread A, become *visible side-effects* within those operations in thread B into which the load operation *carries dependency*, that is, once the atomic load is completed, those operators and functions in thread B that use the value obtained from the load are guaranteed to see what thread A wrote to memory.

> NOTE: 
> 1、"those operators and functions in thread B that use the value " 这段话中，"use"的含义其实是dependent on

### 使用modification order来进行描述

The synchronization is established only between the threads *releasing* and *consuming* the same atomic variable. Other threads can see different order of memory accesses than either or both of the synchronized threads.

> NOTE: 
>
> 1、这段话，总结得非常好
>
> 2、它是使用modification order的角度来进行描述的

### Implementation

On all mainstream CPUs other than DEC Alpha, **dependency ordering** is automatic, no additional CPU instructions are issued for this synchronization mode, only certain **compiler optimizations** are affected (e.g. the compiler is prohibited from performing **speculative loads**(推测的) on the objects that are involved in the **dependency chain**).



See also [std::kill_dependency](https://en.cppreference.com/w/cpp/atomic/kill_dependency) and `[[carries_dependency]]` for fine-grained dependency chain control.

Note that currently (2/2015) no known production compilers track dependency chains: consume operations are lifted to acquire operations.



### Use case

Typical use cases for this ordering involve read access to rarely written concurrent data structures (routing tables, configuration, security policies, firewall rules, etc) and publisher-subscriber situations with pointer-mediated publication, that is, when the producer publishes a pointer through which the consumer can access information: there is no need to make everything else the producer wrote to memory visible to the consumer (which may be an expensive operation on weakly-ordered architectures). An example of such scenario is [rcu_dereference](https://en.wikipedia.org/wiki/Read-copy-update).

### (since C++17)

The specification of release-consume ordering is being revised, and the use of `memory_order_consume` is temporarily discouraged.



### Example

This example demonstrates **dependency-ordered synchronization** for pointer-mediated publication: the integer data is not related to the pointer to string by a data-dependency relationship, thus its value is undefined in the consumer.

> NOTE: 
>
> 1、由于 `data` 和 "pointer to string "`ptr` 之间并没有 "data-dependency relationship"，因此对 `data` 的read、write可能进行modification，因此在 `consumer` 中，`assert(data == 42); // may or may not fire: data does not carry dependency from ptr`。

```C++
#include <thread>
#include <atomic>
#include <cassert>
#include <string>
 
std::atomic<std::string*> ptr;
int data;
 
void producer()
{
    std::string* p  = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}
 
void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_consume)))
        ;
    assert(*p2 == "Hello"); // never fires: *p2 carries dependency from ptr
    assert(data == 42); // may or may not fire: data does not carry dependency from ptr
}
 
int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}
```


# 5.3 Synchronizing operations and enforcing ordering

Suppose you have two threads, one of which is populating a data structure to be read by the second. In order to avoid a problematic race condition, the first thread sets a flag to indicate that the data is ready, and the second thread doesn’t read the data until the flag is set. The following listing shows such a scenario.

![](./Listing-5.2-Reading-and-writing-variables-from-different-threads.jpg)



```c++
#include <atomic>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

std::vector<int> data;
std::atomic<bool> data_ready(false);

void reader_thread()
{
    while (!data_ready.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "The answer=" << data[0] << "\n";
}
void writer_thread()
{
    data.push_back(42);
    data_ready = true;
}

int main()
{
    std::thread t1(writer_thread);
    std::thread t2(reader_thread);
    t1.join();
    t2.join();
}
```



The required enforced ordering comes from the operations on the `std::atomic<bool>` variable `data_ready`; they provide the necessary ordering by virtue of the memory model relations:

1、happens-before 

2、synchronizes-with. 

The write of the `data` **happens-before** the write to the `data_ready` flag, and the read of the flag **happens-before** the read of the `data`. When the value read from `data_ready` is true, the write **synchronizes-with** that read, creating a **happens-before** relationship. Because **happens-before** is **transitive**, the write to the `data` **happens-before** the write to the flag, which **happens-before** the read of the true value from the flag, which **happens-before** the read of the `data`, and you have an **enforced ordering**: the write of the `data` **happens-before** the read of the `data` and everything is OK. Figure 5.2 shows the important **happens-before** relationships in the two threads. I’ve added a couple of iterations of the while loop from the reader thread.



![](./Figure-5.2-Enforce-ordering-between-nonatomic-operation-using-atomic-operations.jpg)





All this might seem fairly intuitive: of course the operation that writes a value **happens before** an operation that reads that value! With the default atomic operations, that’s indeed true (which is why this is the default), but it does need spelling out: the atomic operations also have other options for the ordering requirements, which I’ll come to shortly.

Now that you’ve seen **happens-before** and **synchronizes-with** in action, it’s time to look at what they really mean. I’ll start with **synchronizes-with**.

## 5.3.1 The synchronizes-with relationship

The **synchronizes-with** relationship is something that you can get only between operations on **atomic types**. Operations on a data structure (such as locking a mutex) might provide this relationship if the data structure contains atomic types and the operations on that data structure perform the appropriate atomic operations internally, but fundamentally it comes only from operations on **atomic types**.

The basic idea is this: a **suitably tagged atomic write operation** `W` on a variable `x` **synchronizes-with** a **suitably tagged atomic read operation** on `x` that reads the value stored by either that write (`W`), or a subsequent(随后的) **atomic write operation** on `x` by the same thread that performed the initial write `W`, or a sequence of atomic read-modify-write operations on `x` (such as `fetch_add()` or `compare_exchange_weak()`) by any thread, where the value read by the first thread in the sequence is the value written by `W` (see section 5.3.4).

> NOTE:
>
> 一、上面这段话要如何理解？
>
> 1、对于同一个shared variable `x`
>
> 2、write operation W **synchronizes-with** read operation，简而言之: read the write value
>
> 3、对`x`所有的operation，现成一个sequence
>
> 在 "5.3.4 Release sequences and synchronizes-with" 节对上面这段话的解释如下: 
>
> > Back in section 5.3.1, I mentioned that you could get a synchronizes-with relationship between a store to an atomic variable and a load of that atomic variable from another thread, even when there’s a sequence of read-modify-write operations between the store and the load, provided all the operations are suitably tagged.



Leave the “suitably tagged” part aside for now, because all operations on atomic types are suitably tagged by default. This essentially means what you might expect: if thread A stores a value and thread B reads that value, there’s a synchronizes-with relationship between the store in thread A and the load in thread B, just as in listing 5.2.

As I’m sure you’ve guessed, the nuances(细微差别) are all in the “suitably tagged” part. The C++ memory model allows various ordering constraints to be applied to the operations on atomic types, and this is the tagging to which I refer. The various options for memory ordering and how they relate to the **synchronizes-with relationship** are covered in section 5.3.3. First, let’s step back and look at the **happens-before relationship**.



## 5.3.4 Release sequences and synchronizes-with

Back in section 5.3.1, I mentioned that you could get a synchronizes-with relationship between a store to an atomic variable and a load of that atomic variable from another thread, even when there’s a sequence of read-modify-write operations between the store and the load, provided all the operations are suitably tagged.
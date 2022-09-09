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



![](./Figure-5.2-Enforcing-an-ordering-between-nonatomic-operations-using-atomic-operations.jpg)






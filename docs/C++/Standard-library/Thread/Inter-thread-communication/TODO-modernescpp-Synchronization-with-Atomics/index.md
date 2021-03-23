# [Synchronization with Atomics in C++20](https://www.modernescpp.com/index.php/synchronization-with-atomics-in-c-20)

## Condition Variables

```C++
// threadSynchronisationConditionVariable.cpp

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mutex_;
std::condition_variable condVar;

std::vector<int> myVec{};

void prepareWork() {                                       // (1)

    {
        std::lock_guard<std::mutex> lck(mutex_);
        myVec.insert(myVec.end(), {0, 1, 0, 3});           // (3)
    }
    std::cout << "Sender: Data prepared."  << std::endl;
    condVar.notify_one();
}

void completeWork() {                                       // (2)

    std::cout << "Worker: Waiting for data." << std::endl;
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck, [] { return not myVec.empty(); });
    myVec[2] = 2;                                           // (4)
    std::cout << "Waiter: Complete the work." << std::endl;
    for (auto i: myVec) std::cout << i << " ";
    std::cout << std::endl;
    
}

int main() {

    std::cout << std::endl;

    std::thread t1(prepareWork);
    std::thread t2(completeWork);

    t1.join();
    t2.join();

    std::cout << std::endl;
  
}
```

## Promises and Futures

```c++
// threadSynchronisationPromiseFuture.cpp

#include <iostream>
#include <future>
#include <thread>
#include <vector>

std::vector<int> myVec{};

void prepareWork(std::promise<void> prom) {

    myVec.insert(myVec.end(), {0, 1, 0, 3});
    std::cout << "Sender: Data prepared."  << std::endl;
    prom.set_value();                                     // (1)

}

void completeWork(std::future<void> fut){

    std::cout << "Worker: Waiting for data." << std::endl;
    fut.wait();                                           // (2)
    myVec[2] = 2;
    std::cout << "Waiter: Complete the work." << std::endl;
    for (auto i: myVec) std::cout << i << " ";
    std::cout << std::endl;
    
}

int main() {

    std::cout << std::endl;

    std::promise<void> sendNotification;
    auto waitForNotification = sendNotification.get_future();

    std::thread t1(prepareWork, std::move(sendNotification));
    std::thread t2(completeWork, std::move(waitForNotification));

    t1.join();
    t2.join();

    std::cout << std::endl;
  
}
```

## `std::atomic_flag`

```C++
// threadSynchronisationAtomicFlag.cpp

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::vector<int> myVec{};

std::atomic_flag atomicFlag{};

void prepareWork() {

    myVec.insert(myVec.end(), {0, 1, 0, 3});
    std::cout << "Sender: Data prepared."  << std::endl;
    atomicFlag.test_and_set();                             // (1)
    atomicFlag.notify_one();   

}

void completeWork() {

    std::cout << "Worker: Waiting for data." << std::endl;
    atomicFlag.wait(false);                                // (2)
    myVec[2] = 2;
    std::cout << "Waiter: Complete the work." << std::endl;
    for (auto i: myVec) std::cout << i << " ";
    std::cout << std::endl;
    
}

int main() {

    std::cout << std::endl;

    std::thread t1(prepareWork);
    std::thread t2(completeWork);

    t1.join();
    t2.join();

    std::cout << std::endl;
  
}
```
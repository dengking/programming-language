# modernescpp [Performance Comparison of Condition Variables and Atomics in C++20](https://www.modernescpp.com/index.php/component/jaggyblog/performancecomparison-of-condition-variables-and-atomics-in-c-20)

## Condition Variables

```C++
// pingPongConditionVariable.cpp

#include <condition_variable>
#include <iostream>
#include <atomic>
#include <thread>

bool dataReady{false};

std::mutex mutex_;
std::condition_variable condVar1;          // (1)
std::condition_variable condVar2;          // (2)

std::atomic<int> counter{};
constexpr int countlimit = 1'000'000;

void ping() {

    while(counter <= countlimit) {
        {
            std::unique_lock<std::mutex> lck(mutex_);
            condVar1.wait(lck, []{return dataReady == false;});
            dataReady = true;
        }
        ++counter;                          
        condVar2.notify_one();              // (3)
  }
}

void pong() {

    while(counter < countlimit) {  
        {
            std::unique_lock<std::mutex> lck(mutex_);
            condVar2.wait(lck, []{return dataReady == true;});
            dataReady = false;
        }
        condVar1.notify_one();            // (3)
  }

}

int main(){

    auto start = std::chrono::system_clock::now();  

    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();
  
    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << std::endl;

}
```

## `std::atomic_flag`

```c++
// pingPongAtomicFlags.cpp

#include <iostream>
#include <atomic>
#include <thread>

std::atomic_flag condAtomicFlag1{};
std::atomic_flag condAtomicFlag2{};

std::atomic<int> counter{};
constexpr int countlimit = 1'000'000;

void ping() {
    while(counter <= countlimit) {
        condAtomicFlag1.wait(false);               // (1)
        condAtomicFlag1.clear();                   // (2)

        ++counter;
        
        condAtomicFlag2.test_and_set();           // (4)
        condAtomicFlag2.notify_one();             // (3)
    }
}

void pong() {
    while(counter < countlimit) {
        condAtomicFlag2.wait(false);
        condAtomicFlag2.clear();
        
        condAtomicFlag1.test_and_set();
        condAtomicFlag1.notify_one();
    }
}

int main() {

     auto start = std::chrono::system_clock::now();  

    condAtomicFlag1.test_and_set();                    // (5)
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << std::endl;

}
```

### One Atomic Flag

```c++
// pingPongAtomicFlag.cpp

#include <iostream>
#include <atomic>
#include <thread>

std::atomic_flag condAtomicFlag{};

std::atomic<int> counter{};
constexpr int countlimit = 1'000'000;

void ping() {
    while(counter <= countlimit) {
        condAtomicFlag.wait(true);
        condAtomicFlag.test_and_set();
        
        ++counter;
        
        condAtomicFlag.notify_one();
    }
}

void pong() {
    while(counter < countlimit) {
        condAtomicFlag.wait(false);
        condAtomicFlag.clear();
        condAtomicFlag.notify_one();
    }
}

int main() {

     auto start = std::chrono::system_clock::now();  

    
    condAtomicFlag.test_and_set();
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << std::endl;

}
```

## `std::atomic<bool>`

```c++
// pingPongAtomicBool.cpp

#include <iostream>
#include <atomic>
#include <thread>

std::atomic<bool> atomicBool{};

std::atomic<int> counter{};
constexpr int countlimit = 1'000'000;

void ping() {
    while(counter <= countlimit) {
        atomicBool.wait(true);
        atomicBool.store(true);

        ++counter;
        
        atomicBool.notify_one();
    }
}

void pong() {
    while(counter < countlimit) {
        atomicBool.wait(false);
        atomicBool.store(false);
        atomicBool.notify_one();
    }
}

int main() {

    std::cout << std::boolalpha << std::endl;

    std::cout << "atomicBool.is_lock_free(): "              // (1)
              << atomicBool.is_lock_free()  << std::endl; 

    std::cout << std::endl;

    auto start = std::chrono::system_clock::now();

    atomicBool.store(true);
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << std::endl;

}
```

## All Numbers

As expected, condition variables are the slowest way, and atomic flag the fastest way to synchronize threads. The performance of a` std::atomic<bool>` is in-between. But there is one downside with `std:.atomic<bool>. std::atomic_flag i`s the only atomic data type which is lock-free.

![PerformanceComparison](https://www.modernescpp.com/images/blog/Cpp20/PerformanceComparisonConditionVariablesAtomics/PerformanceComparison.png)
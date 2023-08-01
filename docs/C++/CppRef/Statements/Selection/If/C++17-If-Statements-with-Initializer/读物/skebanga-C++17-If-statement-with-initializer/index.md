# skebanga [C++17 If statement with initializer](https://skebanga.github.io/if-with-initializer/)

Introduced under proposal [P00305r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r0.html), **If statement with initializer** give us the ability to initialize a variable within an `if` statement, and then, once initialized, perform the actual conditional check.



## `std::map::insert`



> NOTE:
>
> 一、cppreference [std::map<Key,T,Compare,Allocator>::insert](https://en.cppreference.com/w/cpp/container/map/insert)
>
> > Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key.



### 写法一

```c++
#include <iostream>
#include <map>

int main()
{
    std::map<std::string, int> map;
    map["hello"] = 1;
    map["world"] = 2;

    // first we have to perform the operation and capture the return value
    auto ret = map.insert({ "hello", 3 });

    // only then can we check the return value
    if (!ret.second)
        std::cout << "hello already exists with value " << ret.first->second << "\n";

    // ret has "leaked" into this scope, so we need to pick a different name, ret2
    auto ret2 = map.insert({ "foo", 4 });

    // now we can check the value of ret2
    if (!ret2.second)
        std::cout << "foo already exists with value " << ret2.first->second << "\n";

    return 0;
}
```

### 写法二

```c++
#include <iostream>
#include <map>

int main()
{
    std::map<std::string, int> map;
    map["hello"] = 1;
    map["world"] = 2;

    // we create a scope to enclose ret, preventing it leaking into the surrounding scope
    {
        auto ret = map.insert({ "hello", 3 });
        if (!ret.second)
            std::cout << "hello already exists with value " << ret.first->second << "\n";
    }
    // we create another scope to enclose ret, again preventing it from leaking out
    {
        auto ret = map.insert({ "foo", 4 });
        if (!ret.second)
            std::cout << "foo already exists with value " << ret.first->second << "\n";
    }

    return 0;
}
```



### 写法三 C++17

```c++
#include <iostream>
#include <map>

int main()
{
    std::map<std::string, int> map;
    map["hello"] = 1;
    map["world"] = 2;

    // initialize the condition we want to check from within the if statement
    if (auto ret = map.insert({ "hello", 3 }); !ret.second)
        std::cout << "hello already exists with value " << ret.first->second << "\n";

    // ret has not leaked, so we can use that for this conditional check too
    if (auto ret = map.insert({ "foo", 4 }); !ret.second)
        std::cout << "foo already exists with value " << ret.first->second << "\n";

    return 0;
}
```



## [`std::unique_lock`](https://en.cppreference.com/w/cpp/thread/unique_lock)



```c++
#include <iostream>
#include <mutex>

int main()
{
    std::mutex mtx;

    // create an RAII style lock guard, but don't block if we can't lock - check to
    //   see whether we were able to get the lock or not
    if (std::unique_lock<std::mutex> l(mtx, std::try_to_lock); l.owns_lock())
    {
        std::cout << "successfully locked the resource\n";

        //...
    }
    else
    {
        std::cout << "resource not currently available\n";
    }
    return 0;
}
```





```c++
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

int main()
{
    // a mutex which will protect a shared resource
    std::mutex mtx;

    // start a thread which obtains the mutex and then sleeps for 2 seconds
    //   (to give enough time for main thread to attempt to lock the mutex)
    std::thread t([&]()
        {
            std::lock_guard<std::mutex> l(mtx);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        });

    // sleep for a short while to give thread t enough time to start and obtain the mutex
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // when we try to lock in our if-with-initializer it should fail, as t owns the lock
    if (std::unique_lock<std::mutex> l(mtx, std::try_to_lock); l.owns_lock())
    {
        std::cout << "successfully locked the resource\n";

        //...
    }
    else
    {
        std::cout << "resource not currently available\n";
    }

    // wait for t to finish before exiting
    if (t.joinable())
        t.join();
    return 0;
}
```



## Combining with [structured bindings](https://skebanga.github.io/structured-bindings/)



```c++
#include <iostream>
#include <map>

int main()
{
    std::map<std::string, int> map;
    map["hello"] = 1;
    map["world"] = 2;

    // intitialize the condition we want to check from within the if statement
    if (auto [it, inserted] = map.insert({ "hello", 3 }); !inserted)
        std::cout << "hello already exists with value " << it->second << "\n";

    // ret has not leaked, so we can use that for this conditional check too
    if (auto [it, inserted] = map.insert({ "foo", 4 }); !inserted)
        std::cout << "foo already exists with value " << it->second << "\n";

    return 0;
}
```





## Switch statements



### Pre C++17

```c++
#include <iostream>

enum Result
{
    SUCCESS,
    DEVICE_FULL,
    ABORTED
};

std::pair<size_t /* bytes */, Result> writePacket()
{
    return { 100, SUCCESS };
}

int main()
{
    // initialize the value we want to switch on (res ends up in surrounding scope)
    auto res = writePacket();

    // then switch on that value
    switch (res.second)
    {
        case SUCCESS:
            std::cout << "successfully wrote " << res.first << " bytes\n";
            break;
        case DEVICE_FULL:
            std::cout << "insufficient space on device\n";
            break;
        case ABORTED:
            std::cout << "operation aborted before completion\n";
            break;
    }
    return 0;
}
```



### C++17



```c++
#include <iostream>

enum Result
{
    SUCCESS,
    DEVICE_FULL,
    ABORTED
};

std::pair<size_t /* bytes */, Result> writePacket()
{
    return { 100, SUCCESS };
}

int main()
{
    switch (auto res = writePacket(); res.second)
    {
        case SUCCESS:
            std::cout << "successfully wrote " << res.first << " bytes\n";
            break;
        case DEVICE_FULL:
            std::cout << "insufficient space on device\n";
            break;
        case ABORTED:
            std::cout << "operation aborted before completion\n";
            break;
    }
    return 0;
}
```




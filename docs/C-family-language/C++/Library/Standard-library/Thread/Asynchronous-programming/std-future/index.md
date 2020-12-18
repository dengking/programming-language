# std::future



## cppreference [std::future](https://en.cppreference.com/w/cpp/thread/future)

The class template `std::future` provides a mechanism to access the result of **asynchronous operations**:

1 An asynchronous operation (created via [std::async](https://en.cppreference.com/w/cpp/thread/async), [std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task), or [std::promise](https://en.cppreference.com/w/cpp/thread/promise)) can provide a `std::future` object to the creator of that **asynchronous operation**.



2 The creator of the asynchronous operation can then use a variety of methods to query, wait for, or extract a value from the `std::future`. These methods may block if the asynchronous operation has not yet provided a value.



3 When the asynchronous operation is ready to send a result to the creator, it can do so by modifying *shared state* (e.g. [std::promise::set_value](https://en.cppreference.com/w/cpp/thread/promise/set_value)) that is linked to the creator's `std::future`.

### `std::future` VS `std::shared_future`

Note that `std::future` references shared state that is not shared with any other asynchronous return objects (as opposed to [std::shared_future](https://en.cppreference.com/w/cpp/thread/shared_future)).



```C++
#include <iostream>
#include <future>
#include <thread>

int main()
{
    // future from a packaged_task
    std::packaged_task<int()> task([]{ return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread t(std::move(task)); // launch on a thread

    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, []{ return 8; });

    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{ p.set_value_at_thread_exit(9); }).detach();

    std::cout << "Waiting..." << std::flush;
    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
    t.join();
}
// g++ --std=c++11 test.cpp -lpthread

```


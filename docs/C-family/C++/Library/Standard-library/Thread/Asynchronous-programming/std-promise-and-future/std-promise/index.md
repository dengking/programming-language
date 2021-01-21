# `std::promise`



## cppreference [std::promise](https://en.cppreference.com/w/cpp/thread/promise)

The class template `std::promise` provides a facility to store a **value** or an **exception** that is later acquired asynchronously via a [std::future](https://en.cppreference.com/w/cpp/thread/future) object created by the `std::promise` object. Note that the `std::promise` object is meant to be used only once.

Each promise is associated with a *shared state*, which contains some state information and a *result* which may be not yet evaluated, evaluated to a value (possibly void) or evaluated to an exception. A promise may do three things with the **shared state**:

1 *make ready*: the promise stores the result or the exception in the **shared state**. Marks the state ready and unblocks any thread waiting on a future associated with the shared state.

2 *release*: the promise gives up its reference to the shared state. If this was the last such reference, the **shared state** is destroyed. Unless this was a **shared state** created by [std::async](http://en.cppreference.com/w/cpp/thread/async) which is not yet ready, this operation does not block.

> NOTE: reference count

3 *abandon*: the promise stores the exception of type [std::future_error](https://en.cppreference.com/w/cpp/thread/future_error) with error code [std::future_errc::broken_promise](https://en.cppreference.com/w/cpp/thread/future_errc), makes the shared state *ready*, and then *releases* it.

The promise is the "**push**" end of the **promise-future communication channel**: the operation that stores a value in the **shared state** *synchronizes-with* (as defined in [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)) the successful return from any function that is waiting on the **shared state** (such as [std::future::get](https://en.cppreference.com/w/cpp/thread/future/get)). Concurrent access to the same shared state may conflict otherwise: for example multiple callers of [std::shared_future::get](https://en.cppreference.com/w/cpp/thread/shared_future/get) must either all be read-only or provide external synchronization.

> NOTE: Producer-consumer model。

### Example

This example shows how `promise<int>` can be used as signals between threads.

```C++
#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>

void accumulate(std::vector<int>::iterator first, std::vector<int>::iterator last, std::promise<int> accumulate_promise)
{
	int sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);  // Notify future
}

void do_work(std::promise<void> barrier)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	barrier.set_value();
}

int main()
{
	// Demonstrate using promise<int> to transmit a result between threads.
	std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
	std::promise<int> accumulate_promise;
	std::future<int> accumulate_future = accumulate_promise.get_future();
	std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));

	// future::get() will wait until the future has a valid result and retrieves it.
	// Calling wait() before get() is not needed
	//accumulate_future.wait();  // wait for result
	std::cout << "result=" << accumulate_future.get() << '\n';
	work_thread.join();  // wait for thread completion

	// Demonstrate using promise<void> to signal state between threads.
	std::promise<void> barrier;
	std::future<void> barrier_future = barrier.get_future();
	std::thread new_work_thread(do_work, std::move(barrier));
	barrier_future.wait();
	new_work_thread.join();
}

```


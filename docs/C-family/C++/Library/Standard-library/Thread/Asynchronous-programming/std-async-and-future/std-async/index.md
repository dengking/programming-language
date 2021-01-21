# `std::async`



## cppreference [std::async](https://en.cppreference.com/w/cpp/thread/async)

### 发展历程

> NOTE: 下面看看`std::async`的发展历程:
>
> C++11->C++17: 
>
> 使用 `std::invoke_result_t` 替换 `std::result_of_t`
>
> C++20->C++17:
>
> 添加了 `[[nodiscard]]`。

C++11

```C++
template<class Function, class ... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args> ...)>>
async(Function &&f, Args &&... args);																					(1)

template<class Function, class ... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args> ...)>>
async(std::launch policy, Function &&f, Args &&... args);                                                               (2)

```

C++17

```c++
template<class Function, class ... Args>
std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>
async(Function &&f, Args &&... args);

template<class Function, class ... Args>
std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>
async(std::launch policy, Function &&f, Args &&... args);

```

C++20

```C++
template<class Function, class ... Args>
[[nodiscard]] std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>
async(Function &&f, Args &&... args);

template<class Function, class ... Args>
[[nodiscard]] std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>
async(std::launch policy, Function &&f, Args &&... args);

```



The function template `async` runs the function `f` asynchronously (potentially in a separate thread which might be a part of a thread pool) and returns a [std::future](https://en.cppreference.com/w/cpp/thread/future) that will eventually hold the result of that function call.

1) Behaves as if (2) is called with policy being [std::launch::async](http://en.cppreference.com/w/cpp/thread/launch) | [std::launch::deferred](http://en.cppreference.com/w/cpp/thread/launch). In other words, `f` might be executed in another thread or it might be run synchronously when the resulting [std::future](https://en.cppreference.com/w/cpp/thread/future) is queried for a value.

> NOTE: asynchronous and synchronous

2) Calls a function `f` with arguments `args` according to a specific launch policy `policy`:

-- If the *async* flag is set (i.e. `(policy &` [std::launch::async](http://en.cppreference.com/w/cpp/thread/launch)) `!= 0`), then `async` executes the callable object `f` on a new thread of execution (with all thread-locals initialized) as if spawned by [std::thread](http://en.cppreference.com/w/cpp/thread/thread)([std::forward](http://en.cppreference.com/w/cpp/utility/forward)`<F>(f),` [std::forward](http://en.cppreference.com/w/cpp/utility/forward)`<Args>(args)...)`, except that if the function `f` returns a value or throws an exception, it is stored in the **shared state** accessible through the [std::future](https://en.cppreference.com/w/cpp/thread/future) that `async` returns to the caller.

-- If the *deferred* flag is set (i.e. `(policy &` [std::launch::deferred](http://en.cppreference.com/w/cpp/thread/launch)) `!= 0)`, then `async` converts `f` and `args...` the same way as by [std::thread](https://en.cppreference.com/w/cpp/thread/thread) constructor, but does not spawn a new thread of execution. Instead, *lazy evaluation* is performed: 

the first call to a non-timed wait function on the [std::future](https://en.cppreference.com/w/cpp/thread/future) that `async` returned to the caller will cause the copy of `f` to be invoked (as an rvalue) with the copies of `args...` (also passed as rvalues) in the current thread (which does not have to be the thread that originally called `std::async`). The result or exception is placed in the **shared state** associated with the `future` and only then it is made ready. All further accesses to the same [std::future](https://en.cppreference.com/w/cpp/thread/future) will return the result immediately.

-- If both the [std::launch::async](https://en.cppreference.com/w/cpp/thread/launch) and [std::launch::deferred](https://en.cppreference.com/w/cpp/thread/launch) flags are set in `policy`, it is up to the implementation whether to perform asynchronous execution or lazy evaluation.

-- If neither [std::launch::async](https://en.cppreference.com/w/cpp/thread/launch) nor [std::launch::deferred](https://en.cppreference.com/w/cpp/thread/launch), nor any implementation-defined policy flag is set in `policy`, the behavior is undefined.



In any case, the call to `std::async` *synchronizes-with* (as defined in [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)) the call to `f`, and the completion of `f` is *sequenced-before* making the **shared state** ready. If the `async` policy is chosen, the associated thread completion *synchronizes-with* the successful return from the first function that is waiting on the shared state, or with the return of the last function that releases the shared state, whichever comes first. If [std::decay](http://en.cppreference.com/w/cpp/types/decay)`<Function>::type` or each type in [std::decay](http://en.cppreference.com/w/cpp/types/decay)`<Args>::type` is not constructible from its corresponding argument, the program is ill-formed.
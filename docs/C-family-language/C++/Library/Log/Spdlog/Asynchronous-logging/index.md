# Asynchronous logging



## Data structure

```
`registry` has a `thread pool` has a `message queue`
```

关于此，参见下面的 "[2. Creating loggers#Creating asynchronous loggers](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers#creating-asynchronous-loggers)"章节。



### `registry`

```C++
class SPDLOG_API registry
{
private:
	std::shared_ptr<thread_pool> tp_;
};
```



### `thread_pool`

```C++
class SPDLOG_API thread_pool
{
	using item_type = async_msg;
	using q_type = details::mpmc_blocking_queue<item_type>;
private:
	q_type q_;

	std::vector<std::thread> threads_;
};

```



## [1. QuickStart](https://github.com/gabime/spdlog/wiki/1.-QuickStart) 



Create an asynchronous logger using factory method

```C++
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"

int main(int, char* [])
{
    try
    {        
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
        for (int i = 1; i < 101; ++i)
        {
            async_file->info("Async message #{}", i);
        }
        // Under VisualStudio, this must be called before main finishes to workaround a known VS issue
        spdlog::drop_all(); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```



## [2. Creating loggers#Creating asynchronous loggers](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers#creating-asynchronous-loggers)



```c++
#include "spdlog/async.h"
void async_example()
{
    // default thread pool settings can be modified *before* creating the async logger:
    // spdlog::init_thread_pool(8192, 1); // queue with 8k items and 1 backing thread.
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
    // alternatively:
    // auto async_file = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("async_file_logger", "logs/async_log.txt");
   
}
```

For **asynchronous logging**, spdlog uses a shared **global thread pool** with a dedicated **message queue**.

For this, it creates fixed number of **pre-allocated slots** in the **message queue** (~256 bytes per slot in 64 bits) and can be modified using `spdlog::init_thread_pool(queue_size, backing_threads_count)`

When trying to log a message and the queue is full, the caller will **block** (default behavior) until a slot becomes available (default), or **overrun** oldest message immediately in the queue with the new one (if the logger was constructed with `async_overflow_policy==overrun_oldest`).

## [6. Asynchronous logging](https://github.com/gabime/spdlog/wiki/6.-Asynchronous-logging)

### spdlog's thread pool

by default, spdlog create a global thread pool with queue size of 8192 and 1 worker thread which to serve **all** async loggers.

This means that creating and destructing **async loggers** is cheap, since they do not own nor create any backing threads or queues- these are created and managed by the **shared thread pool** object.

> NOTE: 上述所谓cheap是相对于将thread pool作为logger的member而言的

All the queue slots are **pre-allocated** on thread-pool construction (each slot occupies ~256 bytes on 64bit systems)

The thread pool size and threads can be reset by

```C++
spdlog::init_thread_pool(queue_size, n_threads);
```

#### Message ordering

By default, spdlog creates a single worker thread which preserves the order of the messages in the queue.

Please note that with thread pool with multiple workers threads, messages may be reordered after dequeuing.

If you want to keep the message order, create only one worker thread in the thread pool.



## `async.h`

Async logging using global thread pool. 

All loggers created here share same global thread pool.

Each **log message** is pushed to a queue along with a **shared pointer** to the logger.

If a logger deleted while having pending messages in the queue, it's actual destruction will defer until all its messages are processed by the thread pool. This is because each message in the queue holds a `shared_ptr` to the originating logger.




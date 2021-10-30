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
    std::unordered_map<std::string, std::shared_ptr<logger>> loggers_;
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



### `async_msg`

```C++
using async_logger_ptr = std::shared_ptr<spdlog::async_logger>;
struct async_msg : log_msg_buffer
{
    async_msg_type msg_type{async_msg_type::log};
    async_logger_ptr worker_ptr;
    
    
};
```



### `async_logger`

```C++
class SPDLOG_API async_logger final : public std::enable_shared_from_this<async_logger>, public logger
{
private:
    std::weak_ptr<details::thread_pool> thread_pool_;
};
```



```C++
// send the log message to the thread pool
SPDLOG_INLINE void spdlog::async_logger::sink_it_(const details::log_msg &msg)
{
    if (auto pool_ptr = thread_pool_.lock())
    {
        pool_ptr->post_log(shared_from_this(), msg, overflow_policy_);
    }
    else
    {
        throw_spdlog_ex("async log: thread pool doesn't exist anymore");
    }
}
```



必须要破环:

1、registry-(`shared_ptr`)->thread_pool

2、registry-(`shared_ptr`)->logger

3、async_logger-(`weak_ptr`)->thread_pool

显然，`thread_pool`的析构是不受`async_logger`控制的

4、thread_pool的message queue-(`shared_ptr`)->async_logger

显然环在3、4，因此在3中只能够使用`weak_ptr`。

当thread_pool的message queue的所有的message被处理完成了，则能够析构`async_logger`


它使用smart pointer的目的是: 

1、只有当所有的asynchronous message被处理了，才能够析构它所依赖的，asynchronous message直接依赖的就是`async_logger`，因此它有member `async_logger_ptr worker_ptr`，显然，只有当所有的asynchronous message被处理完了，才可能析构`async_logger`；

2、asynchronous message之间存在着复杂的依赖关系: 它要入queue，要有异步thread来对它进行处理；

3、为了简化它的使用，不能够让用户显式地调用`stop`函数来通知`thread_pool`

4、对于logger object，它必须要使用pointer，因为只有pointer才能够实现dynamic polymorphism，才能够一定程度上实现type erasure:

```C++
std::unordered_map<std::string, std::shared_ptr<logger>> loggers_;
```

5、按照它的这种依赖关系，什么时候才能够析构: asynchronous message都被处理了

6、`async_logger`、`thread_pool`、asynchronous message是非常典型的场景，它们之间有着依赖关系；

7、`async_logger`往`thread_pool`中`post_log`，即将消息放到队列中，是在方法: `spdlog::async_logger::sink_it_`中实现的；在`thread_pool`的实现中，会调用`spdlog::async_logger`的`backend_sink_it_`方法来处理消息；

8、`thread_pool`并不直接依赖`async_logger`，如果`async_logger`中使用`shared_ptr<async_logger>`会怎样？

9、在使用`thread_pool`的时候，是使用的`std::weak_ptr`，因此，programmer能够判断object是否还在，如果还在，则可以使用，否则就不再使用；当queue中的所有的message都处理完成后，则`async_logger`的reference counter也就归0了，因此可以安全地将它析构了。

10、如果`async_logger`中也使用`shared_ptr<thread_pool>`会发生什么事情？

11、一个误区: 包含`shared_ptr` data member的object的destructor在执行的时候，并不会理会`shared_ptr` 的target object的destruction。

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




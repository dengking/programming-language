

# Read code

## `async.h`



## [`thread_pool.h`](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/thread_pool.h) 

### `enum class async_msg_type`

```C++
/**
 * @brief 异步消息的类型，可以看出，spdlog将所有的控制都以message的方式来实现的
 *
 */
enum class async_msg_type
{
	log, flush, terminate
};

```



### `struct async_msg`

```C++
/**
 * @brief Async msg to move to/from the queue. Movable only. should never be copied
 * 其对movable only的实现是值得借鉴的
 * 
 */
struct async_msg : log_msg_buffer
{
    // should only be moved in or out of the queue..
    async_msg(const async_msg &) = delete;	
    /**
	 * @brief 第一个入参是rvalue 
     * 
     * @param worker
     * @param the_type
     * @param m
     */
    async_msg(async_logger_ptr &&worker, async_msg_type the_type, const details::log_msg &m)
};
```

### `class thread_pool`

```C++
/**
 * @brief 线程池
 *
 */
class thread_pool
{
public:
	using item_type = async_msg;
	/**
	 * 消息队列的类型
	 */
	using q_type = details::mpmc_blocking_queue<item_type>;
	/**
	 * @brief 添加一个log message到message queue中
	 *
	 * @param worker_ptr
	 * @param msg
	 * @param overflow_policy
	 */
	void post_log(async_logger_ptr &&worker_ptr, const details::log_msg &msg, async_overflow_policy overflow_policy);
	/**
	 * @brief 添加一个flush message到message queue中
	 *
	 * @param worker_ptr
	 * @param overflow_policy
	 */
	void post_flush(async_logger_ptr &&worker_ptr, async_overflow_policy overflow_policy);
private:
	/**
	 * 消息队列
	 */
	q_type q_;
	/**
	 * 所有的thread
	 */
	std::vector<std::thread> threads_;
	/**
	 * @brief 线程执行函数
	 *
	 */
	void worker_loop_();
};

```



## [`mpmc_blocking_q.h`](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/mpmc_blocking_q.h) 

### `class mpmc_blocking_queue`

```C++
#include <condition_variable>
#include <mutex>

template<typename T>
class mpmc_blocking_queue
{
public:
	/**
	 * @brief try to enqueue and block if no room left
	 *
	 * @param item
	 */
	void enqueue(T &&item);
	/**
	 * @brief enqueue immediately. overrun oldest message in the queue if no room left.
	 *
	 * @param item
	 */
	void enqueue_nowait(T &&item);
	/**
	 * @brief try to dequeue item. if no item found. wait upto timeout and try again
	 * Return true, if succeeded dequeue item, false otherwise
	 *
	 * @param popped_item
	 * @param wait_duration
	 * @return
	 */
	bool dequeue_for(T &popped_item, std::chrono::milliseconds wait_duration);
private:
	std::mutex queue_mutex_;
	std::condition_variable push_cv_;
	std::condition_variable pop_cv_;
	spdlog::details::circular_q<T> q_;
};

```



## [circular_q.h](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/circular_q.h)

在工程discrete的`Structure\Data-structure\Array\Circular-array\spdlog-circular_q`章节收录了中
# joining_thread

在阅读 [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)** 时，发现的: 

| Feature        | Supported? | Description                                      |
| -------------- | ---------- | ------------------------------------------------ |
| joining_thread | ☐          | a RAII style version of `std::thread` that joins |

这让我想起了，在destructor中，如果不显示的join thread的话，则会抛出exception，那在C++中，如何来做呢？本文对此进行讨论。



## 问题

在destructor中，如果不显示的join thread的话，则会抛出exception: terminate called without an active exception，下面文章中，对此进行了专门的介绍。



### stackoverflow [C++ terminate called without an active exception](https://stackoverflow.com/questions/7381757/c-terminate-called-without-an-active-exception)

I am getting a C++ error with threading:

```cpp
terminate called without an active exception
Aborted
```

Here is the code:

```cpp
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename TYPE>
class blocking_stream
{
public:
    blocking_stream(size_t max_buffer_size_)
        :   max_buffer_size(max_buffer_size_)   
    {
    }

    //PUSH data into the buffer
    blocking_stream &operator<<(TYPE &other)
    {
        std::unique_lock<std::mutex> mtx_lock(mtx); 
        while(buffer.size()>=max_buffer_size)
            stop_if_full.wait(mtx_lock);

        buffer.push(std::move(other));

        mtx_lock.unlock();
        stop_if_empty.notify_one();
        return *this;
    }
    //POP data out of the buffer 
    blocking_stream &operator>>(TYPE &other)
    {
        std::unique_lock<std::mutex> mtx_lock(mtx);
        while(buffer.empty())
            stop_if_empty.wait(mtx_lock);

        other.swap(buffer.front()); 
        buffer.pop();

        mtx_lock.unlock();
        stop_if_full.notify_one();
        return *this;
    }

private:
    size_t max_buffer_size;
    std::queue<TYPE> buffer;
    std::mutex mtx;
    std::condition_variable stop_if_empty,
                            stop_if_full;
    bool eof;   
};
```

I modeled my code around this example: http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

What am I doing wrong and how do I fix the error?

#### [A](https://stackoverflow.com/a/7989043)

When a thread object goes out of scope and it is in joinable state, the program is terminated. The Standard Committee had two other options for the destructor of a joinable thread. It could quietly join -- but join might never return if the thread is stuck. Or it could detach the thread (a detached thread is not joinable). However, detached threads are very tricky, since they might survive till the end of the program and mess up the release of resources. So if you don't want to terminate your program, make sure you join (or detach) every thread.



#### [A](https://stackoverflow.com/a/21978054)

**How to reproduce that error:**

```cpp
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
using namespace std;
void task1(std::string msg){
  cout << "task1 says: " << msg;
}
int main() { 
  std::thread t1(task1, "hello"); 
  return 0;
}
```

Compile and run:

```cpp
el@defiant ~/foo4/39_threading $ g++ -o s s.cpp -pthread -std=c++11
el@defiant ~/foo4/39_threading $ ./s
terminate called without an active exception
Aborted (core dumped)
```

You get that error because you didn't join or detach your thread.

**One way to fix it, join the thread like this:**

```cpp
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
using namespace std;
void task1(std::string msg){
  cout << "task1 says: " << msg;
}
int main() { 
  std::thread t1(task1, "hello"); 
  t1.join();
  return 0;
}
```

Then compile and run:

```cpp
el@defiant ~/foo4/39_threading $ g++ -o s s.cpp -pthread -std=c++11
el@defiant ~/foo4/39_threading $ ./s
task1 says: hello
```

**The other way to fix it, detach it like this:**

```cpp
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <thread>
using namespace std;
void task1(std::string msg){
  cout << "task1 says: " << msg;
}
int main() 
{ 
     {

        std::thread t1(task1, "hello"); 
        t1.detach();

     } //thread handle is destroyed here, as goes out of scope!

     usleep(1000000); //wait so that hello can be printed.
}
```

Compile and run:

```cpp
el@defiant ~/foo4/39_threading $ g++ -o s s.cpp -pthread -std=c++11
el@defiant ~/foo4/39_threading $ ./s
task1 says: hello
```

Read up on detaching C++ threads and joining C++ threads.



### TODO

https://exceptionshub.com/c-terminate-called-without-an-active-exception.html



## How to do?



### stackoverflow [Is it a good idea to shut down a class's thread member in the class's destructor?](https://stackoverflow.com/questions/11223119/is-it-a-good-idea-to-shut-down-a-classs-thread-member-in-the-classs-destructor)



[A](https://stackoverflow.com/a/11269932)

> NOTE: 非常好的回答，它给出的solution code可以作为non-throw destructor的典范。

It is a good idea to release resources a class creates when the class is destroyed, even if one of the resources is a thread. If the resource is created explicitly via a user call, such as `Worker::Start()`, then there should also be an explicit way to release it, such as `Worker::Stop()`. It would also be a good idea to either perform cleanup in the destructor in the event that the user does not call `Worker::Stop()` and/or provide the user a scoped helper class that implements the [RAII](http://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)-idiom, invoking `Worker::Start()` in its constructor and `Worker::Stop()` in its destructor. However, if resource allocation is done implicitly, such as in the `Worker` constructor, then the release of the resource should also be implicit, leaving the destructor as the prime candidate for this responsibility.

------

#### Destruction

Lets examine `Worker::~Worker()`. A general rule is to [not throw exceptions in destructors](http://www.parashift.com/c++-faq/dtors-shouldnt-throw.html). If a `Worker` object is on a stack that is unwinding from another exception, and `Worker::~Worker()` throws an exception, then `std::terminate()` will be invoked, killing the application. While `Worker::~Worker()` is not explicitly throwing an exception, it is important to consider that some of the functions it is invoking may throw:

- [`m_Condition.notify_one()`](http://www.boost.org/doc/libs/1_50_0/doc/html/thread/synchronization.html#thread.synchronization.condvar_ref.condition_variable.notify_one) does not throw.
- [`m_pThread->join()`](http://www.boost.org/doc/libs/1_50_0/doc/html/thread/thread_management.html#thread.thread_management.thread.join) could throw `boost::thread_interrupted`.

If `std::terminate()` is the desired behavior, then no change is required. However, if `std::terminate()` is not desired, then catch `boost::thread_interrupted` and suppress it.

```
Worker::~Worker()
{
  m_Running = false;
  m_Condition.notify_one();
  try
  {
    m_pThread->join();
  }
  catch ( const boost::thread_interrupted& )
  {
    /* suppressed */ 
  }
}
```

------

#### Concurrency

Managing threading can be tricky. It is important to define the exact desired behavior of functions like `Worker::Wake()`, as well as understand the behavior of the types that facilitate threading and synchronization. For example, [`boost::condition_variable::notify_one()`](http://www.boost.org/doc/libs/1_50_0/doc/html/thread/synchronization.html#thread.synchronization.condvar_ref.condition_variable.notify_one) has no effect if no threads are blocked in `boost::condition_variable::wait()`. Lets examine the possible concurrent paths for `Worker::Wake()`.

Below is a crude attempt to diagram concurrency for two scenarios:

- Order-of-operation occurs from top-to-bottom. (i.e. Operations at the top occur before operations at the bottom.
- Concurrent operations are written on the same line.
- `<` and `>` are used to highlight when one thread is waking up or unblocking another thread. For example `A > B` indicates that thread `A` is unblocking thread `B`.

**Scenario**: `Worker::Wake()` invoked while `Worker::ThreadProc()` is blocked on `m_Condition`.

```
Other Thread                       | Worker::ThreadProc
-----------------------------------+------------------------------------------
                                   | lock( m_Mutex )
                                   | `-- m_Mutex.lock()
                                   | m_Condition::wait( lock )
                                   | |-- m_Mutex.unlock()
                                   | |-- waits on notification
Worker::Wake()                     | |
|-- lock( m_Mutex )                | |
|   `-- m_Mutex.lock()             | |
|-- m_Condition::notify_one()      > |-- wakes up from notification
`-- ~lock()                        | `-- m_Mutex.lock() // blocks
    `-- m_Mutex.unlock()           >     `-- // acquires lock
                                   | // do some work here
                                   | ~lock() // end of for loop's scope
                                   | `-- m_Mutex.unlock()
```

**Result**: `Worker::Wake()` returns fairly quickly, and `Worker::ThreadProc` runs.

------

**Scenario**: `Worker::Wake()` invoked while `Worker::ThreadProc()` is not blocked on `m_Condition`.

```
Other Thread                       | Worker::ThreadProc
-----------------------------------+------------------------------------------
                                   | lock( m_Mutex )
                                   | `-- m_Mutex.lock()
                                   | m_Condition::wait( lock )
                                   | |-- m_Mutex.unlock()
Worker::Wake()                     > |-- wakes up
                                   | `-- m_Mutex.lock()
Worker::Wake()                     | // do some work here
|-- lock( m_Mutex )                | // still doing work...
|   |-- m_Mutex.lock() // block    | // hope we do not block on a system call
|   |                              | // and more work...
|   |                              | ~lock() // end of for loop's scope
|   |-- // still blocked           < `-- m_Mutex.unlock()
|   `-- // acquires lock           | lock( m_Mutex ) // next 'for' iteration.
|-- m_Condition::notify_one()      | `-- m_Mutex.lock() // blocked
`-- ~lock()                        |     |-- // still blocked
    `-- m_Mutex.unlock()           >     `-- // acquires lock
                                   | m_Condition::wait( lock )    
                                   | |-- m_Mutex.unlock()
                                   | `-- waits on notification
                                   |     `-- still waiting...
```

**Result**: `Worker::Wake()` blocked as `Worker::ThreadProc` did work, but was a no-op, as it sent a notification to `m_Condition` when no one was waiting on it.

This is not particularly dangerous for `Worker::Wake()`, but it can cause problems in `Worker::~Worker()`. If `Worker::~Worker()` runs while `Worker::ThreadProc` is doing work, then `Worker::~Worker()` may block indefinitely when joining the thread, as the thread may not be waiting on `m_Condition` at the point in which it is notified, and `Worker::ThreadProc` only checks `m_Running` after it is done waiting on `m_Condition`.

------

#### Working Towards a Solution

In this example, lets define the following requirements:

- `Worker::~Worker()` will not cause `std::terminate()` to be invoked.
- `Worker::Wake()` will not block while `Worker::ThreadProc` is doing work.
- If `Worker::Wake()` is called while `Worker::ThreadProc` is not doing work, then it will notify `Worker::ThreadProc` to do work.
- If `Worker::Wake()` is called while `Worker::ThreadProc` is doing work, then it will notify `Worker::ThreadProc` to perform another iteration of work.
- Multiple calls to `Worker::Wake()` while `Worker::ThreadProc` is doing work will result in `Worker::ThreadProc` performing a single additional iteration of work.

Code:

```c++
#include <boost/thread.hpp>
 
class Worker
{
public:
  Worker();
  ~Worker();
  void Wake();
private:
  Worker(Worker const& rhs);             // prevent copying
  Worker& operator=(Worker const& rhs);  // prevent assignment
  void ThreadProc();
 
  enum state { HAS_WORK, NO_WORK, SHUTDOWN };
  
  state                            m_State;
  boost::mutex                     m_Mutex;
  boost::condition_variable        m_Condition;
  boost::thread                    m_Thread;
};
 
Worker::Worker()
  : m_State(NO_WORK)
  , m_Mutex()
  , m_Condition()
  , m_Thread()
{
  m_Thread = boost::thread(&Worker::ThreadProc, this);
}
 
Worker::~Worker()
{
  // Create scope so that the mutex is only locked when changing state and
  // notifying the condition.  It would result in a deadlock if the lock was
  // still held by this function when trying to join the thread.
  {
    boost::lock_guard<boost::mutex> lock(m_Mutex);
    m_State = SHUTDOWN;
    m_Condition.notify_one();
  }
  try { m_Thread.join(); }
  catch ( const boost::thread_interrupted& ) { /* suppress */ };
}
 
void Worker::Wake()
{
  boost::lock_guard<boost::mutex> lock(m_Mutex);
  m_State = HAS_WORK;
  m_Condition.notify_one();
}
 
void Worker::ThreadProc()
{
  for (;;)
  {
    // Create scope to only lock the mutex when checking for the state.  Do
    // not continue to hold the mutex wile doing busy work.
    {
      boost::unique_lock<boost::mutex> lock(m_Mutex);
      // While there is no work (implies not shutting down), then wait on
      // the condition.
      while (NO_WORK == m_State)
      {
        m_Condition.wait(lock);
        // Will wake up from either Wake() or ~Worker() signaling the condition
        // variable.  At that point, m_State will either be HAS_WORK or
        // SHUTDOWN.
      }
      // On shutdown, break out of the for loop.
      if (SHUTDOWN == m_State) break;
      // Set state to indicate no work is queued.
      m_State = NO_WORK;
    }
 
    // do some work here
  }
}
```

Note: As a personal preference, I opted to not allocated `boost::thread` on the heap, and as a result, I do not need to manage it via `boost::scoped_ptr`. `boost::thread` has a [default constructor](http://www.boost.org/doc/libs/1_50_0/doc/html/thread/thread_management.html#thread.thread_management.thread.default_constructor) that will refer to *Not-a-Thread*, and it is [move-assignable](http://www.boost.org/doc/libs/1_50_0/doc/html/thread/thread_management.html#thread.thread_management.thread.move_assignment).



### Examples

```C++
CHQImpl::~CHQImpl()
{

	if (m_StartHqClientThread.joinable())
	{
		m_StartHqClientThread.join();
 	}

}
```



### leimao.github [Ensure Join or Detach Before Thread Destruction in C++](https://leimao.github.io/blog/CPP-Ensure-Join-Detach-Before-Thread-Destruction/)



## C++ Core Guidelines



### modernescpp [C++ Core Guidelines: Taking Care of your Child Thread](https://www.modernescpp.com/index.php/c-core-guidelines-taking-care-of-your-child-thread)

> NOTE: 解释了C++ Core Guidelines中，关于thread的内容

When you create a new child thread, you have to answer an important question: should you wait for the child or detach yourself from it? If you detach yourself from the newly created child, and your child uses variables which are bound to your lifetime as creator a new question arises: Will the variables stay valid during the lifetime of the child thread?

If you don't carefully handle the lifetime and the variables of your child thread, you will end with high probability in undefined behaviour.

> NOTE: 上述问题描述的非常好

### csdn [C++核心准则：CP.25: gsl::joining_thread好于std::thread](https://blog.csdn.net/craftsman1970/article/details/108295078)



## Implementation



### `gsl::joining_thread`

#### github [Implement joining_thread #808](https://github.com/microsoft/GSL/pull/808)



### [jthread](https://en.cppreference.com/w/cpp/thread/jthread) (C++20)
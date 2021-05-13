# C++11 read-write lock implementation

C++11标准库并没有提供read-write lock ，那如何来实现呢？

## 算法

当可以读的时候，需要通知等待的reader；当不能读的时候，需要等待；

当可以写的时候，需要通知等待的write；当不能写的时候，需要等待；

因此，它需要使用两个condition variable；因此它的实现，非常类似于spdlog 中的 MPMC blocking queue；

## Implementation



### stackoverflow [How would you implement your own reader/writer lock in C++11?](https://stackoverflow.com/questions/12033188/how-would-you-implement-your-own-reader-writer-lock-in-c11)

[A](https://stackoverflow.com/a/12657243)

> NOTE: 这个回答给出了分析过程，非常好；

```C++
read_lock() {
  mutex.lock();
  while (writer)
    unlocked.wait(mutex);
  readers++;
  mutex.unlock();
}

read_unlock() {
  mutex.lock();
  readers--;
  if (readers == 0)
    unlocked.signal_all();
  mutex.unlock();
}

write_lock() {
  mutex.lock();
  while (writer || (readers > 0))
    unlocked.wait(mutex);
  writer = true;
  mutex.unlock();
}

write_unlock() {
  mutex.lock();
  writer = false;
  unlocked.signal_all();
  mutex.unlock();
}
```



### stackoverflow [Reader/Writer Locks in C++](https://stackoverflow.com/questions/244316/reader-writer-locks-in-c)

[A](https://stackoverflow.com/a/244344)

Newer versions of [boost::thread](http://www.boost.org/doc/libs/1_50_0/doc/html/thread.html) have read/write locks (1.35.0 and later, apparently the previous versions did not work correctly).

[A](https://stackoverflow.com/a/244376)

> NOTE: 下面给出的实现是非常典型的使用两个condition variable的实现；

Using standard pre-tested, pre-built stuff is always good (for example, Boost as another answer suggested), but this is something that's not too hard to build yourself. Here's a dumb little implementation pulled out from a project of mine:

```cpp
#include <pthread.h>

struct rwlock {
    pthread_mutex_t lock;
    pthread_cond_t read, write;
    unsigned readers, writers, read_waiters, write_waiters;
};

void reader_lock(struct rwlock *self) {
    pthread_mutex_lock(&self->lock);
    if (self->writers || self->write_waiters) {
        self->read_waiters++;
        do pthread_cond_wait(&self->read, &self->lock);
        while (self->writers || self->write_waiters);
        self->read_waiters--;
    }
    self->readers++;
    pthread_mutex_unlock(&self->lock);
}

void reader_unlock(struct rwlock *self) {
    pthread_mutex_lock(&self->lock);
    self->readers--;
    if (self->write_waiters)
        pthread_cond_signal(&self->write);
    pthread_mutex_unlock(&self->lock);
}

void writer_lock(struct rwlock *self) {
    pthread_mutex_lock(&self->lock);
    if (self->readers || self->writers) {
        self->write_waiters++;
        do pthread_cond_wait(&self->write, &self->lock);
        while (self->readers || self->writers);
        self->write_waiters--;
    }
    self->writers = 1;
    pthread_mutex_unlock(&self->lock);
}

void writer_unlock(struct rwlock *self) {
    pthread_mutex_lock(&self->lock);
    self->writers = 0;
    if (self->write_waiters)
        pthread_cond_signal(&self->write);
    else if (self->read_waiters)
        pthread_cond_broadcast(&self->read);
    pthread_mutex_unlock(&self->lock);
}

void rwlock_init(struct rwlock *self) {
    self->readers = self->writers = self->read_waiters = self->write_waiters = 0;
    pthread_mutex_init(&self->lock, NULL);
    pthread_cond_init(&self->read, NULL);
    pthread_cond_init(&self->write, NULL);
}
```

`pthreads` not really being Windows-native, but the general idea is here. This implementation is slightly biased towards writers (a horde of writers can starve readers indefinitely); just modify `writer_unlock` if you'd rather the balance be the other way around.

Yes, this is C and not C++. Translation is an exercise left to the reader.

#### Edit

Greg Rogers pointed out that the POSIX standard does specify `pthread_rwlock_*`. This doesn't help if you don't have `pthreads`, but it stirred my mind into remembering: [Pthreads-w32](http://sourceware.org/pthreads-win32/) should work! Instead of porting this code to non-`pthreads` for your own use, just use Pthreads-w32 on Windows, and native `pthreads` everywhere else.





### stackexchange [Simple rwlock implementation in c++11](https://codereview.stackexchange.com/questions/201556/simple-rwlock-implementation-in-c11)

For answering this question of stackoverflow I have written the code https://stackoverflow.com/questions/12033188/how-would-you-implement-your-own-reader-writer-lock-in-c11

Can someone review it - so that I can understand the possible problems in the code.

> NOTE: 下面给出的实现是非常典型的使用两个condition variable的实现；

```cpp
#include <condition_variable>
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <unistd.h>
#define NR_THREADS 10
#include <mutex>

class MySharedLock {
 public:
  void read_lock() {
    std::unique_lock<std::mutex> lk(rw_mutex);
    std::cout << "\nReader Lock Writers are  " << writers << std::flush;
    if (writers != 0) {
      rw_cv.wait(lk, [this]() { return (this->writers == 0); });
    }
    readers++;
    lk.unlock();
  }

  void write_lock() {
    std::unique_lock<std::mutex> lk(rw_mutex);

    std::cout << "\nWriter Lock Writers are  " << writers << " Readers are "
              << readers << std::flush;
    if (readers == 0 && writers == 0) {
      std::cout << "\nWriter Lock Writers are  " << writers << std::flush;
    } else {
      rw_cv.wait(
          lk, [this]() { return (this->writers == 0 && this->readers == 0); });
    }
    writers++;
    lk.unlock();

  }

  void write_unlock() {
    std::lock_guard<std::mutex> lk(rw_mutex);
    writers--;
    rw_cv.notify_all();
  }

  void read_unlock() {
    std::lock_guard<std::mutex> lk(rw_mutex);
    if (readers == 1) {  // I am the last one.
      rw_cv.notify_all();
    }
    readers--;
  }

  explicit MySharedLock() {}

 private:
  std::mutex rw_mutex;
  std::condition_variable rw_cv;
  uintmax_t readers = {0}, writers = {0};
};
```



### stackoverflow [C++11 equivalent to boost shared_mutex](https://stackoverflow.com/questions/14306797/c11-equivalent-to-boost-shared-mutex)

[A](https://stackoverflow.com/a/14307116)

I tried but failed to get `shared_mutex` into C++11. It has been proposed for a future standard. The proposal is [here](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3427.html).

**Edit**: A revised version (N3659) [was accepted](http://isocpp.org/blog/2013/04/n3659-shared-locking) for C++14.

Here is an implementation:

http://howardhinnant.github.io/shared_mutex

http://howardhinnant.github.io/shared_mutex.cpp



### stackoverflow [A RW lock for c++11 threads [duplicate]](https://stackoverflow.com/questions/16774469/a-rw-lock-for-c11-threads)



### [preshing](https://github.com/preshing)/**[cpp11-on-multicore](https://github.com/preshing/cpp11-on-multicore)**/[common](https://github.com/preshing/cpp11-on-multicore/tree/master/common)/[rwlock.h](https://github.com/preshing/cpp11-on-multicore/blob/master/common/rwlock.h)

> NOTE: 算法都是相同的，只是它使用的了更加成熟的写法、实现，因此，推荐使用它；
>
> 在工程parallel-computing中，收录了这个实现；
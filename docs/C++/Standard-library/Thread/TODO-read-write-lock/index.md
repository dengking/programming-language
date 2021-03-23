# Read Writ Lock

## 标准库的总结

参见cppreference [Thread support library](https://en.cppreference.com/w/cpp/thread)

### mutex

| API                                                          | 说明                                                         |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| [shared_mutex](https://en.cppreference.com/w/cpp/thread/shared_mutex)(C++17) | provides shared mutual exclusion facility                    |      |
| [shared_timed_mutex](https://en.cppreference.com/w/cpp/thread/shared_timed_mutex)(C++14) | provides shared mutual exclusion facility and implements locking with a timeout |      |

### lock

|            | API                                                          | 说明                                                      |
| ---------- | ------------------------------------------------------------ | --------------------------------------------------------- |
| write lock | [lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)(C++11) | implements a strictly scope-based mutex ownership wrapper |
|            | [unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)(C++11) | implements movable mutex ownership wrapper                |
| read lock  | [shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)(C++14) | implements movable shared mutex ownership wrapper         |



## cppreference [std::shared_mutex](https://en.cppreference.com/w/cpp/thread/shared_mutex)

```C++
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
 
class ThreadSafeCounter {
 public:
  ThreadSafeCounter() = default;
 
  // Multiple threads/readers can read the counter's value at the same time.
  unsigned int get() const {
    std::shared_lock lock(mutex_);
    return value_;
  }
 
  // Only one thread/writer can increment/write the counter's value.
  void increment() {
    std::unique_lock lock(mutex_);
    value_++;
  }
 
  // Only one thread/writer can reset/write the counter's value.
  void reset() {
    std::unique_lock lock(mutex_);
    value_ = 0;
  }
 
 private:
  mutable std::shared_mutex mutex_;
  unsigned int value_ = 0;
};
 
int main() {
  ThreadSafeCounter counter;
 
  auto increment_and_print = [&counter]() {
    for (int i = 0; i < 3; i++) {
      counter.increment();
      std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';
 
      // Note: Writing to std::cout actually needs to be synchronized as well
      // by another std::mutex. This has been omitted to keep the example small.
    }
  };
 
  std::thread thread1(increment_and_print);
  std::thread thread2(increment_and_print);
 
  thread1.join();
  thread2.join();
}
```





## Example

### modernescpp [Reader-Writer Locks](https://www.modernescpp.com/index.php/reader-writer-locks)

> NOTE: 使用C++14

With C++14 came reader-writer locks. The idea is straightforward and promising. Arbitrary reading threads can access the critical region at the same time, but only one thread is allowed to write.

### Minimized bottleneck

Reader-writer locks do not solve the fundamental problem - threads competing for access to a critical region. But reader-writer locks help a lot - to minimize the bottleneck. Let's have an example.

```C++
// readerWriterLock.cpp

#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

std::map<std::string,int> teleBook{{"Dijkstra",1972},{"Scott",1976},{"Ritchie",1983}};

std::shared_timed_mutex teleBookMutex;

void addToTeleBook(const std::string& na, int tele){
  std::lock_guard<std::shared_timed_mutex> writerLock(teleBookMutex);
  std::cout << "\nSTARTING UPDATE " << na;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  teleBook[na]= tele;
  std::cout << " ... ENDING UPDATE " << na << std::endl;
}

void printNumber(const std::string& na){
  std::shared_lock<std::shared_timed_mutex> readerLock(teleBookMutex);
  std::cout << na << ": " << teleBook[na];
}

int main(){

  std::cout << std::endl;

  std::thread reader1([]{ printNumber("Scott"); });
  std::thread reader2([]{ printNumber("Ritchie"); });
  std::thread w1([]{ addToTeleBook("Scott",1968); });
  std::thread reader3([]{ printNumber("Dijkstra"); });
  std::thread reader4([]{ printNumber("Scott"); });
  std::thread w2([]{ addToTeleBook("Bjarne",1965); });
  std::thread reader5([]{ printNumber("Scott"); });
  std::thread reader6([]{ printNumber("Ritchie"); });
  std::thread reader7([]{ printNumber("Scott"); });
  std::thread reader8([]{ printNumber("Bjarne"); });

  reader1.join();
  reader2.join();
  reader3.join();
  reader4.join();
  reader5.join();
  reader6.join();
  reader7.join();
  reader8.join();
  w1.join();
  w2.join();

  std::cout << std::endl;

  std::cout << "\nThe new telephone book" << std::endl;
  for (auto teleIt: teleBook){
    std::cout << teleIt.first << ": " << teleIt.second << std::endl;
  }

  std::cout << std::endl;

}
```


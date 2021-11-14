# base/threading

```C++

// A namespace for low-level thread functions.
class BASE_EXPORT PlatformThread {
 public:
  // Implement this interface to run code on a background thread.  Your
  // ThreadMain method will be called on the newly created thread.
  class BASE_EXPORT Delegate {
   public:
    // The interval at which the thread expects to have work to do. Zero if
    // unknown. (Example: audio buffer duration for real-time audio.) Is used to
    // optimize the thread real-time behavior. Is called on the newly created
    // thread before ThreadMain().
    virtual TimeDelta GetRealtimePeriod();

    virtual void ThreadMain() = 0;

   protected:
    virtual ~Delegate() = default;
  };
};

class BASE_EXPORT Thread : PlatformThread::Delegate {};

```

## `PlatformThread`

`class BASE_EXPORT PlatformThread` 中，定义了delegate，它定义了 `ThreadMain` interface。

`PlatformThread` 定义了创建thread、操作thread的interface，它会将这些请求dispatch到各个plateform的implementation。

可以看到，它的很多方法，比如create系列方法的入参都是 `Delegate*`，它主要是用来传递 `ThreadMain()` 的；

需要注意的是: `PlatformThread` 的大多数方法是`static`的，它是 `DISALLOW_IMPLICIT_CONSTRUCTORS(PlatformThread);` 的。



## `Thread`

### `Delegate`、`delegate_` 成员变量



```C++
// The thread's Delegate and RunLoop are valid only while the thread is
// alive. Set by the created thread.
std::unique_ptr<Delegate> delegate_;
```

在[`thread.cc`](https://github.com/chromium/chromium/blob/master/base/threading/thread.cc) 中，提供了 `Thread::Delegate` 的 一个implementation:

```C++
class SequenceManagerThreadDelegate : public Thread::Delegate {
}
```



## bind、unbound

### github [threading_and_tasks.md](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)

> `base::RunLoop`: Drive the `SequenceManager` from the thread it's bound to.


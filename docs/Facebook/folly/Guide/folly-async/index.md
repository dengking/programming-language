# folly async

future、executor、eventbase

## [AsyncSocketBase](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncSocketBase.h)

是interface

```C++
virtual EventBase* getEventBase() const = 0;
```

显然它有一个成员叫做 `EventBase`。



## [`EventBase`](https://github.com/facebook/folly/blob/main/folly/io/async/EventBase.h)

它其实是event loop。

> This class is a wrapper for all asynchronous I/O processing functionality



> A EventBase object can only drive an event loop for a single thread.

这段话是非常重要的。

## [EventHandler](https://github.com/facebook/folly/blob/main/folly/io/async/EventHandler.h)

参见 [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/**[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md)**

它需要 attach 到一个EventBase中，因为:

1、它有如下成员变量:

```C++
EventBase* eventBase_;
```

2、它有如下成员方法:

```C++
void attachEventBase(EventBase* eventBase);
void detachEventBase();
```



## 思考

### Socket 和 EventBase 之间的关系

1、socket可以属于一个event loop

参见 [AsyncSocketBase](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncSocketBase.h) ，其中有如下接口:

```
virtual EventBase* getEventBase() const = 0;
```





2、一个event loop可以有多个socket。

### EventBase 和 thread 之间的关系

1、[`EventBase`](https://github.com/facebook/folly/blob/main/folly/io/async/EventBase.h)

> A EventBase object can only drive an event loop for a single thread.

2、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/**[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md)**



```C++
EventBase base;
auto thread = std::thread([&](){
  base.loopForever();
});
```



## Callback-based notification

是基于callback的，在 [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[io](https://github.com/facebook/folly/tree/main/folly/io)/[async](https://github.com/facebook/folly/tree/main/folly/io/async)/**[AsyncTransport.h](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncTransport.h)** 中，有如下的定义:

```C++
class AsyncReader {
 public:
  class ReadCallback {
  }
};


class AsyncWriter {
 public:
  class ReleaseIOBufCallback {
   public:
    virtual ~ReleaseIOBufCallback() = default;

    virtual void releaseIOBuf(std::unique_ptr<folly::IOBuf>) noexcept = 0;
  };

  class WriteCallback {
  }
};
```


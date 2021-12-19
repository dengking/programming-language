# `folly::IOBuf`

1、`IOBuf object` 相当于`shared_ptr`，它指向`data buffer`



## research.fb [Under the Hood: Building and open-sourcing fbthrift](https://research.fb.com/blog/2014/02/under-the-hood-building-and-open-sourcing-fbthrift/)



To improve **asynchronous workload performance**, we updated the base **Thrift transport** to be [folly’s IOBuf](https://github.com/facebook/folly/blob/main/folly/io/IOBuf.h) class, a chained memory buffer with views similar to BSD’s `mbuf` or Linux’s `sk_buff`. In earlier versions of Thrift, the same memory buffer was reused for all requests, but memory management quickly became tricky to use when we tried to update the buffer to send responses out of order. Instead, we now request new buffers from the memory allocator on every request. To reduce the performance impact of allocating new buffers, we allocate constant-sized buffers from `JEMalloc` to hit the **thread-local buffer cache** as often as possible. Hitting the **thread-local cache** was an impressive performance improvement — for the average Thrift server, it’s just as fast as **reusing** or **pooling** buffers, without any of the complicated code. These buffers are then chained together to become as large as needed, and freed when not needed, preventing some memory issues seen in previous Thrift servers where memory was pooled indefinitely(不确定的). In order to support these chained buffers, all of the existing Thrift protocols had to be rewritten.

> NOTE: 
>
> 一、之前是"the same memory buffer was reused for all requests"，现在是: "now request new buffers from the memory allocator on every request"
>
> 二、上面解释了memory management的一些技巧: "To reduce the performance impact of allocating new buffers, we allocate constant-sized buffers from `JEMalloc` to hit the **thread-local buffer cache** as often as possible. Hitting the **thread-local cache** was an impressive performance improvement — for the average Thrift server, it’s just as fast as **reusing** or **pooling** buffers, without any of the complicated code. "
>
> 



## Data Layout

```
 * The IOBuf itself is a small object containing a pointer to the buffer and
 * information about which segment of the buffer contains valid data.
 *
 * The data layout looks like this:
 *
 *  +-------+
 *  | IOBuf |
 *  +-------+
 *   /
 *  |
 *  v
 *  +------------+--------------------+-----------+
 *  | headroom   |        data        |  tailroom |
 *  +------------+--------------------+-----------+
 *  ^            ^                    ^           ^
 *  buffer()   data()               tail()      bufferEnd()
 *
 *  The length() method returns the length of the valid data; capacity()
 *  returns the entire capacity of the buffer (from buffer() to bufferEnd()).
 *  The headroom() and tailroom() methods return the amount of unused capacity
 *  available before and after the data.
```



> For this reason, `prependChain()` and `appendChain()` take ownership of the new elements being added to this chain.

上面这段话要如何理解？

## 数据成员

```C++
class IOBuf
{
  /*
   * Member variables
   */

  /*
   * Links to the next and the previous IOBuf in this chain.
   *
   * The chain is circularly linked (the last element in the chain points back
   * at the head), and next_ and prev_ can never be null.  If this IOBuf is the
   * only element in the chain, next_ and prev_ will both point to this.
   */
  IOBuf* next_{this};
  IOBuf* prev_{this};

  /*
   * A pointer to the start of the data referenced by this IOBuf, and the
   * length of the data.
   *
   * This may refer to any subsection of the actual buffer capacity.
   */
  uint8_t* data_{nullptr};
  uint8_t* buf_{nullptr};
  std::size_t length_{0};
  std::size_t capacity_{0};

  // Pack flags in least significant 2 bits, sharedInfo in the rest
  uintptr_t flagsAndSharedInfo_{0};
};
```

上面注释中，对 `data_` 进行了解释。

## `struct SharedInfo` reference count

## 使用场景、如何使用

### Socket

folly的IO的buffer都是建立于`IOBuf`上的。

1、application protocol: socket接受到的二进制data，在应用层需要进行decode为应用层的协议。

```
.h C:\Users\Kai\Desktop\share\folly-main\folly-main\ content:IOBuf 
```

2、每个socket都有一个receive queue，一个send queue，显然它们是可以使用`IOBuf`来实现的

3、思考: 需要由谁来创建`IOBuf`并将它传入到`Socket`？

#### [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[io](https://github.com/facebook/folly/tree/main/folly/io)/[async](https://github.com/facebook/folly/tree/main/folly/io/async)/**[AsyncTransport.h](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncTransport.h)**





#### [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[io](https://github.com/facebook/folly/tree/main/folly/io)/[async](https://github.com/facebook/folly/tree/main/folly/io/async)/[`AsyncSocket.h`](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncSocket.h)

```C++
  // Pre-received data, to be returned to read callback before any data from the
  // socket.
  std::unique_ptr<IOBuf> preReceivedData_;
```

```C++
  // a folly::IOBuf can be used in multiple partial requests
  // there is a that maps a buffer id to a raw folly::IOBuf ptr
  // and another one that adds a ref count for a folly::IOBuf that is either
  // the original ptr or nullptr
  uint32_t zeroCopyBufId_{0};

  struct IOBufInfo {
    uint32_t count_{0};
    ReleaseIOBufCallback* cb_{nullptr};
    std::unique_ptr<folly::IOBuf> buf_;
  };

  std::unordered_map<uint32_t, folly::IOBuf*> idZeroCopyBufPtrMap_;
  std::unordered_map<folly::IOBuf*, IOBufInfo> idZeroCopyBufInfoMap_;
  
```

#### [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[io](https://github.com/facebook/folly/tree/main/folly/io)/[async](https://github.com/facebook/folly/tree/main/folly/io/async)/**[AsyncUDPSocket.h](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncUDPSocket.h)**

```C++
uint32_t zeroCopyBufId_{0};
std::unordered_map<uint32_t, std::unique_ptr<folly::IOBuf>> idZeroCopyBufMap_;

```



### `File.h`

```
/**
 * Generator that reads from a file with a buffer of the given size.
 * Reads must be buffered (the generator interface expects the generator
 * to hold each value).
 */
template <class S = detail::FileReader>
S fromFile(File file, size_t bufferSize = 4096) {
  return S(std::move(file), IOBuf::create(bufferSize));
}
```

## IOBufQueue 

https://gitee.com/mirrors/folly/blob/main/folly/io/IOBufQueue.h

```
 * An IOBufQueue encapsulates a chain of IOBufs and provides
 * convenience functions to append data to the back of the chain
 * and remove data from the front.
```

## `TypedIOBuf`



## Example code

vimsky [C++ folly::IOBuf方法代码示例](https://vimsky.com/examples/detail/cpp-ex---folly-IOBuf-method.html)

hotexamples [C++ (Cpp) IOBuf Examples](https://cpp.hotexamples.com/examples/folly/IOBuf/-/cpp-iobuf-class-examples.html)

GitHub [facebook](https://github.com/facebook)/**[mcrouter](https://github.com/facebook/mcrouter)**

GitHub  [facebook](https://github.com/facebook)/**[fbthrift](https://github.com/facebook/fbthrift)**



## 如何实现zero copy

在 `AsyncSocket.h` 中，搜索`zero`，可以查到很多的关于zero-copy的内容。

## `IOBuf`
### `FBIOBuf`
https://research.fb.com/blog/2014/02/under-the-hood-building-and-open-sourcing-fbthrift/

To improve **asynchronous workload performance**, we updated the base **Thrift transport** to be [folly’s IOBuf](https://github.com/facebook/folly/blob/main/folly/io/IOBuf.h) class, a chained memory buffer with views similar to BSD’s `mbuf` or Linux’s `sk_buff`. In earlier versions of Thrift, the same memory buffer was reused for all requests, but memory management quickly became tricky to use when we tried to update the buffer to send responses out of order. Instead, we now request new buffers from the memory allocator on every request. To reduce the performance impact of allocating new buffers, we allocate constant-sized buffers from JEMalloc to hit the **thread-local buffer cache** as often as possible. Hitting the **thread-local cache** was an impressive performance improvement — for the average Thrift server, it’s just as fast as **reusing** or **pooling** buffers, without any of the complicated code. These buffers are then chained together to become as large as needed, and freed when not needed, preventing some memory issues seen in previous Thrift servers where memory was pooled indefinitely(不确定的). In order to support these chained buffers, all of the existing Thrift protocols had to be rewritten.

> NOTE: 之前是"the same memory buffer was reused for all requests"，现在是: "now request new buffers from the memory allocator on every request"


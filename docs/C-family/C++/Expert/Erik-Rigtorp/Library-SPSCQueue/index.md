# [rigtorp](https://github.com/rigtorp)/**[SPSCQueue](https://github.com/rigtorp/SPSCQueue)**

A single producer single consumer wait-free and lock-free fixed size queue written in C++11.

> NOTE: 
>
> 1、很多时候，解决需要这样的queue



> NOTE: 
>
> 1、下面的的内容是值得学习的
>
> Huge page support
>
> ring buffer
>
> false sharing
>
> 

## Huge page support

In addition to supporting custom allocation through the [standard custom allocator interface](https://en.cppreference.com/w/cpp/named_req/Allocator) this library also supports standard proposal [P0401R3 Providing size feedback in the Allocator interface](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0401r3.html). This allows convenient use of [huge pages](https://www.kernel.org/doc/html/latest/admin-guide/mm/hugetlbpage.html) without wasting any allocated space. Using size feedback is only supported when C++17 is enabled.

> NOTE: 
>
> 1、之前听说过huge page，那如何实现呢？显然本节内容给了启发

## Implementation

The underlying implementation is a [ring buffer](https://en.wikipedia.org/wiki/Circular_buffer).

Care has been taken to make sure to avoid any issues with [false sharing](https://en.wikipedia.org/wiki/False_sharing). The head and tail pointers are aligned and padded to the false sharing range (cache line size). The slots buffer is padded with the false sharing range at the beginning and end.

> NOTE: 
> 1这是典型的"align-to-cache line-optimization"

References:

- *Intel*. [Avoiding and Identifying False Sharing Among Threads](https://software.intel.com/en-us/articles/avoiding-and-identifying-false-sharing-among-threads).
- *Wikipedia*. [Ring buffer](https://en.wikipedia.org/wiki/Circular_buffer).
- *Wikipedia*. [False sharing](https://en.wikipedia.org/wiki/False_sharing).



## Read code
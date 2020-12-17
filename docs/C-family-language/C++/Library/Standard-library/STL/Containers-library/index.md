# Containers library



## cppreference [Containers library](https://en.cppreference.com/w/cpp/container)

There are three classes of containers:

1) sequence containers

2) associative containers

3) unordered associative containers 

each of which is designed to support a different set of operations.

> NOTE: C++ Containers library基本上都是基于class template的。



### Sequence containers

> NOTE: 参见`C++\Library\Standard-library\STL\Containers-library\Sequence-container`章节

### Associative containers

> NOTE: 参见`C++\Library\Standard-library\STL\Containers-library\Associative-container`章节



### Container adaptors

Container adaptors provide a different interface for sequential containers.

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [stack](https://en.cppreference.com/w/cpp/container/stack)   | adapts a container to provide stack (LIFO data structure) (class template) |
| [queue](https://en.cppreference.com/w/cpp/container/queue)   | adapts a container to provide queue (FIFO data structure) (class template) |
| [priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue) | adapts a container to provide priority queue (class template) |

## Common issue

参见 `./Common-issue` 章节，涉及到的主题有:

1 Iterator invalidation

2 Thread safety

3 ......





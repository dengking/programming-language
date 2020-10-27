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



### Iterator invalidation

在[Iterator-invalidation](./Iterator-invalidation.md)中对此进行了详细讨论；



### Thread safety

在[Thread safety](./Thread-safety.md)中对此进行了详细讨论；





## Memory management

STL的container library，对于dynamic container，都涉及如下问题：

- expand

- shrink

除[array](https://en.cppreference.com/w/cpp/container/array)(C++11)，其他的container都是dynamic container；



## Name convention: delete、erase and remove

当我们想要表达“删除”的含义的时候，上述三个单词都可以使用；C++ STL选择的是erase（各种container的成员方法`erase`）和remove（algorithm library中有`std::remove`方法），这种选择的背后是有一定的缘由的：`delete`是C++的key word，所以为了避免名称的冲突，实现者最终选择了erase和remove；


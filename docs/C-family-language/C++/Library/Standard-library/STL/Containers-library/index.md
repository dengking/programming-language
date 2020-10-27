# Containers library



## cppreference [Containers library](https://en.cppreference.com/w/cpp/container)

There are three classes of containers:

1) sequence containers

2) associative containers

3) unordered associative containers 

each of which is designed to support a different set of operations.

> NOTE: C++

### Sequence containers



|                                                              |                          |
| ------------------------------------------------------------ | ------------------------ |
| [array](https://en.cppreference.com/w/cpp/container/array)(C++11) | static contiguous array  |
| [vector](https://en.cppreference.com/w/cpp/container/vector) | dynamic contiguous array |
| [deque](https://en.cppreference.com/w/cpp/container/deque)   | double-ended queue       |
| [forward_list](https://en.cppreference.com/w/cpp/container/forward_list)(C++11) | singly-linked list       |
| [list](https://en.cppreference.com/w/cpp/container/list)     | doubly-linked list       |



### Associative containers

Associative containers implement sorted data structures that can be quickly searched (`O(log n)` complexity).

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [set](https://en.cppreference.com/w/cpp/container/set)       | collection of unique keys, sorted by keys (class template)   |
| [map](https://en.cppreference.com/w/cpp/container/map)       | collection of key-value pairs, sorted by keys, keys are unique (class template) |
| [multiset](https://en.cppreference.com/w/cpp/container/multiset) | collection of keys, sorted by keys (class template)          |
| [multimap](https://en.cppreference.com/w/cpp/container/multimap) | collection of key-value pairs, sorted by keys (class template) |

### Unordered associative containers

Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (`O(1)` amortized, `O(n)` worst-case complexity).

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)(C++11) | collection of unique keys, hashed by keys (class template)   |
| [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)(C++11) | collection of key-value pairs, hashed by keys, keys are unique (class template) |
| [unordered_multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset)(C++11) | collection of keys, hashed by keys (class template)          |
| [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)(C++11) | collection of key-value pairs, hashed by keys (class template) |

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



## Name: delete、erase and remove

当我们想要表达“删除”的含义的时候，上述三个单词都可以使用；C++ STL选择的是erase（各种container的成员方法`erase`）和remove（algorithm library中有`std::remove`方法），这种选择的背后是有一定的缘由的：`delete`是C++的key word，所以为了避免名称的冲突，实现者最终选择了erase和remove；


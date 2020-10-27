# Associative container

和原文的组织方式不同，基于是否ordered来进行分类。



## cppreference [Ordered associative containers](https://en.cppreference.com/w/cpp/container#Associative_containers)

Associative containers implement sorted data structures that can be quickly searched (`O(log n)` complexity).

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [set](https://en.cppreference.com/w/cpp/container/set)       | collection of unique keys, sorted by keys (class template)   |
| [map](https://en.cppreference.com/w/cpp/container/map)       | collection of key-value pairs, sorted by keys, keys are unique (class template) |
| [multiset](https://en.cppreference.com/w/cpp/container/multiset) | collection of keys, sorted by keys (class template)          |
| [multimap](https://en.cppreference.com/w/cpp/container/multimap) | collection of key-value pairs, sorted by keys (class template) |

## cppreference [Unordered associative containers](https://en.cppreference.com/w/cpp/container#Unordered_associative_containers)

Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (`O(1)` amortized, `O(n)` worst-case complexity).

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)(C++11) | collection of unique keys, hashed by keys (class template)   |
| [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)(C++11) | collection of key-value pairs, hashed by keys, keys are unique (class template) |
| [unordered_multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset)(C++11) | collection of keys, hashed by keys (class template)          |
| [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)(C++11) | collection of key-value pairs, hashed by keys (class template) |


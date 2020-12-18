# Maps

C++中map包括: 

|               |                                                              | Multiple key                                                 |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Ordered**   | [map](https://en.cppreference.com/w/cpp/container/map)       | [multimap](https://en.cppreference.com/w/cpp/container/multimap) |
| **Unordered** | [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) (C++11) | [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap) (C++11) |



## C++ ordered map VS unordered map

对两者的比较，其实是对它们的底层data structure的比较: BST VS hash table，关于此，参见工程discrete的相关章节。

### Key

ordered map是基于key comparison来组织数据，更加准确说，它是基于key的order来组织数据；

unordered map是基于key hash来组织数据；



### TODO

1) geeksforgeeks [map vs unordered_map in C++](https://www.geeksforgeeks.org/map-vs-unordered_map-c/)

2) geeksforgeeks [Advantages of BST over Hash Table](https://www.geeksforgeeks.org/advantages-of-bst-over-hash-table/)

3) stackoverflow [Is there any advantage of using map over unordered_map in case of trivial keys?](https://stackoverflow.com/questions/2196995/is-there-any-advantage-of-using-map-over-unordered-map-in-case-of-trivial-keys)

### 遍历性能比较

对两者进行遍历的性能进行比较？




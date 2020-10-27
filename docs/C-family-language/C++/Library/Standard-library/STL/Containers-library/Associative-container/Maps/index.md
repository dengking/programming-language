# Maps

C++中map包括: 

|               |                                                              | Multiple key                                                 |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Ordered**   | [map](https://en.cppreference.com/w/cpp/container/map)       | [multimap](https://en.cppreference.com/w/cpp/container/multimap) |
| **Unordered** | [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) (C++11) | [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap) (C++11) |



## C++ ordered map VS unordered map

对两者的比较，其实是对它们的底层data structure的比较

ordered map是基于key comparison来组织数据，更加准确说，它是基于key的order来组织数据；

unordered map是基于key hash来组织数据；



### TODO

geeksforgeeks [map vs unordered_map in C++](https://www.geeksforgeeks.org/map-vs-unordered_map-c/)

### 遍历性能比较

对两者进行遍历的性能如何？




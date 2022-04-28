# 关于本章

本章描述`c++`的`insert_iterator`系列，每种`insert_iterator`都有对应的`inserter`函数来生成`insert_iterator`，显然这是采用的[Object Generator](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Object_Generator) idiom。在STL中，有如下`insert_iterator`：

| `insert_iterator`                                            | `inserter`                                                   | member function | 插入位置 |
| ------------------------------------------------------------ | ------------------------------------------------------------ | --------------- | -------- |
| [std::insert_iterator](https://en.cppreference.com/w/cpp/iterator/insert_iterator) | [std::inserter](https://en.cppreference.com/w/cpp/iterator/inserter) | `insert`        | 任意位置 |
| [std::front_insert_iterator](https://en.cppreference.com/w/cpp/iterator/front_insert_iterator) | [std::front_inserter](https://en.cppreference.com/w/cpp/iterator/front_inserter) | `push_front()`  | front    |
| [std::back_insert_iterator](https://en.cppreference.com/w/cpp/iterator/back_insert_iterator) | [std::back_inserter](https://en.cppreference.com/w/cpp/iterator/back_inserter) | `push_back()`   | back     |



## `insert_iterator` and `inserter`



### cppreference [std::insert_iterator](https://en.cppreference.com/w/cpp/iterator/insert_iterator) 

`std::insert_iterator` is a [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that inserts elements into a container for which it was constructed, at the position pointed to by the supplied iterator. The container's `insert()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::insert_iterator` is a no-op.

> NOTE: 每次调用“container's `insert()` member function”，保证了可以连续地insert。



### cppreference [std::inserter](https://en.cppreference.com/w/cpp/iterator/inserter) 



## `front_insert_iterator` and `front_inserter`



### cppreference [std::front_insert_iterator](https://en.cppreference.com/w/cpp/iterator/front_insert_iterator)

`std::front_insert_iterator` is an [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that prepends elements to a container for which it was constructed. The container's `push_front()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::front_insert_iterator` is a no-op.



### cppreference [front_inserter](https://en.cppreference.com/w/cpp/iterator/front_inserter)



## `back_insert_iterator` and `back_inserter`



### cppreference `std::back_insert_iterator`

`std::back_insert_iterator` is an [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that appends to a container for which it was constructed. The container's `push_back()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::back_insert_iterator` is a no-op.



### cppreference [std::back_inserter](https://en.cppreference.com/w/cpp/iterator/back_inserter)


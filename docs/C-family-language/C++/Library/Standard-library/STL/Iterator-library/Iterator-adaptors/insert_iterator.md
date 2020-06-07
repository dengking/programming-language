# `insert_iterator` and `inserter`



## cppreference [std::insert_iterator](https://en.cppreference.com/w/cpp/iterator/insert_iterator)

`std::insert_iterator` is a [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that inserts elements into a container for which it was constructed, at the position pointed to by the supplied iterator. The container's `insert()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::insert_iterator` is a no-op.

> NOTE: 每次调用“container's `insert()` member function”，保证了可以连续地insert。



## cppreference [std::inserter](https://en.cppreference.com/w/cpp/iterator/inserter)
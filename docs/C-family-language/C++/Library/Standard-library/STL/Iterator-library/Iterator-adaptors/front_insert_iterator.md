# `front_insert_iterator` and `front_inserter`



## cppreference [std::front_insert_iterator](https://en.cppreference.com/w/cpp/iterator/front_insert_iterator)

`std::front_insert_iterator` is an [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that prepends elements to a container for which it was constructed. The container's `push_front()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::front_insert_iterator` is a no-op.



## cppreference [front_inserter](https://en.cppreference.com/w/cpp/iterator/front_inserter)
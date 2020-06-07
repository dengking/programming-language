# `back_insert_iterator` and `back_inserter`



## cppreference `std::back_insert_iterator`

`std::back_insert_iterator` is an [*LegacyOutputIterator*](https://en.cppreference.com/w/cpp/named_req/OutputIterator) that appends to a container for which it was constructed. The container's `push_back()` member function is called whenever the iterator (whether dereferenced or not) is assigned to. Incrementing the `std::back_insert_iterator` is a no-op.



## cppreference [std::back_inserter](https://en.cppreference.com/w/cpp/iterator/back_inserter)
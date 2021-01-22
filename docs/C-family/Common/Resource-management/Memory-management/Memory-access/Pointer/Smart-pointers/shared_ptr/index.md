# `shared_ptr`

destroy of `shard_ptr`对象、destroy of resource。



## cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

A `shared_ptr` can share ownership of an object while storing a pointer to another object. This feature can be used to point to member objects while owning the object they belong to. The **stored pointer** is the one accessed by [get()](https://en.cppreference.com/w/cpp/memory/shared_ptr/get), the dereference and the comparison operators. The **managed pointer** is the one passed to the deleter when use count reaches zero.

> NOTE: stored pointer、managed pointer




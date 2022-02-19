# `std::weak_ptr`

[`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 是 weak reference。

## cppreference [`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 



## Application

它的application有:

### Track object lifetime

由`shared_ptr`来实现reference counting memory management(当reference count为0的时候，最后一个`shared_ptr` object会将pointee析构掉)，`std::weak_ptr` 可以用来判断pointee是否已经被释放，即track object lifetime。

stackoverflow [When is std::weak_ptr useful?](https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful)



### Using `weak_ptr` for circular reference

这是非常常见的用法；


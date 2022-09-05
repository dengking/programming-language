# `std::weak_ptr` break reference cycle

一、[`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 是 weak reference，主要用于break reference cycle。

二、素材:

1、stackoverflow [When is std::weak_ptr useful?](https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful)

2、nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 

3、learncpp [M.8 — Circular dependency issues with std::shared_ptr, and std::weak_ptr](https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/)





## cppreference [`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 

`std::weak_ptr` is a smart pointer that holds a non-owning ("weak") reference to an object that is managed by [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr). It must be converted to [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) in order to access the referenced object.

`std::weak_ptr` models temporary ownership: when an object needs to be accessed only if it exists, and it may be deleted at any time by someone else, `std::weak_ptr` is used to track the object, and it is converted to [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) to assume temporary ownership. If the original [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) is destroyed at this time, the object's lifetime is extended until the temporary [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) is destroyed as well.

> NOTE: 
>
> Application一: track object lifetime: 
>
> 由`shared_ptr`来实现reference counting memory management(当reference count为0的时候，最后一个`shared_ptr` object会将pointee析构掉)，`std::weak_ptr` 可以用来判断pointee是否已经被释放，即track object lifetime，能够极好的解决dangling问题。
>
> 

Another use for `std::weak_ptr` is to break reference cycles formed by objects managed by [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr). If such cycle is orphaned (i.e., there are no outside shared pointers into the cycle), the `shared_ptr` reference counts cannot reach zero and the memory is leaked. To prevent this, one of the pointers in the cycle [can be made weak](https://en.cppreference.com/w/cpp/memory/weak_ptr/~weak_ptr#Example).

> NOTE:
>
> Application二: break reference cycle



## learncpp [M.8 — Circular dependency issues with std::shared_ptr, and std::weak_ptr](https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/)
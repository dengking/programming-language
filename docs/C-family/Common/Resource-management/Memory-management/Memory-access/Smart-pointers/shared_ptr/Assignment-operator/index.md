# `std::shared_ptr<T>::operator=`



## cppreference [std::shared_ptr<T>::operator=](https://en.cppreference.com/w/cpp/memory/shared_ptr/operator%3D)

> NOTE: 原文对于各种assignment operator给出了其对应的**Equivalent** ，可以看到，这些**Equivalent**都用到了`swap`，显然这是在使用copy and swap idiom

### (1)

```C++
shared_ptr& operator=( const shared_ptr& r ) noexcept;
```



```C++
template< class Y >
shared_ptr& operator=( const shared_ptr<Y>& r ) noexcept;
```

**Shares** ownership of the object managed by `r`. If `r` manages no object, `*this` manages no object too. Equivalent to `shared_ptr<T>(r).swap(*this)`.

> NOTE: 此处的share，就是在前章节中提及的acquire；

### (2)

```C++
shared_ptr& operator=( shared_ptr&& r ) noexcept;
```



```C++
template< class Y >
shared_ptr& operator=( shared_ptr<Y>&& r ) noexcept;
```

Move-assigns a `shared_ptr` from `r`. After the assignment, `*this` contains a copy of the previous state of `r`, and `r` is empty. Equivalent to `shared_ptr<T>(std::move(r)).swap(*this)`.

### (4)

```C++
template< class Y, class Deleter >
shared_ptr& operator=( std::unique_ptr<Y,Deleter>&& r );					
```

Transfers the ownership of the object managed by `r` to `*this`. The deleter associated to `r` is stored for future deletion of the managed object. `r` manages no object after the call. Equivalent to `shared_ptr<T>(std::move(r)).swap(*this)`.

> NOTE: 这就是`std::unique_ptr` to `std::shared_ptr`，在下面的章节描述了这个内容:
>
> 1、learncpp [M.7 — std::shared_ptr](https://www.learncpp.com/cpp-tutorial/stdshared_ptr/)
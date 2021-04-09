# Copyable and movable

## Google C++ Style Guide [Copyable and Movable Types](https://google.github.io/styleguide/cppguide.html#Copyable_Movable_Types)





## Copyable concept



### cppreference [C++ named requirements: *CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)

Specifies that an instance of the type can be copy-constructed from an [lvalue expression](https://en.cppreference.com/w/cpp/language/value_category).

| Expression |                       Post-conditions                        |
| :--------: | :----------------------------------------------------------: |
|  T u = v;  | The value of `u` is equivalent to the value of `v`.The value of `v` is unchanged |
|    T(v)    | The value of `T(v)` is equivalent to the value of `v`.The value of `v` is unchanged. |

#### Notes

> NOTE: 
>
> 1、这一段内容其实所讨论的是"address of idiom"，将它放到了`Address-of-idiom`章节了

### cppreference [C++ named requirements: *CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable)

Specifies that an instance of the type can be copy-assigned from an [lvalue expression](https://en.cppreference.com/w/cpp/language/value_category).

| Expression | Return type | Return value |                       Post-conditions                        |
| :--------: | :---------: | :----------: | :----------------------------------------------------------: |
|   t = v    |    `T&`     |     `t`      | The value of `t` is equivalent to the value of `v`.The value of `v` is unchanged. |



## Movable concept



### cppreference [C++ named requirements: *MoveConstructible* (since C++11)](https://en.cppreference.com/w/cpp/named_req/MoveConstructible)

| Expression |                       Post-conditions                        |
| :--------: | :----------------------------------------------------------: |
| T u = rv;  | The value of `u` is equivalent to the value of `rv` before the initialization.The new value of `rv` is unspecified. |
|   T(rv)    | The value of `T(rv)` is equivalent to the value of `rv` before the initialization.The new value of `rv` is unspecified. |

#### Notes

A class does not have to implement a [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) to satisfy this type requirement: a [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor) that takes a `const T&` argument can bind rvalue expressions.

> NOTE: 
>
> 1、虽然可以"bind rvalue expression"，但是无法对它进行修改。也就无法发挥move semantic

If a *MoveConstructible* class implements a move constructor, it may also implement [move semantics](https://en.cppreference.com/w/cpp/utility/move) to take advantage of the fact that the value of `rv` after construction is unspecified.





### cppreference [C++ named requirements: *MoveAssignable* (since C++11)](https://en.cppreference.com/w/cpp/named_req/MoveAssignable)

| Expression | Return type | Return value |                       Post-conditions                        |
| :--------: | :---------: | :----------: | :----------------------------------------------------------: |
|   t = rv   |    `T&`     |     `t`      | If `t` and `rv` do not refer to the same object , the value of `t` is equivalent to the value of `rv` before the assignment.The new value of `rv` is unspecified |

#### Notes

The type does not have to implement [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator) in order to satisfy this type requirement: a [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator) that takes its parameter by value or as a `const Type&`, will bind to rvalue argument.

If a *MoveAssignable* class implements a move assignment operator, it may also implement [move semantics](https://en.cppreference.com/w/cpp/utility/move) to take advantage of the fact that the value of `rv` after assignment is unspecified.



## Non-copyable but movable

### Object-based resource management: Resource wrapper

一、一般，一个resource wrapper，它own(拥有) resource(ownership)，它的lifetime会和resource进行绑定，一般采用RAII、resource handle的方式来进行实现，这就是之前所总结的Object-based resource management。

二、对于这样的resource wrapper，它们一般都是non-copyable but movable的，因为，一旦允许copy，那么久可能导致多个object同时执行同一个resource，如果其中的一个object end，那么在它的destructor中就会将resource给release，这就可能导致其他的依然存活的object使用这个resource的时候，出现错误，这会导致:

1、dangling pointer

2、double free

#### 我的实践



#### Example

1、cppreference [std::thread](https://en.cppreference.com/w/cpp/thread/thread)

No two `std::thread` objects may represent the same thread of execution; `std::thread` is not [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) or [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable), although it is [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable).



2、ariya [C++ Class and Preventing Object Copy](https://ariya.io/2015/01/c-class-and-preventing-object-copy) 

A class that represents a wrapper stream of a file should not have its instance copied around. It will cause a confusion in the handling of the actual I/O system. 

In a similar spirit, if an instance holds a unique private object, copying the pointer does not make sense. 

### boost [Movable but Non-Copyable Types](http://cs.swan.ac.uk/~csoliver/ok-sat-library/internet_html/doc/doc/Boost/1_53_0/doc/html/move/movable_only_classes.html)



## Non-copyable

一、singleton

二、polymorphic class

三、guard

1、cppreference [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)

The `lock_guard` class is non-copyable.

## Neither copyable nor movable

### 一、mutex

Lock是典型的neither copyable nor movable的，下面是一些例子: 

1、[mohaps](https://github.com/mohaps) / [lrucache](https://github.com/mohaps/lrucache) / [LRUCache.hpp](https://github.com/mohaps/lrucache/blob/master/LRUCache.hpp)

```C++
/**
 * a base lock class that can be overridden
 */
class Lock {
public:
	virtual ~Lock() {
	}
	virtual void lock() = 0;
	virtual void unlock() = 0;
protected:
	Lock() {
	}
private:
	Lock(const Lock&);
	const Lock& operator =(const Lock&);
};
```



2、cppreference [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)

`std::mutex` is neither copyable nor movable.

stackoverflow [Why is std::mutex neither copyable nor movable? [duplicate]](https://stackoverflow.com/questions/62369119/why-is-stdmutex-neither-copyable-nor-movable)

stackoverflow [Why is there no need to mark the move constructor of a type with a deleted copy constructor as deleted?](https://stackoverflow.com/questions/62370747/why-is-there-no-need-to-mark-the-move-constructor-of-a-type-with-a-deleted-copy)

stackoverflow [Move constructor for std::mutex](https://stackoverflow.com/questions/7557179/move-constructor-for-stdmutex)

### 二、`std::atomic`

cppreference2015 [std::atomic](https://doc.bccnsoft.com/docs/cppreference2015/en/cpp/atomic/atomic.html)

> `std::atomic` is neither copyable nor movable.
# Copyable and movable



## 对于resource wrapper需要阐明copyable、movable

### 我的经历

1、resource handler是来源自第三方的network library，resource wrapper使用RAII，但是没有指定它的copyable 和 movable，而是依赖于implicit defined implementation；

> 显然我的这种做法是没有遵循The rule of three/five/zero的，因为我custom了destructor，但是没有对它的copyable、movable进行指定

2、我推测compiler会执行copy-elision，即不会发生copy，然而在复杂的C++规则面前，programmer可能无法准确的预测compiler的行为，结果发生了copy，而implicit defined copy是shallow copy，并且在定义了destructor的情况下，compiler是不会生成movable的 ，这就导致了"implicit-define-copy-constructor-shallow-copy-double-free"错误。

### 从resource ownership的角度来分析copy、move

move对应的是transfer ownership，它需要将resource的ownership transfer走，因此在source object中，需要表明它已经不再own resource了，source object就不再release resource。

> CppCoreGuidelines-C.64 A move operation should move and leave its source in a valid state

copy:

deep copy: 显然是重新acquire resource了

shallow copy: 显然表达的是shared ownership的，需要注意的是，raw pointer是无法表达shared ownership，`std::shared_ptr`是可以的。

### 阐明copyable、movable

对于resource wrapper，需要明确指定它的copyable、movable，下面是一些example: 

Copyable and movable: `std::string`

Non-copyable but movable: 很多resource wrapper都是这种类型的、`std::unique_ptr`

Copyable but non-movable: ?

Neither copyable nor movable: `std::mutex`

作为programmer，需要考虑class是否copyable、movable，并进行显式地指定(比如使用`delete`)，不要推测、过分依赖于compiler的行为。不应该死板的套用rule of three/five/zero，而是根据具体的情况进行准确的阐明。



#### 本质是对resource ownership的阐明

其实对copyable、movable的思考，本质上是对ownership的思考，"implicit defined copy constructor shallow copy double free"问题，其实就是典型对ownership描述不清而导致的问题，它到底是unique ownership还是shared ownership？显然raw pointer无法描述清楚，shallow copy对应的更多是shared ownership，而raw pointer无法表达shared ownership，它会导致double free、dangling。

因此在使用resource handle的时候，一定要同时阐明清楚它的ownership，最好是使用`unique_ptr`、`shared_ptr`，它们已经明确的阐明清楚了ownership。







## What is "Copyable concept"、"Movable concept"？

本文首先说明清楚"Copyable concept"、"Movable concept"，简而言之: 

一、本文中的copyable对应的是:

copy constructor、copy assignment operator

二、本文中的movable对应的是:

move constructor、move assignment operator



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




## Google C++ Style Guide [Copyable and Movable Types](https://google.github.io/styleguide/cppguide.html#Copyable_Movable_Types)





## boost [Movable but Non-Copyable Types](http://cs.swan.ac.uk/~csoliver/ok-sat-library/internet_html/doc/doc/Boost/1_53_0/doc/html/move/movable_only_classes.html)



## Non-copyable

一、singleton

二、polymorphic class

三、guard

1、cppreference [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)

The `lock_guard` class is non-copyable.



## Movable but non-copyable 

### Resource wrapper

一、一般，一个resource wrapper，它own(拥有) resource(ownership)，它的lifetime会和resource进行绑定，一般采用RAII、resource handle的方式来进行实现，这就是之前所总结的Object-based resource management。

二、对于这样的resource wrapper，它们一般都是non-copyable but movable的，因为，一旦允许copy，那么久可能导致多个object同时share同一个resource，在这种情况下，就需要考虑shared ownership了:

1、raw pointer无法实现shared ownership，如果其中的一个object end，那么在它的destructor中就会将resource给release，这就可能导致其他的依然存活的object使用这个resource的时候，出现错误，这会导致:

dangling pointer、double free

2、`std::shared_ptr`可以实现shared ownership

### Example

#### [std::thread](https://en.cppreference.com/w/cpp/thread/thread)

No two `std::thread` objects may represent the same thread of execution; `std::thread` is not [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) or [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable), although it is [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable).



#### ariya [C++ Class and Preventing Object Copy](https://ariya.io/2015/01/c-class-and-preventing-object-copy) 

A class that represents a wrapper stream of a file should not have its instance copied around. It will cause a confusion in the handling of the actual I/O system. 

In a similar spirit, if an instance holds a unique private object, copying the pointer does not make sense. 

#### [redis-plus-plus](https://github.com/sewenew/redis-plus-plus#connection) ` class Redis`

在阅读redis client [redis-plus-plus](https://github.com/sewenew/redis-plus-plus#connection) 的时候，其中介绍到：

> `Redis` class is movable but NOT copyable.

#### [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)

The class satisfies the requirements of [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable), but of neither [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) nor [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable).

## Neither copyable nor movable



### Example

#### [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)

`std::mutex` is neither copyable nor movable.



stackoverflow [Why is std::mutex neither copyable nor movable? [duplicate]](https://stackoverflow.com/questions/62369119/why-is-stdmutex-neither-copyable-nor-movable)

stackoverflow [Why is there no need to mark the move constructor of a type with a deleted copy constructor as deleted?](https://stackoverflow.com/questions/62370747/why-is-there-no-need-to-mark-the-move-constructor-of-a-type-with-a-deleted-copy)

stackoverflow [Move constructor for std::mutex](https://stackoverflow.com/questions/7557179/move-constructor-for-stdmutex)



#### `std::atomic`

cppreference2015 [std::atomic](https://doc.bccnsoft.com/docs/cppreference2015/en/cpp/atomic/atomic.html)

> `std::atomic` is neither copyable nor movable.





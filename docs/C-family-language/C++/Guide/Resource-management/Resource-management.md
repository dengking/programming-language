# Resource management

## 有哪些resource？

file、network connection、memory等，都可以看做是resource。

## 对resource的操作

construct、new

copy 

move

swap

destructor、delete

## idiom

### Handler-body idiom

handler-body idiom在c++中非常重要，所有涉及resource 的都可以使用这个idiom。基于这个idiom的library非常多：redispp，thread。对resource的操作都是通过member method的转发到resource的API。raii其实也是如此，各种各样的c wrapper。

### RAII

RAII等都是C++的resource management策略。



### Rule of three/five



## Example of resource management

movable but not copyable：

[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)

> `Redis` class is movable but NOT copyable.

[std::thread](https://en.cppreference.com/w/cpp/thread/thread)

> `std::thread` is not [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) or [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable), although it is [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable).
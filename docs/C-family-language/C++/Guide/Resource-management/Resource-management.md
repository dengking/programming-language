# Resource management

## 有哪些resource？

file、network connection、memory等，都可以看做是resource。

## 对resource的操作

construct、new

copy 

move

destructor、delete

## idiom

### Handler-body idiom

handler-body idiom在c++中非常重要，所有涉及resource 的都可以使用这个idiom。基于这个idiom的library非常多：redispp，thread。对resource的操作都是通过member method的转发到resource的API。raii其实也是如此，各种各样的c wrapper。

### RAII

RAII等都是C++的resource management策略。



### Rule of three/five
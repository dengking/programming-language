# Explicit and implict 

本文标题的含义是: 隐式和显式。

## Type conversion

Type conversion与explicit、implicit密切相关的，下面看看type conversion中的explicit and implicit

### cppreference [explicit specifier](https://en.cppreference.com/w/cpp/language/explicit) 

其实，这里提到了explicit，那么就必须要搞清楚c++中的implicit



### cppreference [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) 

[What are move semantics?](http://note.youdao.com/noteshare?id=ec11cf641add562011d335b955381d09&sub=B1A8901E852F4AF88D7C81ED5C457A03)一文中的**Implicit conversions**章节的内容提醒了我一个非常重要的知识：在进行类型转换的时候，执行过程不是在原来的变量上进行修改，而是重新创建一个临时变量。



## Explicit is Better than Implicit

### talesofcpp [Episode Five: Explicit is Better than Implicit](http://talesofcpp.fusionfenix.com/post-7/episode-five-explicit-is-better-than-implicit)



### implicit and explicit

对于危险的操作，要么在语言层面禁止，要么 make it explicit；一个典型的例子就是C++ move：
move rvalue是安全的
move lvalue是危险的
std::move 是显式的函数，允许 将lvalue转换为 rvalue；

这个思想在  https://stackoverflow.com/questions/3106110/what-are-move-semantics 中给出了

另外一个例子是今天在编译Example 11.5 的时候，pthread_cleanup_push的第二个入参类似是`void *`，如果使用下面的方式来进行调用:
```c++
pthread_cleanup_push(cleanup, "thread 1 first handler");
```
由于"thread 1 first handler"的类型是const char *，则导致编译报错；如果显式地进行转换，即将其转换为void *类型，则编译通过：
```c++
pthread_cleanup_push(cleanup, (void *)"thread 1 first handler");
```



另外一个例子就是在`C++\Language-reference\Basic-concept\Type-system\Type-conversion\reinterpret_cast.md#CV`段中进行了详细介绍的CV



另外一个典型的例子是：implicit conversion 和 explicit conversion
关于implicit conversion，参见 https://en.cppreference.com/w/cpp/language/implicit_conversion 
它是安全的，比如array-to-pointer conversion、function-to-pointer conversion

explicit conversion 是需要由programmer来指定的，它不一定安全

pointer to void pointer是implicit的，但是反过来需要explicit
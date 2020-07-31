# Containers library



## cppreference [Containers library](https://en.cppreference.com/w/cpp/container)



### Iterator invalidation

在[Iterator-invalidation](./Iterator-invalidation.md)中对此进行了详细讨论；

### Thread safety

在[Thread safety](./Thread-safety.md)中对此进行了详细讨论；





## Memory management

STL的container library，对于dynamic container，都涉及如下问题：

- expand

- shrink

除[array](https://en.cppreference.com/w/cpp/container/array)(C++11)，其他的container都是dynamic container；



## Name: delete、erase and remove

当我们想要表达“删除”的含义的时候，上述三个单词都可以使用；C++ STL选择的是erase（各种container的成员方法`erase`）和remove（algorithm library中有`std::remove`方法），这种选择的背后是有一定的缘由的：`delete`是C++的key word，所以为了避免名称的冲突，实现者最终选择了erase和remove；


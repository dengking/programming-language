# Member function table

STL container的接口的定义、命名是遵循一定的规范的，从它的Member function table是可以看出的。

## cppreference [Containers library # Member function table](https://en.cppreference.com/w/cpp/container#Member_function_table)



### Iterators

> NOTE:
>
> Iterator的命名

### Element access



### Capacity



### Modifiers







## Name convention: delete、erase and remove

当我们想要表达“删除”的含义的时候，上述三个单词都可以使用；C++ STL选择的是erase（各种container的成员方法`erase`）和remove（algorithm library中有`std::remove`方法），这种选择的背后是有一定的缘由的：`delete`是C++的key word，所以为了避免名称的冲突，实现者最终选择了erase和remove；
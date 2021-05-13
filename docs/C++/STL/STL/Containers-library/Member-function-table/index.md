# Member function table

1、STL container的接口的定义、命名是遵循一定的规范的，从它的Member function table是可以看出的。

2、consist uniform API interface

提供统一的API，是便于操作的。

## cppreference [Containers library # Member function table](https://en.cppreference.com/w/cpp/container#Member_function_table)



### Iterators

> NOTE:
>
> Name convention:
>
> 1) begin 和 end
>
> 2) `cbegin` `c`的含义是const
>
> 参见下面的"`cbegin` and `begin`"章节。
>
> 3) `rbegin` `r`的含义是reverse

#### `begin` and `end`



### Element access

#### `front` and `back`



### Capacity



### Modifiers







## Name convention: delete、erase and remove

当我们想要表达“删除”的含义的时候，上述三个单词都可以使用；C++ STL选择的是erase（各种container的成员方法`erase`）和remove（algorithm library中有`std::remove`方法），这种选择的背后是有一定的缘由的：`delete`是C++的key word，所以为了避免名称的冲突，实现者最终选择了erase和remove；



## `cbegin` and `begin`

### Why `cbegin` and `cend`?

获得 `const_iterator`，参见 `const_iterator` 章节。



## `begin`、`end` VS `front`、`back`

### stackoverflow [What is the difference between vector.back() and vector.end()?](https://stackoverflow.com/questions/44831793/what-is-the-difference-between-vector-back-and-vector-end)

A

Here's an illustration of which is which

```
v: [ 1 | 2 | 3 | 4 | ... | 999 ]
     🡑                      🡑     🡑
   front()                back() end()
     🡑
   begin()
```

where `front()` and `back()` return a (const) reference to the first and last element respectively, and `end()` returns an *iterator* (sort of a pointer) to one beyond the last element of vector. `begin()` returns an iterator to the first element of a vector.

These are also explained at [`std::vector`](http://en.cppreference.com/w/cpp/container/vector)

> [`front`](http://en.cppreference.com/w/cpp/container/vector/front) access the first element
> [`back`](http://en.cppreference.com/w/cpp/container/vector/back) access the last element
> [`end`/`cend`](http://en.cppreference.com/w/cpp/container/vector/end) returns an iterator to the end
> [`begin`/`cbegin`](http://en.cppreference.com/w/cpp/container/vector/begin) returns an iterator to the beginning

------

Subtracting one from size is because an index in C or C++ starts at zero, and not one as usually. This means, in order to access the first element of an array, or in this case of a vector, you say

```c++
v[0]
```

and not

```c++
v[1]
```

Equally for the last (nth) element, you wouldn't take `size` or `n` of an array (a vector), but rather one less, e.g.

```c++
v[size() - 1]
```

or

```c++
v[n - 1]
```


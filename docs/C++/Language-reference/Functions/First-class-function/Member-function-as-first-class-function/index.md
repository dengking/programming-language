# Member function as first class function

将成员函数作为first class function。



## stackoverflow [Class and std::async on class member in C++](https://stackoverflow.com/questions/11758414/class-and-stdasync-on-class-member-in-c)

> NOTE: 演示了两种典型的写法

[A](https://stackoverflow.com/a/11758662)

> NOTE: `std::async`内部使用`std::invoke`，参见 `Callable` 章节；

`do_rand_stf` is a non-static member function and thus cannot be called without a class instance (the implicit `this` parameter.) Luckily, `std::async` handles its parameters like `std::bind`, and `bind` in turn can use `std::mem_fn` to turn a member function pointer into a functor that takes an explicit `this` parameter, so all you need to do is to pass `this` to the `std::async` invocation and use valid member function pointer syntax when passing the `do_rand_stf`:

```cpp
auto hand=async(launch::async,&A::do_rand_stf,this,i,j);
```

There are other problems in the code, though. First off, you use `std::cout` and `std::endl` without `#include`ing `<iostream>`. More seriously, `std::future` is not copyable, only movable, so you cannot `push_back` the named object `hand` without using `std::move`. Alternatively, just pass the `async` result to `push_back` directly:

```cpp
ran.push_back(async(launch::async,&A::do_rand_stf,this,i,j));
```

[A](https://stackoverflow.com/a/42193934)

> NOTE: 使用lambda

You can pass the `this` pointer to a new thread:

```cpp
async([this]()
{
    Function(this);
});
```

## reddit [What is the proper way to pass a member function to std::async?](https://www.reddit.com/r/learnprogramming/comments/2gavlf/c11_what_is_the_proper_way_to_pass_a_member/)



## 方式总结

### 使用`std::function` + lambda

参见`std::function`章节。

#### 使用lambda

```C++
[this]
{
    this->member_function();
}
```



### 使用pointer to member function

参见`Pointer-to-member-function`。



### 使用`std::mem_fn`



### 使用callable `std::invoke`

就像 stackoverflow [Class and std::async on class member in C++](https://stackoverflow.com/questions/11758414/class-and-stdasync-on-class-member-in-c) # [A](https://stackoverflow.com/a/11758662) 中所描述的方式；



## TODO

stackexchange [C++ member function wrapper for interfacing with C APIs](https://codereview.stackexchange.com/questions/180221/c-member-function-wrapper-for-interfacing-with-c-apis)

stackexchange [Simple wrapper for member function pointers with known signature](https://codereview.stackexchange.com/questions/148824/simple-wrapper-for-member-function-pointers-with-known-signature)


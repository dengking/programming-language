# swap是基础操作

1、swap是基础操作，它是copy constructor、move constructor实现strong safety的基础

2、典型的例子包括： copy-and-swap

下面是关于此的文章：

## stackoverflow [Why do some people use swap for move assignments?](https://stackoverflow.com/questions/6687388/why-do-some-people-use-swap-for-move-assignments)



For example, stdlibc++ has the following:

```cpp
unique_lock& operator=(unique_lock&& __u)
{
    if(_M_owns)
        unlock();
    unique_lock(std::move(__u)).swap(*this);
    __u._M_device = 0;
    __u._M_owns = false;
    return *this;
}
```

Why not just assign the two `__u` members to `*this` directly? Doesn't the swap imply that `__u` is assigned the `*this` members, only to later have then assigned 0 and false... in which case the swap is doing unnecessary work. What am I missing? (the `unique_lock::swap` just does an `std::swap` on each member)



[A](https://stackoverflow.com/a/6687403)

It's about exception safety. Since `__u` is already constructed when the operator is called, we know there's no exception, and `swap` doesn't throw.

If you did the member assignments manually, you'd risk that each of those might throw an exception, and then you'd have to deal with having partially move-assigned something but having to bail out.

Maybe in this trivial example this doesn't show, but it's a general design principle:

- Copy-assign by copy-construct and swap.
- Move-assign by move-construct and swap.
- Write `+` in terms of construct and `+=`, etc.

> NOTE: 
>
> 1、上述总结得不错

Basically, you try to minimize the amount of "real" code and try to express as many other features in terms of the core features as you can.

(The `unique_ptr` takes an explicit rvalue reference in the assignment because it does not permit copy construction/assignment, so it's not the best example of this design principle.)
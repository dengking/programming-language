# swap

本章对swap进行总结，主要关注:

1、如何custom swap并达到strong exception safety，即non-throw

2、如何使用swap并"facilitate generic programming"

> 上述两点其实是在 More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap) 中强调的

## swap usage

swap是非常重要的operation，它可以看做是language-support-function(比如`std::move`)，它是很多其他operation的基石:

一、default constructor + swap

1、reset

2、clear and minimize

3、move constructor

二、rule of four and half

move constructor = default constructor + swap

unified assignment operator = [copy move] constructor + swap

三、copy and swap

### 素材

一、stackoverflow [Why do some people use swap for move assignments?](https://stackoverflow.com/questions/6687388/why-do-some-people-use-swap-for-move-assignments)

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



二、open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html)

> analogous(类似于) to the existing verb "swap," which is equivalent to a move, two move-assignments, and a destroy

总结的比较好
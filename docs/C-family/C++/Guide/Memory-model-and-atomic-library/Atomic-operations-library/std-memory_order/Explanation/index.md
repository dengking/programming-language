# cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation



## Constants



### `memory_order_acq_rel`

A **read-modify-write** operation with this memory order is both an *acquire operation* and a *release operation*. 

> NOTE: 只能怪用于 **read-modify-write** operation 。

No memory reads or writes in the current thread can be reordered before or after this store. 

> NOTE: 显然，同时施加 *acquire operation* 和 *release operation*:
>
> 1、*acquire operation*: reads can not be reordered before 
>
> 2、*release operation*: writes can not be reordered after 

All writes in other threads that release the same atomic variable are visible before the modification and the modification is visible in other threads that acquire the same atomic variable.

> NOTE: 这描述的是实现的效果



> NOTE: 
>
> 1、Relaxed ordering
>
> 2、Release-Acquire ordering
>
> 3、Release-Acquire ordering
>
> 4、Sequentially-consistent ordering
>
> 

## Relaxed ordering

> NOTE: 参见 `Relaxed-ordering` 章节





## Release-Acquire ordering

> NOTE: 参见 `Release-Acquire-ordering` 章节

## Release-Consume ordering

> NOTE: 参见 `Release-Consume-ordering` 章节

## Sequentially-consistent ordering

> NOTE: 参见 `Sequentially-consistent-ordering` 章节

## Relationship with `volatile`

TODO
# `std::hardware_destructive_interference_size`, `std::hardware_constructive_interference_size`

在阅读 [folly](https://github.com/facebook/folly/tree/main/folly)/[**IndexedMemPool.h**](https://github.com/facebook/folly/blob/main/folly/IndexedMemPool.h) 时，其中有这样的写法:

```C++
alignas(hardware_destructive_interference_size) 
```



## destructive interference、constructive interference的含义

destructive interference 相消干涉

constructive interference 相长干涉

参见: zhihu [谁能解释一下destructive and constructive interference？](https://www.zhihu.com/question/23563893)

## 问题: 为什么要搞两个？

这个问题在下面给出了解答

1、stackoverflow [Understanding std::hardware_destructive_interference_size and std::hardware_constructive_interference_size](https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons) # [A](https://stackoverflow.com/a/39887282/10173843)

2、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[lang](https://github.com/facebook/folly/tree/main/folly/lang)/[**Align.h**](https://github.com/facebook/folly/blob/main/folly/lang/Align.h)

```C++
//  Memory locations within the same cache line are subject to destructive
//  interference, also known as false sharing, which is when concurrent
//  accesses to these different memory locations from different cores, where at
//  least one of the concurrent accesses is or involves a store operation,
//  induce contention and harm performance.
//
//  Microbenchmarks indicate that pairs of cache lines also see destructive
//  interference under heavy use of atomic operations, as observed for atomic
//  increment on Sandy Bridge.
//
//  We assume a cache line size of 64, so we use a cache line pair size of 128
//  to avoid destructive interference.
//
//  mimic: std::hardware_destructive_interference_size, C++17
constexpr std::size_t hardware_destructive_interference_size =
    (kIsArchArm || kIsArchS390X) ? 64 : 128;
static_assert(hardware_destructive_interference_size >= max_align_v, "math?");

//  Memory locations within the same cache line are subject to constructive
//  interference, also known as true sharing, which is when accesses to some
//  memory locations induce all memory locations within the same cache line to
//  be cached, benefiting subsequent accesses to different memory locations
//  within the same cache line and heping performance.
//
//  mimic: std::hardware_constructive_interference_size, C++17
constexpr std::size_t hardware_constructive_interference_size = 64;
static_assert(hardware_constructive_interference_size >= max_align_v, "math?");
```

注释中的解释是非常好的，比 stackoverflow [Understanding std::hardware_destructive_interference_size and std::hardware_constructive_interference_size](https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons) # [A](https://stackoverflow.com/a/39887282/10173843) 中的解释更加易懂。

### stackoverflow [Understanding std::hardware_destructive_interference_size and std::hardware_constructive_interference_size](https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons)



[A](https://stackoverflow.com/a/39887282/10173843)

The intent of these constants is indeed to get the **cache-line size**. The best place to read about the rationale for them is in the proposal itself:

http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0154r1.html

I'll quote a snippet of the rationale here for ease-of-reading:

> [...] the granularity of memory that does not interfere (to the first-order) [is] commonly referred to as the *cache-line size*.
>
> > NOTE: 
> >
> > "interfere"的意思是"干涉"
>
> Uses of *cache-line size* fall into two broad categories:
>
> > NOTE: 
> >
> > 使用cache-line能够避免**destructive interference (false-sharing)** 、促进**constructive interference (true-sharing)** 
>
> 1、Avoiding **destructive interference (false-sharing)** between objects with temporally(暂时的) disjoint(不相交) runtime access patterns from different threads.
>
> 2、Promoting **constructive interference (true-sharing)** between objects which have temporally local runtime access patterns.
>
> The most sigificant issue with this useful implementation quantity is the questionable(不确定的、有问题的) portability of the methods used in current practice to determine its value, despite their pervasiveness(普遍性) and popularity as a group. [...]
>
> We aim to contribute a modest invention for this cause, abstractions for this quantity that can be conservatively defined for given purposes by implementations:
>
> > NOTE: 
> >
> > "我们的目标是为此贡献一个适度的发明，这个数量的抽象可以通过实现为给定目的保守地定义："
>
> 1、*Destructive interference size*: a number that’s suitable as an offset between two objects to likely avoid **false-sharing** due to different runtime access patterns from different threads.
>
> 2、*Constructive interference size*: a number that’s suitable as a limit on two objects’ combined memory footprint size and **base alignment** to likely promote true-sharing between them.

### cppreference [std::hardware_destructive_interference_size, std::hardware_constructive_interference_size](https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size)


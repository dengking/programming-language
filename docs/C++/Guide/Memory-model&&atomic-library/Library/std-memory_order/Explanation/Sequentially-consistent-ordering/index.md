# Sequentially-consistent ordering

## preshing [Memory Barriers Are Like Source Control Operations ](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/) #StoreLoad

A StoreLoad barrier ensures that all stores performed before the barrier are visible to other processors, and that all loads performed after the barrier receive the latest value that is visible at the time of the barrier. In other words, it effectively prevents reordering of all stores before the barrier against all loads after the barrier, respecting the way a [sequentially consistent](http://preshing.com/20120612/an-introduction-to-lock-free-programming#sequential-consistency) multiprocessor would perform those operations.

> NOTE: 上述是从hardware implementation的角度来说的，能够帮助我们理解后面的内容

## csdn [C++ memory order循序渐进（三）—— 原子变量上组合应用memory order实现不同的内存序](https://blog.csdn.net/wxj1992/article/details/103843971)  # 4.Sequentially-consistent ordering

使用`memory_order_seq_cst`可以获得顺序一致，是atomic变量默认的内存序，也是最强的保证，在如果形成了该内存序，除了有release acquire的效果，还会外加一个**单独全序**，也就是保证所有的线程观察到内存操作完全**同样的顺序**。这也是代价很大的一种内存序，尤其是在arm等weak order平台上，因为要实现Sequentially-consistent需要全局的数据同步，因此如果对性能有较高要求，不是特别必要不要使用这个内存序。

关于原子变量上的顺序一致形式化定义如下：

对于从原子变量M中读取的 `memory_order_seq_cst` load B , 会观察到以下情况之一:

（1）单独全序中B之前最近的一个对M的`memory_order_seq_cst` store A。

（2）如果有（1）中的这么一个 A, B可能观察到一个不是happen-before A 的非`memory_order_seq_cst`的修改C

（3） 如果没有（1）中的这么一个 A, B 可能观察到 某个M的非`memory_order_seq_cst`的无关修改的结果。

> NOTE: 例子参见下面的Example

该例是要确保z在运行完后不为0，如果要使得z为0，那么必须同时满足以下两个条件：

（1）read_x_then_y()依次观察到x=true，y=false。

（2）read_y_then_x()依次观察到y=true，x=false。

这在Sequentially-consistent下是不可能的，因为是全局顺序一致，各个线程观察到的顺序一致，如果有（1），那么说明x先比y被置为true，那么read_y_then_x()在观察到y=true之后必然会观察到x=true，从而（2）不可能。反之如果有（2）那么（1）肯定不可能，因此z必不为0。

## cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Constants # `std::memory_order_seq_cst`

1、A load operation with this memory order performs an *acquire operation*, 

2、a store performs a *release operation*, and 

3、read-modify-write performs both an *acquire operation* and a *release operation*, plus a single total order exists in which all threads observe all modifications in the same order (see [Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Sequentially-consistent_ordering) below)



## cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Sequentially-consistent ordering

Atomic operations tagged `memory_order_seq_cst` not only order memory the same way as release/acquire ordering (everything that *happened-before* a store in one thread becomes a *visible side effect* in the thread that did a load), but also establish a *single total modification order* of all atomic operations that are so tagged.

> NOTE: 
>
> 1、"establish a *single total modification order* of all atomic operations that are so tagged" 要如何理解？解答如下: 
>
> sequentially-consistent ordering是在所有的core之间建立modification order，需要注意的是，这里是所有的core，而不是所有的thread；因此每个shared data在全局只有一个modification order，因此从全局来看，所有的thread看到的所有的shared data的modification order是一致的，即它是全局一致的，因此就存在一个全局的single total modification order，这就保证了所有的thread看到相同的modification order。

###  (until C++20)

> NOTE: 未阅读
> 


### (since C++20)

> NOTE: 未阅读

### Use case

Sequential ordering may be necessary for **multiple producer-multiple consumer** situations where all consumers must observe the actions of all producers occurring in the same order.

### Implementation

Total sequential ordering requires a **full memory fence CPU instruction** on all multi-core systems. This may become a performance bottleneck since it forces the affected memory accesses to propagate to every core.



### Example

This example demonstrates a situation where sequential ordering is necessary. Any other ordering may trigger the assert because it would be possible for the threads `c` and `d` to observe changes to the atomics `x` and `y` in opposite order.

```C++
#include <thread>
#include <atomic>
#include <cassert>
#include <iostream>

std::atomic<bool> x = {false};
std::atomic<bool> y = {false};
std::atomic<int> z = {0};

void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst))
        ;
    if (y.load(std::memory_order_seq_cst)) {
        ++z;
    }
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
        ;
    if (x.load(std::memory_order_seq_cst)) {
        ++z;
    }
}

int main()
{
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join(); b.join(); c.join(); d.join();
    assert(z.load() != 0);  // will never happen
    std::cout << z << std::endl;
}

```



#### 实现分析

1、thread `a` `write_x`、thread `b` `write_y`执行write，thread `c` `read_x_then_y`、thread `d` `read_y_then_x`执行read；

2、

thread `c` synchronize-with thread `a`，因此，`read_x_then_y`中，它可以观察到`x`；

thread `d` synchronize-with thread `b`，因此，`read_y_then_x`中，它可以观察到`y`；

但是:

thread `c`  没有 synchronize-with thread `b`，因此，如果没有 sequential consistent ordering，它将无法观察到`y`；

thread `d`  没有 synchronize-with thread `a` ，因此，如果没有sequentially-consistent ordering，则它可能观察到`x`；

#### 执行结果分析

下面以**函数名**来代替thread进行说明，因为**函数名**更加容易理解。可能的执行结果:

| 第一步                            | 第二步                                 | 第三步                                | 第四步                                 | 输出结果 |
| --------------------------------- | -------------------------------------- | ------------------------------------- | -------------------------------------- | -------- |
| `write_x` <br>(`x=true,y=false`)  | `read_x_then_y`<br/>(`x=true,y=false`) | `write_y`<br/>(`x=true,y=true`)       | `read_y_then_x`<br/>(`x=true,y=false`) | 1        |
| `write_y`<br/>(`x=false,y=true`)  | `read_y_then_x`<br/>(`x=false,y=true`) | `write_x` <br/>(`x=true,y=true`)      | `read_x_then_y`<br/>(`x=true,y=true`)  | 1        |
| `write_x` <br/>(`x=true,y=false`) | `write_y`<br/>(`x=true,y=true`)        | `read_x_then_y`<br/>(`x=true,y=true`) | `read_y_then_x`<br/>(`x=true,y=true`)  | 2        |
| ...                               |                                        |                                       |                                        |          |



因此，总的来说，输出结果只有两种可能: 0 或者 1。

#### Sequential consistent classic example

这个例子是典型的sequential consistency 的example，与此类似的例子还有:

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) # [A](https://stackoverflow.com/a/6319356)

2、工程parallel-computing的`Sequential-consistency`章节



## TODO

stackoverflow [Memory Model in C++ : sequential consistency and atomicity](https://stackoverflow.com/questions/38425920/memory-model-in-c-sequential-consistency-and-atomicity)


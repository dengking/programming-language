# jemalloc

## [jemalloc](http://jemalloc.net/)

jemalloc is a general purpose malloc(3) implementation that emphasizes fragmentation avoidance and scalable concurrency support. 

> NOTE: 
>
> 1、"fragmentation avoidance"对于频繁进行`malloc`、`free`的application具有非常重要的意义，比如Redis
>
> 2、"scalable concurrency"对于multiple thread非常重要，并且jemalloc如何实现scalability是非常值得借鉴的。
>
> 

## github [jemalloc](https://github.com/jemalloc)/**[jemalloc](https://github.com/jemalloc/jemalloc)**





## csdn [更好的内存管理-jemalloc (redis 默认使用的)](https://blog.csdn.net/weixin_34357436/article/details/92090661)

忽略细节，从架构着眼，最出色的部分仍是arena和thread cache。（事实上，这两个与tcmalloc的架构几乎相同。Jemalloc only的部分将会在另一次posting中继续探讨。）

> NOTE: 
>
> 1、"arena"是dynamic memory management中非常常用的technique

### Arena

与其像malloc一样集中管理一整块内存，不如将其分成许多个小块来分而治之。此小块便称为arena。

> NOTE: 
>
> 一、此处关于Arena的介绍是不清晰的，参见:
>
> paper [A Scalable Concurrent malloc(3) Implementation for FreeBSD](https://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf) # Algorithms and data structures
>
> Multiple processors: Use four times as many arenas as there are processors. By assigning threads to a set of arenas, the probability of a single arena being used concurrently decreases.
>
> 

### Thread cache

如果是开辟小块内存，为使不参照arena而直接malloc，给各自的线程thread cache领域。此idea是google的tcmalloc的核心部分，亦在jemalloc中体现。

> NOTE: 
>
> 1、显然这是借鉴tcmalloc的，参见 [TCMalloc : Thread-Caching Malloc](https://google.github.io/tcmalloc/design.html)



### Layout

下图是jemalloc的核心layout。看着复杂，其实都是上面说明的部分。

![更好的内存管理-jemalloc - Alex - wangkaisino的博客](http://static.oschina.net/uploads/img/201311/06114252_WhXu.jpg)



### 给程序员的最后的免费午餐

> NOTE: 
>
> 1、这段对“[免费午餐的时代结束了](http://www.gotw.ca/publications/concurrency-ddj.htm)“的解读是非常好的

2005年发表了一篇文章“[免费午餐的时代结束了](http://www.gotw.ca/publications/concurrency-ddj.htm)”。在之前，程序就算不用费脑子，随着cpu时钟速度增加，程序性能自己就会上去。但现在不同，现在cpu时钟趋于稳定，而核数不断地增加。程序员需要适应这样的多线程多进程的环境，并要开发出适合的程序。文章讲的大概是这样的内容。

6年之后的如今，这篇文章完全变成现实了。事实上cpu时钟停留在3GHz，而核不断上升。现在程序要适应多线程多进程的分布式计算，速度才能上升。但是这样的程序很难。

#### 最后一道午餐

> NOTE: 
>
> 1、"最后一道午餐"的比喻是非常好的

现在在多线程的环境下，给程序员们的最后一道午餐便是tcmalloc，jemalloc这样的malloc library。对于使用多线程的程序而言，性能会提高数十%。



## jianshu [自顶向下深入分析Netty（十）--JEMalloc分配算法](https://www.jianshu.com/p/15304cd63175)

### 1. JEMalloc分配算法

Netty的分配算法与此相似，可参见下图：

![img](https://upload-images.jianshu.io/upload_images/3288959-67084db542966d29.png?imageMogr2/auto-orient/strip|imageView2/2/w/743/format/webp)

jemalloc内存分配示意图

#### 1.1 Arena

为了提高内存分配效率并减少内部碎片，jemalloc算法将Arena切分为小块Chunk，根据每块的内存使用率又将小块组合为以下几种状态：QINIT，Q0，Q25，Q50，Q75，Q100。Chunk块可以在这几种状态间随着内存使用率的变化进行转移，内存使用率和状态转移可参见下图：



![img](https:////upload-images.jianshu.io/upload_images/3288959-4341098784afbb90.png?imageMogr2/auto-orient/strip|imageView2/2/w/636/format/webp)

Chunk块内存使用率和转移



## stackoverflow [How does jemalloc work? What are the benefits?](https://stackoverflow.com/questions/1624726/how-does-jemalloc-work-what-are-the-benefits)
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
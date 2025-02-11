# Escape analysis

Escape analysis: 逃逸分析

## wikipedia [Escape analysis](https://en.wikipedia.org/wiki/Escape_analysis)

### Optimizations

A compiler can use the results of escape analysis as a basis for optimizations:[[1\]](https://en.wikipedia.org/wiki/Escape_analysis#cite_note-:0-1)

- *Converting [heap allocations](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) to [stack allocations](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)*.[[2\]](https://en.wikipedia.org/wiki/Escape_analysis#cite_note-2) If an object is allocated in a subroutine, and a pointer to the object never escapes, the object may be a candidate for stack allocation instead of heap allocation. In garbage-collected languages this can reduce how often the collector needs to run.
- *Synchronization elision*. If an object is found to be accessible from one thread only, operations on the object can be performed without synchronization.
- *Breaking up objects* or *scalar replacement*.[[3\]](https://en.wikipedia.org/wiki/Escape_analysis#cite_note-3) An object may be found to be accessed in ways that do not require the object to exist as a sequential memory structure. This may allow parts (or all) of the object to be stored in CPU registers instead of in memory.

## 素材

[juejin 一行代码引发的性能暴跌 10 倍](https://juejin.cn/post/7276999034962280508?searchId=202502081115080952EE3C5DE6B77465C5)

其中给出了逃逸分析引起性能变化的一个例子

[juejin JVM调优之逃逸分析与TLAB](https://juejin.cn/post/6985386868405698596?searchId=202502081115080952EE3C5DE6B77465C5) 

[juejin TLAB的全称是Thread Local Allocation Buffer(线程本地分配缓存)](https://juejin.cn/post/7107292213758918670?searchId=202502081115080952EE3C5DE6B77465C5) 

其中展示了开启和关闭TLAB对性能的影响的例子



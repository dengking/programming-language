# `CacheLocality`

## stackoverflow [How does Facebook folly::AccessSpreader work?](https://stackoverflow.com/questions/47006451/how-does-facebook-follyaccessspreader-work)

[A](https://stackoverflow.com/a/47361089/10173843)

The overall idea is that when you have a number of **equivalent** resources/data structures, and want different threads to access different instances to **minimize contention** and **maximize data locality**, you use `AccessSpreader` to suggest the best resource/data to use for the current core/thread.

For an example, see e.g. https://github.com/facebook/folly/blob/master/folly/IndexedMemPool.h. This implementation of a memory pool maintains a number of free object lists, to reduce thread contention on **allocation/deallocation**. And here is how `AccessSpreader` is used:

```C++
AtomicStruct<TaggedPtr,Atom>& localHead() {
  auto stripe = AccessSpreader<Atom>::current(NumLocalLists);
  return local_[stripe].head;
}
```

i.e. it gives an index of an element (in some array or vector etc.) that is recommended for use by the current thread.

**Update (in response to the comment)**: it is not always possible to assign different indices to different threads - e.g. if the number of possible indices (stripes) is less than the number of CPUs; and the comments explicitly tell that "It does NOT guarantee uncontended access". The class can be used not only to minimize contention but also to maximize data locality; for example, you might want to share some instances of data between threads that have common cache. So, the recommended index is a function of two variables: 

1、the current CPU (obtained internally with `getCpuFunc`) 

2、the number of stripes (passed as the parameter `numStripes`) 

this is why a 2D array is needed. 

The array is filled at program initialization time using system-specific information (via class `CacheLocality`), so that the recommended index takes **data locality** into account.

As for `std::atomic`, it is used merely to have separate `AccessSpreader` instantiations for testing and for production use, as explained in the comment right before the class declaration. The class does not have (and does not need) any atomic member variables.

## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[concurrency](https://github.com/facebook/folly/tree/main/folly/concurrency)/**[CacheLocality.h](https://github.com/facebook/folly/blob/main/folly/concurrency/CacheLocality.h)**

### doc

This file contains several classes that might be useful if you are trying to **dynamically** optimize **cache locality**: `CacheLocality` reads cache sharing information from `sysfs` to determine how CPUs should be grouped to **minimize contention**, `Getcpu` provides fast access to the current CPU via `__vdso_getcpu`, and `AccessSpreader` uses these two to optimally spread accesses among a predetermined number of stripes.

> NOTE: 
>
> 一、"`AccessSpreader` uses these two to optimally spread accesses among a predetermined number of stripes"的意思: 
>
> "`AccessSpreader`使用这两种方法在预定数量的条带之间最佳地分散访问"
>
> 上面这段话中的"stripes"的含义是？后面提到了"striped data structures"，不知与此是否有关联
>
> "stripe"的表面意思是: "条纹"
>
> "spread accesses among a predetermined number of stripes" 这说明"stripe"是access的单位，结合具体的例子来说：
>
> 1、`IndexedMemPool` 的 `NumLocalLists`
>
> 
>
> 二、sysfs
>
> 1、quora [**What is sysfs and what is its usage?**](https://www.quora.com/What-is-sysfs-and-what-is-its-usage)
>
> 2、wikipedia [sysfs](https://en.wikipedia.org/wiki/Sysfs)
>
> 

`AccessSpreader<>::current(n)` microbenchmarks at 22 nanos, which is substantially less than the cost of a **cache miss**.  This means that we can effectively use it to reduce **cache line ping-pong** on **striped data structures** such as `IndexedMemPool` or statistics counters.

> NOTE: 
>
> 一、"striped data structures" 要如何理解？参见:
>
> 1、wikipedia [Data striping](https://en.wikipedia.org/wiki/Data_striping)
>
> 

Because `CacheLocality` looks at all of the cache levels, it can be used for different levels of optimization.  `AccessSpreader(2)` does per-chip spreading on a dual socket system.  `AccessSpreader(numCpus)` does perfect per-cpu spreading.  `AccessSpreader(numCpus / 2)` does perfect L1 spreading in a system with hyperthreading enabled.

> NOTE: 
>
> 一、关于"dual socket system"，参见: 
>
> 1、iiiff [什么是双插座主板(Dual Socket Motherboard)？](https://www.iiiff.com/article/317551)
>
> 上面这篇文章是下面这篇文章的翻译:
>
> easytechjunkie [What Is a Dual Socket Motherboard?](https://www.easytechjunkie.com/what-is-a-dual-socket-motherboard.htm)
>
> 二、上面这段话说明 `AccessSpreader` 是支持多种情况的spread的:
>
> 1、`AccessSpreader(2)` does per-chip spreading on a dual socket system
>
> 2、`AccessSpreader(numCpus)` does perfect per-cpu spreading
>
> 3、`AccessSpreader(numCpus / 2)` does perfect L1 spreading in a system with hyperthreading enabled
>
> 

## `AccessSpreader`

`AccessSpreader` arranges access to a **striped data structure** in such a way that concurrently executing threads are likely to be accessing different stripes.  It does NOT guarantee uncontended access. Your underlying algorithm must be thread-safe without spreading, this is merely an optimization.  `AccessSpreader::current(n)` is typically much faster than a **cache miss** (12 nanos on my dev box, tested fast in both 2.6 and 3.2 kernels).

> NOTE: 
>
> 一、仅仅是一种optimization方式，并不能够保证"uncontended access"，因此使用它并不能够保证消除contention，"Your underlying algorithm must be thread-safe without spreading"
>
> 二、它optimization的方式是: 
>
> "arranges access to a **striped data structure** in such a way that concurrently executing threads are likely to be accessing different stripes"
>
> 即尽可能地让并发执行的thread去"accessing different stripes"

If available (and not using the deterministic testing implementation) `AccessSpreader` uses the `getcpu` system call via `VDSO` and the precise **locality information** retrieved from `sysfs` by `CacheLocality`. This provides optimal **anti-sharing** at a fraction of the cost of a **cache miss**.

> NOTE: 
>
> 一、"This provides optimal anti-sharing at a fraction of the cost of a cache miss"的意思是: 
>
> "这以缓存未命中的一小部分成本提供了最佳的反共享"
>
> "spread"的含义是"分散"，其实就意味中"anti-sharing"

When there are not as many stripes as processors, we try to optimally place the cache sharing boundaries.  This means that if you have 2 stripes and run on a dual-socket system, your 2 stripes will each get all of the cores from a single socket.  If you have 16 stripes on a 16 core system plus hyperthreading (32 cpus), each core will get its own stripe and there will be no cache sharing at all.

> NOTE: 
>
> 一、"dual-socket system"意味中它有两个CPU
>
> 二、上面这段话其实是在强调 `AccessSpreader` 是会尽可能地分散的: 当 stripe 的数量比 processor 更多的时候，它会 "place the cache sharing boundaries" 以避免cache sharing

`AccessSpreader` has a fallback mechanism for when `__vdso_getcpu` can't be loaded, or for use during deterministic testing.  Using `sched_getcpu` or the `getcpu` syscall would negate the performance advantages of access spreading, so we use a thread-local value and a shared atomic counter to spread access out.  On systems lacking both a fast `getcpu()` and TLS, we hash the thread id to spread accesses.

> NOTE: 
>
> 上面这段话描述的是"fallback"

`AccessSpreader` is templated on the template type that is used to implement atomics, as a way to instantiate the underlying heuristics differently for production use and deterministic unit testing.  See `DeterministicScheduler` for more.  If you aren't using `DeterministicScheduler`, you can just use the default template parameter all of the time.

## Cache-Conscious Data Structures

显然，借助于它，我们是可以实现 "Cache-Conscious Data Structures"，"Cache-Conscious Data Structures"是在下面论文中提出的:

microsoft [Cache-Conscious Data Structures](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/12/ccds.pdf)
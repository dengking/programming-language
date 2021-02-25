# Full memory barrier/fence

## Release-acquire and full memory barrier 

在下面文章中讨论了这个问题: 

1、stackoverflow [What's the difference between InterlockedCompareExchange Release() and Acquire()?](https://stackoverflow.com/questions/9764181/whats-the-difference-between-interlockedcompareexchange-release-and-acquire)

[A](https://stackoverflow.com/a/9764313)

The plain version uses a **full barrier** while the suffixed(带后缀的) versions only deals with loads **or** stores, this can be faster on some CPUs (Itanium-based processors etc)

> NOTE: level

MSDN has a article about [Acquire and Release Semantics](http://msdn.microsoft.com/en-us/library/ff540496.aspx) and the [Interlocked* API](http://msdn.microsoft.com/en-us/library/windows/desktop/ms684122(v=vs.85).aspx) as well as [this great blog post](http://blogs.msdn.com/b/oldnewthing/archive/2008/10/03/8969397.aspx). The [Linux memory barrier documentation](http://www.kernel.org/doc/Documentation/memory-barriers.txt) might also be useful...

> NOTE: 上述  [this great blog post](http://blogs.msdn.com/b/oldnewthing/archive/2008/10/03/8969397.aspx) ，所链接的是下面的: microsoft [Acquire and release sound like bass fishing terms, but they also apply to memory models](https://devblogs.microsoft.com/oldnewthing/20081003-00/?p=20663).

2、stackoverflow [How to understand acquire and release semantics?](https://stackoverflow.com/questions/24565540/how-to-understand-acquire-and-release-semantics) 

[A](https://stackoverflow.com/a/24565699)

Acquire says "only worry about stuff after me". Release says "only worry about stuff before me". Combining those both is a **full memory barrier**.
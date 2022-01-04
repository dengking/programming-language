# Folly

在阅读dist-prog-book [Futures and Promises](http://dist-prog-book.com/chapter/2/futures.html) 的"Futures and Promises in Action"章节时，其中介绍了Folly对"asynchronous C++"的支持，遂引发了我对Folly的兴趣。



## github [facebook](https://github.com/facebook)/[folly](https://github.com/facebook/folly)



Performance concerns permeate(渗透、弥漫) much of Folly, sometimes leading to designs that are more idiosyncratic(特殊的、独特的) than they would otherwise be (see e.g. `PackedSyncPtr.h`, `SmallLocks.h`). Good performance at large scale is a unifying theme in all of Folly.

> NOTE: 
>
> 可以作为性能优化的参考

### Physical Design

At the top level Folly uses the classic "stuttering" scheme `folly/folly` used by Boost and others. The first directory serves as an installation root of the library (with possible versioning a la `folly-1.0/`), and the second is to distinguish the library when including files, e.g. `#include <folly/FBString.h>`.



## 官方文档

### github [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/**[docs](https://github.com/facebook/folly/tree/master/folly/docs)**/

### facebook [ELI5: Folly - Battle-Tested C++ Library](https://developers.facebook.com/blog/post/2021/05/24/eli5-folly-battle-tested-c-plus-plus-library/)



## Expert

1、[Lu Pan](https://blog.the-pans.com/about/)



## TODO

cnblogs [folly学习心得](https://www.cnblogs.com/Leo_wl/archive/2012/06/27/2566346.html)

> NOTE: 
>
> 其中对folly的component进行了说明

developer.51cto [揭秘Facebook官方底层C++函数Folly](https://developer.51cto.com/art/201206/340607.htm)

> NOTE: 
>
> 其中对folly的component进行了说明

the-pans [Global Data Locality – Why and How](https://blog.the-pans.com/cache-locality/)


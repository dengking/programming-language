# Reference counting



## wikipedia [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)



## wikipedia [Dealing with reference cycles](https://en.wikipedia.org/wiki/Reference_counting#Dealing_with_reference_cycles)

> NOTE: 第一次阅读的时候，并没有搞清楚reference cycle的含义，原文中的这样的一段话：
>
> the [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) framework, for instance, recommends using "strong" references for parent-to-child relationships and "weak" references for child-to-parent relationships.
>
> 当parent-to-child和child-to-parent都使用strong reference的时候，则两者相互引用，则两者都不可能的reference count都不可能成为0，则两者最终都无法被析构。



## Implementation

### spdlog

spdlog使用`std::shared_ptr`来实现reference counting。
# Object Swapping

> NOTE: Andrew Koenig的这个系列文章向我们揭示了swap的重要性

## [Object Swapping Part 1: Its Surprising Importance](https://www.drdobbs.com/cpp/object-swapping-part-1-its-surprising-im/232602568)

**I am beginning to believe that swapping is as fundamental an operation as copying, and that it may even be more fundamental than assignment.**

> NOTE: 
>
> 1、swap、copy、assignment
>
> 2、这一篇，这样强调swap的重要性

## [Object Swapping, Part 2: Algorithms Can Often Swap Instead of Copy](https://www.drdobbs.com/cpp/object-swapping-part-2-algorithms-can-of/232602979)

**Swapping is often faster than assignment — a fact that often allows algorithms that swap to be dramatically faster than they would be if they used assignment. Here's why.**

> NOTE: 
>
> 1、assignment VS copy
>
> 2、上面这段话，让我想起了: Erase-remove

## [Object Swapping, Part 3: Swapping and Moving](https://www.drdobbs.com/cpp/object-swapping-part-3-swapping-and-movi/232700458)

I would like to continue this series by discussing the notion of moving, rather than copying, values.

## [Object Swapping, Part 7: How Do You Call It?](https://www.drdobbs.com/cpp/object-swapping-part-7-how-do-you-call-i/232900925)

**We've talked about why swapping is important, and about how to use it to implement other operations such as assignment. We shall now look more closely at how to use it.**


# modernescpp [Acquire-Release Fences](https://www.modernescpp.com/index.php/acquire-release-fences)

Acquire and release fences guarantees similar [synchronisation and ordering constraints](https://www.modernescpp.com/index.php/synchronization-and-ordering-constraints) as atomics with [acquire-release semantic](https://www.modernescpp.com/index.php/acquire-release-semantic). Similar, because the differences are in the details.

The most obvious difference between **acquire and release memory barriers (fences)** and **atomics with acquire-release semantic** is that **memory barriers** need no operations on atomics. But there is a more subtle difference. The acquire and release memory barriers are more heavyweight.

> NOTE: 两种不同的technique:
>
> 1、**acquire and release memory barriers (fences)** 
>
> 2、**atomics with acquire-release semantic**

## Atomic operations versus memory barriers

To make my job of writing simpler, I will now simply speak of acquire operations, if I use memory barriers or atomic operations with acquire semantic. The same will hold for release operations.

The key idea of an acquire and a release operation is, that it establishes synchronisations and ordering constraints between thread. This will also hold for atomic operations with relaxed semantic or non-atomic operations. So you see, the acquire and release operations come in pairs. In addition, for the operations on atomic variables with acquire-release semantic must hold that these act on the same atomic variable. Said that I will in the first step look at these operations in isolation.

I start with the acquire operation.
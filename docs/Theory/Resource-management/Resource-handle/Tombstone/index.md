# wikipedia [Tombstone (programming)](https://en.wikipedia.org/wiki/Tombstone_(programming))

> NOTE: 
>
> 1、是在阅读 wikipedia [Handle (computing)](https://en.wikipedia.org/wiki/Handle_(computing)) 时，发现的[tombstone](https://en.wikipedia.org/wiki/Tombstone_(programming)).
>
> 2、"tombstone"的表明意思是"墓石"。
>
> 3、Tombstone可以看做是handle的实现，它是abstraction layer、intermediary
>
> 4、tombstone非常类似于virtual address，它们背后的思想都是abstraction

**Tombstones** are a mechanism to detect [dangling pointers](https://en.wikipedia.org/wiki/Dangling_pointer) that can appear in certain computer [programming languages](https://en.wikipedia.org/wiki/Programming_language), e. g. [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B) and [assembly languages](https://en.wikipedia.org/wiki/Assembly_language), and to act as a containment(围堵策略) to their dangerous effects.

## Implementation

A tombstone is a structure that acts as an intermediary(中间人) between a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) and the [heap-dynamic](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) data in [memory](https://en.wikipedia.org/wiki/Random_access_memory). The pointer – sometimes called the [handle](https://en.wikipedia.org/wiki/Handle_(computing)) – points only at tombstones and never to the memory that holds the actual value. When the data is deallocated, the tombstone is set to a [null](https://en.wikipedia.org/wiki/Null_pointer) (or, more generally, to a value that is illegal for a pointer in the given runtime environment), indicating that the variable no longer exists. This prevents the use of invalid pointers, which would otherwise access the memory area that once belonged to the now deallocated variable, although it may already contain other data, in turn leading to corruption of in-memory data. Depending on the [operating system](https://en.wikipedia.org/wiki/Operating_system), the [CPU](https://en.wikipedia.org/wiki/Central_processing_unit) can automatically detect such an invalid access (e. g. for the null value: a ***null pointer dereference error***). This supports in analyzing the actual reason, a programming error, in [debugging](https://en.wikipedia.org/wiki/Debugging), and it can also be used to abort the program in production use, to prevent it from continuing with invalid data structures.

## Downside

> NOTE: 
>
> 1、performance tradeoff

The downsides of using tombstones include a [computational overhead](https://en.wikipedia.org/wiki/Computational_overhead) and additional memory consumption: extra processing is necessary to follow the path from the pointer to data through the tombstone, and extra memory is necessary to retain tombstones for every pointer throughout the program. One other problem is that all the code that needs to work with the pointers in question needs to be implemented to use the tombstone mechanism.
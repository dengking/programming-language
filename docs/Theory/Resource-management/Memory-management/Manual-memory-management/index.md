# Manual memory management

“manual memory management”即“手工内存管理”

## wikipedia [Manual memory management](https://en.wikipedia.org/wiki/Manual_memory_management)



## Implementation of manual memory management

manual memory management的所有工作由programmer来完成，所以对programmer的要求较高，需要programmer对process的memory model（参见工程[Linux-OS](https://dengking.github.io/Linux-OS/)）有一定了解。在进行programming的时候，memory resource用“variable”来表示，可以通过variable概念来分析manual memory management。



### Memory management of c++

参见维基百科[C++](https://en.wikipedia.org/wiki/C%2B%2B)的[Object storage](https://en.wikipedia.org/wiki/C%2B%2B#Object_storage)段。



## Problem in manual memory management 

手工memory management中，可能存在多种问题：

在维基百科[Garbage collection (computer science)](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))中进行了总结：

1、*[Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer) bugs*, which occur when a piece of memory is freed while there are still [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) to it, and one of those pointers is [dereferenced](https://en.wikipedia.org/wiki/Dereference_operator). By then the memory may have been reassigned to another use, with unpredictable results.

2、*Double free bugs*, which occur when the program tries to free a region of memory that has already been freed, and perhaps already been allocated again.

3、Certain kinds of *[memory leaks](https://en.wikipedia.org/wiki/Memory_leak)*, in which a program fails to free memory occupied by objects that have become [unreachable](https://en.wikipedia.org/wiki/Unreachable), which can lead to memory exhaustion. (Garbage collection typically[*[who?](https://en.wikipedia.org/wiki/Wikipedia:Manual_of_Style/Words_to_watch#Unsupported_attributions)*] does not deal with the unbounded accumulation of data that is reachable, but that will actually not be used by the program.)



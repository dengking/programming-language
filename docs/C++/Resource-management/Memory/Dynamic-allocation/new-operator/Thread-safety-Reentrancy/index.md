# Thread-safety and reentrancy of new operator

## 分析

一、new operator最终还是会使用类似于`malloc`的allocator，因此它的reentrancy是由allocator决定的，一般allocator是non-reentrant的

二、thread safety，C++标准没有明确说明；并且由于C++支持overload new operator，在这种情况下就需要由programmer来决定了。

## stackoverflow [Thread safety of ::new in C++11](https://stackoverflow.com/questions/26375732/thread-safety-of-new-in-c11)



### [A](https://stackoverflow.com/a/26378187)

I believe this is *implicitly* guaranteed by the C++11 standard. If it were not, then usage of the operator new or new expression might cause a data race, and that would not be allowed by the standard. For reference, see *§17.6.5.9 Data race avoidance* and also

**18.6.1.4 Data races [new.delete.dataraces]**

"The library versions of operator new and operator delete, user replacement versions of global operator new and operator delete, and the C standard library functions calloc, malloc, realloc, and free shall not introduce data races (1.10) as a result of concurrent calls from different threads. Calls to these functions that allocate or deallocate a particular unit of storage shall occur in a single total order, and each such deallocation call shall happen before the next allocation (if any) in this order."

Your own overrides or your own replacements for the global operators *should* fulfill this requirement as well.

See also this [proposal N3664 "Clarifying Memory Allocation"](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3664.html), which puts more emphasis on that matter.



## stackoverflow [Thread safety for overloaded operator new](https://stackoverflow.com/questions/7739144/thread-safety-for-overloaded-operator-new)
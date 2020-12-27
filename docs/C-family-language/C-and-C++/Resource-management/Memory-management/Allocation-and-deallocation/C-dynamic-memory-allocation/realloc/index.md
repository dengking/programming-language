# [realloc](https://en.cppreference.com/w/c/memory/realloc)

Defined in header `<stdlib.h>`

```C
void *realloc( void *ptr, size_t new_size );
```



Reallocates the given area of memory. It must be previously allocated by [malloc()](https://en.cppreference.com/w/c/memory/malloc), [calloc()](https://en.cppreference.com/w/c/memory/calloc) or `realloc()` and not yet freed with a call to [free](https://en.cppreference.com/w/c/memory/free) or `realloc`. Otherwise, the results are undefined.

The reallocation is done by either:

a) expanding or contracting（缩小） the existing **area** pointed to by `ptr`, if possible. The contents of the area（由`ptr`所指向的区域） remain unchanged up to the lesser of the new and old sizes（the lesser of A and B的意思是A和B中的小者）. If the area is expanded, the contents of the new part of the array are undefined.



> NOTE : `new_size`可能大于`ptr`所指向的区域的长度，也可能小于`ptr`所指向区域的长度；即`realloc`即可用expand，也可以contract；那么新分配的空间的初始值是什么呢？最后两句话给出了解释。



b) allocating a new memory block of size `new_size` bytes, copying memory area with size equal the lesser of the new and the old sizes, and freeing the old block.

If there is not enough memory, the **old memory block** is not freed and null pointer is returned.

If `ptr` is [NULL](https://en.cppreference.com/w/c/types/NULL), the behavior is the same as calling [malloc](https://en.cppreference.com/w/c/memory/malloc)(`new_size`).

If `new_size` is zero, the behavior is **implementation defined** (null pointer may be returned (in which case the old memory block may or may not be freed), or some **non-null pointer** may be returned that may not be used to access storage).


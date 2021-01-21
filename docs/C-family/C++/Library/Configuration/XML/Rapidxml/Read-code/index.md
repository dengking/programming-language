# Read code

## [class template rapidxml::memory_pool](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool)

Defined in [rapidxml.hpp](http://rapidxml.sourceforge.net/rapidxml.hpp)
Base class for [xml_document](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1xml__document)

### Description

This class is used by the parser to create new nodes and attributes, without overheads of **dynamic memory allocation**. In most cases, you will not need to use this class directly. However, if you need to create nodes manually or modify names/values of nodes, you are encouraged to use [memory_pool](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool) of relevant [xml_document](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1xml__document) to allocate the memory. Not only is this faster than allocating them by using `new` operator, but also their lifetime will be tied to the lifetime of document, possibly simplyfing memory management.

> NOTE: "simplyfing memory management"是使用memory pool的一种优势；从下面的描述可以看出: 
>
> 1、`rapidxml::memory_pool`只提供了allocation，没有提供deallocation，显然这样的设计大大简化了实现，因为一旦考虑deallocation，那么这将是一个非常复杂的工程了
>
> 2、使用了placement new

Call [allocate_node()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_750ba3c610b129ac057d817509d08f41_1750ba3c610b129ac057d817509d08f41) or [allocate_attribute()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_462de142669e0ff649e8e615b82bf457_1462de142669e0ff649e8e615b82bf457) functions to obtain new nodes or attributes from the pool. You can also call [allocate_string()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_69729185bc59b0875192d667c47b8859_169729185bc59b0875192d667c47b8859) function to allocate strings. Such strings can then be used as names or values of nodes without worrying about their lifetime. Note that there is no `free()` function -- all allocations are freed at once when [clear()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_c8bb3912a3ce86b15842e79d0b421204_1c8bb3912a3ce86b15842e79d0b421204) function is called, or when the pool is destroyed.

> NOTE: 如果加上`free`，那么这将是一个非常复杂的工程。

It is also possible to create a standalone [memory_pool](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool), and use it to allocate nodes, whose lifetime will not be tied to any document.

Pool maintains `RAPIDXML_STATIC_POOL_SIZE` bytes of statically allocated memory. Until static memory is exhausted, no dynamic memory allocations are done. When static memory is exhausted, pool allocates additional blocks of memory of size `RAPIDXML_DYNAMIC_POOL_SIZE` each, by using global `new[]` and `delete[]` operators. This behaviour can be changed by setting custom allocation routines. Use [set_allocator()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_c0a55a6ef0837dca67572e357100d78a_1c0a55a6ef0837dca67572e357100d78a) function to set them.

> NOTE: static and dynamic memory

Allocations for nodes, attributes and strings are aligned at `RAPIDXML_ALIGNMENT` bytes. This value defaults to the size of pointer on target architecture.

> NOTE: 这样做的目的是什么？

To obtain absolutely top performance from the parser, it is important that all nodes are allocated from a single, contiguous block of memory. Otherwise, cache misses when jumping between two (or more) disjoint blocks of memory can slow down parsing quite considerably. If required, you can tweak `RAPIDXML_STATIC_POOL_SIZE`, `RAPIDXML_DYNAMIC_POOL_SIZE` and `RAPIDXML_ALIGNMENT` to obtain best wasted memory to performance compromise. To do it, define their values before [rapidxml.hpp](http://rapidxml.sourceforge.net/manual.html#rapidxml_8hpp) file is included.

> NOTE: cache Miss





### macro

```C++
// Pool sizes

#ifndef RAPIDXML_STATIC_POOL_SIZE
    // Size of static memory block of memory_pool.
    // Define RAPIDXML_STATIC_POOL_SIZE before including rapidxml.hpp if you want to override the default value.
    // No dynamic memory allocations are performed by memory_pool until static memory is exhausted.
    #define RAPIDXML_STATIC_POOL_SIZE (64 * 1024)
#endif

#ifndef RAPIDXML_DYNAMIC_POOL_SIZE
    // Size of dynamic memory block of memory_pool.
    // Define RAPIDXML_DYNAMIC_POOL_SIZE before including rapidxml.hpp if you want to override the default value.
    // After the static block is exhausted, dynamic blocks with approximately this size are allocated by memory_pool.
    #define RAPIDXML_DYNAMIC_POOL_SIZE (64 * 1024)
#endif

#ifndef RAPIDXML_ALIGNMENT
    // Memory allocation alignment.
    // Define RAPIDXML_ALIGNMENT before including rapidxml.hpp if you want to override the default value, which is the size of pointer.
    // All memory allocations for nodes, attributes and strings will be aligned to this value.
    // This must be a power of 2 and at least 1, otherwise memory_pool will not work.
    #define RAPIDXML_ALIGNMENT sizeof(void *)
#endif
```



### `class memory_pool`



#### 数据结构

```C++
struct header
{
    char *previous_begin;
};

char *m_begin;// Start of raw memory making up current pool
char *m_ptr;// First free byte in current pool
char *m_end;// One past last available byte in current pool
char m_static_memory[RAPIDXML_STATIC_POOL_SIZE];    // Static raw memory
```



#### `char *align(char *ptr)`

```c++
char *align(char *ptr)
{
    std::size_t alignment = ((RAPIDXML_ALIGNMENT - (std::size_t(ptr) & (RAPIDXML_ALIGNMENT - 1))) & (RAPIDXML_ALIGNMENT - 1));
    return ptr + alignment;
}
        
```

下面的描述以假设`RAPIDXML_ALIGNMENT`为16($2^4$)为前提进行展开，显然，`align`的目的是: 使返回值的后`4`位为0，显然这样它就是multiple of 16:

1、由`ptr + alignment`可知，`alignment`表示`ptr`需要向后移动多少位；

2、 `(std::size_t(ptr) & (RAPIDXML_ALIGNMENT - 1))` 是取`ptr`的后四位；

3、`(RAPIDXML_ALIGNMENT - (std::size_t(ptr) & (RAPIDXML_ALIGNMENT - 1))`

计算的是 要向前移动多少位，后四位才能够归0，这样才能够被16整除。

4、最后的 `& (RAPIDXML_ALIGNMENT - 1)` 表示取后四位；



####  `void *allocate_aligned(std::size_t size)`



```C++
 void *allocate_aligned(std::size_t size)
 {
     // Calculate aligned pointer
     char *result = align(m_ptr);

     // If not enough memory left in current pool, allocate a new pool
     if (result + size > m_end)
     {
         // Calculate required pool size (may be bigger than RAPIDXML_DYNAMIC_POOL_SIZE)
         std::size_t pool_size = RAPIDXML_DYNAMIC_POOL_SIZE;
         if (pool_size < size)
             pool_size = size;

         // Allocate
         std::size_t alloc_size = sizeof(header) + (2 * RAPIDXML_ALIGNMENT - 2) + pool_size;     // 2 alignments required in worst case: one for header, one for actual allocation
         char *raw_memory = allocate_raw(alloc_size);

         // Setup new pool in allocated memory
         char *pool = align(raw_memory);
         header *new_header = reinterpret_cast<header *>(pool);
         new_header->previous_begin = m_begin;
         m_begin = raw_memory;
         m_ptr = pool + sizeof(header);
         m_end = raw_memory + alloc_size;

         // Calculate aligned pointer again using new pool
         result = align(m_ptr);
     }

     // Update pool and return aligned pointer
     m_ptr = result + size;
     return result;
 }
```

> NOTE: 上面这段代码让我想到了:
>
> 1、cppreference object中的placement
>
> 2、placement new
>
> 3、object 的lifetime is bound by its storage
>
> 
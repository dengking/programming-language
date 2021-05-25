# Align to

# stackoverflow [How to allocate aligned memory only using the standard library?](https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library)

I just finished a test as part of a job interview, and one question stumped me, even using Google for reference. I'd like to see what the StackOverflow crew can do with it:

> The `memset_16aligned` function requires a 16-byte aligned pointer passed to it, or it will crash.
>
> a) How would you allocate 1024 bytes of memory, and align it to a 16 byte boundary?
> b) Free the memory after the `memset_16aligned` has executed.

```c
{    
   void *mem;
   void *ptr;

   // answer a) here

   memset_16aligned(ptr, 0, 1024);

   // answer b) here    
}
```

## [A](https://stackoverflow.com/a/227900)

### Original answer

```c
{
    void *mem = malloc(1024+16);
    void *ptr = ((char *)mem+16) & ~ 0x0F;
    memset_16aligned(ptr, 0, 1024);
    free(mem);
}
```

> NOTE: 测试程序如下:
>
> ```C
> #include <stdlib.h>
> void memset_16aligned(void *ptr, int foo, int bar)
> {
> }
> void test()
> {
> 	void *mem = malloc(1024 + 16);
> 	void *ptr = ((char*) mem + 16) & ~0x0F;
> 	memset_16aligned(ptr, 0, 1024);
> 	free(mem);
> }
> int main()
> {
> 	test();
> }
> // gcc test.c
> ```
>
> 编译报错如下:
>
> ```C
> test.c: 在函数‘test’中:
> test.c:8:33: 错误：双目运算符 & 操作数(‘char *’和‘int’)无效
>   void *ptr = ((char*) mem + 16) & ~0x0F;
> ```
>
> 

### Fixed answer

```c
{
    void *mem = malloc(1024+15);
    void *ptr = ((uintptr_t)mem+15) & ~ (uintptr_t)0x0F;
    memset_16aligned(ptr, 0, 1024);
    free(mem);
}
```

> NOTE: 
>
> 1、上述`~`是**逻辑非运算符**。
>
> 2、`0x0F` = 15 = 16 - 1 = alignment - 1
>
> 3、关于为什么使用`~ (uintptr_t)0x0F`，在下面的**Comments**中，有介绍；
>
> 测试程序如下:
>
> ```c
> #include <stdlib.h>
> #include <stdint.h>//uintptr_t
> void memset_16aligned(void *ptr, int foo, int bar)
> {
> }
> void test()
> {
> 	void *mem = malloc(1024 + 15);
> 	void *ptr = ((uintptr_t) mem + 15) & ~(uintptr_t) 0x0F;
> 	memset_16aligned(ptr, 0, 1024);
> 	free(mem);
> }
> int main()
> {
> 	test();
> }
> // gcc test.c
> ```
>
> 编译警告如下:
>
> ```C
> test.c: 在函数‘test’中:
> test.c:9:14: 警告：初始化时将整数赋给指针，未作类型转换 [默认启用]
>   void *ptr = ((uintptr_t) mem + 15) & ~(uintptr_t) 0x0F;
> ```
>
> 

### Explanation as requested

The first step is to allocate enough spare space, just in case. Since the memory must be 16-byte aligned (meaning that the leading(起始的) byte address needs to be a multiple of 16), adding 16 extra bytes guarantees that we have enough space. Somewhere in the first 16 bytes, there is a 16-byte aligned pointer. (Note that `malloc()` is supposed to return a pointer that is sufficiently well aligned for *any* purpose. However, the meaning of 'any' is primarily for things like basic types — `long`, `double`, `long double`, `long long`, and pointers to objects and pointers to functions. When you are doing more specialized things, like playing with graphics systems, they can need more stringent(严厉的) alignment than the rest of the system — hence questions and answers like this.)

> NOTE: 在`mem`所指向的memory region的前16字节中，存在地址满足16 alignment的；

The next step is to convert the void pointer to a char pointer; GCC notwithstanding(固然), you are not supposed to do pointer arithmetic on void pointers (and GCC has warning options to tell you when you abuse it). 

> NOTE: 上面这段话解释了`(char *)mem`的原因

Then add 16 to the start pointer. Suppose `malloc()` returned you an impossibly badly aligned pointer: `0x800001`. Adding the 16 gives `0x800011`. Now I want to round down to the 16-byte boundary — so I want to reset the last 4 bits to 0. `0x0F` has the last 4 bits set to one; therefore, `~0x0F` has all bits set to one except the last four. Anding that with `0x800011` gives `0x800010`. You can iterate over the other offsets and see that the same arithmetic works.

> NOTE: 
>
> 1、只要地址值的最后4 bit是0，那么它就满足16 alignment；
>
> 2、需要起始地址满足16 alignment；
>
> 3、将地址值的最后四位置为0，其实是将它缩小了；也就是将它后退了；一旦后退，那么就可能导致它后退到不属于自己的区域；因此，需要首先`+16`来确保后退依然在自己的区域；

The last step, `free()`, is easy: you always, and only, return to `free()` a value that one of `malloc()`, `calloc()` or `realloc()` returned to you — anything else is a disaster. You correctly provided `mem` to hold that value — thank you. The free releases it.

Finally, if you know about the internals of your system's `malloc` package, you could guess that it might well return 16-byte aligned data (or it might be 8-byte aligned). If it was 16-byte aligned, then you'd not need to dink with the values. However, this is dodgy and non-portable — other `malloc` packages have different minimum alignments, and therefore assuming one thing when it does something different would lead to core dumps. Within broad limits, this solution is portable.

> NOTE: 最后，如果您了解系统' malloc '包的内部结构，那么您可以猜测它很可能返回16字节对齐的数据(或者8字节对齐的数据)。
> 如果它是16字节对齐的，那么您就不需要与值丁克。
> 然而，这是危险的和不可移植的——其他的“malloc”包有不同的最小对齐，因此当它做一些不同的事情时，假设一件事将导致核心转储。
> 在很大的范围内，这个解决方案是可移植的。

Someone else mentioned `posix_memalign()` as another way to get the aligned memory; that isn't available everywhere, but could often be implemented using this as a basis. Note that it was convenient that the alignment was a power of 2; other alignments are messier.

One more comment — this code does not check that the allocation succeeded.

### Amendment(修正案)

[Windows Programmer](https://stackoverflow.com/users/23705/windows-programmer) pointed out that you can't do bit mask operations on pointers, and, indeed, GCC (3.4.6 and 4.3.1 tested) does complain like that. So, an amended version of the basic code — converted into a main program, follows. I've also taken the liberty of adding just 15 instead of 16, as has been pointed out. 

> NOTE: 为什么是15而不是16？

I'm using `uintptr_t` since C99 has been around long enough to be accessible on most platforms. If it wasn't for the use of `PRIXPTR` in the `printf()` statements, it would be sufficient to `#include <stdint.h>` instead of using `#include <inttypes.h>`. *[This code includes the fix pointed out by [C.R.](https://stackoverflow.com/users/832878/c-r), which was reiterating a point first made by [Bill K](https://stackoverflow.com/users/12943/bill-k) a number of years ago, which I managed to overlook until now.]*

```c
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void memset_16aligned(void *space, char byte, size_t nbytes)
{
    assert((nbytes & 0x0F) == 0);
    assert(((uintptr_t)space & 0x0F) == 0);
    memset(space, byte, nbytes);  // Not a custom implementation of memset()
}

int main(void)
{
    void *mem = malloc(1024+15);
    void *ptr = (void *)(((uintptr_t)mem+15) & ~ (uintptr_t)0x0F);
    printf("0x%08" PRIXPTR ", 0x%08" PRIXPTR "\n", (uintptr_t)mem, (uintptr_t)ptr);
    memset_16aligned(ptr, 0, 1024);
    free(mem);
    return(0);
}
// gcc test.c
```

> NOTE: 上述code，没有任何warning。

And here is a marginally more generalized version, which will work for sizes which are a power of 2:

```c
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void memset_16aligned(void *space, char byte, size_t nbytes)
{
    assert((nbytes & 0x0F) == 0);
    assert(((uintptr_t)space & 0x0F) == 0);
    memset(space, byte, nbytes);  // Not a custom implementation of memset()
}

static void test_mask(size_t align)
{
    uintptr_t mask = ~(uintptr_t)(align - 1);
    void *mem = malloc(1024+align-1);
    void *ptr = (void *)(((uintptr_t)mem+align-1) & mask);
    assert((align & (align - 1)) == 0);
    printf("0x%08" PRIXPTR ", 0x%08" PRIXPTR "\n", (uintptr_t)mem, (uintptr_t)ptr);
    memset_16aligned(ptr, 0, 1024);
    free(mem);
}

int main(void)
{
    test_mask(16);
    test_mask(32);
    test_mask(64);
    test_mask(128);
    return(0);
}
```

To convert `test_mask()` into a general purpose allocation function, the single return value from the allocator would have to encode the release address, as several people have indicated in their answers.

### Problems with interviewers

[Uri](https://stackoverflow.com/users/23072/uri) commented: Maybe I am having [a] reading comprehension problem this morning, but if the interview question specifically says: "How would you allocate 1024 bytes of memory" and you clearly allocate more than that. Wouldn't that be an automatic failure from the interviewer?

My response won't fit into a 300-character comment...

It depends, I suppose. I think most people (including me) took the question to mean "How would you allocate a space in which 1024 bytes of data can be stored, and where the base address is a multiple of 16 bytes". If the interviewer really meant how can you allocate 1024 bytes (only) and have it 16-byte aligned, then the options are more limited.

- Clearly, one possibility is to allocate 1024 bytes and then give that address the 'alignment treatment'; the problem with that approach is that the actual available space is not properly determinate (the usable space is between 1008 and 1024 bytes, but there wasn't a mechanism available to specify which size), which renders it less than useful.
- Another possibility is that you are expected to write a full memory allocator and ensure that the 1024-byte block you return is appropriately aligned. If that is the case, you probably end up doing an operation fairly similar to what the proposed solution did, but you hide it inside the allocator.

However, if the interviewer expected either of those responses, I'd expect them to recognize that this solution answers a closely related question, and then to reframe their question to point the conversation in the correct direction. (Further, if the interviewer got really stroppy, then I wouldn't want the job; if the answer to an insufficiently precise requirement is shot down in flames without correction, then the interviewer is not someone for whom it is safe to work.)

### The world moves on

The title of the question has changed recently. It was *Solve the memory alignment in C interview question that stumped me*. The revised title (*How to allocate aligned memory only using the standard library?*) demands a slightly revised answer — this addendum provides it.

C11 (ISO/IEC 9899:2011) added function `aligned_alloc()`:

> **7.22.3.1 The `aligned_alloc` function**
>
> **Synopsis**
>
> ```c
> #include <stdlib.h>
> void *aligned_alloc(size_t alignment, size_t size);
> ```
>
> **Description**
> The `aligned_alloc` function allocates space for an object whose alignment is specified by `alignment`, whose size is specified by `size`, and whose value is indeterminate. The value of `alignment` shall be a valid alignment supported by the implementation and the value of `size` shall be an integral multiple of `alignment`.
>
> **Returns**
> The `aligned_alloc` function returns either a null pointer or a pointer to the allocated space.

And POSIX defines [`posix_memalign()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/posix_memalign.html):

> ```c
> #include <stdlib.h>
> 
> int posix_memalign(void **memptr, size_t alignment, size_t size);
> ```
>
> DESCRIPTION
>
> The `posix_memalign()` function shall allocate `size` bytes aligned on a boundary specified by `alignment`, and shall return a pointer to the allocated memory in `memptr`. The value of `alignment` shall be a power of two multiple of `sizeof(void *)`.
>
> Upon successful completion, the value pointed to by `memptr` shall be a multiple of `alignment`.
>
> If the size of the space requested is 0, the behavior is implementation-defined; the value returned in `memptr` shall be either a null pointer or a unique pointer.
>
> The `free()` function shall deallocate memory that has previously been allocated by `posix_memalign()`.
>
> RETURN VALUE
>
> Upon successful completion, `posix_memalign()` shall return zero; otherwise, an error number shall be returned to indicate the error.

Either or both of these could be used to answer the question now, but only the POSIX function was an option when the question was originally answered.

Behind the scenes, the new aligned memory function do much the same job as outlined in the question, except they have the ability to force the alignment more easily, and keep track of the start of the aligned memory internally so that the code doesn't have to deal with specially — it just frees the memory returned by the allocation function that was used.

### **Comments**

#### `~ 0x0F`

And i'm rusty(生疏) with C++, but I don't really trust that `~ 0x0F` will properly expand to the size of the pointer. If it doesn't, all hell will break loose because you will mask off the most significant bits of your pointer as well. I could be wrong about that though. – [Bill K](https://stackoverflow.com/users/12943/bill-k) [Oct 22 '08 at 23:48](https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library#comment94644_227900)

#### `+15`

BTW '+15' works as well as '+16'...no practical impact in this situation though. – [Menkboy](https://stackoverflow.com/users/29539/menkboy) [Oct 22 '08 at 23:50](https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library#comment94647_227900)

The '+ 15' comments from Menkboy and Greg are correct, but malloc() would almost certainly round that up to 16 anyway. Using +16 is marginally easier to explain. The generalized solution is fiddly, but doable. – [Jonathan Leffler](https://stackoverflow.com/users/15168/jonathan-leffler) [Oct 22 '08 at 23:59](https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library#comment94667_227900)



## [A](https://stackoverflow.com/a/228015)

Three slightly different answers depending how you look at the question:

1) Good enough for the exact question asked is Jonathan Leffler's solution, except that to round up to 16-aligned, you only need 15 extra bytes, not 16.

A:

```c
/* allocate a buffer with room to add 0-15 bytes to ensure 16-alignment */
void *mem = malloc(1024+15);
ASSERT(mem); // some kind of error-handling code
/* round up to multiple of 16: add 15 and then round down by masking */
void *ptr = ((char*)mem+15) & ~ (size_t)0x0F;
```

B:

```c
free(mem);
```

2) For a more generic memory allocation function, the caller doesn't want to have to keep track of two pointers (one to use and one to free). So you store a pointer to the 'real' buffer below the aligned buffer.

A:

```c
void *mem = malloc(1024+15+sizeof(void*));
if (!mem) return mem;
void *ptr = ((char*)mem+sizeof(void*)+15) & ~ (size_t)0x0F;
((void**)ptr)[-1] = mem;
return ptr;
```

B:

```c
if (ptr) free(((void**)ptr)[-1]);
```

Note that unlike (1), where only 15 bytes were added to `mem`, this code could actually *reduce* the alignment if your implementation happens to guarantee 32-byte alignment from `malloc` (unlikely, but in theory a C implementation could have a 32-byte aligned type). That doesn't matter if all you do is call `memset_16aligned`, but if you use the memory for a struct then it could matter.

I'm not sure off-hand what a good fix is for this (other than to warn the user that the buffer returned is not necessarily suitable for arbitrary structs) since there's no way to determine programatically what the implementation-specific alignment guarantee is. I guess at startup you could allocate two or more 1-byte buffers, and assume that the worst alignment you see is the guaranteed alignment. If you're wrong, you waste memory. Anyone with a better idea, please say so...

[*Added*: The 'standard' trick is to create a union of 'likely to be maximally aligned types' to determine the requisite alignment. The maximally aligned types are likely to be (in C99) '`long long`', '`long double`', '`void *`', or '`void (*)(void)`'; if you include `<stdint.h>`, you could presumably use '`intmax_t`' in place of `long long` (and, on Power 6 (AIX) machines, `intmax_t` would give you a 128-bit integer type). The alignment requirements for that union can be determined by embedding it into a struct with a single char followed by the union:

```c
struct alignment
{
    char     c;
    union
    {
        intmax_t      imax;
        long double   ldbl;
        void         *vptr;
        void        (*fptr)(void);
    }        u;
} align_data;
size_t align = (char *)&align_data.u.imax - &align_data.c;
```

You would then use the larger of the requested alignment (in the example, 16) and the `align` value calculated above.

On (64-bit) Solaris 10, it appears that the basic alignment for the result from `malloc()` is a multiple of 32 bytes.
]

In practice, aligned allocators often take a parameter for the alignment rather than it being hardwired. So the user will pass in the size of the struct they care about (or the least power of 2 greater than or equal to that) and all will be well.

3) Use what your platform provides: `posix_memalign` for POSIX, `_aligned_malloc` on Windows.

4) If you use C11, then the cleanest - portable and concise - option is to use the standard library function [`aligned_alloc`](http://en.cppreference.com/w/c/memory/aligned_alloc) that was introduced in this version of the language specification.



# jongampark.wordpress [Implementation of aligned memory allocation](https://jongampark.wordpress.com/2008/06/12/implementation-of-aligned-memory-alloc/)

Thanks to evolution in CPU architecture, you have super computers in your house. However, to get benefit from it, it requires some techniques. There are something new, while there are something very traditional.

One of such traditional approach is to use **aligned memory allocation**. Because CPU access memory most efficiently when accessing in certain unit, it is better if the allocated memory lies in the certain unit boundary. For example, let’s assume a CPU architecture is most efficient when memory is aligned every 4 bytes. When it accesses memory located at multiples of 4 in its address space, it is much faster than when the accessed memory is at `0x03` or `0x05`.

The Unix `malloc` functions usually return aligned memory space, while Windows version doesn’t. Instead, the Windows provide `_aligned_malloc()`.

## How to create a `aligned malloc()` function?

Then, how to create a `aligned malloc()` function? There can be some special cases that you want to implement your own aligned `malloc`, although I don’t imagine such a case. Let’s figure out how to by looking at one of existing implementations. ( You can search one using the Google, and you will find out that they are similar.)

```C
// size : the size of allocated memory
//        The actual size of allocation will be greater than this size.
// alignment : the alignment boundary
void *aligned_memory_alloc( size_t size, size_t alignment )
{
    void *pa, *ptr;
 
    //pa=malloc(((size+alignment-1)&~(alignment-1))+sizeof(void *)+alignment-1);
     
    // 1
    pa=malloc((size+alignment-1)+sizeof(void *));
    if(!pa)
        return NULL;
 
    // 2
    ptr=(void*)( ((ULONG_PTR)pa+sizeof(void *)+alignment-1)&~(alignment-1) );
 
    // 3
    *((void **)ptr-1)=pa;
     
    printf("CAlignedAlloc::new(%d,%d)=%x\n", (ULONG)size, alignment, (ULONG)ptr);
 
    return ptr;
}
```

Point is to allocate more space than required and make it point to some position in the allocated memory. The pointed position is aligned location.

At 1, the total space to allocate is :

`size` ; the space a user want to allocate
`+ (alignment-1)` ; additional space due to the alignment
`+ sizeof (void *)` ; The head location of the newly allocated memory
; contains an address where the aligned memory block starts

The `size` part is obvious. The `sizeof (void *)` part follows the design of **aligned memory allocation**. Without this part, it will not know from where to free the **aligned memory space**, and from where to access the memory to read/write from/to the space.

### `(alignment-1)`

For the `(alignment-1)`, please take a look at this picture.

[![img](https://i0.wp.com/jongampark.files.wordpress.com/2008/06/how_it_is_relocated.jpg)](https://jongampark.files.wordpress.com/2008/06/how_it_is_relocated.jpg)

The red arrows show what the destination address should be if the allocated memory is 1, 2, 3 or 5, 6, 7. They are relocated to 4 and 8, respectively. So, it can be shifted up to 3 slots to the right. So, it is (`alignment-1`)

At 2, the `ptr` points to the location of aligned place. After reserving the space for storing where the whole allocated memory block is, i.e. pa, it calculates the aligned location. If you look at the picture above, you will see why (`alignment-1`) is added and the address is “AND”ed with 1’s complement of the (alignment-1). For example, 4-bytes alignment means masking out the last 2bits. It is like removing the last 2 bits.

At 3, “address length” bytes before, it saves the address which points where the whole allocated memory starts. Why it uses (`void **`) instead of (`void *`) is because the pointer (`ptr-1`) is points to an address, which is a pointer to pointer.

[![img](https://i1.wp.com/jongampark.files.wordpress.com/2008/06/how_starting_address_is_saved.jpg)](https://jongampark.files.wordpress.com/2008/06/how_starting_address_is_saved.jpg)

And finally it returns the aligned memory location, `ptr`.

## How about the free() function? 

How about the `free()` function? You can’t free the address to the aligned position. The whole memory space should be freed. That is why the starting address of the whole memory space was saved above.

```C++
void aligned_free( void *ptr )
{
    printf("CAlignedAlloc::free(%x)\n", (ULONG)ptr);
    if(ptr)
        free(*((void **)ptr-1));
}
```

At Just 1 address-width, i.e. 4bytes for 32bit CPU and 8bytes for 64bit CPU, before the location of the aligned space, it contains the starting address of the whole block.
It is better to use (void **) or (void *) to calculate how much space is required to save a pointer, because it works for 64bit architecture as well as 32bit architecture. Actually it works for any architecture.

# embeddedartistry [Generating Aligned Memory](https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/)



# malloc and memory alignment

https://zhuanlan.zhihu.com/p/38276637

https://stackoverflow.com/a/227900
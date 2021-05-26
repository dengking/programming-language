# `malloc`



## creference [malloc](https://en.cppreference.com/w/c/memory/malloc)



```C++
void* malloc( size_t size );
```

Allocates `size` bytes of uninitialized storage.

If allocation succeeds, returns a pointer that is suitably aligned for any object type with [fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment).

> NOTE: 
>
> 一、首先需要对上面这段话的结构进行说明:  "[fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment)"所修饰的是"object type"，"[fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment)"其实就是alignment requirement、默认的alignment，比如`char`是1、`int`是4。
>
> 上面这段话的意思是: `malloc`的返回值是能够保证所有的"object type with [fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment)" "suitably aligned"，由于`malloc`并不知道object type，因此，它只能够使用一个最大的alignment，这个alignment能够保证所有的C type都是对齐的，这个align应该就是word。
>
> 因此当我们使用`malloc`的时候，基本上是不需要考虑alignment的。

### since C11

`malloc` is thread-safe: it behaves as though only accessing the memory locations visible through its argument, and not any static storage.

> NOTE: 
>
> 对thread safety进行了明确的规定



## alignment的说明

在 cppreference [std::max_align_t](https://en.cppreference.com/w/cpp/types/max_align_t) 中，对此有说明。

## Implementation

### stackoverflow [How malloc() and sbrk() works in unix?](https://stackoverflow.com/questions/19676688/how-malloc-and-sbrk-works-in-unix)



### holbertonschool Chapter 3: [Hack the Virtual Memory: malloc, the heap & the program break](https://blog.holbertonschool.com/hack-the-virtual-memory-malloc-the-heap-the-program-break/)
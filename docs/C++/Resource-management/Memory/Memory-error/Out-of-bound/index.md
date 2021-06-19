# Out of bound

一、Accessing array out of bounds in C/C++

https://www.geeksforgeeks.org/accessing-array-bounds-ccpp/

二、heap buffer overflow、heap buffer underflow

参见 :

1、stackoverflow [addressSanitizer: heap-buffer-overflow on address](https://stackoverflow.com/questions/51579267/addresssanitizer-heap-buffer-overflow-on-address) # [A](https://stackoverflow.com/a/51579419)

## kb.iu.edu [Identify what's causing segmentation faults (segfaults)](https://kb.iu.edu/d/aqsj)

> NOTE: 
>
> 1、其中讨论了out of bound错误



## AddressSanitizer

一、对于out of bound，一般的AddressSanitizer都是能够检查出来的

二、在使用LeetCode的时候，它就报了"AddressSanitizer: heap-buffer-overflow on address"类错误

### clang [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)

Out-of-bounds accesses to heap, stack and globals



### stackoverflow [addressSanitizer: heap-buffer-overflow on address](https://stackoverflow.com/questions/51579267/addresssanitizer-heap-buffer-overflow-on-address)

> NOTE: 
>
> 在使用LeetCode的时候，它就报了"AddressSanitizer: heap-buffer-overflow on address"类错误

#### [A](https://stackoverflow.com/a/51579419)

> what is fsanitizer=address flag?

Usually C compiler doesn't add boundaries check for memory access. Sometimes due to code error, there is read or write from outside the buffer, such an error is usually hard to detect. Using this flag the compiler add some boundaries check, to ensure you won't use a buffer to reach outside of its allocation.

> what is heap-buffer-overflow?

use an array to reach after its allocation,

```c
char* x = malloc(10);
char n=x[11]; //heap-buffer-overflow
```

(underflow is to reach before its allocation)

```c
char* x = malloc(10);
char n=x[-11]; //heap-buffer-underflow
```

> what is address and thread?

Address is position in memory, thread is part of process running sequence of code.

> and why it says 'read of size 1 at address.." ?

It means you read single byte form the given address.

------

I think your problem is that you allocate the `BUFFER_SIZE` for the buffer and read the same `BUFFER_SIZE` into it. The correct approach is to always declare at least one more byte than you read. like this:

```c
char* buff = malloc(BUFFER_SIZE+1);//notice to +1
fread(buff,1,BUFFER_SIZE,fp);
```
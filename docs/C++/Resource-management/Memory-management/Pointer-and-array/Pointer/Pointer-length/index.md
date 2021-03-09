# Length of pointer

## Length of pointer 和 length of Word 相等？

1、Length of pointer由它的virtual memory address space决定的，它和length of Word 之间没有必然联系，但是两者之间貌似有着关联，不同的implementation有着不同的设计

2、由于"aligned pointers are atomic" 的、Word是CPU memory access的单位，因此可以断言:

Length of pointer <= length of Word 

3、正如在 holbertonschool [Pointers in C](https://www.holbertonschool.com/coding-resource-pointers-in-c) 中所述的，Word是CPU的最大memory access unit，因此将length of pointer设置为Lenght of Word，能够最大地发挥CPU power。

### holbertonschool [Pointers in C](https://www.holbertonschool.com/coding-resource-pointers-in-c)

In the above, I exemplified a pointer to an integer, but pointers can refer to any C data type.

```C++

#include <stdio.h>

int main(void)
{
    printf("The size of an int pointer is %ld bytes!\
", sizeof(char *));
    printf("The size of a char pointer is %ld bytes!\
", sizeof(int *));
    printf("The size of a short pointer is %ld bytes!\
", sizeof(short *));
    printf("The size of a long pointer is %ld bytes!\
", sizeof(long *));
    printf("The size of a float pointer is %ld bytes!\
", sizeof(float *));
    printf("The size of a double pointer is %ld bytes!\
", sizeof(double *));
    printf("The size of a void pointer is %ld bytes!\
", sizeof(void *));
    return (0);
}
// gcc main.c -o size

```



```C++
The size of an int pointer is 8 bytes!
The size of a char pointer is 8 bytes!
The size of a short pointer is 8 bytes!
The size of a long pointer is 8 bytes!
The size of a float pointer is 8 bytes!
The size of a double pointer is 8 bytes!
The size of a void pointer is 8 bytes!
```



The above prints the size of eight different pointers referring to eight different data types, but the size of each remains constant at, well, eight bytes. In other words, pointers are no more than 8-byte [on 64-bit machine] blocks of memory that refer to the memory address of something, anything, else.

Aside: **Why 8 bytes?**
Why do pointers take up 8 bytes on 64-bit computers, you ask? Thank you for clarifying the computer architecture. The 8-byte count taken up by pointers is crucially exclusive to 64-bit machines, and for a reason - 8 bytes is the largest possible address size available on that architecture. Since one byte is equal to eight bits, 64 bits / 8 = 8 represents the size of a pointer. On 32-bit machines, pointers correspondingly take up 32 bits / 8 = 4 bytes.

> NOTE: 翻译如下:
>
> "您可能会问，为什么指针在64位计算机上占用8个字节?
> 感谢您澄清了计算机架构。
> 指针所占用的8字节数是64位机器所独有的，这是有原因的——8字节是该体系结构上可用地址的最大可能大小。
> 因为一个字节等于8位，所以64位/ 8 = 8表示指针的大小。
> 在32位机器上，指针相应地占用32位/ 8 = 4字节。"

### superuser [How is the word size often same as pointer size?](https://superuser.com/questions/1166698/how-is-the-word-size-often-same-as-pointer-size)

A

> NOTE: 
>
> 1、这个作者的观点是: 它们不必相等

They don't have to be the same, and the number of computer systems where they aren't vastly outnumbers the ones where they are. Still, there are some relationships between pointer size and "word" size.

Programs do a LOT of pointer arithmetic. Array subscripting is pointer addition. Pointers get compared to each other. If your CPU can't do arithmetic on something the size of a pointer, your address has to be split across multiple variables, which results in a segmented or banked address space. These are complicated to manage and cause proliferation of pointer-like types (near pointer, far pointer, based pointer). As a consequence you get additional weird limitations such as linked lists being able to hold more items than an array can.

Still, microcontrollers with 8-bit data busses and larger address spaces (perhaps only for code and not data) are common. In the other direction modern general purpose CPUs with 32-bit or 64-bit pointers have much wider data words than pointers (for example, 512-bit data words in the AVX subsystem of modern x86_64 CPUs). Many lock-free algorithms depend on atomic access to data which are a minimum of twice the size of a pointer.



## TODO



http://books.msspace.net/mirrorbooks/kerneldevelopment/0672327201/ch19lev1sec2.html



https://stackoverflow.com/questions/56582195/what-will-be-the-word-length-size-of-int-pointer-on-a-8-16-32-64-bit-micro-c

https://en.wikipedia.org/wiki/Word_(computer_architecture)#Table_of_word_sizes

https://blog.csdn.net/weixin_34376562/article/details/94257072

https://blog.csdn.net/viafcccy/article/details/84337359

https://www.holbertonschool.com/coding-resource-pointers-in-c




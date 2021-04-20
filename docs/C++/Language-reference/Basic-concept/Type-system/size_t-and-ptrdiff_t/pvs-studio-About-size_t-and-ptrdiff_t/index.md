# pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/)

> NOTE:
>
> 1、
>
> `size_t` 对应的是 `uintptr_t`
>
> `ptrdiff_t` 对应的是 `intptr_t`
>
> 它们都和pointer有着密切关联: 
>
> `size_t` length等于pointer length
>
> `ptrdiff_t` length等于pointer length
>
> 在 "Portability of `size_t` and `ptrdiff_t`"段中，有着这样对待总结: 
>
> The size of `size_t` and `ptrdiff_t` always coincide with the pointer's size. Because of this, it is these types which should be used as indexes for large arrays, for storage of pointers and, pointer arithmetic.



The article will help the reader understand what `size_t` and `ptrdiff_t` types are, what they are used for, and when they must be used. The article will be interesting for those developers who begin creation of 64-bit applications where use of `size_t` and `ptrdiff_t` types provides high performance possibility to operate large data sizes, and portability between different platforms.

> NOTE: 
>
> 一、使用 `size_t`、`ptrdiff_t` 能够获得的: portability 、
>
> 1、"high performance possibility to operate large data sizes"
>
> 2、"portability between different platforms"
>
> 3、intentional programming

## Introduction

The types `size_t` and `ptrdiff_t` were created to perform correct [address arithmetic](https://pvs-studio.com/en/t/0005/).

> NOTE: 
>
> 1、作为programmer，我们更多的是做address arithmetic，因此，我们更多的是关注pointer length，而不是Word length



It had been assumed for a long time that the size of `int` coincides with the size of a computer word (microprocessor's capacity) and it can be used as indexes to store sizes of objects or pointers. Correspondingly, address arithmetic was built with the use of `int` and unsigned types as well. `int` type is used in most training materials on programming in C and C++ in the loops' bodies and as indexes. The following example is nearly canon(标准):

```
for (int i = 0; i < n; i++)
  a[i] = 0;
```

As microprocessors developed over time and their capacity increased, it became irrational(不合理的) to further increase `int` type's sizes. There are a lot of reasons for that: economy of memory used, maximum portability etc. As a result, several data models appeared declaring the relations of C/C++ base types. Table N1 shows the main [data models](https://pvs-studio.com/en/t/0012/), and lists the most popular systems using them.

[![a0050_size_t_and_ptrdiff_t/image1.png](https://import.viva64.com/docx/blog/a0050_size_t_and_ptrdiff_t/image1.png)](https://import.viva64.com/docx/blog/a0050_size_t_and_ptrdiff_t/image2.png)

Table N1. Data models

As you can see from the table, it is not so easy to choose a variable's type to store a pointer or an object's size. To find the smartest solution to this problem `size_t` and `ptrdiff_t` types were created. They are guaranteed to be used for address arithmetic. And now the following code must become canon:

```C++
for (ptrdiff_t i = 0; i < n; i++)
  a[i] = 0;
```

It is this code that can provide safety, portability, and good performance. The rest of the article explains why.

## `size_t` type

> NOTE: 
>
> 1、从这段内容可知: `size_t` length需要大于等于pointer length: "The type's size is chosen so that it can store the maximum size of a theoretically possible array of any type"

size_t type is a base unsigned integer type of C/C++ language. It is the type of the result returned by `sizeof` operator. The type's size is chosen so that it can store the maximum size of a theoretically possible array of any type. On a 32-bit system size_t will take 32 bits, on a 64-bit one 64 bits. In other words, a variable of `size_t` type can safely store a pointer. The exception is pointers to class functions, but this is a special case. Although `size_t` can store a pointer, it is better to use another unsigned integer type [uintptr_t](https://pvs-studio.com/en/t/0050/) for that purpose (its name reflects its capability). The types size_t and `uintptr_t` are synonyms(同义词). `size_t` type is usually used for loop counters, array indexing, and address arithmetic.

The maximum possible value of `size_t` type is constant `SIZE_MAX`.



## `ptrdiff_t` type

> 1、从这段内容可知: `size_t` length需要大于等于pointer length相同: "The type's size is chosen so that it can store the maximum size of a theoretically possible array of any type"

`ptrdiff_t` type is a base signed integer type of C/C++ language. The type's size is chosen so that it can store the maximum size of a theoretically possible array of any type. On a 32-bit system `ptrdiff_t` will take 32 bits, on a 64-bit one 64 bits. Like in `size_t`, `ptrdiff_t` can safely store a pointer except for a pointer to a class function. Also, `ptrdiff_t` is the type of the result of an expression where one pointer is subtracted from the other (ptr1-ptr2). `ptrdiff_t` type is usually used for loop counters, array indexing, size storage, and address arithmetic. `ptrdiff_t` type has its synonym [intptr_t](https://pvs-studio.com/en/t/0023/) whose name indicates more clearly that it can store a pointer.



## Portability of `size_t` and `ptrdiff_t`

> NOTE:
>
> 1、这两个类型都是机器相关的，因此，它们都是implementation defined，因此，它们能够较好的portability 

The types `size_t` and `ptrdiff_t` enable you to write well-portable code. The code created with the use of `size_t` and `ptrdiff_t` types is easily portable. The size of `size_t` and `ptrdiff_t` always coincide with the pointer's size. Because of this, it is these types which should be used as indexes for large arrays, for storage of pointers and, pointer arithmetic.



### Linux `long`

Linux-application developers often use `long` type for these purposes. Within the framework of 32-bit and 64-bit data models accepted in Linux, this really works. long type's size coincides with the pointer's size. But this code is incompatible with Windows data model and, consequently, you cannot consider it easily portable. A more correct solution is to use types `size_t` and `ptrdiff_t`.

### Windows `DWORD_PTR`, `SIZE_T`, `SSIZE_T`

As an alternative to `size_t` and `ptrdiff_t`, Windows-developers can use types `DWORD_PTR`, `SIZE_T`, `SSIZE_T` etc. However, it is still desirable to confine to size_t and `ptrdiff_t` types.

## Safety of `ptrdiff_t` and `size_t` types in address arithmetic

**Address arithmetic** issues have occured frequently since the beginning of the adaptation of 64-bit systems. Most problems in porting 32-bit applications to 64-bit systems relate to the use of such types as `int` and `long`, which are unsuitable for working with pointers and type arrays. The problems of porting applications to 64-bit systems are not limited by this, but most errors relate to address arithmetic and operation with indexes.

### Example: operation with indexes

Here is a simple example:

```c++
size_t n = ...;
for (unsigned i = 0; i < n; i++)
  a[i] = 0;
```

> NOTE:
>
> 1、index `i`的类型和`n`的类型是不同的，并且index `i`的类型长度小于`n`的类型长度，因此，存在这样的一种可能性: `i` 永远都无法和`n`相等，`i`一直递增到overflow都比`n`小。
>
> 2、关于上述程序的，更加深入的讨论，参见: 
>
> CppCoreGuidelines [ES.107: Don't use `unsigned` for subscripts, prefer `gsl::index`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex)
>
> 它收录在了`Array-index`章节
>
> 3、上述code，是非常常见的

If we deal with the array consisting of more than `UINT_MAX` items, this code is incorrect. It is not easy to detect an error and predict the behavior of this code. The debug-version will hang but hardly anyone will process gigabytes of data in the debug-version. And the release-version, depending on the optimization settings and code's peculiarities(其特性), can either hang, or suddenly fill all the array cells correctly, thereby producing the illusion of correct operation. As a result, there appears floating errors in the program, occurring and vanishing with the subtlest change of the code. To learn more about such phantom errors and their dangerous consequences see the article "[A 64-bit horse that can count](https://pvs-studio.com/en/a/0043/)" [1].



> NOTE: 
>
> 1、这种错误是非常隐晦的，一旦出现，非常难以排查



### Example: mix signed and unsigned arithmetic

> NOTE:
>
> 1、CppCoreGuidelines [ES.100: Don't mix signed and unsigned arithmetic](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es100-dont-mix-signed-and-unsigned-arithmetic)

Another example of one more "sleeping" error which occurs at a particular combination of the input data (values of A and B variable):

```C
#include <stdio.h>
int main()
{
	int A = -2;
	unsigned B = 1;
	int array[5] = { 1, 2, 3, 4, 5 };
	int *ptr = array + 3;
	ptr = ptr + (A + B); //Error
	printf("%i\n", *ptr);
}
// gcc  test.c

```

> NOTE: 
>
> 1、64-bit上运行，会core dump

This code will be correctly performed in the 32-bit version, and print number "3". After compilation in 64-bit mode there will be a fail when executing the code. Let's examine the sequence of code execution and the cause of the error:

1、`A` variable of int type is cast into unsigned type;

2、`A` and `B` are summed. As a result, we get `0xFFFFFFFF` value of unsigned type;

3、"`ptr + 0xFFFFFFFFu`" expression is calculated. The result depends on the pointer's size on the current platform. In the 32-bit program, the expression will be equal to "`ptr - 1`" and we will successfully print number 3. In the 64-bit program, `0xFFFFFFFFu` value will be added to the pointer, and as a result, the pointer will be far beyond the array's limits.

> NOTE:
>
> 一、上述内容需要结合具体来进行分析:
>
> `A + B`，会将A由signed转换为unsigned，它和B相加，结果为`0xFFFFFFFF`
>
> 对于`ptr + (A + B)`:
>
> 1、在32-bit机器上，pointer是signed 32-bit，因此，`0xFFFFFFFF`的值大于`-1`
>
> 2、在64-bit机器上，pointer是signed 64-bit，因此，`0xFFFFFFFF`的值等于 268,435,455。
>
> 显然，此时会发现access array out of bound memory error。

Such errors can be easily avoided by using `size_t` or `ptrdiff_t` types. In the first case, if the type of "i" variable is size_t, there will be no infinite loop. In the second case, if we use size_t or ptrdiff_t types for "A" and "B" variable, we will correctly print number "3".

Let's formulate a guideline: wherever you deal with pointers or arrays, you should use size_t and ptrdiff_t types.

To learn more about the errors you can avoid by using `size_t` and `ptrdiff_t` types, see the following articles:

- [20 issues of porting C++ code on the 64-bit platform](https://pvs-studio.com/en/a/0004/) [2];
- [Safety of 64-bit code](https://pvs-studio.com/en/a/0046/) [3];
- [Traps detection during migration of C and C++ code to 64-bit Windows](https://pvs-studio.com/en/a/0012/) [4].

## Performance of code using `ptrdiff_t` and `size_t`

Besides code safety, the use of `ptrdiff_t` and `size_t` types in address arithmetic can give you an additional gain in performance. For example, using int type as an index, the former's capacity being different from that of the pointer, will lead to the binary code containing additional **data conversion commands**. We speak about 64-bit code where pointers' size is 64 bits, and int type's size remains 32 bits.

> NOTE:
>
> 1、省去了data conversion command

## Code refactoring with the purpose of moving to ptrdiff_t and size_t

> NOTE:
>
> 未阅读
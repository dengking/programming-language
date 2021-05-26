# `new` operator

## 发展概述



### C++17 

1、[allocation functions](https://en.cppreference.com/w/cpp/memory/new/operator_new) with explicit alignment



## 内容概述

cppreference [new expression](https://en.cppreference.com/w/cpp/language/new) 整体描述了new expression的内容，它主要两个功能:

1、allocation

2、construction

其中"allocation"是通过调用allocation function来实现的，这些allocation function在 cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new) 中进行了描述

### cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new)

一、在 cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new) 中，给出了 new operator 的allocation function的function signature，显然这是符合uniform function model的。

二、在 cppreference [new expression](https://en.cppreference.com/w/cpp/language/new) 中，将它们称为"allocation function"，因为它们主要用于allocate memory，因此其中会讨论alignment的问题

三、关于new expression 和 allocation function之间的对应关系，参见:

1、"How to call allocation function" 章节



## Alignment说明

C++对dynamic allocation的alignment进行了要求，它们(大多数情况)都要求保证alignment，显然这简化了programmer的操作、保证了performance。主要是在 cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new) 中，对alignment进行了说明。

分为三种情况:

1、

```C++
void* operator new  ( std::size_t count ); // (1)
```

This function is required to return a pointer suitably aligned to point to an object of the requested size.

2、placement new

```C++
void* operator new  ( std::size_t count, void* ptr ); // (9)
```

不对alignment做任何处理，由programmer自己来处理alignment

3、alignment requirement exceeds `__STDCPP_DEFAULT_NEW_ALIGNMENT__`  (since C++17)

```c++
void* operator new  ( std::size_t count, std::align_val_t al ); // (3)
```

能够保证alignment。

### `__STDCPP_DEFAULT_NEW_ALIGNMENT__`

一、`__STDCPP_DEFAULT_NEW_ALIGNMENT__` 是`new expression`的default alignment，显然这个default alignment是能够大多数类型的alignment requirement的，就好比 [malloc](https://en.cppreference.com/w/c/memory/malloc) 的default alignment。

二、对于alignment requirement超过`__STDCPP_DEFAULT_NEW_ALIGNMENT__`的，可以使用"C++17 [allocation functions](https://en.cppreference.com/w/cpp/memory/new/operator_new) with explicit alignment"特性

## cppreference [new expression](https://en.cppreference.com/w/cpp/language/new) 

Creates and initializes objects with dynamic [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration), that is, objects whose lifetime is not necessarily limited by the **scope** in which they were created.

### Syntax

```C++
::(optional) new (placement_params)(optional) ( type ) initializer(optional)	(1)	
::(optional) new (placement_params)(optional) type initializer(optional)	(2)	
```

> NOTE: 关于 `::` operator，参见下面的"Allocation"章节

1) Attempts to create an object of type, denoted by the [type-id](https://en.cppreference.com/w/cpp/language/type#Type_naming) `type`, which may be 

1 array type, and may include 

2 a [placeholder type specifier](https://en.cppreference.com/w/cpp/language/auto) (since C++11), or include 

> NOTE: 其实就是`auto`用法

3 a class template name whose argument is to be deduced by [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) (since C++17).

> NOTE: 并不清楚它的具体用法 

2) Same, but `type` cannot include parentheses:

### Explanation

> NOTE: 下面是对new expression的解释，基于原文的内容， 进行了一些划分。

### What `new` do?

The `new` expression attempts to **allocate storage** and then attempts to construct and initialize either a single unnamed object, or an unnamed array of objects in the allocated storage. 

> NOTE: 做了两件事:
>
> 1) allocate storage 
>
> 2) construct and initialize

### What `new` return?

The new-expression returns a **prvalue pointer** to the constructed object or, if an array of objects was constructed, a pointer to the initial element of the array.

> NOTE: 它的返回值就是是一个value，更加具体说是address value

### Array type

> NOTE: 原文的这一段，是比较繁杂的，还没有读完。

If `type` is an array type, all dimensions other than the first must be specified as positive [integral constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) (until C++14)[converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of type [std::size_t](https://en.cppreference.com/w/cpp/types/size_t) (since C++14), but (only when using un-parenthesized syntax (2)) the first dimension may be an expression of integral type, enumeration type, or class type with a single non-explicit conversion function to integral or enumeration type (until C++14)any expression convertible to [std::size_t](https://en.cppreference.com/w/cpp/types/size_t) (since C++14). This is the only way to directly create an array with size defined at runtime, such arrays are often referred to as *dynamic arrays*:

```C++
int n = 42;
double a[n][5]; // error
auto p1 = new  double[n][5];  // OK
auto p2 = new  double[5][n];  // error: only the first dimension may be non-constant
auto p3 = new (double[n][5]); // error: syntax (1) cannot be used for dynamic arrays
```

### Allocation

The new-expression allocates storage by calling the appropriate [allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new). If `type` is a non-array type, the name of the function is `operator new`. If `type` is an array type, the name of the function is `operator new[]`.

> NOTE: 
>
> 一、上述 "[allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new)" ，其实就是 cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new)
>
> 1、在  cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new) 给出了new-expression 和 function signature的对应关系
>
> 2、需要注意的是， cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new) 仅仅用于 "allocates storage"，因此在其中讨论了alignment的问题。

As described in [allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new), the C++ program may provide global and class-specific replacements for these functions. If the new-expression begins with the optional `::` operator, as in `::new T` or `::new T[n]`, **class-specific replacements** will be ignored (the function is [looked up](https://en.cppreference.com/w/cpp/language/lookup) in global [scope](https://en.cppreference.com/w/cpp/language/scope)). Otherwise, if `T` is a class type, lookup begins in the class scope of `T`.

> NOTE: 
>
> 一、这描述的是C++中常见的"use the name in the global namespace"技巧，参见 `C++\Language-reference\Basic-concept\Organization\Scope\Scope-resolution-operator` 章节。
>
> 

#### How to call allocation function

When calling the allocation function, the new-expression passes the number of bytes requested as the first argument, of type [std::size_t](http://en.cppreference.com/w/cpp/types/size_t), which is exactly `sizeof(T)` for non-array `T`.

| new expression | allocation function                      | allocation function (C++17)                                  |
| -------------- | ---------------------------------------- | ------------------------------------------------------------ |
| `new T;`       | `operator new(sizeof(T))` <br>           | `operator new(sizeof(T), std::align_val_t(alignof(T))))`     |
| `new T[5];`    | `operator new[](sizeof(T)*5 + overhead)` | `operator new(sizeof(T)*5+overhead, std::align_val_t(alignof(T))))` |
| `new(2,f) T;`  | `operator new(sizeof(T), 2, f)`          | `operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)` |

> NOTE: 
>
> 上述内容是摘自原文:
>
> ```C++
> new T;      // calls operator new(sizeof(T))
>             // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T))))
> new T[5];   // calls operator new[](sizeof(T)*5 + overhead)
>             // (C++17) or operator new(sizeof(T)*5+overhead, std::align_val_t(alignof(T))))
> new(2,f) T; // calls operator new(sizeof(T), 2, f)
>             // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)
> ```
>
> 

#### Overhead

> NOTE: 
>
> 原文的这一段解释了为什么要使用"overhead"。通过下面的描述来看，主要出于两个意图，参见下面的内容。
>
> 

Array allocation may supply unspecified **overhead**, which may vary from one call to new to the next, unless the allocation function selected is the **standard non-allocating form**. The pointer returned by the new-expression will be offset by that value(指的是 **overhead** ) from the pointer returned by the allocation function. 

> NOTE: 
>
> 1、上面这段话中的"the standard non-allocating form"指的是placement new。

Many implementations use the array overhead to store the number of objects in the array which is used by the **[`delete[]`](https://en.cppreference.com/w/cpp/language/delete) expression** to call the correct number of destructors. 

> NOTE: 
>
> 上面说明的是使用overhead的原因之一

In addition, if the new-expression is used to allocate an array of `char`, `unsigned char`, or [`std::byte`](https://en.cppreference.com/w/cpp/types/byte) (since C++17), it may request additional memory from the allocation function if necessary to guarantee correct alignment of objects of all types no larger than the requested array size, if one is later placed into the allocated array.

> NOTE: 
>
> 上面说明的是使用overhead的原因之二

#### since C++14

> NOTE: 
>
> 未阅

#### since C++20

> NOTE: 
>
> 未阅

### Placement new

> NOTE: 
>
> 参见 `Placement-new` 章节

### Custom alignment requirement(since C++17)

> NOTE: 
>
> 关于这种用法，参见: 
>
> bfilipek [New new() - The C++17's Alignment Parameter for Operator new()](https://www.bfilipek.com/2019/08/newnew-align.html)

When allocating an object whose alignment requirement exceeds `__STDCPP_DEFAULT_NEW_ALIGNMENT__` or an array of such objects, the new-expression passes the **alignment requirement** (wrapped in [std::align_val_t](https://en.cppreference.com/w/cpp/memory/new/align_val_t)) as the second argument for the allocation function (for placement forms, `placement_params` appear after the alignment, as the third, fourth, etc arguments). If overload resolution fails (which happens when a class-specific allocation function is defined with a different signature, since it hides the globals), overload resolution is attempted a second time, without alignment in the argument list. This allows alignment-unaware class-specific allocation functions to take precedence over the global alignment-aware allocation functions.

```C++
new T;      // calls operator new(sizeof(T))
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T))))
new T[5];   // calls operator new[](sizeof(T)*5 + overhead)
            // (C++17) or operator new(sizeof(T)*5+overhead, std::align_val_t(alignof(T))))
new(2,f) T; // calls operator new(sizeof(T), 2, f)
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)
```

### Null pointer

> NOTE: 
>
> [std::nothrow](http://en.cppreference.com/w/cpp/memory/new/nothrow)

If a non-throwing allocation function (e.g. the one selected by `new(std::nothrow) T`) returns a null pointer because of an allocation failure, then the new-expression returns immediately, it does not attempt to initialize an object or to call a deallocation function. If a null pointer is passed as the argument to a non-allocating placement new-expression, which makes the selected standard non-allocating placement allocation function return a null pointer, the behavior is undefined.

### Construction

> NOTE: 
>
> 原文这一段将如何进行initialization。
>
> 

### Memory leaks

The objects created by new-expressions (objects with dynamic storage duration) persist until the pointer returned by the new-expression is used in a matching [delete-expression](https://en.cppreference.com/w/cpp/language/delete). If the original value of pointer is lost, the object becomes unreachable and cannot be deallocated: a *memory leak* occurs.



## TODO

### cnblogs [C++ 内存分配操作符new和delete详解](https://www.cnblogs.com/Philip-Tell-Truth/p/6567808.html)

### stackoverflow [Use new operator to initialise an array](https://stackoverflow.com/questions/9603696/use-new-operator-to-initialise-an-array)

**A**

You can use `memcpy` after the allocation.

```c++
int originalArray[] ={1,2,3,4,5,6,7,8,9,10};
int *array = new int[10];
memcpy(array, originalArray, 10*sizeof(int) );
```

I'm not aware of any syntax that lets you do this automagically.

Much later edit:

```C++
const int *array = new int[10]{1,2,3,4,5,6,7,8,9,10};
```




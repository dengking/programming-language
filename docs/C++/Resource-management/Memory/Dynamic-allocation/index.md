# Dynamic memory allocation

memory的管理非常重要。memory也是一种resource，与本文相关的有：

- [RAII](../../../Idiom/RAII/RAII.md)
- [The-rule-of-three-five-zero](../../../Idiom/The-rule-of-three-five-zero/The-rule-of-three-five-zero.md)

C++: `new` 和 `delete`

C: `malloc` 和 `free`



## wikipedia [C dynamic memory allocation](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation)

**C dynamic memory allocation** refers to performing [manual memory management](https://en.wikipedia.org/wiki/Manual_memory_management) for [dynamic memory allocation](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) in the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) via a group of functions in the [C standard library](https://en.wikipedia.org/wiki/C_standard_library), namely malloc, realloc, calloc and free.[[1\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-c99-1)[[2\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-3)

The [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming language includes these functions; however, the operators [new and delete](https://en.wikipedia.org/wiki/New_and_delete_(C%2B%2B)) provide similar functionality and are recommended by that language's authors.[[4\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-4) Still, there are several situations in which using `new/delete` is not applicable, such as garbage collection code or performance-sensitive code, and a combination of `malloc` and `placement new` may be required instead of the higher-level `new` operator.

> NOTE: 上面这一段描述，显然比较了`new/delete` 和 `malloc`

Many different implementations of the actual memory allocation mechanism, used by malloc, are available. Their performance varies in both execution time and required memory.



## alignment的处理

C++、C都对dynamic allocation的alignment进行了要求，它们(大多数情况)都要求保证alignment，显然这简化了programmer的操作、保证了performance。

关于C的dynamic allocation的alignment，参见 `malloc` 章节。

关于C++的dynamic allocation的alignment，参见 `new-operator` 章节。

### cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new)

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

能够满足所有的C++ type的alignment requirement。




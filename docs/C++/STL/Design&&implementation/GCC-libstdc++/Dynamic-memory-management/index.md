# Dynamic memory management

在下面章节中，对它的"dynamic memory management"进行了论述。

## [II. Standard Contents](https://gcc.gnu.org/onlinedocs/libstdc++/manual/std_contents.html) # [4. Support](https://gcc.gnu.org/onlinedocs/libstdc++/manual/support.html) # [Dynamic Memory](https://gcc.gnu.org/onlinedocs/libstdc++/manual/dynamic_memory.html)

In C++98 there are six flavors each of `operator new` and `operator delete`

> NOTE: 
>
> 主要讲述`operator new`、`operator delete`的API
>
> 

## [II. Standard Contents](https://gcc.gnu.org/onlinedocs/libstdc++/manual/std_contents.html) # [6. Utilities](https://gcc.gnu.org/onlinedocs/libstdc++/manual/utilities.html) # [Memory](https://gcc.gnu.org/onlinedocs/libstdc++/manual/memory.html) 

Memory contains three general areas. 

First, function and operator calls via `new` and `delete` operator or member function calls. 

Second, allocation via `allocator`. 

Finally, smart pointer and intelligent pointer abstractions.

### Extension Allocators

[Table B.6. Extension Allocators](https://gcc.gnu.org/onlinedocs/libstdc++/manual/api.html#table.extension_allocators)

| Allocator (3.4)                  | Header (3.4)               | Allocator (3.[0-3])                       | Header (3.[0-3]) |
| -------------------------------- | -------------------------- | ----------------------------------------- | ---------------- |
| `__gnu_cxx::new_allocator<T>`    | `<ext/new_allocator.h>`    | `std::__new_alloc`                        | `<memory>`       |
| `__gnu_cxx::malloc_allocator<T>` | `<ext/malloc_allocator.h>` | `std::__malloc_alloc_template<int>`       | `<memory>`       |
| `__gnu_cxx::debug_allocator<T>`  | `<ext/debug_allocator.h>`  | `std::debug_alloc<T>`                     | `<memory>`       |
| `__gnu_cxx::__pool_alloc<T>`     | `<ext/pool_allocator.h>`   | `std::__default_alloc_template<bool,int>` | `<memory>`       |
| `__gnu_cxx::__mt_alloc<T>`       | `<ext/mt_allocator.h>`     |                                           |                  |
| `__gnu_cxx::bitmap_allocator<T>` | `<ext/bitmap_allocator.h>` |                                           |                  |

## [II. Standard Contents](https://gcc.gnu.org/onlinedocs/libstdc++/manual/std_contents.html) # [6. Utilities](https://gcc.gnu.org/onlinedocs/libstdc++/manual/utilities.html) # [Memory](https://gcc.gnu.org/onlinedocs/libstdc++/manual/memory.html) # [Allocators](https://gcc.gnu.org/onlinedocs/libstdc++/manual/memory.html#std.util.memory.allocator)



## [III. Extensions](https://gcc.gnu.org/onlinedocs/libstdc++/manual/extensions.html) # [19. The mt_allocator](https://gcc.gnu.org/onlinedocs/libstdc++/manual/mt_allocator.html)



This allocator is tunable, very flexible, and capable of high-performance.

## [III. Extensions](https://gcc.gnu.org/onlinedocs/libstdc++/manual/extensions.html) # [20. The bitmap_allocator](https://gcc.gnu.org/onlinedocs/libstdc++/manual/bitmap_allocator.html)



## csdn [gnu c++的allocator](https://blog.csdn.net/princeteng/article/details/103398576)

1.gnu c++中的allocator类型

array_allocator

bitmap_allocator(和mfc中的bitmap无关)

malloc_allocator

mt_allocator(多线程)

new_allocator

extptr_allocator

debug_allocator

pool_allocator

throw_allocator

其中new_allocator与pool_allocator在STL内存分配allocator介绍过。

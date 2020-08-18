# `dynamic_cast`



## cppreference [`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast)

Safely converts **pointers** and **references** to classes up, down, and sideways（平级） along the inheritance hierarchy.

> NOTE: 操作对象：pointer and reference



### Syntax

If the cast is successful, `dynamic_cast` returns a value of type *new-type*. If the cast fails and *new-type* is a pointer type, it returns a null pointer of that type. If the cast fails and *new-type* is a reference type, it throws an exception that matches a handler of type [std::bad_cast](https://en.cppreference.com/w/cpp/types/bad_cast).

> NOTE: 从上述对“cast fail”情况的讨论：
>
> - 如果new-type是pointer type，则return null pointer
> - 如果new-type是reference type，则throw exception
>
> 可以看出pointer 和 reference的差异所在：存在null pointer，但是不存在reference to nothing，所以在reference type的情况下，只能够throw exception。

### 1) Add constness



### 2) Null pointer value



### 3) Upcast

Note: an implicit conversion and static_cast can perform this conversion as well.

### 4) To the most derived object

> NOTE:关于这一点，在cppreference [Pointer declaration#Pointer to void](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_void)中也描述了。不知道这个有什么用途。
>
> 在文章panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/)

### 5) dynamic downcast

## `c++` `isinstanceof` `issubclass`

http://www.cplusplus.com/forum/general/50090/

https://www.gamedev.net/forums/topic/322781-c-instanceof/

https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof

https://stackoverflow.com/questions/27595076/instanceof-equivalent-in-c

https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof






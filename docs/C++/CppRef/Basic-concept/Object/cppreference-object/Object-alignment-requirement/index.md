# Object alignment requirement



## cppreference [Object # alignment requirement](https://en.cppreference.com/w/cpp/language/object#Alignment)

Every [object type](https://en.cppreference.com/w/cpp/language/type) has the property called *alignment requirement*, which is an integer value (of type [std::size_t](https://en.cppreference.com/w/cpp/types/size_t), always a power of 2) representing the number of bytes between successive addresses at which objects of this type can be allocated.

### since C++11

The alignment requirement of a type can be queried with [alignof](https://en.cppreference.com/w/cpp/language/alignof) or [std::alignment_of](https://en.cppreference.com/w/cpp/types/alignment_of). The pointer alignment function [std::align](https://en.cppreference.com/w/cpp/memory/align) can be used to obtain a suitably-aligned pointer within some buffer, and [std::aligned_storage](https://en.cppreference.com/w/cpp/types/aligned_storage) can be used to obtain suitably-aligned storage.

---

Each object type imposes its alignment requirement on every object of that type; **stricter alignment** (with larger alignment requirement) can be requested using [`alignas`](https://en.cppreference.com/w/cpp/language/alignas) (since C++11).

In order to satisfy alignment requirements of all non-static members of a [class](https://en.cppreference.com/w/cpp/language/class), *padding* may be inserted after some of its members.



## stackoverflow [Order between __STDCPP_DEFAULT_NEW_ALIGNMENT__ and alignof(std::max_align_t)](https://stackoverflow.com/questions/59172291/order-between-stdcpp-default-new-alignment-and-alignofstdmax-align-t)

With GCC and Clang on x86-64/Linux `alignof(std::max_align_t)` and `__STDCPP_DEFAULT_NEW_ALIGNMENT__` are both equal to `16`.

With MSVC on x86-64/Windows `alignof(std::max_align_t)` is `8` and `__STDCPP_DEFAULT_NEW_ALIGNMENT__` is `16`.

The standard defines the two terms corresponding to these quantities in [[basic.align\]/3](https://timsong-cpp.github.io/cppwp/n4659/basic.align#3):

> An *extended alignment* is represented by an alignment greater than `alignof(std::max_­align_­t)`. [...] A type having an extended alignment requirement is an *over-aligned type*. [...] A *new-extended alignment* is represented by an alignment greater than `_­_­STDCPP_­DEFAULT_­NEW_­ALIGNMENT_­_­`.



**A**

1、[`std::max_align_t`](https://en.cppreference.com/w/cpp/types/max_align_t): The alignment of the biggest scalar type

2、[`__STDCPP_DEFAULT_NEW_ALIGNMENT__`](https://en.cppreference.com/w/cpp/preprocessor/replace#Predefined_macros): The alignment of allocated memory

> NOTE: 
>
> C++17引入


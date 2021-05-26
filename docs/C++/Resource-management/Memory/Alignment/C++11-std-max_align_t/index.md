# cppreference [std::max_align_t](https://en.cppreference.com/w/cpp/types/max_align_t)



## Notes

Pointers returned by allocation functions such as [std::malloc](https://en.cppreference.com/w/cpp/memory/c/malloc) are suitably aligned for any object, which means they are aligned at least as strictly as **std::max_align_t**.

**std::max_align_t** is usually synonymous with the largest scalar type, which is `long double` on most platforms, and its alignment requirement is either 8 or 16.



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


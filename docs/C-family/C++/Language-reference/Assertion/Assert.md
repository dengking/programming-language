# Assertion

## Introduction

learncpp [Assert and static_assert](https://www.learncpp.com/cpp-tutorial/7-12a-assert-and-static_assert/)

geeksforgeeks https://www.geeksforgeeks.org/assertions-cc/

## Static assertion

### cppreference [`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) 



### Examples

#### cppreference [`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert)  Example

```c++
#include <type_traits>
 
template <class T>
void swap(T& a, T& b)
{
    static_assert(std::is_copy_constructible<T>::value,
                  "Swap requires copying");
    static_assert(std::is_nothrow_copy_constructible<T>::value
               && std::is_nothrow_copy_assignable<T>::value,
                  "Swap requires nothrow copy/assign");
    auto c = b;
    b = a;
    a = c;
}
 
template <class T>
struct data_structure
{
    static_assert(std::is_default_constructible<T>::value,
                  "Data Structure requires default-constructible elements");
};
 
struct no_copy
{
    no_copy ( const no_copy& ) = delete;
    no_copy () = default;
};
 
struct no_default
{
    no_default () = delete;
};
 
int main()
{
    int a, b;
    swap(a, b);
 
    no_copy nc_a, nc_b;
    swap(nc_a, nc_b); // 1
 
    data_structure<int> ds_ok;
    data_structure<no_default> ds_error; // 2
}
```

上述程序无法编译通过，报错如下：

```c++
test.cpp: In instantiation of ‘struct data_structure<no_default>’:
test.cpp:43:32:   required from here
test.cpp:19:5: 错误：static assertion failed: Data Structure requires default-constructible elements
     static_assert(std::is_default_constructible<T>::value,
     ^
test.cpp: In instantiation of ‘void swap(T&, T&) [with T = no_copy]’:
test.cpp:40:20:   required from here
test.cpp:6:5: 错误：static assertion failed: Swap requires copying
     static_assert(std::is_copy_constructible<T>::value,
     ^
test.cpp:8:5: 错误：static assertion failed: Swap requires nothrow copy/assign
     static_assert(std::is_nothrow_copy_constructible<T>::value
     ^
test.cpp:11:14: 错误：使用了被删除的函数‘no_copy::no_copy(const no_copy&)’
     auto c = b;
              ^
test.cpp:25:5: 错误：在此声明
     no_copy ( const no_copy& ) = delete;
```



#### `spdlog/common.h`

https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/common.h

```c++
// make_unique support for pre c++14

#if __cplusplus >= 201402L // C++14 and beyond
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
```

## Dynamic assertion

### cppreference [`assert`](https://en.cppreference.com/w/cpp/error/assert) 








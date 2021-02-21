# Specialization

本文主要给出template specializaiton的例子，关于实现，在`C++\Guide\Implementation-of-polymorphism`中进行了解释；

## Primary template and template specialization

primary template和template specialization是两个相对的概念，关于两者之间的关系，在下面章节中进行了介绍:

| 路径                                             | 章节                                                    | 观点                                                         |
| ------------------------------------------------ | ------------------------------------------------------- | ------------------------------------------------------------ |
| `C++\Language-reference\Template\Implementation` | Partial template specialization是primary template的附庸 | Partial template specialization是primary template的附庸      |
| `C++\Guide\Polymorphism`                         |                                                         | Overload、specialization、subclass都是对某种“特殊情况的说明”（即**最最特殊的实现**） |





## cppreference [explicit (full) template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)



### Examples

#### `is_void`

```c++
#include <iostream>
template<typename T>   // primary template
struct is_void: std::false_type
{
};
template<>  // explicit specialization for T = void
struct is_void<void> : std::true_type
{
};
int main()
{
	// for any type T other than void, the
	// class is derived from false_type
	std::cout << is_void<char>::value << '\n';
	// but when T is void, the class is derived
	// from true_type
	std::cout << is_void<void>::value << '\n';
}

```



## cppreference [partial template specialization](https://en.cppreference.com/w/cpp/language/partial_specialization)



### Examples

#### `is_pointer`: specialization of pointer type

accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)

```c++
#include <iostream>

template<typename T>
struct is_pointer
{
	static const bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
	static const bool value = true;
};

int main()
{
	// for any type T other than void, the
	// class is derived from false_type
	std::cout << is_pointer<char>::value << '\n';
	// but when T is void, the class is derived
	// from true_type
	std::cout << is_pointer<void*>::value << '\n';
}

```

#### [`remove_reference`](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/type_traits): specialization of reference type



```c++
  // Reference transformations.

  /// remove_reference
  template<typename _Tp>
    struct remove_reference
    { typedef _Tp   type; };

  template<typename _Tp>
    struct remove_reference<_Tp&>
    { typedef _Tp   type; };

  template<typename _Tp>
    struct remove_reference<_Tp&&>
    { typedef _Tp   type; };
```



#### `std::make_unique`: specialization of array type

[gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[unique_ptr.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h) 

```c++
#if __cplusplus >= 201402L
  /// @relates unique_ptr @{
#define __cpp_lib_make_unique 201304

  /// @cond undocumented

  template<typename _Tp>
    struct _MakeUniq
    { typedef unique_ptr<_Tp> __single_object; };

  template<typename _Tp>
    struct _MakeUniq<_Tp[]>
    { typedef unique_ptr<_Tp[]> __array; };

  template<typename _Tp, size_t _Bound>
    struct _MakeUniq<_Tp[_Bound]>
    { struct __invalid_type { }; };

  /// @endcond

  /// std::make_unique for single objects
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__single_object
    make_unique(_Args&&... __args)
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }

  /// std::make_unique for arrays of unknown bound
  template<typename _Tp>
    inline typename _MakeUniq<_Tp>::__array
    make_unique(size_t __num)
    { return unique_ptr<_Tp>(new remove_extent_t<_Tp>[__num]()); }

  /// Disable std::make_unique for arrays of known bound
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__invalid_type
    make_unique(_Args&&...) = delete;
  // @} relates unique_ptr
#endif // C++14
```

#### `std::unique_ptr`: specialization of array type

[gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[unique_ptr.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h) 

```c++
/// 20.7.1.2 unique_ptr for single objects.
template<typename _Tp, typename _Dp = default_delete<_Tp>>
class unique_ptr
{
};

/// 20.7.1.3 unique_ptr for array objects with a runtime length
// [unique.ptr.runtime]
// _GLIBCXX_RESOLVE_LIB_DEFECTS
// DR 740 - omit specialization for array objects with a compile time length
template<typename _Tp, typename _Dp>
class unique_ptr<_Tp[], _Dp>
{

};
```



## fluentcpp [Template Partial Specialization In C++](https://www.fluentcpp.com/2017/08/11/how-to-do-partial-template-specialization-in-c/)



## stackoverflow [Understanding (simple?) C++ Partial Template Specialization](https://stackoverflow.com/questions/6138439/understanding-simple-c-partial-template-specialization)



## Example

1、trait

2、[Mazrog](https://github.com/Mazrog)/**[srlzio](https://github.com/Mazrog/srlzio)**

其中对partial template specialization的运用非常值得借鉴


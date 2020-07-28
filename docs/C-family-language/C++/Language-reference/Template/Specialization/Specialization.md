# Specialization

template specialization，需要搞清楚如下问题：

- 如何认知template specialization
- compiler如何选择specialization
- usage

这些内容，在`C++\Guide\Implementation-of-polymorphism`中进行了解释；

## Primary template and template specialization

primary template和template specialization是两个相对的概念；

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

#### `is_pointer`

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

#### [`remove_reference`](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/type_traits) 



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



## fluentcpp [Template Partial Specialization In C++](https://www.fluentcpp.com/2017/08/11/how-to-do-partial-template-specialization-in-c/)



## Usage

- trait
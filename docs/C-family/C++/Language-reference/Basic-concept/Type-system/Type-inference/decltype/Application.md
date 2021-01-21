# Application and examples

本节通过一些例子来学习`decltype`、`std::declval`的用法。

## Example1: cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)

本地路径: `C++\Library\Standard-library\Utility-library\Language-support\Type-support\Type-traits\Type-transformations\void_t`

```C++
#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
template<typename ...Ts> struct make_void
{
	using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

class A
{
};

template<typename T, typename = void>
struct is_iterable: std::false_type
{
};

template<typename T>
struct is_iterable<T, void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type
{
};

// An iterator trait which value_type is always the value_type of the
// iterated container, even with back_insert_iterator which value_type is void
template<typename T, typename = void>
struct iterator_trait: std::iterator_traits<T>
{
};

template<typename T>
struct iterator_trait<T, void_t<typename T::container_type>> : std::iterator_traits<typename T::container_type::iterator>
{
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << is_iterable<std::vector<double>>::value << '\n';
	std::cout << is_iterable<std::map<int, double>>::value << '\n';
	std::cout << is_iterable<double>::value << '\n';
	std::cout << is_iterable<A>::value << '\n';

	std::vector<int> v;

	std::cout << std::is_same<iterator_trait<decltype(std::back_inserter(v))>::value_type, iterator_trait<decltype(v.cbegin())>::value_type>::value << '\n';
}
// g++ --std=c++11 test.cpp

```



```C++
template<typename T, typename = void>
struct is_iterable: std::false_type
{
};

template<typename T>
struct is_iterable<T, void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type
{
};
```

上述code是为了detect type `T`是否有method `begin`、method `end`，由于没有object，只有type，所以需要使用`std::declva(T)()`。



```C++
std::vector<int> v;

	std::cout << std::is_same<iterator_trait<decltype(std::back_inserter(v))>::value_type, iterator_trait<decltype(v.cbegin())>::value_type>::value << '\n';
```

有object `V`，因此可以直接书写expression `decltype(v.cbegin())`，这是直接使用的`decltype`的用法。

## Example2: cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)

本地路径: `C++\Library\Standard-library\Utility-library\Language-support\Type-support\Type-traits\Type-transformations\void_t`

```C++
// primary template handles types that do not support pre-increment:
template< class, class = void >
struct has_pre_increment_member : std::false_type { };
// specialization recognizes types that do support pre-increment:
template< class T >
struct has_pre_increment_member<T,
           std::void_t<decltype( ++std::declval<T&>() )>
       > : std::true_type { };
```


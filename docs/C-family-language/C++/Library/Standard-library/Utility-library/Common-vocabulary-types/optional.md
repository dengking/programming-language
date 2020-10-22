# `std::optional`

在阅读[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)的时候，其中使用到了[`Optional`](https://github.com/sewenew/redis-plus-plus#optional)，并给出了自定义实现。



## cppreference [std::optional](https://en.cppreference.com/w/cpp/utility/optional)



## 维基百科 [C++17#Library](https://en.wikipedia.org/wiki/C%2B%2B17#Library)



## 维基百科 [Option type](https://en.wikipedia.org/wiki/Option_type)



## Custom implementation

### [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)/[src](https://github.com/sewenew/redis-plus-plus/tree/master/src)/[sw](https://github.com/sewenew/redis-plus-plus/tree/master/src/sw)/[redis++](https://github.com/sewenew/redis-plus-plus/tree/master/src/sw/redis%2B%2B)/[utils.h](https://github.com/sewenew/redis-plus-plus/blob/master/src/sw/redis%2B%2B/utils.h)

非常简单的实现。

```C++
// Refer to https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
#if __cplusplus >= 201703L
#  if defined __has_include
#    if __has_include(<string_view>)
#      define REDIS_PLUS_PLUS_HAS_STRING_VIEW
#    endif
#    if __has_include(<optional>)
#      define REDIS_PLUS_PLUS_HAS_OPTIONAL
#    endif
#  endif
#endif

#include <cstring>
#include <string>
#include <type_traits>

#if defined REDIS_PLUS_PLUS_HAS_OPTIONAL
#include <optional>
#endif

#if defined REDIS_PLUS_PLUS_HAS_OPTIONAL

template <typename T>
using Optional = std::optional<T>;

#else

template<typename T>
class Optional
{
public:
#if defined(_MSC_VER) && (_MSC_VER < 1910)
    Optional() : _value() {}    // MSVC 2015 bug
#else
	Optional() = default;
#endif

	Optional(const Optional&) = default;
	Optional& operator=(const Optional&) = default;

	Optional(Optional&&) = default;
	Optional& operator=(Optional&&) = default;

	~Optional() = default;

	template<typename ...Args>
	explicit Optional(Args &&...args) :
					_value(true, T(std::forward<Args>(args)...))
	{
	}

	explicit operator bool() const
	{
		return _value.first;
	}

	T& value()
	{
		return _value.second;
	}

	const T& value() const
	{
		return _value.second;
	}

	T* operator->()
	{
		return &(_value.second);
	}

	const T* operator->() const
	{
		return &(_value.second);
	}

	T& operator*()
	{
		return _value.second;
	}

	const T& operator*() const
	{
		return _value.second;
	}

private:
	std::pair<bool, T> _value;
};

#endif

using OptionalString = Optional<std::string>;

using OptionalLongLong = Optional<long long>;

using OptionalDouble = Optional<double>;

using OptionalStringPair = Optional<std::pair<std::string, std::string>>;

```



### [akrzemi1](https://github.com/akrzemi1)/[Optional](https://github.com/akrzemi1/Optional)
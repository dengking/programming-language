# implementation

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



## [chromium](https://github.com/chromium/chromium)/[third_party](https://github.com/chromium/chromium/tree/main/third_party)/[abseil-cpp](https://github.com/chromium/chromium/tree/main/third_party/abseil-cpp)/[absl](https://github.com/chromium/chromium/tree/main/third_party/abseil-cpp/absl)/[types](https://github.com/chromium/chromium/tree/main/third_party/abseil-cpp/absl/types)/[**optional.h**](https://github.com/chromium/chromium/blob/main/third_party/abseil-cpp/absl/types/optional.h)
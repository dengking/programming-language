# [**Utilities**](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gslutil-utilities): [util](https://github.com/microsoft/GSL/blob/main/include/gsl/util)、[narrow](https://github.com/microsoft/GSL/blob/main/include/gsl/narrow)

| [**4. Utilities**](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gslutil-utilities) |      |                                                              |
| ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| `move_owner`                                                 | ☐    | a helper function that moves one `owner` to the other        |
| `byte`                                                       | ☑    | either an alias to std::byte or a byte type                  |
| `final_action`                                               | ☑    | a RAII style class that invokes a functor on its destruction |
| `finally`                                                    | ☑    | a helper function instantiating `final_action`               |
| `GSL_SUPPRESS`                                               | ☑    | a macro that takes an argument and turns it into `[[gsl::suppress(x)]]` or `[[gsl::suppress("x")]]` |
| `[[implicit]]`                                               | ☐    | a "marker" to put on single-argument constructors to explicitly make them non-explicit |
| `index`                                                      | ☑    | a type to use for all container and array indexing (currently an alias for std::ptrdiff_t) |
| `joining_thread`                                             | ☐    | a RAII style version of `std::thread` that joins             |
| `narrow`                                                     | ☑    | a checked version of narrow_cast; it can throw `narrowing_error` |
| `narrow_cast`                                                | ☑    | a narrowing cast for values and a synonym for static_cast    |
| `narrowing_error`                                            | ☑    | a custom exception type thrown by `narrow()`                 |

## Read the code



### `final_action` and `finally`

1、其实就是scope guard

2、`finally`其实就是object generator

```C++
#include <type_traits>      // for is_signed, integral_constant
#include <utility>          // for exchange, forward

//
// make suppress attributes parse for some compilers
// Hopefully temporary until suppression standardization occurs
//
#if defined(__clang__)
#define GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#else
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define GSL_SUPPRESS(x)
#endif // _MSC_VER
#endif // __clang__

#if defined(__cplusplus) && (__cplusplus >= 201703L)
#define GSL_NODISCARD [[nodiscard]]
#else
#define GSL_NODISCARD
#endif // defined(__cplusplus) && (__cplusplus >= 201703L)

// final_action allows you to ensure something gets run at the end of a scope
template<class F>
class final_action
{
public:
	static_assert(!std::is_reference<F>::value && !std::is_const<F>::value &&
					!std::is_volatile<F>::value,
					"Final_action should store its callable by value");

	explicit final_action(F f) noexcept :
					f_(std::move(f))
	{
	}

	final_action(final_action &&other) noexcept :
					f_(std::move(other.f_)), invoke_(std::exchange(other.invoke_, false))
	{
	}

	final_action(const final_action&) = delete;
	final_action& operator=(const final_action&) = delete;
	final_action& operator=(final_action&&) = delete;

	// clang-format off
	GSL_SUPPRESS(f.6) // NO-FORMAT: attribute // terminate if throws
	// clang-format on
	~final_action() noexcept
	{
		if (invoke_)
			f_();
	}

private:
	F f_;
	bool invoke_ { true };
};

// finally() - convenience function to generate a final_action
template<class F>
GSL_NODISCARD final_action<typename std::remove_cv<typename std::remove_reference<F>::type>::type> finally(F &&f) noexcept
{
	return final_action<typename std::remove_cv<typename std::remove_reference<F>::type>::type>(std::forward < F > (f));
}

#include  <cassert>
void f(int &i)
{
	i += 1;
}

int main()
{
	int i = 0;
	{
		auto _ = finally([&](){	f(i);});

	}
	assert(i == 1);
}
// g++ test.cpp --std=c++14

```

### `index`

```c++
#include <cstddef>          // for ptrdiff_t, size_t
// index type for all container indexes/subscripts/sizes
using index = std::ptrdiff_t;
```



### `at()` - Bounds-checked way of accessing builtin arrays, `std::array`, `std::vector`

```C++
#include <array>
#include <cstddef>          // for ptrdiff_t, size_t
#include <initializer_list> // for initializer_list
#include <type_traits>      // for is_signed, integral_constant
#include <utility>          // for exchange, forward


//
// at() - Bounds-checked way of accessing builtin arrays, std::array, std::vector
//
template <class T, std::size_t N>
// clang-format off
GSL_SUPPRESS(bounds.4)// NO-FORMAT: attribute
GSL_SUPPRESS(bounds.2)// NO-FORMAT: attribute
// clang-format on
constexpr T& at(T (&arr)[N], const index i)
{
	Expects(i >= 0 && i < narrow_cast<index>(N));
	return arr[narrow_cast<std::size_t>(i)];
}

template <class Cont>
// clang-format off
GSL_SUPPRESS(bounds.4)// NO-FORMAT: attribute
GSL_SUPPRESS(bounds.2)// NO-FORMAT: attribute
// clang-format on
constexpr auto at(Cont& cont, const index i) -> decltype(cont[cont.size()])
{
	Expects(i >= 0 && i < narrow_cast<index>(cont.size()));
	using size_type = decltype(cont.size());
	return cont[narrow_cast<size_type>(i)];
}

template <class T>
// clang-format off
GSL_SUPPRESS(bounds.1)// NO-FORMAT: attribute
// clang-format on
constexpr T at(const std::initializer_list<T> cont, const index i)
{
	Expects(i >= 0 && i < narrow_cast<index>(cont.size()));
	return *(cont.begin() + i);
}

```







### `narrow_cast`

```c++
// narrow_cast(): a searchable way to do narrowing casts of values
template <class T, class U>
// clang-format off
GSL_SUPPRESS(type.1)// NO-FORMAT: attribute
// clang-format on
constexpr T narrow_cast(U&& u) noexcept
{
	return static_cast<T>(std::forward<U>(u));
}

```


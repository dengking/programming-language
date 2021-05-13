# cppreference [std::numeric_limits](https://en.cppreference.com/w/cpp/types/numeric_limits)

The `numeric_limits` class template provides a standardized way to query various properties of arithmetic types (e.g. the largest possible value for type int is `std::numeric_limits<int>::max())`.

> NOTE: 
> 1、trait consistent interface abstraction

Implementations may provide specializations of **std::numeric_limits** for implementation-specific types: e.g. GCC provides `std::numeric_limits<__int128>`. Non-standard libraries may [add specializations](https://en.cppreference.com/w/cpp/language/extending_std) for library-provided types, e.g. [OpenEXR](http://openexr.com/) provides `std::numeric_limits<half>` for a 16-bit floating-point type.

> NOTE: 
>
> 1、上述描述的是extend std library，C++是允许通过添加specialization来进行extend的



## cppreference [`std::numeric_limits<T>::epsilon`](https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon)

```C++
static T epsilon() throw(); // (until C++11)
static constexpr T epsilon() noexcept; // (since C++11)
```

Returns the machine epsilon, that is, the difference between 1.0 and the next value representable by the floating-point type `T`. It is only meaningful if `std::numeric_limits<T>::is_integer == false`.

> NOTE: 
>
> 1、仅仅对float point number才有用

### Example

Demonstrates the use of machine epsilon to compare floating-point values for equality

> NOTE: 
>
> 1、这是非常好的例子

```C++
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <algorithm>

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y, int ulp)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
	// unless the result is subnormal
					|| std::fabs(x - y) < std::numeric_limits<T>::min();
}

int main()
{
	double d1 = 0.2;
	double d2 = 1 / std::sqrt(5) / std::sqrt(5);
	std::cout << std::fixed << std::setprecision(20) << "d1=" << d1 << "\nd2=" << d2 << '\n';

	if (d1 == d2)
		std::cout << "d1 == d2\n";
	else
		std::cout << "d1 != d2\n";

	if (almost_equal(d1, d2, 2))
		std::cout << "d1 almost equals d2\n";
	else
		std::cout << "d1 does not almost equal d2\n";
}
// g++ test.cpp --std=c++11

```

> NOTE: 
>
> 1、上述例子涉及了如下内容:
>
> a、ULPs (units in the last place)
>
> b、subnormal
>
> 这些是理解上述例子的关键
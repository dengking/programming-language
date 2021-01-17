# `sizeof...` operator



## cppreference [`sizeof...` operator](https://en.cppreference.com/w/cpp/language/sizeof...)

Queries the number of elements in a [parameter pack](parameter_pack.html).

### Example

```c++
#include <iostream>
#include <array>
#include <type_traits>

template<typename ... Ts>
constexpr auto make_array(Ts&&... ts) -> std::array<std::common_type_t<Ts...>,sizeof...(ts)>
{
	return {	std::forward<Ts>(ts)...};
}

int main()
{
	auto b = make_array(1, 2, 3);
	std::cout << b.size() << '\n';
	for (auto i : b)
		std::cout << i << ' ';
}
// g++ --std=c++11 test.cpp

```


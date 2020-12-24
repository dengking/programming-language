# `std::for_each`



## cppreference [std::for_each](https://en.cppreference.com/w/cpp/algorithm/for_each)

## stackoverflow [Should I use std::for_each?](https://stackoverflow.com/questions/6260600/should-i-use-stdfor-each)





## Example

fluentcpp [On Design Patterns in C++](https://www.fluentcpp.com/2020/12/18/on-design-patterns-in-cpp/) # Strategy with `std::for_each`

```C++
#include <vector>
#include <algorithm>
#include <iostream>
int main()
{
	auto numbers = std::vector<int> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::for_each(begin(numbers), end(numbers), [](int &i)
	{	i *= 10;});
	// number now contains { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 }
	for (auto &&number : numbers)
	{
		std::cout << number << std::endl;
	}

}

```


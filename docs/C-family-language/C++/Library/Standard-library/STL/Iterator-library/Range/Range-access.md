# Range access

C++11为了支持[Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for)，iterator library 中引入了**Range access**。





## cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)





## cppreference [std::end, std::cend](https://en.cppreference.com/w/cpp/iterator/end)



## Example

### rosettacode [Loop over multiple arrays simultaneously](https://rosettacode.org/wiki/Loop_over_multiple_arrays_simultaneously)

```C++
#include <iostream>
#include <iterator>

int main(int argc, char *argv[])
{
	char lowers[] = { 'a', 'b', 'c' };
	char uppers[] = { 'A', 'B', 'C' };
	int nums[] = { 1, 2, 3 };

	auto ilow = std::begin(lowers);
	auto iup = std::begin(uppers);
	auto inum = std::begin(nums);

	for (; ilow != std::end(lowers) and iup != std::end(uppers) and inum != std::end(nums); ++ilow, ++iup, ++inum)
	{
		std::cout << *ilow << *iup << *inum << "\n";
	}
}

```





## User-defined overloads and ADL

关于这一点，在cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)、cppreference [std::end, std::cend](https://en.cppreference.com/w/cpp/iterator/end)中，都有描述，下面是截取自cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)：

> Similar to the use of `swap` (described in [*Swappable*](../named_req/Swappable.html)), typical use of the `begin` function in generic context is an equivalent of `using std::begin; begin(arg);`, which allows both the [ADL](../language/adl.html)-selected overloads for user-defined types and the standard library function templates to appear in the same overload set.
# Range access

在 [Iterator library# Range access](https://en.cppreference.com/w/cpp/iterator#Range_access) 总结了 range access interface，显然这样的设计是遵循"the interface principle"的，这样的设计能够充分发挥ADL。



## cppreference [Iterator library# Range access](https://en.cppreference.com/w/cpp/iterator#Range_access) 

| interface                                                    | explanation                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [begin cbegin](https://en.cppreference.com/w/cpp/iterator/begin)(C++11)(C++14) | returns an iterator to the beginning of a container or array (function template) |
| [end cend](https://en.cppreference.com/w/cpp/iterator/end)(C++11)(C++14) | returns an iterator to the end of a container or array (function template) |
| [rbegin crbegin](https://en.cppreference.com/w/cpp/iterator/rbegin)(C++14) | returns a reverse iterator to a container or array (function template) |
| [rend crend](https://en.cppreference.com/w/cpp/iterator/rend)(C++14) | returns a reverse end iterator for a container or array (function template) |
| [size ssize](https://en.cppreference.com/w/cpp/iterator/size)(C++17)(C++20) | returns the size of a container or array (function template) |
| [empty](https://en.cppreference.com/w/cpp/iterator/empty)(C++17) | checks whether the container is empty (function template)    |
| [data](https://en.cppreference.com/w/cpp/iterator/data)(C++17) | obtains the pointer to the underlying array (function template) |



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
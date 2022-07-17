# Zip

Python zip能够让我们:

1 Parallel iteration

> NOTE: 这是  realpython [Using the Python zip() Function for Parallel Iteration](https://realpython.com/python-zip-function/) 中的说法

2 Loop over multiple arrays simultaneously

> NOTE: 这是 rosettacode [Loop over multiple arrays simultaneously](https://rosettacode.org/wiki/Loop_over_multiple_arrays_simultaneously) 中的说法

C++中如何实现zip呢？本文对此进行讨论。

> NOTE: 关于Python zip，参见:
>
> 1 realpython [Using the Python zip() Function for Parallel Iteration](https://realpython.com/python-zip-function/)



## rosettacode [Loop over multiple arrays simultaneously](https://rosettacode.org/wiki/Loop_over_multiple_arrays_simultaneously)



### `std::vector`

```C++
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
	std::vector<char> ls(3);
	ls[0] = 'a';
	ls[1] = 'b';
	ls[2] = 'c';
	std::vector<char> us(3);
	us[0] = 'A';
	us[1] = 'B';
	us[2] = 'C';
	std::vector<int> ns(3);
	ns[0] = 1;
	ns[1] = 2;
	ns[2] = 3;

	std::vector<char>::const_iterator lIt = ls.begin();
	std::vector<char>::const_iterator uIt = us.begin();
	std::vector<int>::const_iterator nIt = ns.begin();
	for (; lIt != ls.end() && uIt != us.end() && nIt != ns.end(); ++lIt, ++uIt, ++nIt)
	{
		std::cout << *lIt << *uIt << *nIt << "\n";
	}
}
// g++ test.cpp
```

### Static array

```C++
#include <iostream>

int main(int argc, char *argv[])
{
	char ls[] = { 'a', 'b', 'c' };
	char us[] = { 'A', 'B', 'C' };
	int ns[] = { 1, 2, 3 };

	for (size_t li = 0, ui = 0, ni = 0; li < sizeof(ls) && ui < sizeof(us) && ni < sizeof(ns) / sizeof(int); ++li, ++ui, ++ni)
	{
		std::cout << ls[li] << us[ui] << ns[ni] << "\n";
	}
}

```

### C++11

#### `std::vector`

```C++
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
	auto lowers = std::vector<char>( { 'a', 'b', 'c' });
	auto uppers = std::vector<char>( { 'A', 'B', 'C' });
	auto nums = std::vector<int>( { 1, 2, 3 });

	auto ilow = lowers.cbegin();
	auto iup = uppers.cbegin();
	auto inum = nums.cbegin();

	for (; ilow != lowers.end() and iup != uppers.end() and inum != nums.end(); ++ilow, ++iup, ++inum)
	{
		std::cout << *ilow << *iup << *inum << "\n";
	}
}

```

#### Static array

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



## stackoverflow [Sequence-zip function for c++11?](https://stackoverflow.com/questions/8511035/sequence-zip-function-for-c11)

### [A](https://stackoverflow.com/a/8513803):  boost

**Warning:** `boost::zip_iterator` and `boost::combine` as of Boost 1.63.0 (2016 Dec 26) will cause undefined behavior if the length of the input containers are not the same (it may crash or iterate beyond the end).

------

Starting from Boost 1.56.0 (2014 Aug 7) you could [use `boost::combine`](http://www.boost.org/doc/libs/release/libs/range/doc/html/range/reference/utilities/combine.html) (the function exists in earlier versions but undocumented):

```cpp
#include <boost/range/combine.hpp>
#include <vector>
#include <list>
#include <string>

int main() {
    std::vector<int> a {4, 5, 6};
    double b[] = {7, 8, 9};
    std::list<std::string> c {"a", "b", "c"};
    for (auto tup : boost::combine(a, b, c, a)) {    // <---
        int x, w;
        double y;
        std::string z;
        boost::tie(x, y, z, w) = tup;
        printf("%d %g %s %d\n", x, y, z.c_str(), w);
    }
}
```

This would print

```
4 7 a 4
5 8 b 5
6 9 c 6
```

------

In earlier versions, you could define a range yourself like this:

```cpp
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>

template <typename... T>
auto zip(T&&... containers) -> boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(containers)...))>>
{
    auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
    auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
    return boost::make_iterator_range(zip_begin, zip_end);
}
```

### [A](https://stackoverflow.com/a/18771618): variadic template + first class function

So I wrote this `zip` before when I was bored, I decided to post it because it's different than the others in that it doesn't use boost and looks more like the c++ stdlib.

```cpp
#include <vector>
#include <iostream>
template<typename Iterator>
void advance_all(Iterator &iterator)
{
	++iterator;
}
template<typename Iterator, typename ... Iterators>
void advance_all(Iterator &iterator, Iterators &... iterators)
{
	++iterator;
	advance_all(iterators...);
}
template<typename Function, typename Iterator, typename ... Iterators>
Function zip(Function func, Iterator begin, Iterator end, Iterators ... iterators)
{
	for (; begin != end; ++begin, advance_all(iterators...))
		func(*begin, *(iterators)...);
	//could also make this a tuple
	return func;
}
int main()
{
	std::vector<int> v1 { 1, 2, 3 };
	std::vector<int> v2 { 3, 2, 1 };
	std::vector<float> v3 { 1.2, 2.4, 9.0 };
	std::vector<float> v4 { 1.2, 2.4, 9.0 };
	zip([](int i, int j, float k, float l)
	{
		std::cout << i << " " << j << " " << k << " " << l << std::endl;
	}, v1.begin(), v1.end(), v2.begin(), v3.begin(), v4.begin());
}
// g++ --std=c++11 test.cpp

```

Example use:

```cpp
int main () {
    std::vector<int> v1{1,2,3};
    std::vector<int> v2{3,2,1};
    std::vector<float> v3{1.2,2.4,9.0};
    std::vector<float> v4{1.2,2.4,9.0};
     zip (
            [](int i,int j,float k,float l){
                std::cout << i << " " << j << " " << k << " " << l << std::endl;
            },
            v1.begin(),v1.end(),v2.begin(),v3.begin(),v4.begin());
}
```



### [A](https://stackoverflow.com/a/51032349): [std::transform](https://en.cppreference.com/w/cpp/algorithm/transform)

> NOTE: 局限是: 最多只支持两个iterator

```c++
#include <vector>
#include <algorithm>
#include <iostream>
int main()
{
	std::vector<int> a = { 1, 2, 3, 4, 5 };
	std::vector<int> b = { 1, 2, 3, 4, 5 };
	std::vector<int> c;
	std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(c), [](const auto &aa, const auto &bb)
	{
		return aa*bb;
	});
	for (auto cc : c)
		std::cout << cc << std::endl;
}
// g++ --std=c++14 test.cpp

```



```C++
#include <vector>
#include <algorithm>
#include <iostream>
int main()
{
	std::vector<int> a = { 1, 2, 3, 4, 5 };
	std::vector<int> b = { 1, 2, 3, 4, 5 };
	std::vector<int> c;
	std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(c), [](const int &aa, const int &bb)
	{
		return aa*bb;
	});
	for (auto cc : c)
		std::cout << cc << std::endl;
}
// g++ --std=c++11 test.cpp

```

### [A](https://stackoverflow.com/a/43413384):  [range-v3](https://github.com/ericniebler/range-v3/)



### [A](https://stackoverflow.com/a/38339661): tuple

If you like operator overloading, here are three possibilities. The first two are using `std::pair<>` and `std::tuple<>`, respectively, as iterators; the third extends this to range-based `for`. Note that not everyone will like these definitions of the operators, so it's best to keep them in a separate namespace and have a `using namespace` in the functions (not files!) where you'd like to use these.

```cpp
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>

// put these in namespaces so we don't pollute global
namespace pair_iterators
{
template<typename T1, typename T2>
std::pair<T1, T2> operator++(std::pair<T1, T2> &it)
{
	++it.first;
	++it.second;
	return it;
}
}

namespace tuple_iterators
{
// you might want to make this generic (via param pack)
template<typename T1, typename T2, typename T3>
auto operator++(std::tuple<T1, T2, T3> &it)
{
	++(std::get<0>(it));
	++(std::get<1>(it));
	++(std::get<2>(it));
	return it;
}

template<typename T1, typename T2, typename T3>
auto operator*(const std::tuple<T1, T2, T3> &it)
{
	return std::tie(*(std::get<0>(it)), *(std::get<1>(it)), *(std::get<2>(it)));
}

// needed due to ADL-only lookup
template<typename ... Args>
struct tuple_c
{
	std::tuple<Args...> containers;
};

template<typename ... Args>
auto tie_c(const Args &... args)
{
	tuple_c<Args...> ret = { std::tie(args...) };
	return ret;
}

template<typename T1, typename T2, typename T3>
auto begin(const tuple_c<T1, T2, T3> &c)
{
	return std::make_tuple(std::get<0>(c.containers).begin(), std::get<1>(c.containers).begin(), std::get<2>(c.containers).begin());
}

template<typename T1, typename T2, typename T3>
auto end(const tuple_c<T1, T2, T3> &c)
{
	return std::make_tuple(std::get<0>(c.containers).end(), std::get<1>(c.containers).end(), std::get<2>(c.containers).end());
}

// implement cbegin(), cend() as needed
}

int main()
{
	using namespace pair_iterators;
	using namespace tuple_iterators;

	std::vector<double> ds = { 0.0, 0.1, 0.2 };
	std::vector<int> is = { 1, 2, 3 };
	std::vector<char> cs = { 'a', 'b', 'c' };

	// classical, iterator-style using pairs
	for (auto its = std::make_pair(ds.begin(), is.begin()), end = std::make_pair(ds.end(), is.end()); its != end; ++its)
	{
		std::cout << "1. " << *(its.first) + *(its.second) << " " << std::endl;
	}

	// classical, iterator-style using tuples
	for (auto its = std::make_tuple(ds.begin(), is.begin(), cs.begin()), end = std::make_tuple(ds.end(), is.end(), cs.end()); its != end; ++its)
	{
		std::cout << "2. " << *(std::get<0>(its)) + *(std::get<1>(its)) << " " << *(std::get<2>(its)) << " " << std::endl;
	}

	// range for using tuples
	for (const auto &d_i_c : tie_c(ds, is, cs))
	{
		std::cout << "3. " << std::get<0>(d_i_c) + std::get<1>(d_i_c) << " " << std::get<2>(d_i_c) << " " << std::endl;
	}
}
// g++ --std=c++11 test.cpp
```



```c++
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>

namespace tuple_iterators
{
// you might want to make this generic (via param pack)
template<typename ... Args>
auto operator++(std::tuple<Args...> &it)
{
	int size = std::tuple_size<std::tuple<Args...> >::value;
	for (int i = 0; i < size; ++i)
	{
		++(std::get<i>(it));
	}
	return it;
}
}

int main()
{

	using namespace tuple_iterators;

	std::vector<double> ds = { 0.0, 0.1, 0.2 };
	std::vector<int> is = { 1, 2, 3 };
	std::vector<char> cs = { 'a', 'b', 'c' };

	// classical, iterator-style using tuples
	for (auto its = std::make_tuple(ds.begin(), is.begin(), cs.begin()), end = std::make_tuple(ds.end(), is.end(), cs.end()); its != end; ++its)
	{
		std::cout << "2. " << *(std::get<0>(its)) + *(std::get<1>(its)) << " " << *(std::get<2>(its)) << " " << std::endl;
	}

}

```



## stackoverflow [What's the best way to iterate over two or more containers simultaneously](https://stackoverflow.com/questions/12552277/whats-the-best-way-to-iterate-over-two-or-more-containers-simultaneously)
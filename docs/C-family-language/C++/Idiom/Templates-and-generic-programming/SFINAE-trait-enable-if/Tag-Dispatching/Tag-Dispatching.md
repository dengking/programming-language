# Tag Dispatching



## [More C++ Idioms/Tag Dispatching](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)

### Intent

Simplify writing multiple SFINAE-constrained overloads

> NOTE: 这段话说明了tag dispatching和SFINAE-constrained overloads之间的关联，显然两者能够实现相同的目的。

### Motivation

Tag dispatching is a useful complement to `enable_if`.

It can also be used in conjunction with trailing return type and `decltype`. ([expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE))

### Application: Preference ordering

It is most useful when you have multiple overloads for the same function, and they all have **conditions** for when they can be called. With just `enable_if`, you have to test for not just the **overloads condition**, but also the negation of all the other overloads conditions, lest（避免） you get overload ambiguity. **Tag dispatch** will help reduce the mess:

> NOTE: 上面这段话中的“they all have conditions for when they can be called”其实就是conditional compile，在C++中，最常使用SFINAE来实现这个功能。
>
> 上面这段话需要结合具体例子来理解，结合下面的例子：
>
> `std::list`同时满足first overload、second overload、third overload，那如何来实现preference ordering呢？如果使用`enable_if`，则正如上面这段话中所说的：“you have to test for not just the **overloads condition**, but also the negation of all the other overloads conditions, lest（避免） you get overload ambiguity”。
>
> 使用tag dispatch + trailing return type + expression SFINAE可以实现：
>
> - trailing return type + expression SFINAE 来描述condition
>
> - tag实现preference ordering，tag之间存在着继承关系，可以对overload resolution进行控制（参见`C++\Language-reference\Functions\Overload-resolution.md`）
>
> 下面例子中，`pick_3 <- pick_2 <- pick_1`，显然，preference ordering为:`pick_3 < pick_2 < pick_1`。
>
> 

#### Example: tag dispatch + trailing return type + expression SFINAE 

This works because exact match is a better match than a base-class, which in turn is a better match than base of base, etc.

```c++
#include <array>
#include <algorithm> // std::remove_if
#include <iostream>
#include <string>
#include <list> // std::list

namespace detail
{
// tags for dispatching
struct pick_3
{
};
struct pick_2: pick_3
{
};
struct pick_1: pick_2
{
};
static pick_1 selector;

// first choice - member preferred if exists
template<typename Cont, typename Op>
auto remove_if(pick_1, Cont& cont, Op&& op)-> decltype(cont.remove_if(op), void())
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	cont.remove_if(std::forward<Op>(op));
}

// second choice - erase remove idiom
template<typename Cont, typename Op>
auto remove_if(pick_2, Cont& cont, Op&& op)-> decltype(cont.erase(std::remove_if(std::begin(cont), std::end(cont), std::forward<Op>(op)), std::end(cont)), void())
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	cont.erase(std::remove_if(std::begin(cont), std::end(cont), std::forward<Op>(op)), std::end(cont));
}

// last choice - manual looping
template<typename Cont, typename Op>
auto remove_if(pick_3, Cont& cont, Op&& op)-> void
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto it = std::begin(cont);
	while (it != std::end(cont))
	{
		if (op(*it))
			it = cont.erase(it);
		else
			++it;
	}
}
}

template<typename Cont, typename Op>
auto remove_if(Cont& cont, Op&& op)->void
{
	detail::remove_if(detail::selector, cont, std::forward<Op>(op));
}
template<typename Cont>
void print(Cont& cont)
{
	for (auto&& x : cont)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
}
int main()
{
	std::string str1 = "Text with some   spaces";
	std::cout << "Before:" << str1 << std::endl;
	remove_if(str1, [](unsigned char x){	return std::isspace(x);}); // 删除空格
	std::cout << "After:" << str1 << std::endl;

	std::list<int> mylist { 0, 1, 2, 3, 4, 5, 6 };
	std::cout << "Before:";
	print(mylist);
	remove_if(mylist, [](int x){	return x%2==0;}); // 删除偶数
	std::cout << "After:";
	print(mylist);
}
// g++ --std=c++11 test.cpp

```

上述程序的输出如下:

```c++
Before:Text with some   spaces
decltype ((cont.erase(std::remove_if(std::begin(cont), std::end(cont), forward<Op>(op)), std::end(cont)), void())) detail::remove_if(detail::pick_2, Cont&, Op&&) [with Cont = std::basic_string<char>; Op = main()::__lambda0; decltype ((cont.erase(std::remove_if(std::begin(cont), std::end(cont), forward<Op>(op)), std::end(cont)), void())) = void]
After:Textwithsomespaces
Before:0 1 2 3 4 5 6 
decltype ((cont.remove_if(op), void())) detail::remove_if(detail::pick_1, Cont&, Op&&) [with Cont = std::list<int>; Op = main()::__lambda1; decltype ((cont.remove_if(op), void())) = void]
After:1 3 5 
```

### Application: Category info

Tag dispatch can also be used when the tag carries useful information, not just a preference ordering.

For example 'dispatching' on `std::iterator_traits<It>::iterator_category{}` and have different algorithms for `std::random_access_iterator_tag` and `std::forward_iterator_tag`。

> NOTE: 从下面的例子可以看出，对于`implementation_details::alg`，两者都是template function，并且两者的入参都是相同的，如果不使用tag，仅仅依赖于overload resolution，无法实现对每种category都提供一个专门的overload。添加一个tag，能够实现准确的overload resolution。

#### Example: STL iterator tag

> NOTE: tag dispatch的一个典型的例子就是[Iterator library](https://en.cppreference.com/w/cpp/iterator)的[iterator tag](https://en.cppreference.com/w/cpp/iterator/iterator_tags)，下面的例子取自：https://en.cppreference.com/w/cpp/iterator/iterator_tags ：

```c++
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

// quite often implementation details are hidden in a dedicated namespace
namespace implementation_details
{
template<class BDIter>
void alg(BDIter, BDIter, std::bidirectional_iterator_tag)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << "alg() called for bidirectional iterator\n";
}

template<class RAIter>
void alg(RAIter, RAIter, std::random_access_iterator_tag)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << "alg() called for random-access iterator\n";
}
} // namespace implementation_details

template<class Iter>
void alg(Iter first, Iter last)
{
	implementation_details::alg(first, last,
			typename std::iterator_traits<Iter>::iterator_category());
}

int main()
{
	std::vector<int> v;
	alg(v.begin(), v.end());

	std::list<int> l;
	alg(l.begin(), l.end());

//    std::istreambuf_iterator<char> i1(std::cin), i2;
//    alg(i1, i2); // compile error: no matching function for call
}
// g++ test.cpp

```

上述程序的输出如下:

```c++
void implementation_details::alg(RAIter, RAIter, std::random_access_iterator_tag) [with RAIter = __gnu_cxx::__normal_iterator<int*, std::vector<int> >]
alg() called for random-access iterator
void implementation_details::alg(BDIter, BDIter, std::bidirectional_iterator_tag) [with BDIter = std::_List_iterator<int>]
alg() called for bidirectional iterator
```


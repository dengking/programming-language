# Range-based algorithms

主要用于实现generic programming。

## cpppatterns [Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html)

```c++
#include <iterator>
#include <utility>
template<typename ForwardRange>
void algorithm(ForwardRange& range)
{
	using std::begin;
	using std::end;
	using iterator = decltype(begin(range));
	iterator it_begin = begin(range);
	iterator it_end = end(range);
	// Now use it_begin and it_end to implement algorithm
}
```

### INTENT

Implement algorithms that can be applied to any generic range of elements.

### DESCRIPTION

On [lines 12–13](https://cpppatterns.com/patterns/range-based-algorithms.html#line12), we call `begin` and `end` on the range to get the respective iterators to the beginning and end of the range. We use using-declarations on [lines 7–8](https://cpppatterns.com/patterns/range-based-algorithms.html#line7) to allow these calls to be found via [argument-dependent lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup) before using the standard [`std::begin`](http://en.cppreference.com/w/cpp/iterator/begin) and [`std::end`](http://en.cppreference.com/w/cpp/iterator/end) functions. With these iterators, we can now implement the algorithm over the elements between them.

If the iterators are not necessary to implement the algorithm, we may instead be able to use a simple [range-based `for` loop](https://cpppatterns.com/common-tasks/range-iteration.html).



> NOTE: 在下面文章中，描述了ADL：
>
> - `C++\Language-reference\Functions\Function-calls\ADL\ADL.md`
> - `C++\Language-reference\Classes\The-interface-principle.md`



## [C++ algorithms library](https://en.cppreference.com/w/cpp/algorithm/)

C++ algorithms library中的大部分algorithm是典型的“range-based algorithm”，即它们的入参一般都是：

- `InputIt first`
- `InputIt last`
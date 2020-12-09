# Copy a range of elements



## cpppatterns [Copy a range of elements](https://cpppatterns.com/patterns/copy-range-of-elements.html)

> NOTE: 原文描述了三种情况的copy a range of elements：
>
> - copy the elements from the `source` range into a container
> - copy the elements of `source` into a range or container which already has the appropriate number of elements allocated
> - copy into a container that does not yet contain any elements
>
> `foo`函数，使用 [`std::begin`](http://en.cppreference.com/w/cpp/iterator/begin) 、[`std::end`](http://en.cppreference.com/w/cpp/iterator/end) ，这就限制了`RangeOfInts`必须要支持这两种operation。阅读[std::begin, std::cbegin#User-defined overloads](https://en.cppreference.com/w/cpp/iterator/begin#User-defined_overloads)节可知，可以通过提供user-defined overload来使user defined type支持std::begin、std::end；最好的做法是：结合[Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html)中的做法，采用类似于swap idiom的方式来实现。

```c++
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<int> target2(5);
std::vector<int> target3;

template<typename RangeOfInts>
void foo(RangeOfInts source)
{
	std::vector<int> target1 { std::begin(source),
			std::end(source) }; // 情况1
	std::copy(std::begin(source), std::end(source),
			std::begin(target2)); // 情况2
	std::copy(std::begin(source), std::end(source),
			std::back_inserter(target3)); // 情况3
}

int main()
{
    
}
```

### INTENT

Copy elements from a **range** to another range or container.

> NOTE: 函数`foo`的入参是range

### DESCRIPTION

#### 情况1

On [lines 11–12](https://cpppatterns.com/patterns/copy-range-of-elements.html#line11), we copy the elements from the `source` range into a container, `target1`, simply by passing the begin and end iterators of the range to the `std::vector<T>`’s constructor. We use [`std::begin`](http://en.cppreference.com/w/cpp/iterator/begin) and [`std::end`](http://en.cppreference.com/w/cpp/iterator/end) to obtain these iterators.

#### 情况2

To copy the elements of `source` into a range or container which already has the appropriate number of elements allocated, represented by `target2` on [line 5](https://cpppatterns.com/patterns/copy-range-of-elements.html#line5), we use [`std::copy`](http://en.cppreference.com/w/cpp/algorithm/copy) on [lines 14–15](https://cpppatterns.com/patterns/copy-range-of-elements.html#line14). The first two iterator arguments denote the source range, and the third iterator argument denotes the start of the target range. For this to work, the elements must already exist in the target range.

#### 情况3

To demonstrate how we can copy into a container that does not yet contain any elements, we have an empty `std::vector<int>` called `target3` on [line 6](https://cpppatterns.com/patterns/copy-range-of-elements.html#line6). For the third argument of `std::copy` ([lines 17–18](https://cpppatterns.com/patterns/copy-range-of-elements.html#line17)), we call [`std::back_inserter`](http://en.cppreference.com/w/cpp/iterator/back_inserter) to get an iterator that automatically calls `push_back` on `target3` for each element that is copied.




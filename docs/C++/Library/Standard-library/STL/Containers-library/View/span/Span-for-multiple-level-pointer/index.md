# Span for multidimensional array/multiple level pointer



## `gsl::multispan`

1、`gsl::multispan`已经被废弃了，关于此，在 microsoft [GSL 3.0.0 Release](https://devblogs.microsoft.com/cppblog/gsl-3-0-0-release/) 中进行了说明:

> ## Deprecation of `multi_span` and `strided_span`
>
> To more closely align Microsoft’s GSL to the C++ Core Guidelines, we decided to deprecate our implementation of `gsl::multi_span` and `gsl::strided_span`. For the time being, we will continue to provide these headers, but they will not be actively worked on or maintained unless the C++ Core Guidelines identifies a need for them.

2、`gsl::multispan` 是典型的view

### stackoverflow [CPPCoreGuidelines span for a T** interface?](https://stackoverflow.com/questions/41880770/cppcoreguidelines-spant-for-a-t-interface)

In digital signal processing audio is commonly passed around as a 2D array of channels and samples i.e.

```cpp
void use(float** buffer, int channels, int samples) { ... }
```

A lot of libraries I use expect this format.

In terms of my own code, for safer manipulation, is there a way I could use `gsl::span<T>` to set up views on these buffers?

(I understand 1D but I'm not sure how I would set up for a 2D `float**` array)

Thanks in advance.

#### Comment

Use `gsl::multispan` (not well documented) – [Galik](https://stackoverflow.com/users/3807729/galik) Jan 26 '17 at 19:25



### stackoverflow [What is gsl::multi_span to be used for?](https://stackoverflow.com/questions/45201524/what-is-gslmulti-span-to-be-used-for)

The C++ core guidelines mention spans, not "multi-spans". But - I see that Microsoft's GSL implementation has a [`multi_span` class](https://github.com/Microsoft/GSL/blob/master/include/gsl/multi_span)

```c++
template <
    typename ValueType,
    std::ptrdiff_t FirstDimension,
    std::ptrdiff_t... RestDimensions
>
class multi_span { ... };
```

So, obviously this is some sort of a multi-dimensional version of `gsl::span`. But what is that supposed to mean? Why do we need this multi-dimensional span, or rather - when would we use it? I can't seem to find any documentation on this.

#### A

In short, it is a span over contiguous piece of memory, which represents multidimensional array.

Here is an example of use:

```c++
int data[6] = {0, 1, 2, 3, 4, 5};
multi_span<int, 2, 3> span{data, 6};
std::cout << span[1][1] << '\n'; //Outputs 4
```

From the linked source, it seems, that it also supports runtime bounds, but I am not sure about proper syntax for those.



## library [nitronoid](https://github.com/nitronoid)/**[multi_span](https://github.com/nitronoid/multi_span)**

```C++
#include <stdex/multi_span>
#include <vector>
#include <iostream>
#include <string>
#include <numeric>

int main()
{
	using namespace std;
	/*
	 * Print a vector as a 2D matrix
	 */

	// Define a vector
	vector<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	// Create a 2D-view over the vector
	stdex::multi_span<int, 3, 4> my_view(arr);

	// Print all elements of the vector, via the view
	printf("2D view:\n");
	for (int r = 0; r < my_view.extent<0>(); ++r)
	{
		for (int c = 0; c < my_view.extent<1>(); ++c)
		{
			printf("%d ", my_view(r, c));
		}
		printf("\n");
	}
	printf("\n");

	/*
	 * Sum the elements within a sliding 2D sub-view
	 */

	// Define the size of the sub-view
	auto const wsize = 2;
	// Obtain the bounds of our 2D-view
	auto const range = my_view.bounds();

	// Use static_bounds as counting iterators
	using counting_iter = stdex::static_bounds<stdex::dynamic_range, stdex::dynamic_range>;
	// Create a lazy 2D iterable sequence
	counting_iter bnds( { range[0] - wsize + 1, range[1] - wsize + 1 });

	// Iterate through the sequence, obtaining a 2D-coordinate
	for_each(begin(bnds), end(bnds), [&](auto idx)
	{
		// Obtain a sub-view of our main view
					auto const section = my_view.section(idx,
									{	wsize, wsize});
					// Print all elements in the sub-view
					copy(begin(section), end(section), ostream_iterator<int>(cout, " "));
					// Iterate over the sub-view and sum the elements within
					auto const total = accumulate(begin(section), end(section), 0);
					// Print the total
					printf(": sums to %d.\n", total);
				});
	printf("\n");

	return 0;
}

```



## 我的实现

```C++
/**
 * @brief 对二级指针数组的简单封装
 *
 * @tparam T
 */
template<class T>
class multi_span
{
	std::vector<T*> data_;
	std::size_t size_ { 0 };
public:
	/// The type of value, including cv qualifiers
	using element_type = T;

	using value_type = typename std::vector<T*>::value_type;

	/// The type of integer used to index the span
	using index_type = std::ptrdiff_t;

	/// A pointer to a span element
	using pointer = T*;

	/// A reference to a span element
	using reference = T&;

	/// The iterator used by the container
	using iterator = typename std::vector<T*>::iterator;

	/// The const pointer used by the container
	using const_pointer = T const*;

	/// The const reference used by the container
	using const_reference = T const&;

	/// The const iterator used by the container
	using const_iterator = typename std::vector<T*>::const_iterator;

	/// Constructor
	multi_span() = default;

	/// Constructor
	multi_span(multi_span const&) = default;

	/// Assignment
	multi_span& operator=(multi_span const&) = default;

	/** Constructor

	 @param data A pointer to the beginning of the range of elements

	 @param size The number of elements pointed to by `data`
	 */
	multi_span(std::size_t size) :
					size_(size)
	{
		data_.reserve(size_);
	}

	/// Returns `true` if the span is empty
	bool empty() const
	{
		return size_ == 0;
	}

	/// Returns a pointer to the beginning of the span
	T**
	data() const
	{
		return data_.data();
	}

	/// Returns the number of elements in the span
	std::size_t size() const
	{
		return size_;
	}

	/// Returns an iterator to the beginning of the span
	const_iterator begin() const
	{
		return data_.begin();
	}

	/// Returns an iterator to the beginning of the span
	const_iterator cbegin() const
	{
		return data_.cbegin();
	}

	/// Returns an iterator to one past the end of the span
	const_iterator end() const
	{
		return data_.end();
	}

	/// Returns an iterator to one past the end of the span
	const_iterator cend() const
	{
		return data_.cend();
	}

	void push_back(value_type p)
	{
		data_.push_back(p);
	}
};
```


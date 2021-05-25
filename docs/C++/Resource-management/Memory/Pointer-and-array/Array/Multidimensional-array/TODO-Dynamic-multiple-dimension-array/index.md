# Dynamic multiple dimension array

## geeksforgeeks [How to declare a 2D array dynamically in C++ using new operator](https://www.geeksforgeeks.org/how-to-declare-a-2d-array-dynamically-in-c-using-new-operator/)

Below is the diagrammatic representation of 2D arrays:

![img](./gfg5.JPG)



### **Method 1:** using a single [pointer](https://www.geeksforgeeks.org/pointers-in-c-and-c-set-1-introduction-arithmetic-and-array/)

```C++
// C++ program to dynamically allocate
// the memory for 2D array in C++
// using new operator
#include <iostream>
using namespace std;

// Driver Code
int main()
{
	// Dimensions of the 2D array
	int m = 3, n = 4, c = 0;

	// Declare a memory block of
	// size m*n
	int *arr = new int[m * n];

	// Traverse the 2D array
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Assign values to
			// the memory block
			*(arr + i * n + j) = ++c;
		}
	}

	// Traverse the 2D array
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{

			// Print values of the
			// memory block
			cout << *(arr + i * n + j) << " ";
		}
		cout << endl;
	}

	return 0;
}
// g++ test.cpp

```



### **Method 2:** using an [array of pointer](https://www.geeksforgeeks.org/difference-between-pointer-to-an-array-and-array-of-pointers/): Here an array of pointers is created and then to each memory block. Below is the diagram to illustrate the concept:

![img](./Capture4.JPG)



```C++
// C++ program to dynamically allocate
// the memory for 3D array in C++
// using new operator
#include <iostream>
using namespace std;

// Driver Code
int main()
{
	// Dimensions of the array
	int m = 3, n = 4, c = 0;

	// Declare memory block of size M
	int **a = new int*[m];

	for (int i = 0; i < m; i++)
	{

		// Declare a memory block
		// of size n
		a[i] = new int[n];
	}

	// Traverse the 2D array
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{

			// Assign values to the
			// memory blocks created
			a[i][j] = ++c;
		}
	}

	// Traverse the 2D array
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{

			// Print the values of
			// memory blocks created
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
// g++ test.cpp

```

## 我的实践

```C++
#include <iostream>
#include <vector>
#include <stdexcept>
template<typename T>
class TwoDimensionArray
{
public:
	TwoDimensionArray(std::size_t row, std::size_t col) :
					m_row { row }, m_col { col }
	{
		m_data.resize(m_row * m_col);
	}
	T& at(std::size_t row_index, std::size_t col_index)
	{
		if (row_index < m_row && col_index < m_col)
		{
			return m_data[row_index * m_col + col_index];
		}
		else
		{
			throw std::logic_error("无效的索引");
		}
	}
private:
	std::size_t m_row { 0 };
	std::size_t m_col { 0 };
	std::vector<T> m_data;
};

int main()
{
	int row = 3, col = 4;
	TwoDimensionArray<int> a { 3, 4 };
	int c = 0;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			a.at(i, j) = c++;
		}
	}
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			std::cout << a.at(i, j) << " ";
		}
		std::cout << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```





## stackoverflow [Create a multidimensional array dynamically in C++](https://stackoverflow.com/questions/47664127/create-a-multidimensional-array-dynamically-in-c)



### [A](https://stackoverflow.com/a/47664858)

> NOTE: 
>
> 1、下面的实现是非常通用的，它支持任意维度的array
>
> 2、它是典型的flat，即将一个multiple dimension array flat为 一个one dimension array

In general, nesting `std::vector` is not a great idea. It's usually a better plan to allocate memory which will hold the entirety of your multidimensonal array as a contiguous block, and then index into it *as if* it were multidimensional. This memory block *could* be allocated via `new`, but unless you need some precise control over how it's allocated (custom allocator), I'd recommend sticking with a single `std::vector`.

It's not difficult to create a class to manage such a resource in which the number of dimensions can be set dynamically. A good way to organize such a class is to keep track of the allocated memory, the sizes of each dimension, and the *stride pattern* for each dimension. The strides describe how many elements must be incremented over in order to reach the next element along a given dimension.

> NOTE: 
>
> 一、"stride"的意思是**跨步**，那如何来计算"stride"呢？
>
> 正确计算stride，需要从"containing、nesting、recursive definition"的思维来看待multiple array，即它是array of array，每一维度的跨度由它的subarray的长度决定。
>
> 下面代码中的`compute_strides()`就是采用的这个思路

This allows efficient indexing (just pointer arithmetic), as well as *very* efficient reshaping: as long as the number of elements doesn't change, this just requires changing the shape and stride arrays.

------

**Example:**

Here's a very basic class which will store such a dynamical multidimensional array of `double`s. It stores data in row-major order, meaning that the last index varies the fastest. So for a 2D array, the first row is stored contiguously, followed by the second row, and so on.

You can reshape the array, changing the number of dimensions, if you want. A basic element access `operator[]` is shown, too. There's nothing else fancy about the class, but you can extend it to provide whatever functionality you want, e.g., iterators, mathematical operations on the data, I/O operators, etc.

```cpp
/*! \file dynamic_array.h
 * Basic dynamic multi-dimensional array of doubles.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <vector>
#include <numeric>
#include <functional>

class dynamic_array
{
public:
	dynamic_array(const std::vector<int> &shape) :
					m_nelem(std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>())), m_ndim(shape.size()), m_shape(shape)
	{
		compute_strides();
		m_data.resize(m_nelem, 0.0);
	}

	~dynamic_array()
	{
	}

	const double& operator[](int i) const
	{
		return m_data.at(i);
	}

	double& operator[](int i)
	{
		return m_data.at(i);
	}

	const double& operator[](const std::vector<int> &indices) const
	{
		auto flat_index = std::inner_product(indices.begin(), indices.end(), m_strides.begin(), 0);
		return m_data.at(flat_index);
	}

	double& operator[](const std::vector<int> &indices)
	{
		auto flat_index = std::inner_product(indices.begin(), indices.end(), m_strides.begin(), 0);
		return m_data.at(flat_index);
	}

	void reshape(const std::vector<int> &new_shape)
	{
		auto new_nelem = std::accumulate(new_shape.begin(), new_shape.end(), 1, std::multiplies<int>());
		if (new_nelem != m_nelem)
		{
			throw std::invalid_argument("dynamic_array::reshape(): "
							"number of elements must not change.");
		}
		m_nelem = new_nelem;
		m_ndim = new_shape.size();
		m_shape = new_shape;
		compute_strides();
	}

	const std::vector<int>& shape() const
	{
		return m_shape;
	}

	const std::vector<int>& strides() const
	{
		return m_strides;
	}

	int ndim() const
	{
		return m_ndim;
	}

	int nelem() const
	{
		return m_nelem;
	}

private:
	int m_ndim;
	int m_nelem;
	std::vector<int> m_shape;
	std::vector<int> m_strides;
	std::vector<double> m_data;

	void compute_strides()
	{
		m_strides.resize(m_ndim);
		m_strides.at(m_ndim - 1) = 1; // 最后一个维度是一个一维数组，因此，它的跨度是1
		std::partial_sum(m_shape.rbegin(), m_shape.rend() - 1, m_strides.rbegin() + 1, std::multiplies<int>());
	}
};

#endif // include guard

```

> NOTE: 
>
> 1、[std::inner_product](https://en.cppreference.com/w/cpp/algorithm/inner_product)
>
> ```C++
> template<class InputIt1, class InputIt2, class T>
> constexpr // since C++20
> T inner_product(InputIt1 first1, InputIt1 last1,
>              InputIt2 first2, T init)
> {
>  while (first1 != last1) {
>       init = std::move(init) + *first1 * *first2; // std::move since C++20
>       ++first1;
>       ++first2;
>  }
>  return init;
> }
> ```
>
> 
>
> 2、[std::accumulate](https://en.cppreference.com/w/cpp/algorithm/accumulate)
>
> ```C++
> template<class InputIt, class T, class BinaryOperation>
> constexpr // since C++20
> T accumulate(InputIt first, InputIt last, T init, 
>              BinaryOperation op)
> {
>     for (; first != last; ++first) {
>         init = op(std::move(init), *first); // std::move since C++20
>     }
>     return init;
> }
> ```
>
> 3、[std::partial_sum](https://en.cppreference.com/w/cpp/algorithm/partial_sum)
>
> ```C++
> *(d_first)   = *first;
> *(d_first+1) = *first + *(first+1);
> *(d_first+2) = *first + *(first+1) + *(first+2);
> *(d_first+3) = *first + *(first+1) + *(first+2) + *(first+3);
> ...
> ```
>
> ```c++
> template <class InputIterator, class OutputIterator>
>    OutputIterator partial_sum (InputIterator first, InputIterator last,
>                                OutputIterator result)
> {
>   if (first!=last) {
>     typename iterator_traits<InputIterator>::value_type val = *first;
>     *result = val;
>     while (++first!=last) {
>       val = val + *first;   // or: val = binary_op(val,*first)
>       *++result = val;
>     }
>     ++result;
>   }
>   return result;
> }
> ```
>
> 

Here's a basic demo of the functionality.

```cpp
/*! \file test.cc
 * Basic test of the dynamic_array class.
 */
#include "dynamic_array.h"
#include <iostream>

int main(int /* argc */, const char * /* argv */[])
{
    dynamic_array arr({2, 3});
    std::cout << "Shape: { ";
    for (auto& each : arr.shape())
        std::cout << each << " ";
    std::cout << "}" << std::endl;

    std::cout << "Strides: { ";
    for (auto& each : arr.strides())
        std::cout << each << " ";
    std::cout << "}" << std::endl;

    // Reshape array, changing number of dimensions, but
    // keeping number of elements constant.
    arr.reshape({6});
    std::cout << "Shape: { ";
    for (auto& each : arr.shape())
        std::cout << each << " ";
    std::cout << "}" << std::endl;

    // Verify that the stride pattern has now also changed.
    std::cout << "Strides: { ";
    for (auto& each : arr.strides())
        std::cout << each << " ";
    std::cout << "}" << std::endl;

    return 0;
}
```

You can compile the test program with `g++ -std=c++14 -o test test.cc`, assuming the file defining the class is in the same directory as `test.cc`.



#### 完整测试程序

```C++
#include <vector>
#include <numeric>
#include <functional>
#include <iostream>

class dynamic_array
{
public:
	dynamic_array(const std::vector<int> &shape) :
					m_nelem(std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>())), m_ndim(shape.size()), m_shape(shape)
	{
		compute_strides();
		m_data.resize(m_nelem, 0.0);
	}

	~dynamic_array()
	{
	}

	const double& operator[](int i) const
	{
		return m_data.at(i);
	}

	double& operator[](int i)
	{
		return m_data.at(i);
	}

	const double& operator[](const std::vector<int> &indices) const
	{
		auto flat_index = std::inner_product(indices.begin(), indices.end(), m_strides.begin(), 0);
		return m_data.at(flat_index);
	}

	double& operator[](const std::vector<int> &indices)
	{
		auto flat_index = std::inner_product(indices.begin(), indices.end(), m_strides.begin(), 0);
		return m_data.at(flat_index);
	}

	void reshape(const std::vector<int> &new_shape)
	{
		auto new_nelem = std::accumulate(new_shape.begin(), new_shape.end(), 1, std::multiplies<int>());
		if (new_nelem != m_nelem)
		{
			throw std::invalid_argument("dynamic_array::reshape(): "
							"number of elements must not change.");
		}
		m_nelem = new_nelem;
		m_ndim = new_shape.size();
		m_shape = new_shape;
		compute_strides();
	}

	const std::vector<int>& shape() const
	{
		return m_shape;
	}

	const std::vector<int>& strides() const
	{
		return m_strides;
	}

	int ndim() const
	{
		return m_ndim;
	}

	int nelem() const
	{
		return m_nelem;
	}

private:
	int m_ndim;
	int m_nelem;
	std::vector<int> m_shape;
	std::vector<int> m_strides;
	std::vector<double> m_data;

	void compute_strides()
	{
		m_strides.resize(m_ndim);
		m_strides.at(m_ndim - 1) = 1; // 最后一个维度是一个一维数组，因此，它的跨度是1
		std::partial_sum(m_shape.rbegin(), m_shape.rend() - 1, m_strides.rbegin() + 1, std::multiplies<int>());
	}
};

int main(int /* argc */, const char* /* argv */[])
{
	dynamic_array arr( { 2, 3 });
	std::cout << "Shape: { ";
	for (auto &each : arr.shape())
		std::cout << each << " ";
	std::cout << "}" << std::endl;

	std::cout << "Strides: { ";
	for (auto &each : arr.strides())
		std::cout << each << " ";
	std::cout << "}" << std::endl;

	// Reshape array, changing number of dimensions, but
	// keeping number of elements constant.
	arr.reshape( { 6 });
	std::cout << "Shape: { ";
	for (auto &each : arr.shape())
		std::cout << each << " ";
	std::cout << "}" << std::endl;

	// Verify that the stride pattern has now also changed.
	std::cout << "Strides: { ";
	for (auto &each : arr.strides())
		std::cout << each << " ";
	std::cout << "}" << std::endl;

	return 0;
}
// g++ --std=c++11 test.cpp

```


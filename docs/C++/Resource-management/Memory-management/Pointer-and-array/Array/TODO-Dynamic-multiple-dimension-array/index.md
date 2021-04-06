# Dynamic multiple dimension array

geeksforgeeks [How to declare a 2D array dynamically in C++ using new operator](https://www.geeksforgeeks.org/how-to-declare-a-2d-array-dynamically-in-c-using-new-operator/)

## stackoverflow [Create a multidimensional array dynamically in C++](https://stackoverflow.com/questions/47664127/create-a-multidimensional-array-dynamically-in-c)



### [A](https://stackoverflow.com/a/47664858)

In general, nesting `std::vector` is not a great idea. It's usually a better plan to allocate memory which will hold the entirety of your multidimensonal array as a contiguous block, and then index into it *as if* it were multidimensional. This memory block *could* be allocated via `new`, but unless you need some precise control over how it's allocated (custom allocator), I'd recommend sticking with a single `std::vector`.

It's not difficult to create a class to manage such a resource in which the number of dimensions can be set dynamically. A good way to organize such a class is to keep track of the allocated memory, the sizes of each dimension, and the *stride pattern* for each dimension. The strides describe how many elements must be incremented over in order to reach the next element along a given dimension.

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
		m_strides.at(m_ndim - 1) = 1;
		std::partial_sum(m_shape.rbegin(), m_shape.rend() - 1, m_strides.rbegin() + 1, std::multiplies<int>());
	}
};

#endif // include guard

```

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
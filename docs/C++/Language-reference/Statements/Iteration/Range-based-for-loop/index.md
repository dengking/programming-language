# Range-based for loop 

## cpppatterns [Range iteration](https://cpppatterns.com/patterns/range-iteration.html)

> NOTE: 这篇文章，对range-based for loop的介绍是更加易懂的。

The [range-based `for` loop](http://en.cppreference.com/w/cpp/language/range-for) provides a simple syntax for iterating over elements of a **range** without using iterators or indices. It supports arrays, types that provide `begin` and `end` **member functions**, and types for which `begin` and `end` functions are found via [argument-dependent lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup).



## cppreference [Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for)





*`begin_expr`* and *`end_expr`* are defined as follows:

> NOTE: 如何实现？



## Example



stackoverflow [Is there a range class in C++11 for use with range based for loops?](https://stackoverflow.com/questions/7185437/is-there-a-range-class-in-c11-for-use-with-range-based-for-loops)

```c++
#include <iostream>

template<long int T_begin, long int T_end>
class range_class
{
public:
	class iterator
	{
		friend class range_class;
		public:
		long int operator *() const
		{
			return i_;
		}
		const iterator &operator ++()
		{
			++i_;
			return *this;
		}
		iterator operator ++(int)
		{
			iterator copy(*this);
			++i_;
			return copy;
		}

		bool operator ==(const iterator &other) const
				{
			return i_ == other.i_;
		}
		bool operator !=(const iterator &other) const
				{
			return i_ != other.i_;
		}

	protected:
		iterator(long int start)
				: i_(start)
		{
		}

	private:
		unsigned long i_;
	};

	iterator begin() const
	{
		return iterator(T_begin);
	}
	iterator end() const
	{
		return iterator(T_end);
	}
};

template<long int T_begin, long int T_end>
const range_class<T_begin, T_end>
range()
{
	return range_class<T_begin, T_end>();
}

int main()
{
	for (auto i : range<0, 10>())
	{
		std::cout << i << std::endl;
	}
}
// g++ --std=c++11 test.cpp
```

输出如下:

```
0
1
2
3
4
5
6
7
8
9
```



## Example 

https://stackoverflow.com/a/7185723

```c++
#include <iostream>
#include <sstream>

class range
{
public:
	class iterator
	{
		friend class range;
		public:
		long int operator *() const
		{
			return i_;
		}
		const iterator &operator ++()
		{
			++i_;
			return *this;
		}
		iterator operator ++(int)
		{
			iterator copy(*this);
			++i_;
			return copy;
		}

		bool operator ==(const iterator &other) const
				{
			return i_ == other.i_;
		}
		bool operator !=(const iterator &other) const
				{
			return i_ != other.i_;
		}

	protected:
		iterator(long int start)
				: i_(start)
		{
		}

	private:
		unsigned long i_;
	};

	iterator begin() const
	{
		return begin_;
	}
	iterator end() const
	{
		return end_;
	}
	range(long int begin, long int end)
			: begin_(begin), end_(end)
	{
	}
private:
	iterator begin_;
	iterator end_;
};

int main()
{
	int m, n;
	std::istringstream in("10 20");
	if (in >> m >> n) //using in, because std::cin cannot be used at coliru.
	{
		if (m > n)
		{
			std::swap(m, n);
		}
		for (auto i : range(m, n))
		{
			std::cout << i << " ";
		}
	}
	else
	{
		std::cout << "invalid input";
	}
}
// g++ --std=c++11 test.cpp
```



## Example

https://stackoverflow.com/a/8578101

```c++
#include <iostream>
using namespace std;

struct r_iterator
{
	long int value;
	r_iterator(long int _v)
			: value(_v)
	{
	}
	operator long int() const
	{
		return value;
	}
	operator long int&()
	{
		return value;
	}
	long int operator*() const
	{
		return value;
	}
};
template<long int _begin, long int _end>
struct range
{
	static r_iterator begin()
	{
		return _begin;
	}
	static r_iterator end()
	{
		return _end;
	}
};
int main()
{
	for (auto i : range<0, 10>())
	{
		cout << i << endl;
	}
	return 0;
}
// g++ --std=c++11 test.cpp
```

## Example: 

stackoverflow [Viewing a raw pointer as a range in range-based for-loop](https://stackoverflow.com/questions/28242073/viewing-a-raw-pointer-as-a-range-in-range-based-for-loop) # [A](https://stackoverflow.com/a/28242402)

A workaround for what you want to do is just make a simple wrapper:

```cpp
template <typename T> 
struct PtrWrapper {
    T* p;
    T* begin() const { return p; }
    T* end() const { return p ? p+1 : nullptr; }
};

for (double& d : PtrWrapper<double>{dptr}) { .. }
```



## geeksforgeeks [Reversed Range-based for loop in C++ with Examples](https://www.geeksforgeeks.org/reversed-range-based-for-loop-in-c-with-examples/)

```c++
// C++ program for reverse 
// range-based for loop 
#include <bits/stdc++.h> 
  
// For reversing range based loop 
#include <boost/range/adaptor/reversed.hpp> 
using namespace std; 
  
// Driver Code 
int main() 
{ 
    string s = "geeksforgeeks"; 
  
    int y[] = { 1, 2, 3, 4, 5, 6, 7, 8 }; 
  
    vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8 }; 
  
    // Reverse range-based for loop 
    // to reverse string 
    for (auto x : boost::adaptors::reverse(s)) 
        cout << x << " "; 
    cout << endl; 
  
    // Reverse range-based for loop 
    // to reverse array 
    for (auto x : boost::adaptors::reverse(y)) 
        cout << x << " "; 
    cout << endl; 
  
    // Reverse range-based for loop 
    // to reverse vector 
    for (auto x : boost::adaptors::reverse(v1)) 
        cout << x << " "; 
    cout << endl; 
  
    return 0; 
} 
```


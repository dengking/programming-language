# `std::sort`

## geeksforgeeks [Sorting a vector in C++](https://www.geeksforgeeks.org/sorting-a-vector-in-c/)

```c++
// C++ program to sort a vector in non-decreasing
// order.
#include <bits/stdc++.h>
using namespace std;

int main()
{
	vector<int> v{ 1, 5, 8, 9, 6, 7, 3, 4, 2, 0 };

	sort(v.begin(), v.end());

	cout << "Sorted \n";
	for (auto x : v)
		cout << x << " ";

	return 0;
}

```

Output :

```
Sorted 
0 1 2 3 4 5 6 7 8 9
```

### **How to sort in descending order?**

```C++
// C++ program to sort a vector in non-increasing
// order.
#include <bits/stdc++.h>
using namespace std;

int main()
{
	vector<int> v{ 1, 5, 8, 9, 6, 7, 3, 4, 2, 0 };

	sort(v.begin(), v.end(), greater<int>());

	cout << "Sorted \n";
	for (auto x : v)
		cout << x << " ";

	return 0;
}

```

```
Sorted 
9 8 7 6 5 4 3 2 1 0 
```



### **How to sort in particular order?**

```c++
// A C++ program to sort vector using
// our own comparator
#include <bits/stdc++.h>
using namespace std;

// An interval has start time and end time
struct Interval {
	int start, end;
};

// Compares two intervals according to staring times.
bool compareInterval(Interval i1, Interval i2)
{
	return (i1.start < i2.start);
}

int main()
{
	vector<Interval> v { { 6, 8 }, { 1, 9 }, { 2, 4 }, { 4, 7 } };

	// sort the intervals in increasing order of
	// start time
	sort(v.begin(), v.end(), compareInterval);

	cout << "Intervals sorted by start time : \n";
	for (auto x : v)
		cout << "[" << x.start << ", " << x.end << "] ";

	return 0;
}

```



## cppreference [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort)

Sorts the elements in the range `[first, last)` in non-descending order. The order of equal elements is not guaranteed to be preserved.

> NOTE:
>
> 默认是升序

### Example

```C++
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <string_view>

int main()
{
	std::array<int, 10> s = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };

	auto print = [&s](std::string_view const rem)
	{
		for (auto a : s)
		{
			std::cout << a << ' ';
		}
		std::cout << ": " << rem << '\n';
	};

	std::sort(s.begin(), s.end());
	print("sorted with the default operator<");

	std::sort(s.begin(), s.end(), std::greater<int>());
	print("sorted with the standard library compare function object");

	struct
	{
		bool operator()(int a, int b) const
		{
			return a < b;
		}
	} customLess;
	std::sort(s.begin(), s.end(), customLess);
	print("sorted with a custom function object");

	std::sort(s.begin(), s.end(), [](int a, int b)
	{
		return a > b;
	});
	print("sorted with a lambda expression");
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -g

```



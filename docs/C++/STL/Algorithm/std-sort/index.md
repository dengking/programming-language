# std::sort

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
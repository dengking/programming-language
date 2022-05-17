# `resize` and `reserve`



## Example

```C++
#include <iostream>
#include <vector>

int main()
{
	std::vector<int> v;
	v.reserve(1);
	std::cout << v.size() << std::endl;
	std::vector<int> v1;
	v1.resize(1);
	std::cout << v1.size() << std::endl;
}
// g++ test.cpp

```

输出如下:

```C++
0
1
```





## stackoverflow [std::vector::resize() vs. std::vector::reserve()](https://stackoverflow.com/questions/13029299/stdvectorresize-vs-stdvectorreserve)

I am pre-allocating some memory to my a `vector` member variable. Below code is minimal part

```cpp
class A {
  vector<string> t_Names;
public:
  A () : t_Names(1000) {}
};
```

Now at some point of time, if the `t_Names.size()` equals `1000`. I am intending to increase the size by `100`. Then if it reaches `1100`, again increase by `100` and so on.

My question is, what to choose between `vector::resize()` and `vector::reserve()`. Is there any better choice in this kind of scenario ?

**Edit**: I have sort of precise estimate for the `t_Names`. I estimate it to be around `700` to `800`. However in *certain* (seldom) situations, it can grow more than `1000`.

### [A](https://stackoverflow.com/a/7397862)

The two functions do vastly different things!

The [`resize()`](http://en.cppreference.com/w/cpp/container/vector/resize) method (and passing argument to constructor is equivalent to that) will insert or delete appropriate number of elements to the vector to make it given size (it has optional second argument to specify their value). It will affect the `size()`, iteration will **go over**(遍历) all those elements, `push_back` will insert after them and you can directly access them using the `operator[]`.

The [`reserve()`](http://en.cppreference.com/w/cpp/container/vector/reserve) method only allocates memory, but leaves it uninitialized. It only affects `capacity()`, but `size()` will be unchanged. There is no value for the objects, because nothing is added to the vector. If you then insert the elements, no reallocation will happen, because it was done in advance, but that's the only effect.

> NOTE: 
>
> `resize` size
>
> `reserve` capacity

So it depends on what you want. If you want an array of 1000 default items, use `resize()`. If you want an array to which you expect to insert 1000 items and want to avoid a couple of allocations, use `reserve()`.

**EDIT:** Blastfurnace's comment made me read the question again and realize, that in your case the correct answer is **don't preallocate** manually. Just keep inserting the elements at the end as you need. The vector will automatically reallocate as needed and will do it *more* efficiently than the manual way mentioned. The only case where `reserve()` makes sense is when you have reasonably precise estimate of the total size you'll need easily available in advance.

> NOTE: optimization principle

**EDIT2:** Ad question edit: If you have initial estimate, then `reserve()` that estimate. If it turns out to be not enough, just let the vector do it's thing.

### [A](https://stackoverflow.com/a/7397849)

`resize()` not only allocates memory, it also *creates* as many instances as the *desired* size which you pass to `resize()` as argument. But `reserve()` only allocates memory, it doesn't create instances. That is,

```cpp
std::vector<int> v1;
v1.resize(1000); //allocation + instance creation
cout <<(v1.size() == 1000)<< endl;   //prints 1
cout <<(v1.capacity()==1000)<< endl; //prints 1

std::vector<int> v2;
v2.reserve(1000); //only allocation
cout <<(v2.size() == 1000)<< endl;   //prints 0
cout <<(v2.capacity()==1000)<< endl; //prints 1
```

Output ([online demo](http://www.ideone.com/PQfRp)):

```cpp
1
1
0
1
```

So `resize()` may not be desirable, if you don't want the default-created objects. It will be slow as well. Besides, if you `push_back()` new elements to it, the `size()` of the vector will further increase *by allocating new memory* (which also means moving the existing elements to the newly allocated memory space). If you have used `reserve()` at the start to ensure there is already enough allocated memory, the `size()` of the vector will increase when you `push_back()` to it, **but it will not allocate new memory again until it runs out of the space you reserved for it**.

### [A](https://stackoverflow.com/a/7397885)

From your description, it looks like that you want to "reserve" the allocated storage space of vector t_Names.

Take note that `resize` initialize the newly allocated vector where `reserve` just allocates but does not construct. Hence, 'reserve' is **much faster** than 'resize'

You can refer to the documentation regarding the difference of [resize](http://www.cplusplus.com/reference/stl/vector/resize/) and [reserve](http://www.cplusplus.com/reference/stl/vector/reserve/)

## stackoverflow [Choice between vector::resize() and vector::reserve()](https://stackoverflow.com/questions/7397768/choice-between-vectorresize-and-vectorreserve)





## 误用`reserve`、`resize`的一个例子

### 第一个版本: 只有 `reserve`

在编写leetcode [698. 划分为k个相等的子集](https://leetcode-cn.com/problems/partition-to-k-equal-sum-subsets/) 时，我误用了`reserve`，导致了一个错误，错误程序如下:

```C++
#include <bits/stdc++.h>
using namespace std;

class Solution
{
	int m_k { 0 }; // 桶的个数
	vector<int> m_buckets; // 所有的桶
	int m_target { 0 }; // 每个桶的目标装载量
public:
	bool canPartitionKSubsets(vector<int> &nums, int k)
	{
		int len = nums.size();
		m_k = k;
		if (m_k > len)
		{
			return false;
		}
		int sum = accumulate(nums.begin(), nums.end(), 0);

		if (sum % m_k != 0) // 无法均匀分配
			return false;
		m_target = sum / m_k;
		m_buckets.reserve(k); // 桶
		return DFS(0, nums);
	}
	bool DFS(int index, vector<int> &nums)
	{
		if (index == nums.size()) // 所有的数都放完了
		{
			for (auto &&i : m_buckets)
			{
				if (i != m_target)
				{
					return false;
				}
			}
			return true;
		}
		for (int j = 0; j < m_k; ++j)
		{
			if (nums[index] + m_buckets[j] > m_target) // 第i个数，不应当放入 m_buckets[j] 中
				continue;
			m_buckets[j] += nums[index];
			if (DFS(index + 1, nums))
			{
				return true;
			}
			m_buckets[j] -= nums[index];
		}
		return false;
	}
};

int main()
{
	Solution s;
	vector<int> nums { 2, 2, 2, 2, 3, 4, 5 };
	int k = 4;
	cout << s.canPartitionKSubsets(nums, k) << endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g
```



需要注意:

```C++
m_buckets.reserve(k); // 桶
```

此时 `m_buckets` 的size依然是0；





### 第二个版本: 只有 `reserve` + `push_back`

```C++
#include <bits/stdc++.h>
using namespace std;

class Solution
{
	int m_k { 0 }; // 桶的个数
	vector<int> m_buckets; // 所有的桶
	int m_target { 0 }; // 每个桶的目标装载量
public:
	bool canPartitionKSubsets(vector<int> &nums, int k)
	{
		int len = nums.size();
		m_k = k;
		if (m_k > len)
		{
			return false;
		}
		int sum = accumulate(nums.begin(), nums.end(), 0);

		if (sum % m_k != 0) // 无法均匀分配
			return false;
		m_target = sum / m_k;
		m_buckets.reserve(k); // 桶
		for (int i = 0; i < m_k; ++i)
		{
			m_buckets.push_back(0);
		}
		return DFS(0, nums);
	}
	bool DFS(int index, vector<int> &nums)
	{
		if (index == nums.size()) // 所有的数都放完了
		{
			for (auto &&i : m_buckets)
			{
				if (i != m_target)
				{
					return false;
				}
			}
			return true;
		}
		for (int j = 0; j < m_k; ++j)
		{
			if (nums[index] + m_buckets[j] > m_target) // 第i个数，不应当放入 m_buckets[j] 中
				continue;
			m_buckets[j] += nums[index];
			if (DFS(index + 1, nums))
			{
				return true;
			}
			m_buckets[j] -= nums[index];
		}
		return false;
	}
};

int main()
{
	Solution s;
	vector<int> nums { 2, 2, 2, 2, 3, 4, 5 };
	int k = 4;
	cout << s.canPartitionKSubsets(nums, k) << endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g


```

这种写法是正确的，此时size是准确的，但是这种写法肯定是不够简洁的，下面的写法是更加简洁的。



### 第三个版本: `resize(k, 0)`

```C++
#include <bits/stdc++.h>
using namespace std;

class Solution
{
	int m_k { 0 }; // 桶的个数
	vector<int> m_buckets; // 所有的桶
	int m_target { 0 }; // 每个桶的目标装载量
public:
	bool canPartitionKSubsets(vector<int> &nums, int k)
	{
		int len = nums.size();
		m_k = k;
		if (m_k > len)
		{
			return false;
		}
		int sum = accumulate(nums.begin(), nums.end(), 0);

		if (sum % m_k != 0) // 无法均匀分配
			return false;
		m_target = sum / m_k;
		m_buckets.resize(k, 0); // 桶
		return DFS(0, nums);
	}
	bool DFS(int index, vector<int> &nums)
	{
		if (index == nums.size()) // 所有的数都放完了
		{
			for (auto &&i : m_buckets)
			{
				if (i != m_target)
				{
					return false;
				}
			}
			return true;
		}
		for (int j = 0; j < m_k; ++j)
		{
			if (nums[index] + m_buckets[j] > m_target) // 第i个数，不应当放入 m_buckets[j] 中
				continue;
			m_buckets[j] += nums[index];
			if (DFS(index + 1, nums))
			{
				return true;
			}
			m_buckets[j] -= nums[index];
		}
		return false;
	}
};

int main()
{
	Solution s;
	vector<int> nums { 2, 2, 2, 2, 3, 4, 5 };
	int k = 4;
	cout << s.canPartitionKSubsets(nums, k) << endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g


```


# Erase–remove idiom

## labuladong [如何高效对有序数组/链表去重？](https://mp.weixin.qq.com/s/6Eb7gKqNqXH9B0hSZvMs5A)

其实，**对于数组相关的算法问题，有一个通用的技巧：要尽量避免在中间删除元素，那我就先想办法把这个元素换到最后去**。

这样的话，最终待删除的元素都拖在数组尾部，一个一个 pop 掉就行了，每次操作的时间复杂度也就降低到 O(1) 了。



## wikipedia [Erase–remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom)

### Motivation

`erase` can be used to delete an element from a collection, but for containers which are based on an array, such as `vector`, all elements after the deleted element have to be moved forward to avoid "gaps" in the collection. Calling erase multiple times on the same container generates lots of overhead from moving the elements.

> NOTE: 主要原因



> NOTE: 关于`algorithm` library的`std::remove`、`std::remove_if`，参见：https://en.cppreference.com/w/cpp/algorithm/remove



### Limitation

The erase–remove idiom cannot be used for containers that return `const_iterator` (e.g.: [set](https://en.wikipedia.org/wiki/Set_(C%2B%2B)))[[5\]](https://en.wikipedia.org/wiki/Erase–remove_idiom#cite_note-Erase–remove_idiom_with_std::set-5)

`std::remove` and `std::remove_if` do not maintain elements that are removed (unlike `std::partition`, `std::stable_partition`). Thus, erase–remove can only be used with containers holding elements with full value semantics without incurring resource leaks.[[6\]](https://en.wikipedia.org/wiki/Erase–remove_idiom#cite_note-effective_stl-6)

### Example

```c++
// Use g++ -std=c++11 or clang++ -std=c++11 to compile.

#include <algorithm>  // remove and remove_if
#include <iostream>
#include <vector>  // the general-purpose vector container

bool IsOdd(int i) { return i & 1; }

void Print(const std::vector<int>& vec) {
  for (const auto& i : vec) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
}

int main() {
  // Initializes a vector that holds numbers from 0-9.
  std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  Print(v);

  // Removes all elements with the value 5.
  v.erase(std::remove(v.begin(), v.end(), 5), v.end());
  Print(v);

  // Removes all odd numbers.
  v.erase(std::remove_if(v.begin(), v.end(), IsOdd), v.end());
  Print(v);
}
// g++ -std=c++11 test.cpp

/*
Output:
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 6 7 8 9
0 2 4 6 8
*/
```

## makeinjava [STL Erase-Remove Idiom in C++ (with example)](https://makeinjava.com/stl-erase-remove-idiom-c-example/)

Its very common practice to remove element(s) from collection(s) like vector. The **common** practice is just iterate through the container to remove the desired element. But STL has builtin feature specifically to avoid this situation. Erase-Remove idiom comes in our rescue. **Erase-Remove** is an **efficient** way to permanently remove element(s) from an STL container.

> NOTE: iterate and delete是非常任意出错的

### `erase` and `remove`

Let’s see couple of methods to understand it further – **erase**() and **remove**() (and **remove_if**()).

1、`erase`: – STL containers provide erase method to remove an **element** or a **range** of elements from the container. `erase` method **reduces** the size of the container.

2、`remove`:– `remove` and `remove_if` methods are also a part of STL algorithm.

- These methods pushes the element(s), which matches the ‘remove’ criteria, to end of the container.
- `remove` method returns an iterator pointing to the first removed element.
- The removed element is still part of the container **but** it is pushed to the **end** of container.

### Example

```c++
#include <algorithm> // remove
#include <vector> // vector
#include <iostream> // cout

void print_vec(const std::vector<int>& vec)
{
	for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl
			<< "Vector Size :" << vec.size() << std::endl;
	return;
}

bool greater_than_five(int i)
{
	return (i > 5);
}

void remove_example(void)
{
	std::vector<int> my_vec;
	for (int i = 0; i < 10; i++)
	{
		my_vec.push_back(i % 2);
	}
	std::cout << "Printing input vector contents for remove example" << std::endl;
	print_vec(my_vec);

	std::vector<int>::iterator new_end = std::remove(my_vec.begin(), my_vec.end(), 0);
	std::cout << "Printing vector contents after remove operation " << std::endl;
	for (std::vector<int>::const_iterator it = my_vec.begin(); it != new_end; it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl
			<< "Vector New Size :" << my_vec.size() << std::endl;
}
void erase_remove_example(void)
{
	std::vector<int> my_vec;
	for (int i = 0; i < 10; i++)
	{
		my_vec.push_back(i % 2);
	}
	std::cout << "Printing input vector contents for erase_remove example" << std::endl;
	print_vec(my_vec);

	my_vec.erase(std::remove(my_vec.begin(), my_vec.end(), 0), my_vec.end());
	std::cout << "Printing vector contents after erase remove operation " << std::endl;
	print_vec(my_vec);
}
void erase_remove_if_example(void)
{
	std::vector<int> my_vec;
	for (int i = 0; i < 10; i++)
	{
		my_vec.push_back(i);
	}
	std::cout << "Printing input vector contents for erase_remove_if example" << std::endl;
	print_vec(my_vec);

	my_vec.erase(std::remove_if(my_vec.begin(), my_vec.end(), greater_than_five), my_vec.end());
	std::cout << "Printing vector contents after erase_remove_if operation " << std::endl;
	print_vec(my_vec);
}

int main(int argc, char** argv)
{
	std::cout << "================================================================" << std::endl;
	remove_example();
	std::cout << "================================================================" << std::endl;
	erase_remove_example();
	std::cout << "================================================================" << std::endl;
	erase_remove_if_example();

	return 0;
}
// g++ test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> ================================================================
> Printing input vector contents for remove example
> 0 1 0 1 0 1 0 1 0 1 
> Vector Size :10
> Printing vector contents after remove operation 
> 1 1 1 1 1 
> Vector New Size :10
> ================================================================
> Printing input vector contents for erase_remove example
> 0 1 0 1 0 1 0 1 0 1 
> Vector Size :10
> Printing vector contents after erase remove operation 
> 1 1 1 1 1 
> Vector Size :5
> ================================================================
> Printing input vector contents for erase_remove_if example
> 0 1 2 3 4 5 6 7 8 9 
> Vector Size :10
> Printing vector contents after erase_remove_if operation 
> 0 1 2 3 4 5 
> Vector Size :6
> 
> ```
>
> 


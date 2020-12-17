# Iterator invalidation

使用container需要格外关注的一件事情就是：iterator invalidation。

可以将iterator看做是pointer， 显然一个iterator所指向的memory如果被修改了，那么再次使用这个iterator去取这个memory的内容，那么就可能取到的是invalid的内容。从这个角度来出发，再结合各种container的实现细节，就可以方便的判断出是否发生了invalidation。

## cppreference [Containers library#Iterator invalidation](https://en.cppreference.com/w/cpp/container)



对于不同的container，它的iterator的实现是不同的：

对于vector，它的iterator是：

- a pointer to element type
- `iterator++` is just `++` of  pointer

对于list，它的iterator是：

- a pointer to node type，需要注意的是，它不是element type
- `iterator++` is get next node

对于map，它的iterator是：

- a pointer to dict item type

不管如何，iterator最终都是pointer，所以一旦底层的memory发生了变化，那么这个iterator就invalidation了。



## `std::vector` iterator invalidation

参考：

- [c++ std::vector and Iterator Invalidation example](https://thispointer.com/stdvector-and-iterator-invalidation/)

`std::vector`是dynamic array，下面行为会造成`realloc`

- An element is inserted to vector at any location
- An element is deleted from vector.

### Iterator Invalidation Example on Element Deletion in vector:

For example, in the below code we are deleting an element from vector using `erase` function. This `erase` function invalidates the current pointer. So if after calling the `erase()` function , if one uses the same invalidated iterator then it can result in **undefined behavior**.

```c++
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
	std::vector<int> vecArr;
	for (int i = 1; i <= 10; i++)
		vecArr.push_back(i);

	for (auto it = vecArr.begin(); it != vecArr.end(); it++)
	{
		std::cout << (*it) << "  ";
	}

	std::cout << std::endl;

	// Erase and element with value 5.
	auto it = std::find(vecArr.begin(), vecArr.end(), 5);
	if (it != vecArr.end())
	{
		vecArr.erase(it);
	}

	// Now iterator 'it' is invalidated because it still points to
	// old location, which has been deleted. So, if you will try to
	// do the use the same iterator then it can show undefined
	// behavior.

	for (; it != vecArr.end(); it++)   // Unpredicted Behavior
	{
		std::cout << (*it) << "  " << std::endl;          // Unpredicted Behavior
		*it = 1;
	}

	return 0;
}

```



wild pointer；

#### Solution:	

After calling the erase function update the value of iterator ‘`it`’ i.e.

```c++
// Erase and element with value 5.
auto it = std::find(vecArr.begin(), vecArr.end(), 5);
if(it != vecArr.end())
   it = vecArr.erase(it);
```

As, `erase()` function returns an iterator pointing to the **new location** of the element that followed the last element erased by the same function. Also, if the element deleted was the last element of the container then it returns the **end** of the container.

### Iterator Invalidation Example on Element Insertion in vector:

When a new element is inserted in vector then it internally shifts its elements and hence the old iterators become invalidated.

Reasons for element shift are as follows,

- If element is inserted in between then it shift all the right elements by 1.
- If the new size of vector is more than its current capacity, then it relocates a bigger chunk of memory and copies all the elements there.

Therefore, when a new element is inserted in vector then its old iterator can become invalidated. Using this old invalidated iterators can result in undefined behavior i.e.

```c++
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
	std::vector<int> vecArr;
	for (int i = 1; i <= 10; i++)
		vecArr.push_back(i);

	auto it = vecArr.begin();
	for (; it != vecArr.end(); it++)
	{
		std::cout << (*it) << "  " << std::endl;
	}

	std::cout << std::endl;

	it = vecArr.begin();

	// Insert an element in position 2,
	vecArr.insert(it + 2, 1, 200);

	// Now old iterator it has become invalidated
	// SO, using it as it is can result in undefined behavior

	for (; it != vecArr.end(); it++)   // Undefined Behavior

	{
		std::cout << (*it) << "  " << std::endl;          // Undefined Behavior
	}

	return 0;
}

```

#### Solution:

After calling the insert function update the value of iterator ‘`it`’ i.e. by re-assigning it.

```c++
// Insert an element in position 2,
vecArr.insert ( it + 2, 1 , 200 ); 
// Reinitialize the invalidated iterator to the begining.
it = vecArr.begin();
```



## TO READ

https://www.geeksforgeeks.org/iterator-invalidation-cpp/

https://stackoverflow.com/questions/16904454/what-is-iterator-invalidation

https://wiki.c2.com/?IteratorInvalidationProblem
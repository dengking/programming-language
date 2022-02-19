# std::default_delete



## cppreference [std::default_delete](https://en.cppreference.com/w/cpp/memory/default_delete)

```C++
template< class T > struct default_delete; (1)	(since C++11)
template< class T > struct default_delete<T[]>; (2)	(since C++11)
```

> NOTE: 
>
> (2)是(1)的specialization

`std::default_delete` is the default destruction policy used by [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) when no deleter is specified.

> NOTE: 
>
> 一、`std::default_delete` 只是 [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) 的default deleter，它不是`std::shared_ptr`的default deleter，因此如果`std::shared_ptr` 要使用它的话，就需要作为参数进行传入，下面的例子就展示了这种用法

### Example

```C++
#include <memory>
#include <vector>
#include <algorithm>

int main()
{
	//  {
	//      std::shared_ptr<int> shared_bad(new int[10]);
	//  } // the destructor calls delete, undefined behavior

	{
		std::shared_ptr<int> shared_good(new int[10], std::default_delete<int[]>());
	} // the destructor calls delete[], ok

	{
		std::unique_ptr<int> ptr(new int(5));
	} // unique_ptr<int> uses default_delete<int>

	{
		std::unique_ptr<int[]> ptr(new int[10]);
	} // unique_ptr<int[]> uses default_delete<int[]>

	// default_delete can be used anywhere a delete functor is needed
	std::vector<int*> v;
	for (int n = 0; n < 100; ++n)
		v.push_back(new int(n));
	std::for_each(v.begin(), v.end(), std::default_delete<int>());
}

// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

上述例子非常好
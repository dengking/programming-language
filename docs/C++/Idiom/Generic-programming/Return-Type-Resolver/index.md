# Return Type Resolver



## [More C++ Idioms/Return Type Resolver](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Return_Type_Resolver)

> NOTE: 提供**templatized conversion operator function**，充分运用compiler自动地调用**conversion operator function**来讲将**temporary object**转换为目标变量（即等号左侧的变量）从而达到简化代码的目的

### Motivation

```c++
#include<list>
#include<vector>
#include <stdlib.h>     /* srand, rand */
template <class Container>
Container getRandomN(std::size_t n) 
{
  Container c;
  for(size_t i = 0;i < n; ++i)
    c.insert(c.end(), rand());
  return c;
}

int main (void)
{
   std::list<int> l = getRandomN<std::list<int> > (10);
   std::vector<long> v = getRandomN<std::vector<long> > (100);
}
```

### Solution and Sample Code

Return type resolver idiom makes use of a proxy class and **templatized conversion operator functions** in the class. `getRandomN` function above can be implemented with a class and a member conversion function as follows.

```c++
#include<list>
#include<vector>
#include<set>
#include <stdlib.h>     /* srand, rand */

class getRandomN
{
	std::size_t count;

public:
	getRandomN(int n = 1) :
					count(n)
	{
	}

	template<class Container>
	operator Container()
	{
		Container c;
		for (size_t i = 0; i < count; ++i)
			c.insert(c.end(), rand()); // push_back is not supported by all standard containers.
		return c;
	}
};

int main()
{
	std::set<int> random_s = getRandomN(10);
	std::vector<int> random_v = getRandomN(10);
	std::list<int> random_l = getRandomN(10);
}
// g++ test.cpp --std=c++03
```

`getRandomN` class has a constructor and a **templatized conversion operator function**. For initialization, a temporary object of `getRandomN` class is created and assigned to the desired container class. C++ compiler attempts to convert the **temporary object** into the container class object. The only way to do that is via the **conversion operator**. The conversion operator is instantiated with the type of the container that is being populated. Due to automatic resolution of the return type, the user does not have to spell it out again. Note that `insert` member function has been used instead of `push_back` because `std::set` does not support `push_back`.



### Known Uses

The [nullptr](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr) idiom makes use of the return type resolver idiom to automatically deduce a null pointer of the correct type depending upon the pointer variable it is assigning to.



## 补充样例

1、[microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/pointers `not_null`
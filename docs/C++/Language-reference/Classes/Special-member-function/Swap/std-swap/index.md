# `std::swap`



## cppreference [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap)



### Type requirements

-`T` must meet the requirements of [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) and [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible).

-`T2` must meet the requirements of [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable).

> NOTE: 下面有程序进行验证

## `std::swap` 要求  [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) and [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible).



### 测试程序

```C++
#include <iostream>
#include <vector>
#include <utility>

class IntVector
{
	std::vector<int> v;
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
	std::swap(v1, v2);
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

上述程序能够正常编译通过。



```C++
#include <iostream>
#include <vector>

class IntVector
{
	std::vector<int> v;
	
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
	IntVector& operator=(const IntVector& v) = default; // not assignable
	IntVector& operator=(IntVector&& v) = delete; // not assignable
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
    std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable
	// std::iter_swap(&v1, &v2); // OK: library calls unqualified swap()
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

编译报错如下:

```C++
main.cpp: In function 'int main()':
main.cpp:25:21: error: no matching function for call to 'swap(IntVector&, IntVector&)'
   25 |     std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable

/usr/local/include/c++/10.2.0/bits/move.h:189:5: note: candidate: 'template<class _Tp> std::_Require<std::__not_<std::__is_tuple_like<_Tp> >, std::is_move_constructible<_Tp>, std::is_move_assignable<_Tp> > std::swap(_Tp&, _Tp&)'
  189 |     swap(_Tp& __a, _Tp& __b)
```


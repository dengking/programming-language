# RVO



RVO有多种方式:

1、copy elision

2、两阶段overload resolution

## boost [Implicit Move when returning a local object](https://www.boost.org/doc/libs/master/doc/html/move/move_return.html)



## Example

### RVO and NRVO

```C++
#include <iostream>
#include <utility>

class MoveOnly
{
public:
	MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly(MoveOnly&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
MoveOnly foo()
{
	return MoveOnly();
}

MoveOnly bar()
{
	MoveOnly m;
	return m;
}


MoveOnly foobar()
{
	MoveOnly m;
	return std::move(m);
}
int main()
{
	MoveOnly m1 = foo();
	MoveOnly m2 = bar();
	MoveOnly m3 = foobar();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

运行结果如下:

```C++
MoveOnly::MoveOnly()
MoveOnly::MoveOnly()
MoveOnly::~MoveOnly()
MoveOnly::~MoveOnly()
```

可以看到，compiler执行了RVO、NRVO，即使在使用默认的optimization的情况下。



### moving a local object in a return statement prevents copy elision [`-Wpessimizing-move`]

```C++
#include <iostream>
#include <utility>

class MoveOnly
{
public:
	MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly(MoveOnly&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

MoveOnly foobar()
{
	MoveOnly m;
	return std::move(m);
}
int main()
{

	MoveOnly m3 = foobar();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

编译、运行结果如下:

```C++
main.cpp: In function 'MoveOnly foobar()':
main.cpp:25:18: warning: moving a local object in a return statement prevents copy elision [-Wpessimizing-move]
   25 |  return std::move(m);
      |         ~~~~~~~~~^~~
main.cpp:25:18: note: remove 'std::move' call
MoveOnly::MoveOnly()
MoveOnly::MoveOnly(MoveOnly&&)
MoveOnly::~MoveOnly()
MoveOnly::~MoveOnly()
```


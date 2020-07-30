# C++ std::move and std::forward

两者都和reference相关，`std::move`用于辅助move semantic，`std::forward`用于辅助perfect forwarding；

## bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)

C++ `std::move` does not **move** and `std::forward` does not **forward**. 



### Motivation

```c++
std::map<std::string, std::function<void()>> commands;

template<typename ftor>
void install_command(std::string name, ftor && handler)
{
  commands.insert({
    std::move(name),
    std::forward<ftor>(handler)
  });
}
```



### std::move

```c++
#include <algorithm>
#include <iostream>
#include <string>

struct X
{
	X()
	{
	}

	X(const X & other)
			: s_ { other.s_ }
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	X(X && other)
			: s_ { std::move(other.s_) }
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	// other is an lvalue, and other.s_ is an lvalue too
	// use std::move to force using the move constructor for s_
	// don't use other.s_ after std::move (other than to destruct)

	std::string s_;
};

int main()
{
	X a;

	X b = std::move(a);
	// a is an lvalue
	// use std::move to convert to a rvalue,
	// xvalue to be precise,
	// so that the move constructor for X is used
	// don't use a after std::move (other than to destruct)
}
// g++ --std=c++11 test.cpp
```



### std::forward



```c++
#include <iostream>
#include <utility>

struct Y
{
	Y()
	{
	}
	Y(const Y &)
	{
		std::cout << "Copy constructor\n";
	}
	Y(Y &&)
	{
		std::cout << "Move constructor\n";
	}
};

struct X
{
	template<typename A, typename B>
	X(A && a, B && b)
			:
					// retrieve the original value category from constructor call
					// and pass on to member variables
					a_ { std::forward < A > (a) },
					b_ { std::forward < B > (b) }
	{
	}

	Y a_;
	Y b_;
};

template<typename A, typename B>
X factory(A && a, B && b)
{
	// retrieve the original value category from the factory call
	// and pass on to X constructor
	return X(std::forward < A > (a), std::forward < B > (b));
}

int main()
{
	Y y;
	X two = factory(y, Y());
	// the first argument is a lvalue, eventually a_ will have the
	// copy constructor called
	// the second argument is an rvalue, eventually b_ will have the
	// move constructor called
}

// prints:
// Copy constructor
// Move constructor

// g++ --std=c++11 test.cpp
```


# C++ `std::move` and `std::forward`

两者可以放到一起来进行比较：

- 两者都和reference、value category相关相关，`std::move`用于辅助move semantic，`std::forward`用于辅助perfect forwarding
- C++ `std::move` does not **move** and `std::forward` does not **forward**（bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)）
- 

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



### `std::forward`



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





### stackoverflow [How to bind function to an object by reference?](https://stackoverflow.com/questions/26187192/how-to-bind-function-to-an-object-by-reference) # A

**Note on using `std::forward`**

First of all, `std::forward` is meant to be used for [perfect forwarding](https://stackoverflow.com/q/3582001/873025), i.e. to forward the reference type (l-value or r-value).

If you pass an *l-value* reference to `std::forward` that is what is returned, and likewise if an *r-value* reference is passed then an r-value is returned. This works as opposed to `std::move` that will always return an r-value reference. Also remember that *named* r-value references are l-value references.

```c++
/* Function that takes r-value reference. */
void func(my_type&& t) {
    // 't' is named and thus is an l-value reference here.

    // Pass 't' as lvalue reference.
    some_other_func(t);
    // Pass 't' as rvalue reference (forwards rvalueness).
    some_other_func(std::forward<my_type>(t));
    // 'std::move' should be used instead as we know 't' is always an rvalue.
    // e.g. some_other_func(std::move(t));
}
```


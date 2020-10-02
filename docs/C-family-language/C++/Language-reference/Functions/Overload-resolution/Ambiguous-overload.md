# Ambiguous overload

这是一类非常典型的与overload resolution有关的compile error。



## Example: User-Defined Conversion Sequence

这种错误，往往是出现在User-Defined Conversion Sequences中，下面是收集的一些例子:

### Example1

来源: accu [Overload Resolution - Selecting the Function](https://accu.org/journals/overload/13/66/kilpelainen_268/) `#` “Ordering of User-Defined Conversion Sequences”

```C++
#include <iostream>

struct A;
struct B
{
	B(A const&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

struct A
{
	operator B() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	operator int() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
void func(B)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
void func(int)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main()
{
	func(A());
}
// g++ test.cpp
```

NOTE: 上述函数编译报错如下: 

```c++
test3.cpp: In function ‘int main()’:
test3.cpp:34:10: error: call of overloaded ‘func(A)’ is ambiguous
  func(A());
          ^
test3.cpp:34:10: note: candidates are:
test3.cpp:23:6: note: void func(B)
 void func(B)
      ^
test3.cpp:27:6: note: void func(int)
 void func(int)

```

因为`class A`有如下两个conversion function:

- `operator B()`
- `operator int()`

函数`func`的两个重载的入参类型正好是:

- `func(B)`
- `func(int)`

所以，compiler无法决定到底使用哪个overload。

### Example2

来源: cppreference [Overload resolution#Ranking of implicit conversion sequences](https://en.cppreference.com/w/cpp/language/overload_resolution#Ranking_of_implicit_conversion_sequences) :

```c++
class B;
class A
{
	A(B&)
	{
	}
};
// converting constructor
class B
{
	operator A()
	{
	}
};
// user-defined conversion function
class C
{
	C(B&)
	{
	}
};
// converting constructor
void f(A)
{
} // overload #1
void f(C)
{
} // overload #2

int main()
{
	B b;
	f(b); // B -> A via ctor or B -> A via function (ambiguous conversion)
		  // b -> C via ctor (user-defined conversion)
		  // the conversions for overload #1 and for overload #2
		  // are indistinguishable; compilation fails
}
// g++ test.cpp


```

上述程序编译报错如下:

```c++
test3.cpp: In function ‘int main()’:
test3.cpp:33:5: error: call of overloaded ‘f(B&)’ is ambiguous
  f(b); // B -> A via ctor or B -> A via function (ambiguous conversion)
     ^
test3.cpp:33:5: note: candidates are:
test3.cpp:23:6: note: void f(A)
 void f(A)
      ^
test3.cpp:26:6: note: void f(C)
 void f(C)
```

### Example3



## Example: Standard conversion sequence

### Example1

来源: cppreference [Overload resolution#Ranking of implicit conversion sequences](https://en.cppreference.com/w/cpp/language/overload_resolution#Ranking_of_implicit_conversion_sequences) :

```C++
int f(const int&) // overload #1
{
	return 0;
}

int f(int&)       // overload #2 (both references)
{
	return 0;
}
int g(const int&) // overload #1
{
	return 0;
}
int g(int)         // overload #2
{
	return 0;
}
int main()
{
	int i;
	int j = f(i); // lvalue i -> int& is better than lvalue int -> const int&
				  // calls f(int&)
	int k = g(i); // lvalue i -> const int& ranks Exact Match
				  // lvalue i -> rvalue int ranks Exact Match
				  // ambiguous overload: compilation error
}
// g++ test.cpp


```

上述程序是典型的ambiguous overload，它被收录到了[Ambiguous-overload](./Ambiguous-overload.md) 中

```C++
test2.cpp: In function ‘int main()’:
test2.cpp:23:13: error: call of overloaded ‘g(int&)’ is ambiguous
  int k = g(i); // lvalue i -> const int& ranks Exact Match
             ^
test2.cpp:23:13: note: candidates are:
test2.cpp:10:5: note: int g(const int&)
 int g(const int&) // overload #1
     ^
test2.cpp:14:5: note: int g(int)
 int g(int)         // overload #2

```


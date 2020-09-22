# Non-static member functions



## cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions)



Within the body of a non-static member function of `X`, any [id-expression](https://en.cppreference.com/w/cpp/language/expressions) E (e.g. an identifier) that resolves to a non-type non-static member of X or of a base class of X, is transformed to a member access expression (*this).E (unless it's already a part of a member access expression). This does not occur in template definition context, so a name may have to be prefixed with this-> explicitly to become [dependent](https://en.cppreference.com/w/cpp/language/dependent_name).



### const- and volatile-qualified member functions

Differently cv-qualified functions have different types and so may overload each other.

> NOTE: CV修饰的是不同的type

In the body of a cv-qualified function, `*this` is cv-qualified, e.g. in a const member function, only other const member functions may be called normally. (A non-const member function may still be called if [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) is applied or through an access path that does not involve [`this`](https://en.cppreference.com/w/cpp/language/this).)

> NOTE: 这样的涉及保证了C++的type safety.
>
> 在cppreference [const_cast conversion](https://en.cppreference.com/w/cpp/language/const_cast)中给出了例子。

```c++
#include <vector>
#include <iostream>

struct Array
{
	std::vector<int> data;
	Array(int sz) :
			data(sz)
	{
	}
	// const member function
	int operator[](int idx) const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		// this has type const Array*
		return data[idx]; // transformed to (*this).data[idx];
	}
	// non-const member function
	int& operator[](int idx)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		// this has type Array*
		return data[idx]; // transformed to (*this).data[idx]
	}
};
int main()
{
	Array a(10);
	/**
	 * OK: the type of a[1] is int&
	 * 选择非const版本
	 */
	a[1] = 1;
	const Array ca(10);
	// ca[1] = 2; // Error: the type of ca[1] is int
	/**
	 * 选择const版本
	 */
	ca[1];
}
// g++ --std=c++11 test.cpp
```

> NOTE:上述程序的输出如下:
>
> ```c++
> int& Array::operator[](int)
> int Array::operator[](int) const
> ```
>
> 

### ref-qualified member functions(since C++11)

| ref-qualifier        | syntax        | [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) |
| -------------------- | ------------- | ------------------------------------------------------------ |
| no ref-qualifier     |               |                                                              |
| lvalue ref-qualifier | `void f() &`  | the implicit object parameter has type **lvalue reference** to cv-qualified `X` |
| rvalue ref-qualifier | `void f() &&` | the implicit object parameter has type **rvalue reference** to cv-qualified `X` |



```c++
#include <iostream>
struct S
{
	void f() &
	{
		std::cout << "lvalue\n";
	}
	void f() &&
	{
		std::cout << "rvalue\n";
	}
};

int main()
{
	S s;
	s.f();            // prints "lvalue"
	std::move(s).f(); // prints "rvalue"
	S().f();          // prints "rvalue"
}
// g++ --std=c++11 test.cpp

```

### qualified方式总结

| qualified方式                 | 对`this`pointer的影响                                        | 对overload resolution的影响 |
| ----------------------------- | ------------------------------------------------------------ | --------------------------- |
| const- and volatile-qualified | In the body of a cv-qualified function, `*this` is cv-qualified |                             |
| ref-qualified                 | ref-qualification does not change the properties of the [this](https://en.cppreference.com/w/cpp/language/this) pointer: within a rvalue ref-qualified function, *this remains an lvalue expression. |                             |



### Special member functions

> NOTE: 这在`C++\Language-reference\Classes\Special-member-functions`章节进行介绍


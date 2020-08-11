# Catch-all operator

catch-all operator是指：

```
...
```

它是我自己命名的，在标志中并没有这样的命名。

## cppreference [Other operators](https://en.cppreference.com/w/cpp/language/operator_other)



## Catch-all exception

- 参考stackoverflow [C++ catching all exceptions](https://stackoverflow.com/questions/315948/c-catching-all-exceptions)



[A](https://stackoverflow.com/a/315967)

```cpp
try{
    // ...
} catch (...) {
    // ...
}
```

will catch all C++ exceptions, but it should be considered bad design. You can use c++11's new [std::current_exception](https://en.cppreference.com/w/cpp/error/current_exception) mechanism, but if you don't have the ability to use c++11 (legacy code systems requiring a rewrite), then you have no named exception pointer to use to get a message or name. You may want to add separate catch clauses for the various exceptions you can catch, and only catch everything at the bottom to record an unexpected exception. E.g.:

```cpp
try{
    // ...
} catch (const std::exception& ex) {
    // ...
} catch (const std::string& ex) {
    // ...
} catch (...) {
    // ...
}
```

[A](https://stackoverflow.com/a/24142104)

```cpp
#include <iostream> // std::clog

#include <typeinfo>
#include <iostream>
#include <exception>      // std::exception_ptr, std::current_exception, std::rethrow_exception
#include <stdexcept>      // std::logic_error'

int main()
{
	try
	{
		throw std::logic_error("some logic_error exception"); // throw something
	}
	catch (...)
	{
		std::exception_ptr p = std::current_exception();
		std::clog << (p ? p.__cxa_exception_type()->name() : "null") << std::endl;
	}
	return 1;
}
// g++ --std=c++11 test.cpp
```

> NOTE:上述程序的输出：
>
> ```
> St11logic_error
> ```



## Catch-all argument

这主要对应的是这种情况:

```c++
a(...)
```

是通过学习detection idiom发现了这种用法的。

### Example: stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae) `#` [A](https://stackoverflow.com/a/12654277)



```cpp
#include <iostream>

// catch-all case
void test(...)
{
	std::cout << "Couldn't call\n";
}

// catch when C is a reference-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c.*f)(), void()) // 'C' is reference type
{
	std::cout << "Could call on reference\n";
}

// catch when C is a pointer-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c->*f)(), void()) // 'C' is pointer type
{
	std::cout << "Could call on pointer\n";
}

struct X
{
	void f()
	{
	}
};

int main()
{
	X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出如下:
>
> ```
> Could call on reference
> Could call on pointer
> Couldn't call
> ```

### Example: stackoverflow [Templated check for the existence of a class member function?](https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function) `#` [A](https://stackoverflow.com/a/257382)



## Parameter pack

参见cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack)。
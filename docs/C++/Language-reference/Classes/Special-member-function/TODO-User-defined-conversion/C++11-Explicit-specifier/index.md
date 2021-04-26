# C++11 explicit specifier

## What's new in C++

1、C++11 explicit specifier的引入主要是为了解决"safe bool problem"的，这在 `operator-bool-and-safe-bool-problem` 章节中进行了讨论

2、C++11 explicit specifier的引入给予了programmer对implicit、explicit type conversion的控制，它体现了C++的灵活性。



### stackoverflow [What does the explicit keyword mean?](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean)



[A](https://stackoverflow.com/a/121163)

The compiler is allowed to make one implicit conversion to resolve the parameters to a function. What this means is that the compiler can use constructors callable with a **single parameter** to convert from one type to another in order to get the right type for a parameter.

Here's an example class with a constructor that can be used for implicit conversions:

```C++
#include <iostream>

class Foo
{
public:
	// single parameter constructor, can be used as an implicit conversion
	Foo(int foo)
			: m_foo(foo)
	{
	}

	int GetFoo()
	{
		return m_foo;
	}

private:
	int m_foo;
};

void DoBar(Foo foo)
{
	int i = foo.GetFoo();
	std::cout << i << std::endl;
}
int main()
{
	DoBar(42);
}
// g++ --std=c++11 test.cpp
```

> NOTE:  
>
> 1、输出如下：
>
> ```
> 42
> ```
>
> 2、上述是典型的 implicit conversion + copy initialization + converting constructor 

The argument is not a `Foo` object, but an `int`. However, there exists a constructor for `Foo` that takes an `int` so this constructor can be used to convert the parameter to the correct type.

The compiler is allowed to do this once for each parameter.

Prefixing the `explicit` keyword to the constructor prevents the compiler from using that constructor for implicit conversions. Adding it to the above class will create a compiler error at the function call `DoBar (42)`. It is now necessary to call for conversion explicitly with `DoBar (Foo (42))`

The reason you might want to do this is to avoid accidental construction that can hide bugs. Contrived example:

1、You have a `MyString(int size)` class with a constructor that constructs a string of the given size. You have a function `print(const MyString&)`, and you call `print(3)` (when you *actually* intended to call `print("3")`). You expect it to print "3", but it prints an empty string of length 3 instead.



```c++
#include <iostream>

class Foo
{
public:
	// single parameter constructor, can be used as an implicit conversion
	explicit Foo(int foo)
			: m_foo(foo)
	{
	}

	int GetFoo()
	{
		return m_foo;
	}

private:
	int m_foo;
};

void DoBar(Foo foo)
{
	int i = foo.GetFoo();
	std::cout << i << std::endl;
}
int main()
{
	DoBar( { 42 });
}
// g++ --std=c++11 test.cpp
```

> NOTE: 编译报错如下：
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:28:15: 错误：从初始化列表转换为‘Foo’将使用显式构造函数‘Foo::Foo(int)’
> DoBar( { 42 });
> ```

```c++
#include <iostream>

class Foo
{
public:
	// single parameter constructor, can be used as an implicit conversion
	explicit Foo(int foo)
			: m_foo(foo)
	{
	}

	int GetFoo()
	{
		return m_foo;
	}

private:
	int m_foo;
};

void DoBar(Foo foo)
{
	int i = foo.GetFoo();
	std::cout << i << std::endl;
}
int main()
{
	DoBar(Foo { 42 });
}

```



[A](https://stackoverflow.com/a/121216)

> NOTE: 
>
> 1、这个回答的例子非常好

Suppose, you have a class `String`:

```c++
#include <iostream>
class String
{
public:
	String(int n) // allocate n bytes to the String object
	{

		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	String(const char *p) // initializes object with char *p
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	String mystring = 'x';
}
// g++ test.cpp -pedantic -Wall -Wextra

```

> NOTE: 
>
> 1、输出如下:
>
> ```
> String::String(int)
> ```
>
> 2、显然，上述输出是出人意外的

The character `'x'` will be implicitly converted to `int` and then the `String(int)` constructor will be called. But, this is not what the user might have intended. So, to prevent such conditions, we shall define the constructor as `explicit`:

```c++
#include <iostream>
class String
{
public:
	explicit String(int n) // allocate n bytes to the String object
	{

		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	String(const char *p) // initializes object with char *p
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	String mystring = 'x';
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp: In constructor 'String::String(int)':
> test.cpp:5:22: warning: unused parameter 'n' [-Wunused-parameter]
> explicit String(int n) // allocate n bytes to the String object
>                ~~~~^
> test.cpp: In constructor 'String::String(const char*)':
> test.cpp:10:21: warning: unused parameter 'p' [-Wunused-parameter]
> String(const char *p) // initializes object with char *p
>       ~~~~~~~~~~~~^
> test.cpp: In function 'int main()':
> test.cpp:18:20: error: invalid conversion from 'char' to 'const char*' [-fpermissive]
> String mystring = 'x';
>                  ^~~
> test.cpp:10:21: note:   initializing argument 1 of 'String::String(const char*)'
> String(const char *p) // initializes object with char *p
> 
> ```
>
> 2、从上述编译报错中可以看到，此时compiler没有选择`String(int n)`



## 使用场景

1、 `explicit`主要由于对user-defined conversion的implicit、explicit 进行控制，它主要用于: 

a、explicit constructor

b、user-defined conversion function

2、C++17 deduction guide

## cppreference [explicit specifier](https://en.cppreference.com/w/cpp/language/explicit)

|                                         |      |      |
| --------------------------------------- | ---- | ---- |
| `explicit`                              | 1)   |      |
| `explicit ( expression )` (since C++20) | 2)   |      |

1) Specifies that 

1、constructor 

2、conversion function (since C++11) 

3、[deduction guide](https://en.cppreference.com/w/cpp/language/ctad) (since C++17)

that is, it cannot be used for [implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_cast) and [copy-initialization](https://en.cppreference.com/w/cpp/language/copy_initialization).

2)

> NOTE: TODO

### Example

```C++
#include <iostream>

struct A
{
	A(int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}      // converting constructor
	A(int, int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	} // converting constructor (C++11)
	operator bool() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}
};

struct B
{
	explicit B(int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	explicit B(int, int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	explicit operator bool() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}
};

int main()
{
	A a1 = 1;      // OK: copy-initialization selects A::A(int)
	A a2(2);       // OK: direct-initialization selects A::A(int)
	A a3 { 4, 5 };   // OK: direct-list-initialization selects A::A(int, int)
	A a4 = { 4, 5 }; // OK: copy-list-initialization selects A::A(int, int)
	A a5 = (A) 1;   // OK: explicit cast performs static_cast
	if (a1)
		;      // OK: A::operator bool()
	bool na1 = a1; // OK: copy-initialization selects A::operator bool()
	bool na2 = static_cast<bool>(a1); // OK: static_cast performs direct-initialization

//  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
	B b2(2);       // OK: direct-initialization selects B::B(int)
	B b3 { 4, 5 };   // OK: direct-list-initialization selects B::B(int, int)
//  B b4 = {4, 5}; // error: copy-list-initialization does not consider B::B(int,int)
	B b5 = (B) 1;   // OK: explicit cast performs static_cast
	if (b2)
		;      // OK: B::operator bool()
//  bool nb1 = b2; // error: copy-initialization does not consider B::operator bool()
	bool nb2 = static_cast<bool>(b2); // OK: static_cast performs direct-initialization
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```



## Example

### `libstdc++ unique_ptr`

```c++
       /** Takes ownership of a pointer.
        *
        * @param __p  A pointer to an object of @c element_type
        *
        * The deleter will be value-initialized.
        */
       template <typename _Up = _Dp,
                 typename = _DeleterConstraint<_Up>>
         explicit
         unique_ptr(pointer __p) noexcept
         : _M_t(__p)
         { }
```

下面是错误用法

```c++
	static std::unique_ptr<Api> New(const char *WorkPath)
	{
		return { NewApi(WorkPath)};
	}
```
编译报错如下：

```c++
In file included from main.cpp:8:
./../common.h:99:10: fatal error: chosen constructor is explicit in copy-initialization
                return { NewStockOptApi(WorkPath)};
                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/bin/../lib/gcc/x86_64-redhat-linux/4.8.5/../../../../include/c++/4.8.5/bits/unique_ptr.h:141:7: note: constructor declared
      here
      unique_ptr(pointer __p) noexcept

```
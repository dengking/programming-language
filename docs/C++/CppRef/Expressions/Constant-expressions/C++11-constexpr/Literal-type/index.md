# Literal type



## 如何理解literal type？

cppreference [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) 中对其定义如下：

> Specifies that a type is a *literal type*. Literal types are the types of [constexpr variables](https://en.cppreference.com/w/cpp/language/constexpr) and they can be constructed, manipulated, and returned from [constexpr functions](https://en.cppreference.com/w/cpp/language/constexpr).

microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170&viewFallbackFrom=vs-2019) 中，对literal type的总结如下:

> A literal type is one whose layout can be determined at compile time. The following are the literal types:
>
> 1、void
>
> 2、scalar types
>
> 3、references
>
> 4、Arrays of void, scalar types or references
>
> 5、A class that has a trivial destructor, and one or more constexpr constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and not volatile.

显然**literal type**和`constexpr`密切相关，所以，理解literal type的第一步是理解`constexpr`。

`constexpr`的核心思想是：“evaluate the value of the **function** or **variable** at **compile time**”，所以literal type的value也是需要compile-time获得的，它是在compile-time allocation、initialization，它的value就像是literal一样，在compile-time就已知了，这应该是literal type的命名缘由。

关于literal，参见：

- 在`C-family-language\C++\Language-reference\Expressions\Literals`中对它进行了介绍



## How to construct user-defined literal type？

在cppreference [Constructors and member initializer lists#Explanation](https://en.cppreference.com/w/cpp/language/constructor#Explanation) 中有这样的描述：

> The constructors with a `constexpr` specifier make their type a [*LiteralType*](https://en.cppreference.com/w/cpp/named_req/LiteralType).

显然，literal type的constructor需要使用 `constexpr` 来修饰。

在[C++ named requirements: LiteralType#Requirements](https://en.cppreference.com/w/cpp/named_req/LiteralType)对此也进行了说明:

> has a trivial (until C++20)constexpr (since C++20) destructor

上面这段话如何理解呢？在microsoft [Trivial, standard-layout, POD, and literal types#literal_types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#literal_types) 对literal type的解释:

> A class that has a **trivial destructor**, and one or more `constexpr` constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and not volatile.

## Examples



### cppreference [C++ named requirements: LiteralType#Example](https://en.cppreference.com/w/cpp/named_req/LiteralType) 

```c++
#include <cstddef>
#include <iostream>
#include <stdexcept>

// literal class
class conststr
{
	const char* p;
	std::size_t sz; // 字符串的长度
	public:
	template<std::size_t N>
	constexpr conststr(const char (&a)[N])
			: p(a), sz(N - 1) // N - 1 的目的是去除最后的 0
	{
	}

	// constexpr functions signal errors by throwing exceptions
	// in C++11, they must do so from the conditional operator ?:
	constexpr char operator[](std::size_t n) const
			{
		return n < sz ? p[n] : throw std::out_of_range("");
	}
	constexpr std::size_t size() const
	{
		return sz;
	}
};

// C++11 constexpr functions had to put everything in a single return statement
// (C++14 doesn't have that requirement)
constexpr std::size_t countlower(conststr s, std::size_t n = 0, std::size_t c = 0)
{
	return n == s.size() ? c :
			'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
											countlower(s, n + 1, c);
}

// output function that requires a compile-time constant, for testing
template<int n>
struct constN
{
	constN()
	{
		std::cout << n << '\n';
	}
};

int main()
{
	std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
	constN<countlower("Hello, world!")> out2; // implicitly converted to conststr
}
// g++ -std=c++11 test.cpp 
```

上述程序 输出如下：

```c++
the number of lowercase letters in "Hello, world!" is 9
```

上述例子有几个点需要特殊说明：

- `constexpr conststr(const char (&a)[N])` 运用的是 stackoverflow [Using a `constexpr` array size function](https://stackoverflow.com/a/7439261) 中介绍的技巧，在`C++\Language-reference\Expressions\Constant-expressions\constexpr\Constexpr-specifier.md`中收录了这个技巧
- `countlower("Hello, world!")`运用了 implicit conversion，因为`constexpr conststr(const char (&a)[N])` conversion function



### `spdlog/common.h`




```c++
// visual studio upto 2013 does not support noexcept nor constexpr
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define SPDLOG_NOEXCEPT _NOEXCEPT
#define SPDLOG_CONSTEXPR
#else
#define SPDLOG_NOEXCEPT noexcept
#define SPDLOG_CONSTEXPR constexpr
#endif

struct source_loc
{
    SPDLOG_CONSTEXPR source_loc() = default;
    SPDLOG_CONSTEXPR source_loc(const char *filename_in, int line_in, const char *funcname_in)
        : filename{filename_in}
        , line{line_in}
        , funcname{funcname_in}
    {}

    SPDLOG_CONSTEXPR bool empty() const SPDLOG_NOEXCEPT
    {
        return line == 0;
    }
    const char *filename{nullptr};
    int line{0};
    const char *funcname{nullptr};
};
```

`source_loc`的入参`filename_in`、`line_in`、`funcname_in`就确定了 ，所以它可以在compile-time构造。



### `static constexpr`

```c++
#include <string>
struct Test
{
static const std::string t = "hello";

};
int main()
{

}

```

上述代码编译报如下错误：

```c++
test.cpp:8:30: 错误：in-class initialization of static data member ‘const string Test::t’ of non-literal type
```

这是因为`std::string`不是**literal type**，所以它无法在compile-time确定它的值。

将上述代码修改为如下：

```c++
#include<string>
struct Test
{
static const char* t = "hello";

};
int main()
{

}
```

编译包如下错误：

```c++
test.cpp:4:24: 错误：‘constexpr’ needed for in-class initialization of static data member ‘const char* Test::t’ of non-integral type [-fpermissive]
 static const char* t = "hello";
```

修改为如下形式编译通过：

```c++
#include<string>
struct Test
{
static constexpr const char* t = "hello";

};
int main()
{

}
```



## cppreference [std::is_literal_type](https://en.cppreference.com/w/cpp/types/is_literal_type)



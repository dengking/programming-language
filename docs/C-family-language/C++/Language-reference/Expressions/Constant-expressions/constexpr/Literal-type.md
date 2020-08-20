# Literal type

## 如何理解literal type？

谈及literal type，联想到了literal，在`C-family-language\C++\Language-reference\Expressions\Literals`中对它进行了介绍。

在compile-time进行allocation、initialization

cppreference [constexpr variable](https://en.cppreference.com/w/cpp/language/constexpr)

## cppreference [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType)

> NOTE: Literal type和constexpr密切相关。

### Example

#### Literal class

cppreference [constexpr specifier#Example](https://en.cppreference.com/w/cpp/language/constexpr#Example)

```c++
#include <cstddef>
#include <iostream>
#include <stdexcept>

// literal class
class conststr
{
	const char* p;
	std::size_t sz;
	public:
	template<std::size_t N>
	constexpr conststr(const char (&a)[N])
			: p(a), sz(N - 1)
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

```

> ```
> g++ -std=c++11 test.cpp 
> ./a.out 
> the number of lowercase letters in "Hello, world!" is 9
> ```



在`C++\Language-reference\Expressions\Constant-expressions\constexpr\Constexpr-specifier.md`中的 `spdlog/common.h`也是literal class的一个例子。

#### `static constexpr`

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



## microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)

> NOTE:  这篇文章中也对literal type进行了描述。
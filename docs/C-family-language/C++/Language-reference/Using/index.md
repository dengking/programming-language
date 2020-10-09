# 关于本章

本章总结C++中keyword `using`的用法。

## cppreference [C++ keywords: using](https://en.cppreference.com/w/cpp/keyword/using)

> NOTE: “using”的字面含义是“使用”，它在C++中，有着多出用途，这正好验证了`Theory\Programming-language\Design-of-programming-language.md`的“Design of specifier/keyword”段中描述的观点。



## `using` and name lookup

`using` 和 compiler的name lookup密切相关，在下面文章中对此进行了说明:

- cppreference [Partial template specialization#Name lookup](https://en.cppreference.com/w/cpp/language/partial_specialization#Name_lookup)
- TODO: 添加the interface principle文章，其中也包含这个主题

## TODO

csdn https://blog.csdn.net/shift_wwx/article/details/78742459

这篇文章，总结的还是不错的。

## Examples

### cppreference [Partial template specialization#Name lookup](https://en.cppreference.com/w/cpp/language/partial_specialization#Name_lookup)



```C++
#include<iostream>

namespace N
{
// primary template
template<class T1, class T2> class Z
{
public:
	Z()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

}
using N::Z; // refers to the primary template
namespace N
{
// partial specialization    
template<class T> class Z<T, T*>
{
public:
	Z()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

}
Z<int, int*> z; // name lookup finds N::Z (the primary template),
				// the partial specialization with T = int is then used
int main()
{

}
// g++ test.cpp

```

上述程序的输出如下:

```C++
N::Z<T, T*>::Z() [with T = int]
```


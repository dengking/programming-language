# Enum



## enum is literal constant

本节的标题的含义是: enum本质上是literal constant；这就决定了enum的如下属性:

| 属性                    | 参见                                                  |
| ----------------------- | ----------------------------------------------------- |
| 可以用于static context  | `C-and-C++\Compile-time-and-run-time`                 |
| value category是prvalue | `C++\Language-reference\Expressions\Value-categories` |



## cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)

An *enumeration* is a distinct type whose value is restricted to a range of values (see below for details), which may include several explicitly named constants ("*enumerators*"). The values of the constants are values of an integral type known as the *underlying type* of the enumeration.

> NOTE: *underlying type*的概念是非常重要的，如何获取它呢？参见：[std::underlying_type](https://en.cppreference.com/w/cpp/types/underlying_type)

## Enum Classes VS Enum

https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/



## Enum and string

为enum的每个可选值指定一个name（string），这是我们的一种常见需求，那如何来实现呢？这是本节讨论的问题。下面讨论了各种实现方式: 

### switch-case

使用switch-case来实现，在cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)的[Example](https://en.cppreference.com/w/cpp/language/enum#Example)中给出的例子非常好。

### map + TMP

这是我在阅读stackoverflow [Specialization of template in different namespace](https://stackoverflow.com/questions/25311512/specialization-of-template-in-different-namespace) （`C++\Language-reference\Template\Specialization\Specialization-of-template-in-different-namespace.md`中收录了这篇文章）时，发现的一种实现，下面是对原文的代码的整理，将它放到了同一个文件中，并且修正了compiler error: 

```C++
#include <iostream>
#include <map>
#include <cstring>
namespace MyLib
{

#define DECLARE_ENUM( type ) template<> std::map<const char*, type>  \
            MyLib::Enum<type>::mMap = std::map<const char*, type>(); \
            template<> MyLib::Enum<type>::Enum (void)

template<typename Type> class Enum
{
public:
	Enum(void);
	static int Size(void)
	{ /* ... */
		return mMap.size();
	}

private:
	static std::map<const char*, Type> mMap;
};

}

enum MyEnum
{
	value1, value2, value3,
};

namespace MyLib
{
DECLARE_ENUM (MyEnum)
{
	mMap["value1"] = value1;
	mMap["value2"] = value2;
	mMap["value3"] = value3;
}
}
void SomeFunc(void)
{
	std::cout << MyLib::Enum<MyEnum>::Size() << std::endl;
}
// 触发构造
static MyLib::Enum<MyEnum> g_E;

int main()
{

	SomeFunc();
}
// g++ --std=c++11 test.cpp


```

上述代码的实现是非常值得借鉴的，每个enum都有一个对应的`Enum`类，这个类保存了enum的一个mapping: name to value。

输出为:

```
3
```



### array + enum as index

保存enum的name，使用array的方式，不使用map，这是我在阅读 cppreference [Array initialization](https://en.cppreference.com/w/c/language/array_initialization) # Example 时看到的: 

```C++
#include <iostream>
int main(void)
{

	// Character names can be associated with enumeration constants
	// using arrays with designators:
	enum
	{
		RED, GREEN, BLUE
	};
	const char *nm[] = { [RED] = "red", [GREEN] = "green", [BLUE] = "blue", };
	std::cout << sizeof(nm) / sizeof(nm[0]) << std::endl;
}
g++ -g test.cpp

```

输出为:

```
3
```



### TODO

stackoverflow [enum to string in modern C++11 / C++14 / C++17 and future C++20](https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20)

stackoverflow [enum class of type string in C++](https://stackoverflow.com/questions/43510615/enum-class-of-type-string-in-c)

github [magic_enum](https://github.com/Neargye/magic_enum)



## underlying type of enum

TODO:

- stackoverflow [What is the underlying type of a c++ enum?](https://stackoverflow.com/questions/1122096/what-is-the-underlying-type-of-a-c-enum)



### Output the value of enum class

在 stackoverflow [How can I output the value of an enum class in C++11](https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11) 的回答中给出的code，值的借鉴：

```c++
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
```


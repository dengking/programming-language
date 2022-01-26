# Enum



## cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)

An *enumeration* is a distinct type whose value is restricted to a range of values (see below for details), which may include several explicitly named **constants** ("***enumerators***"). The values of the constants are values of an integral type known as the *underlying type* of the enumeration.

> NOTE:
>
> ### enumerator is named constant
>
> 本节的标题的含义是: enumerator 本质上是constant；这就决定了enum的如下属性:
>
> | 属性                    | 参见                                                  |
> | ----------------------- | ----------------------------------------------------- |
> | 可以用于static context  | `C-and-C++\Compile-time-and-run-time`                 |
> | value category是prvalue | `C++\Language-reference\Expressions\Value-categories` |
>
> 关于constant，参见`C++\Language-reference\Expressions\Constant-expressions`。
>
> ### enumerator is value and is not object
>
> enumerator 是 value，而不是 object，这是一个非常重要的事实，这决定了enumerator的value category是prvalue。





There are two distinct kinds of enumerations: 

1) *unscoped enumeration* (declared with the *enum-key* `enum`) 

1) *scoped enumeration* (declared with the *enum-key* `enum class` or `enum struct`). (since C++11)



### Underlying type

> NOTE: 原文并没有Underlying type标题，我添加的目的是对underlying type进行统一描述。
>
> *underlying type*的概念是非常重要的，在C++11之前，C++语言并没有对enum的underlying type进行统一，也没有提供syntax来供programmer对它进行指定，C++11则解决了这个问题，它提供了syntax来供programmer对underlying type进行指定

#### Specify underlying type

| syntax                                         | 注解                                                         |
| ---------------------------------------------- | ------------------------------------------------------------ |
| `enum-key attr(optional) enum-name enum-base;` | colon (`:`), followed by a *type-specifier-seq* that names an **integral type** (if it is cv-qualified, qualifications are ignored) that will serve as the **fixed underlying type** for this enumeration type |

```C++
#include <cstdint>
 
// enum that takes 16 bits
enum smallenum: std::int16_t
{
    a,
    b,
    c
};

// altitude may be altitude::high or altitude::low
enum class altitude: char
{ 
     high='h',
     low='l', // C++11 allows the extra comma
}; 

```

#### Query underlying type

如何获取它呢？参见：[std::underlying_type](https://en.cppreference.com/w/cpp/types/underlying_type)。



#### TODO

1) stackoverflow [What is the underlying type of a c++ enum?](https://stackoverflow.com/questions/1122096/what-is-the-underlying-type-of-a-c-enum)



#### Output the value of enum class

在 stackoverflow [How can I output the value of an enum class in C++11](https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11) 的回答中给出的code，值的借鉴：

```c++
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
```





### Unscoped enumeration

### Scoped enumerations





## Enum Classes VS Enum

https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/



## Simulate scoped enumeration before C++11

如何在C++11之前的版本中模拟scoped enumeration ？

### stackoverflow [How to use enums in C++](https://stackoverflow.com/questions/12183008/how-to-use-enums-in-c)

[A](https://stackoverflow.com/a/12183377)

This code is **wrong**:

```cpp
enum Days { Saturday,Sunday,Tuesday,Wednesday,Thursday,Friday};
Days day = Days.Saturday;
if(day == Days.Saturday)
```

Because `Days` is not a **scope**, nor **object**. It is a **type**. And Types themselves don't have members. What you wrote is the equivalent to `std::string.clear`. `std::string` is a type, so you can't use `.`on it. You use `.` on an *instance* of a class.

Unfortunately, enums are magical and so the analogy stops there. Because with a class, you can do `std::string::clear` to get a pointer to the member function, but in C++03, `Days::Sunday` is invalid. (Which is sad). This is because` C++` is (somewhat) backwards compatable with C, and C had no namespaces, so enumerations had to be in the global namespace. So the syntax is simply:

```cpp
enum Days { Saturday,Sunday,Tuesday,Wednesday,Thursday,Friday};
Days day = Saturday;
if(day == Saturday)
```

Fortunately, [Mike Seymour](https://stackoverflow.com/users/204847/mike-seymour) observes that this has been addressed in C++11. Change `enum` to `enum class` and it gets its own **scope**; so `Days::Sunday` is not only valid, but is the *only* way to access `Sunday`. Happy days!

[A](https://stackoverflow.com/a/25786472)

You can use a trick to use scopes as you wish, just declare enum in such way:

```cpp
struct Days 
{
   enum type
   {
      Saturday,Sunday,Tuesday,Wednesday,Thursday,Friday
   };
};

Days::type day = Days::Saturday;
if (day == Days::Saturday)
```



## Enum and string

为enum的每个可选值指定一个name（string），这是我们的一种常见需求，那如何来实现呢？这是本节讨论的问题。下面讨论了各种实现方式: 

### 1) switch-case

使用switch-case来实现，在cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)的[Example](https://en.cppreference.com/w/cpp/language/enum#Example)中给出的例子非常好。

### 2) map + TMP

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



### 3) array + enum as index

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



## enum in TMP

1、我记得在TMP中，有多处是使用到了enum的，这些都整理到了 `Dispatch-based-on-constant`。

2、将此称为 Enum dispatch



## TODO enum and callback function

这其实是要实现polymorphism。

https://www.codeproject.com/Articles/10006/How-to-create-an-enum-and-callback-function

https://www.drdobbs.com/callback-enumeration-apis-the-input-ite/184401766
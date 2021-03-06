# `const char *` as key



## 问题描述

今天碰到了一个非常容易出现错误的常见:

```C++
#include <unordered_map> // std::unordered_map
#include <iostream>
#include <string>

std::unordered_map<const char*, int> M { { "1", 1 }, { "2", 2 } };
int* Find(const char *Key)
{
	auto Itor = M.find(Key);
	if (Itor == M.end())
	{
		return nullptr;
	}
	else
	{
		return &Itor->second;
	}
}
int main()
{

	std::string Key;
	std::cin >> Key;
	int *Value = Find(Key.c_str());
	if (Value)
	{
		std::cout << "Value is " << *Value << std::endl;
	}
	else
	{
		std::cout << "Not found" << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```

运行结果如下:

```C++
[dengkai@localhost ~]$ ./a.out 
1
Not found
```

`M`中明明包含key为1的entry，但是最终结果却是: Not found。这是为什么？问题根源在于使用了`const char *`作为key，在`unordered_map`进行hash的时候，是hash的**地址值**，而不是它所指向的**C string**。

通过Google:

1) "`const char*` as key of `unordered_map`"

2) "`const char*` as map key"



### 问题总结

C++标准库并没有为`const char*`提供如下operator:

1) comparator 

导致它无法作为`std::map`的key。

2) hasher

导致它无法作为`std::unordered_map`的key。



## 解决方法

### 1) 使用`std::string`来作为key

C++标准库为`std::string`提供了comparator、hasher，因此它可以作为`std::map`的key、`std::unordered_map`的key。但是将`const  char *`转换为`std::string`涉及deep copy。

关于`std::string`的comparator、hasher，参见cppreference [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string)。



### 2) 使用`std::string_view`(C++17)来作为key

C++标准库为`std::string_view`提供了comparator、hasher，因此它可以作为`std::map`的key、`std::unordered_map`的key。将`const  char *`转换为`std::string_view`不涉及deep copy。

关于`std::string_view`的comparator、hasher，参见cppreference [std::std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view)。

在stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) # [A](https://stackoverflow.com/a/64616123) 中给出了用法示例:

> Since C++17 added [`std::string_view`](https://en.cppreference.com/w/cpp/header/string_view) including a `std::hash` specialization for it you can use that to compute the hash value of a C-string.
>
> Example:
>
> ```cpp
> #include <string_view>
> #include <cstring>
> 
> static size_t hash_cstr(const char *s)
> {
>     return std::hash<std::string_view>()(std::string_view(s, std::strlen(s)));
> }
> ```

### 3) custom comparator、hasher

即自定义comparator、hasher

这种方式非常灵活，用户可以以不deep copy的方式来实现。



## Custom comparator、hasher examples



### stackoverflow [Using const char* as key for map/unordered_map](https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map)

> 其中推荐使用[`std::string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)，并且其中还从performance的角度进行了讨论，值得一读。

How to create map/unordered_map that will use `const char*` as key directly?

If I use `map<std::string,..>`, then on each resolving `map["abc"] = ...` a new `std::string` object will be created. That causes a big overhead for allocating memory, creating a string object and copying the string into it.

How do I declare a map object that uses `const char*` directly without any overhead?

**[A](https://stackoverflow.com/a/50608442)**

> NOTE: 使用

You can use a [`std::string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view):

```cpp
std::map<std::string_view, int> Map;
Map["abc"] = 1; // no allocation necessary to store "abc"
```

It is basically a wrapper around string objects. And it's a view, which means that it doesn't own the string and thus doesn't copy and allocate memory to store a string.

Note that for small strings (and also literals), `std::string` doesn't allocate too due to [SSO](https://stackoverflow.com/a/10319672/2477364) and so the overhead is minimal. Always measure before optimizing.

**COMMENTS:**

- And if you don't have C++17 you can use [`absl::string_view`](https://abseil.io/docs/cpp/guides/strings) (in fact you can use it in both cases, since it will be `typedef`'d to `std::string_view` if it exists). – [jdehesa](https://stackoverflow.com/users/1782792/jdehesa) [May 30 '18 at 15:32](https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map#comment88226781_50608442)

- Or [`boost::string_view`](https://www.boost.org/doc/libs/1_67_0/boost/utility/string_view.hpp) :) – [Rakete1111](https://stackoverflow.com/users/3980929/rakete1111) [May 30 '18 at 15:33](https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map#comment88226813_50608442)



**[A](https://stackoverflow.com/a/50608687)**

As an alternative to [Rakete1111's `string_view` answer](https://stackoverflow.com/a/50608442/1782465), you can equip your map with a suitable **comparator** (and **hasher**, for the `unordered_map`):

```cpp
struct LesserString
{
  bool operator() (const char *lhs, const char *rhs) const
  {
    return std::strcmp(lhs, rhs) < 0;
  }
};

struct HashString
{
  std::size_t operator() (const char *arg) const
  {
    return however_you_want_to_hash_the_string();
  }
};

struct EqualString
{
  bool operator() (const char *lhs, const char *rhs) const
  {
    return !strcmp(lhs, rhs);
  }
};

std::map<const char*, WhateverValue, LesserString> m1;
std::unorderd_map<const char*, WhateverValue, HashString, EqualString> m2;
```



### stackoverflow [Using char* as a key in std::map](https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap)

> 其中给出了样例程序，非常值得借鉴。
>

[A](https://stackoverflow.com/a/4157729)

You need to give a comparison functor to the map otherwise it's comparing the pointer, not the null-terminated string it points to. In general, this is the case anytime you want your map key to be a pointer.

For example:

```cpp
struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

map<char *, int, cmp_str> BlahBlah;
```



### hash for `const char *`

参见`C++\Library\Standard-library\Utility-library\Hash\General-purpose-hash`章节。
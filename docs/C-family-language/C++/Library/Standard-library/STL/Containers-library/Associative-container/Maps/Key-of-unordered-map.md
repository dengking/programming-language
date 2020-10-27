# key of unordered map



## `const char *` as key

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

`M`中明明包含key为1的entry，但是最终结果却是: Not found。这是为什么？问题根源在于使用了`const char *`作为key，在`unordered_map`进行hash的时候，是hash的地址值，而不是它所指向的cstring。

通过Google:

1) "`const char*` as key of `unordered_map`"

2) "`const char*` as map key"



找到如下跟这个问题相关的内容:

1) bugsdb [C++ unordered_map with char* as key](https://bugsdb.com/_en/debug/272c82a107ca800554a208494707cc6e)

2) stackoverflow [Using const char* as key for map/unordered_map](https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map)

其中推荐使用[`std::string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)，并且其中还从performance的角度进行了讨论，值得一读。

3) stackoverflow [Using char* as a key in std::map](https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap)

其中给出了样例程序，非常值得借鉴。

4) stackoverflow [Right way to use const char* as map key in C++ [duplicate]](https://stackoverflow.com/questions/24781160/right-way-to-use-const-char-as-map-key-in-c)

5) stackoverflow [Error trying to find const char* key from std::map](https://stackoverflow.com/questions/24398824/error-trying-to-find-const-char-key-from-stdmap)

## enum class as key



### stackoverflow [Can't use enum class as unordered_map key](https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key)

Q

I have a class containing an enum class.

```cpp
class Shader {
public:
    enum class Type {
        Vertex   = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };
    //...
```

Then, when I implement the following code in another class...

```cpp
std::unordered_map<Shader::Type, Shader> shaders;
```

...I get a compile error.

```cpp
...usr/lib/c++/v1/type_traits:770:38: 
Implicit instantiation of undefined template 'std::__1::hash<Shader::Type>'
```

What is causing the error here?

[A](https://stackoverflow.com/a/24847480)

I use a functor object to calculate hash of `enum class`:

```cpp
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};
```

Now you can use it as 3rd template-parameter of `std::unordered_map`:

```cpp
enum class MyEnum {};

std::unordered_map<MyEnum, int, EnumClassHash> myMap;
```

So you don't need to provide a specialization of `std::hash`, the **template argument deduction** does the job. Furthermore, you can use the word `using` and make your own `unordered_map` that use `std::hash` or `EnumClassHash` depending on the `Key` type:

```cpp
template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;
```

Now you can use `MyUnorderedMap` with `enum class` or another type:

```cpp
MyUnorderedMap<int, int> myMap2;
MyUnorderedMap<MyEnum, int> myMap3;
```

Theoretically, `HashType` could use `std::underlying_type` and then the `EnumClassHash` will not be necessary. That could be something like this, **but I haven't tried yet**:

```cpp
template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, std::hash<std::underlying_type<Key>::type>, std::hash<Key>>::type;
```

If using `std::underlying_type` works, could be a very good proposal for the standard.



### cplusplus [enum class as map key](http://www.cplusplus.com/forum/general/74363/)









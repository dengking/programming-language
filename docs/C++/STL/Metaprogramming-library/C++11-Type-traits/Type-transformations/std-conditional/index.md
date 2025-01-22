# std::conditional

是在阅读stackoverflow [Can't use enum class as unordered_map key](https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key)的[回答](https://stackoverflow.com/a/24847480)时，其中使用了`std::conditional`:

```c++
#include <unordered_map>
#include <iostream>
#include <type_traits>

/**
 * @brief 着色器
 *
 */
class Shader
{
public:
	enum class Type
	{
		Vertex = 1, Geometry = 2, Fragment = 3
	};
	//...
};

struct EnumClassHash
{
	template<typename T>
	std::size_t operator()(T t) const
	{
		std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
		return static_cast<std::size_t>(t);
	}
};

template<typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template<typename Key, typename T>
using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;

int main()
{
	MyUnorderedMap<int, int> myMap2;
	MyUnorderedMap<Shader::Type, int> myMap3;
}
// g++ --std=c++11 test.cpp

```

上述使用 `std::conditional` 构建了一个类型抽象层。



## cppreference [std::conditional](https://en.cppreference.com/w/cpp/types/conditional)

#### Possible implementation

```C++
template<bool B, class T, class F>
struct conditional {
	typedef T type;
};

template<class T, class F>
struct conditional<false, T, F> {
	typedef F type;
};
```



## 素材

stackoverflow [How std::conditional works](https://stackoverflow.com/questions/44550976/how-stdconditional-works)
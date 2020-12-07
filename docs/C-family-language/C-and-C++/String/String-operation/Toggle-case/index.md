# Toggle case

大小写转换。

## `std::transform`

**new**

新创建一个`std::string`

```C++
#include <string>
#include <cctype>
#include <iostream>
#include <algorithm> // std::transform
std::string make_lowercase(const std::string &in)
{
	std::string out;
	std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
	return out;
}

int main()
{
	std::string s = "hello";
	std::string s1 = "HELLO";
	if (make_lowercase(s1) == s)
	{
		std::cout << "equal" << std::endl;
	}
	else
	{
		std::cout << "not equal" << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```

**inplace**

就地修改，不新创建

```C++
#include <string>
#include <cctype>
#include <iostream>
#include <algorithm> // std::transform
void to_lowercase(std::string &in)
{
	std::transform(in.begin(), in.end(), in.begin(), ::tolower);
}

int main()
{
	std::string s = "hello";
	std::string s1 = "HELLO";
	to_lowercase(s1);
	if (s1 == s)
	{
		std::cout << "equal" << std::endl;
	}
	else
	{
		std::cout << "not equal" << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```



```C++
static void ToUpper(std::string& S)
{
	std::transform(S.begin(), S.end(), S.begin(), ::toupper);
}
```


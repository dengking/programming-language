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



### stackoverflow [What does the “::” mean in “::tolower”?](https://stackoverflow.com/questions/5270780/what-does-the-mean-in-tolower) 

> NOTE: 这是"Use a name in global namespace"，参见 `C++\Language-reference\Basic-concept\Organization\Scope` 章节。

[A](https://stackoverflow.com/a/5270970)

As to why the `::` is necessary: the standard defines two `tolower`'s, a function template in `std::`, and a simple function in both `::` and `std::`. Depending on which headers have been included (and that includes headers indirectly included from other headers, which you may not know about), either one, the other, or both may be visible. Using `::` ensures that the older one, from the C standard, is used. (If the one in `std::` is considered, the call will be ambiguous, since `transform` is a template itself, and the compiler will not be able to deduce the template arguments.)

> NOTE: why？

> NOTE: 关于后半段的内容，移到了`C-and-C++\String\Char`章节中。



## draft



[Converting to uppercase in C++](https://stackoverflow.com/questions/8693990/converting-to-uppercase-in-c)

[Convert a String In C++ To Upper Case](https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case)

```c++
#include "ctype.h"
#include <algorithm>
#include <iterator>
#include <string>

namespace StringUtil
{
static void ToUpper(std::string& S)
{
	std::transform(S.begin(), S.end(), S.begin(), ::toupper);
}
}
```


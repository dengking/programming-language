# basic_string



## cppreference [`basic_string`](https://en.cppreference.com/w/cpp/string/basic_string) 



### [`std::basic_string<CharT,Traits,Allocator>::basic_string`](https://en.cppreference.com/w/cpp/string/basic_string/basic_string) 



#### 使用`const  char *`来构造

原文的`(5)`描述了使用`const  char *`来构造，相关问题如下：

[C++ : Exception occurred in script: basic_string::_S_construct NULL not valid](https://stackoverflow.com/questions/12052997/c-exception-occurred-in-script-basic-string-s-construct-null-not-valid)

[C++报错解决：what(): basic_string::_S_construct null not valid](https://blog.csdn.net/zhangpeterx/article/details/99478357)

下面是解决该问题的一个简单的封装:

```c++
std::string pchar_to_string(const char * pchar)
{
    if (pchar)
    {
        return std::string(pchar); 
    }
	else
    {
    	return "";	    
    }
    
}
```





## Operation

### Upper Case

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


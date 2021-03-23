# basic_string



## cppreference [`basic_string`](https://en.cppreference.com/w/cpp/string/basic_string) 



### [`std::basic_string<CharT,Traits,Allocator>::basic_string`](https://en.cppreference.com/w/cpp/string/basic_string/basic_string) 



## 使用`const  char *`来构造

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





## Implementation

### [libstdc++ basic_string.h](https://gcc.gnu.org/onlinedocs/gcc-4.9.0/libstdc++/api/a00998_source.html)

#### [basic_string](https://gcc.gnu.org/onlinedocs/gcc-4.9.0/libstdc++/api/a00454.html)的operator的实现

[operator+=](https://gcc.gnu.org/onlinedocs/gcc-4.9.0/libstdc++/api/a00454.html#a3f985c5b2f4e7220ca0f49fba85ee836) modify 对应的object，所以将它们作为member method；

[operator+](https://gcc.gnu.org/onlinedocs/gcc-4.9.0/libstdc++/api/a01644.html#gaf2a59d9d4f1421dcdba7b5ce55ca9358) 需要构造一个新的object，所以没有将它作为member method；

无论是否是member method，它们都是`basic_string`的interface，关于此，参见gotw [What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)；


# `alignas`



## stackoverflow [Memory alignment : how to use alignof / alignas?](https://stackoverflow.com/questions/17091382/memory-alignment-how-to-use-alignof-alignas)





## cppreference [alignas specifier (since C++11)](https://en.cppreference.com/w/cpp/language/alignas)



```C++
// the array "cacheline" will be aligned to 128-byte boundary
alignas(128) char cacheline[128];
```

> NOTE: 
>
> "tag-align-to-cache line-optimization-alignas"



## zhihu [C++11中的alignas能否完全代替#pragma pack？](https://www.zhihu.com/question/307847194/answer/565472919)



### [Weyne Chen的回答 - 知乎](https://www.zhihu.com/question/307847194/answer/565472919) 

这段话中重要的一句就是，不能指定比本身还小的对齐。比如下面的代码

```C++
#include <bits/stdc++.h>
using namespace std;
struct alignas(1) Point
{
	int a;
	char b;
} p;

#pragma pack(push)
#pragma pack(1)
struct Point2
{
	int a;
	char b;
} p2;
#pragma pop(pop)

int main()
{
	cout << alignof (p) << endl;
	cout << sizeof(p) << endl;

	cout << alignof (p2) << endl;
	cout << sizeof(p2) << endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra


```

输出如下:

```C++
4
8
1
5
```



### [海鹏的回答](https://www.zhihu.com/question/307847194/answer/565426786) 

1、不能

2、有的平台不支持未对齐内存访问，`alignas`的目的是允许你往更大的字节数去对齐，比如char对齐到32位供SIMD load
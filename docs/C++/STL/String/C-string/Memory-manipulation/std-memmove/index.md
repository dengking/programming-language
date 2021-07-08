# `std::memmove`

## cppreference [`std::memmove`](http://cplusplus.com/reference/cstring/memmove/)



## Implementation

### [nowcoder](https://www.nowcoder.com/test/question/done?tid=45402489&qid=14696#summary) 

补充下面函数代码：
如果两段内存重叠，用`memcpy`函数可能会导致行为未定义。 而`memmove`函数能够避免这种问题，下面是一种实现方式，请补充代码。

```C++
#include <iostream>
using namespace std;
void* memmove(void* str1,const void* str2,size_t n)
{
    char* pStr1= (char*) str1;
    char* pStr2=(char*)str2;
    if  ( ) {
        for(size_t i=0;i!=n;++i){
            *(pStr1++)=*(pStr2++);
        }
    }
    else{
        pStr1+=n-1;
        pStr2+=n-1;
        for(size_t i=0;i!=n;++i){
            *(pStr1--)=*(pStr2--);
        }
    }
    return ( );
}
// g++ test.cpp

```

正确答案: A

```C++
pStr1< pStr2   str1
```

原型：`void *memmove( void* dest, const void* src, size_t count );`

头文件：`<string.h>`

功能：由`src`所指内存区域复制`count`个字节到`dest`所指内存区域。

如果目标区域和源区域有重叠的话，`memmove`能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中。但复制后`src`内容会被更改。但是当目标区域与源区域没有重叠则和`memcpy`函数功能相同。 

结合高票答案的图：

当`pStr1< pStr2`时，即 `pStr1`的首地址小于  `pStr2`的首地址（图1），正序拷贝

当`pStr1> pStr2`时，即 `pStr1`的首地址大于  `pStr2`的首地址（图2），倒叙拷贝
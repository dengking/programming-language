# C/C++ 中生成特定范围内的随机数

## csdn [C/C++ 中生成特定范围内的随机数](https://blog.csdn.net/Justme0/article/details/41547761)



## cnblogs [C++指定范围生成随机数](https://www.cnblogs.com/vathena/articles/4356646.html)

> NOTE: 
>
> module用于指定随机数的范围

```c++
#include "stdafx.h"
#include <iostream>
#include <ctime>
 
#define Randmod(x) rand()%x
using namespace std;
 
int _tmain(int argc, _TCHAR* argv[])
{
    //根据系统时间设置随机数种子
    srand( (unsigned)time(NULL) );
 
    //随机生成10个[0,10]的数
    for(int i = 0; i < 10; i++ )
    {
         cout << Randmod(11) << endl;
    }
    return 0;
}
```



```c++
srand((unsigned)time(null));
(low,up) #define Random (rand()%(up-low+1)) + low - 1
[low,up) #define Random (rand()%(up-low)) + low
(low,up] #define Random (rand()%(up-low))+ low + 1
[low,up] #define Random (rand()%(up-low+1)) + low
```


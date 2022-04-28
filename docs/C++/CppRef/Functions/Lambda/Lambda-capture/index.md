# lambda capture

C++ lambda的发展主要集中在capture上。





## Capture by reference



```C++
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <typeinfo>
#include <limits>
using namespace std;

int main(void)
{
    int i = 0;
    std::cout << &i << std::endl;
    auto F = [&i]()
    {
        std::cout << &i << std::endl;
    };
    F();
}
```

输出如下:

```
00D6F848
00D6F848
```

从输出来看，两者的地址相等，也就是是同一个object，因此是capture by reference。

## Capture by value


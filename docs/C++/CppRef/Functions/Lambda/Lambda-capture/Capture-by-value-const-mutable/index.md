# Capture by value always const

## stackoverflow [C++0x lambda capture by value always const?](https://stackoverflow.com/questions/2835626/c0x-lambda-capture-by-value-always-const)

Is there any way to capture by value, and make the captured value non-const? I have a library functor that I would like to capture & call a method that is non-const but should be.

The following doesn't compile but making `foo::operator() const` fixes it.

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

struct foo
{
    bool operator()(const bool &a)
    {
        return a;
    }
};

int main()
{
    foo afoo;

    auto bar = [=]() -> bool
    {
        afoo(true);
    };

    return 0;
}
```

> NOTE:
>
> 编译报错如下:
>
> ```c++
> [build] D:\GitHub-src\TestCpp\test.cpp(27,1): error C3848: 具有类型“const foo”的表达式会丢失一些 const-volatile 限定符以调用“bool foo::operator ()(const bool &)” [D:\GitHub-src\TestCpp\build\test.vcxproj]
> [build] Build finished with exit code 1
> ```
>
> 如果不添加`mutable` ，则`args`都是capture by value、const，则在编译的时候会报如下错误:

### [A](https://stackoverflow.com/a/2835645/10173843)

Use mutable.

```cpp
auto bar = [=] () mutable -> bool ....
```

Without mutable you are declaring the operator () of the lambda object const.

> NOTE: 
>
> 完整代码如下：
>
> ```C++
> #include <iostream>
> #include <string>
> #include <unordered_map>
> #include <array>
> #include <vector>
> #include <string>
> #include <iostream>
> #include <variant>
> #include <typeinfo>
> #include <limits>
> using namespace std;
> 
> struct foo
> {
>     bool operator()(const bool &a)
>     {
>         return a;
>     }
> };
> 
> int main()
> {
>     foo afoo;
> 
>     auto bar = [=]() mutable -> bool
>     {
>         afoo(true);
>     };
> 
>     return 0;
> }
> ```
>
> 

## see also

stackoverflow [How would one call std::forward on all arguments in a variadic function?](https://stackoverflow.com/questions/2821223/how-would-one-call-stdforward-on-all-arguments-in-a-variadic-function)


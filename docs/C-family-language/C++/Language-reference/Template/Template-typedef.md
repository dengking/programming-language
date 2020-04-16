# Template typedef

https://stackoverflow.com/questions/2795023/c-template-typedef

https://stackoverflow.com/questions/15099826/typedef-with-template-parameter-in-c

https://en.cppreference.com/w/cpp/language/type_alias



##  type alias can introduce a member typedef name

在例子中演示了这种写法：

https://en.cppreference.com/w/cpp/language/type_alias

```c++
// type alias can introduce a member typedef name
template<typename T>
struct Container { using value_type = T; };
```

下面是一个demo

```c++
template<class T>
class C
{
using TT = T;
};

int main(){}
```


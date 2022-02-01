# Member initialization

member initialization涉及到的内容非常多:

1、[Constructors and member initializer lists](https://pd.codechef.com/docs/cpp/cpp/language/constructor.html)

关于此，可以参见 `Constructor` 章节

## cppreference [Non-static data members # Member initialization](https://pd.codechef.com/docs/cpp/cpp/language/data_members.html#Member_initialization)

Non-static data members may be initialized in one of two ways:

### 1) 

In the [member initializer list](https://pd.codechef.com/docs/cpp/cpp/language/constructor.html) of the constructor.

```C++
struct S
{
    int n;
    std::string s;
    S() : n(7) // direct-initializes n, default-initializes s
    { }
};
```

### 2) (since C++11)

Through a *default member initializer*, which is simply a brace or equals [initializer](https://pd.codechef.com/docs/cpp/cpp/language/initialization.html) included in the member declaration, which is used if the member is omitted in the **member initializer list**

```C++
struct S
{
    int n = 7;
    std::string s{'a', 'b', 'c'};
    S() // copy-initializes n, list-initializes s
    { }
};
```

#### **default member initializer** and  **member initialization list** 

If a member has a **default member initializer** and also appears in the **member initialization list** in a constructor, the default member initializer is ignored.

```C++
#include <iostream>

int x = 0;
struct S
{
    int n = ++x;
    S() { }                 // uses default member initializer
    S(int arg) : n(arg) { } // uses member initializer list
};

int main()
{
    std::cout << x << '\n'; // prints 0
    S s1;
    std::cout << x << '\n'; // prints 1
    S s2(7);
    std::cout << x << '\n'; // prints 1
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、输出如下:
>
> ```
> 0
> 1
> 1
> ```
>
> 

Default member initializers are not allowed for [bit field](https://pd.codechef.com/docs/cpp/cpp/language/bit_field.html) members.

> NOTE: 
>
> 需要补充一个例子

Members of array type cannot deduce their size from member initializers:

```C++

struct X {
    int a[] = { 1,2,3 }; // error
    int b[3] = { 1,2,3 }; // OK
};
int main()
{

}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 编译报错如下:
>
> ```C++
> test.cpp:3:9: error: ISO C++ forbids flexible array member ‘a’ [-Werror=pedantic]
> 
>     3 |     int a[] = { 1,2,3 }; // error
> 
>       |         ^
> 
> test.cpp:3:9: error: flexible array member ‘X::a’ not at end of ‘struct X’
> 
> test.cpp:3:9: error: initializer for flexible array member ‘int X::a []’
> 
> cc1plus: all warnings being treated as errors
> ```
>
> 
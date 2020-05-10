# enable_if



## boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)

> NOTE: 这篇文章讲到了精髓



## thegreenplace [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

```C++
#include <iostream>
void foo(unsigned i) {
  std::cout << "unsigned " << i << "\n";
}

template <typename T>
void foo(const T& t) {
  std::cout << "template " << t << "\n";
}

int main()
{
    foo(42);
    return 0;
}
```

> NOTE: 编译`g++ test.cpp`，输出为：
>
> template 42

What do you think a call to `foo(42)` would print? The answer is `"template 42"`, and the reason for this is that integer literals are signed by default (they only become unsigned with the `U` suffix).



## `std::enable_if`

cppreference [std::enable_if](https://en.cppreference.com/w/cpp/types/enable_if)

cplusplus [std::enable_if](http://www.cplusplus.com/reference/type_traits/enable_if/)







## Usage

[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function)


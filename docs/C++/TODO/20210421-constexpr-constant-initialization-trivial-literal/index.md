# pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

## [The Yellow Zone - Dynamic Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-yellow-zone---dynamic-initialization)



```C++
#include <string>
static constexpr auto VERSION = std::string("3.4.1");

int main()
{
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```



```C++
test.cpp:2:52: error: the type 'const std::__cxx11::basic_string<char>' of 'constexpr' variable 'VERSION' is not literal
 static constexpr auto VERSION = std::string("3.4.1");
                                                    ^
In file included from /usr/include/c++/8/string:52,
                 from test.cpp:1:
/usr/include/c++/8/bits/basic_string.h:77:11: note: 'std::__cxx11::basic_string<char>' is not literal because:
     class basic_string
           ^~~~~~~~~~~~
/usr/include/c++/8/bits/basic_string.h:77:11: note:   'std::__cxx11::basic_string<char>' has a non-trivial destructor
```

The compiler is complaining because `std::string` defines a non-trivial destructor. That means that `std::string` is probably allocating some resource that must be freed upon destruction, in this case memory. This is a problem, if we create an `std::string` at compile time the managed memory must be somehow copied into the binary as well, as it won’t be available when the executable is run!

In other words, `std::string("3.4.1")` is not a constant expression so we can’t force the compiler to const-initialize it!
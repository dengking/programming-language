# `std::variant`

一、variant对union的增强，能够实现static type safety

二、"variant"的含义是"变体"

三、素材

https://arne-mertz.de/2018/05/modern-c-features-stdvariant-and-stdvisit/

https://arne-mertz.de/2019/10/multiple-dispatch-over-covariant-functions/

## cppreference [std::variant](https://en.cppreference.com/w/cpp/utility/variant)



## cppstories [Everything You Need to Know About std::variant from C++17](https://www.cppstories.com/2018/06/variant/)

### The Basics

```c++
#include <iostream>
#include <string>
#include <vector>

union S
{
    std::string str;
    std::vector<int> vec;
    ~S() {} // what to delete here?
};

int main()
{
    S s = {"Hello, world"};
    // at this point, reading from s.vec is undefined behavior
    std::cout << "s.str = " << s.str << '\n';

    // you have to call destructor of the contained objects!
    s.str.~basic_string<char>();

    // and a constructor!
    new (&s.vec) std::vector<int>;

    // now, s.vec is the active member of the union
    s.vec.push_back(10);
    std::cout << s.vec.size() << '\n';

    // another destructor
    s.vec.~vector<int>();
}
```



```cmake
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (test)
# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 11)

# Add an executable
add_executable(test test.cpp)
```

As you see, the `S` union needs a lot of maintenance from your side. You have to know which type is active and adequately call destructors/constructors before switching to a new variant.

That’s the reason you probably won’t see a lot of unions that use “advanced” types such as vectors, strings, containers, etc, etc. Union is mostly for basic types.

What could make unions better?

1、the ability to use complex types and the full support of their lifetime: if you switch the type then a proper destructor is called. That way we don’t leak.

2、a way to know what’s the active type

Before C++17 you could use some third-party library…. or use boost variant. But now you have `std::variant`.

```c++
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <variant>

struct SampleVisitor
{
    void operator()(int i) const
    {
        std::cout << "int: " << i << "\n";
    }
    void operator()(float f) const
    {
        std::cout << "float: " << f << "\n";
    }
    void operator()(const std::string &s) const
    {
        std::cout << "string: " << s << "\n";
    }
};

int main()
{
    std::variant<int, float, std::string> intFloatString;
    static_assert(std::variant_size_v<decltype(intFloatString)> == 3);

    // default initialized to the first alternative, should be 0
    std::visit(SampleVisitor{}, intFloatString);

    // index will show the currently used 'type'
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = 100.0f;
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = "hello super world";
    std::cout << "index = " << intFloatString.index() << std::endl;

    // try with get_if:
    if (const auto intPtr(std::get_if<int>(&intFloatString)); intPtr)
        std::cout << "int!" << *intPtr << "\n";
    else if (const auto floatPtr(std::get_if<float>(&intFloatString)); floatPtr)
        std::cout << "float!" << *floatPtr << "\n";

    if (std::holds_alternative<int>(intFloatString))
        std::cout << "the variant holds an int!\n";
    else if (std::holds_alternative<float>(intFloatString))
        std::cout << "the variant holds a float\n";
    else if (std::holds_alternative<std::string>(intFloatString))
        std::cout << "the variant holds a string\n";

    // try/catch and bad_variant_access
    try
    {
        auto f = std::get<float>(intFloatString);
        std::cout << "float! " << f << "\n";
    }
    catch (std::bad_variant_access &)
    {
        std::cout << "our variant doesn't hold float at this moment...\n";
    }

    // visit:
    std::visit(SampleVisitor{}, intFloatString);
    intFloatString = 10;
    std::visit(SampleVisitor{}, intFloatString);
    intFloatString = 10.0f;
    std::visit(SampleVisitor{}, intFloatString);
}
```



```cmake
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (test)
# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 17)

# Add an executable
add_executable(test test.cpp)
```


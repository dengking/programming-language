# Default member initializer

default member initializer是C++11中引入的特性，下面是一些相关的内容:

1) cppreference [Non-static data members#Member initialization](https://en.cppreference.com/w/cpp/language/data_members)

其中对default member initialization进行了介绍

2) arne-mertz [Modern C++ Features – Default Initializers for Member Variables](https://arne-mertz.de/2015/08/new-c-features-default-initializers-for-member-variables/)

3) abseil [Tip of the Week #61: Default Member Initializers](https://abseil.io/tips/61)



## rule of zero

CppCoreGuidelines [C.45: Don’t define a default constructor that only initializes data members; use in-class member initializers instead](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-default)

CppCoreGuidelines  [C.48: Prefer in-class initializers to member initializers in constructors for constant initializers](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-in-class-initializer)


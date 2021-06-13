# STL container `std::swap` overload and member `swap` 

1、STL是采用的`Non-throwing-swap-idiom`的，下面的这些overload的function parameter type提供了各自高效的member `swap` 和 non-member function `swap` overload

2、cppreference [Member function table](https://en.cppreference.com/w/cpp/container) # Modifiers # swap

可以看到，各种container都提供了`swap` member function、non-member function `std::swap` overload


# Function overload

本文讨论function overload。

## What is function overload

在cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions)中的介绍如下: 

> Multiple functions in the same scope may have the same name, as long as their parameter lists and, for **non-static member functions**, cv/ref (since C++11)-qualifications are different. This is known as [function overloading](https://en.cppreference.com/w/cpp/language/overload_resolution). Function declarations that differ only in the return type and the noexcept specification (since C++17) cannot be overloaded.

关于non-static member functions，参见`C++\Language-reference\Classes\Members\Non-static-member\Function-member`。

需要注意的是: function overload是不考虑return type的，这和function type不同。

## Overload is a kind of static polymorphism

参见`C++\Guide\Plymorphism`。



## TODO

[Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)

[Address of an overloaded function](https://en.cppreference.com/w/cpp/language/overloaded_address)


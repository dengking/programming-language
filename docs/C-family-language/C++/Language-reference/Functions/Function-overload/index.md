# Function overload

本文讨论function overload。

## What is function overload

在cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions)中的介绍如下: 

> Multiple functions in the same scope may have the same name, as long as their parameter lists and, for **non-static member functions**, cv/ref (since C++11)-qualifications are different. This is known as [function overloading](https://en.cppreference.com/w/cpp/language/overload_resolution). Function declarations that differ only in the return type and the noexcept specification (since C++17) cannot be overloaded.

关于non-static member functions，参见`C++\Language-reference\Classes\Members\Non-static-member\Function-member`。



## TODO: overload is a kind of static polymorphism

增加overload is a kind of static polymorphism的说明。
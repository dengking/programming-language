# user-defined conversion 



## user-defined conversion function



### cppreference [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator)

Enables [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) or [explicit conversion](https://en.cppreference.com/w/cpp/language/explicit_cast) from a [class type](https://en.cppreference.com/w/cpp/language/class) to another type.

#### Syntax

Conversion function is declared like a [non-static member function](https://en.cppreference.com/w/cpp/language/member_functions) or member [function template](https://en.cppreference.com/w/cpp/language/function_template) with no parameters, no explicit return type, and with the name of the form:

### Microsoft [User-Defined Type Conversions (C++)](https://docs.microsoft.com/en-us/cpp/cpp/user-defined-type-conversions-cpp?view=vs-2019)



### Conversion isn't considered for member access

stackoverflow [Why user-defined conversion is not implicitly taking place on the calling object](https://stackoverflow.com/questions/44699176/why-user-defined-conversion-is-not-implicitly-taking-place-on-the-calling-object)

[A](https://stackoverflow.com/a/44699334)

Conversion isn't considered for member access (§5.2.5/2 [expr.ref]).

> In either case, the *id-expression* shall name a member of the class or of one of its base classes

Here the *id-expression* is `func()`

So the compiler considers that `func` must be a member of `B` or a class that `B` derives from. Implicit conversion to other types that might have a `func` member is not considered.



### Example

[C++ overloading conversion operator for custom type to std::string](https://stackoverflow.com/questions/3518145/c-overloading-conversion-operator-for-custom-type-to-stdstring)



## Converting constructor



### cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)
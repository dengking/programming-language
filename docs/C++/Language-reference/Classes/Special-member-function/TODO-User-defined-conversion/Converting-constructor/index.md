# Converting constructor



## cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)



#### What is converting constructor？

A constructor that is not declared with the specifier [explicit](https://en.cppreference.com/w/cpp/language/explicit) and which can be called with a single parameter (until C++11) is called a *converting constructor*.

Implicitly-declared and user-defined non-explicit [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor) and [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) are converting constructors.

> NOTE: 
>
> 1、从C++11开始，对"single parameter"放开了限制，允许有多个parameter，后面描述了这样的example



Unlike explicit constructors, which are only considered during [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (which includes [explicit conversions](https://en.cppreference.com/w/cpp/language/explicit_cast) such as [static_cast](https://en.cppreference.com/w/cpp/language/static_cast)), converting constructors are also considered during [copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization), as part of [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast).

> NOTE: 
>
> 1、这是取自下面的例子:
>
> ```C++
> A a1 = 1;      // OK: copy-initialization selects A::A(int)
> ```
>
> 2、上述  [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast) ，其实就是 [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast)
>
> 3、copy constructor会参与两种initialization: 
>
> copy initialization 和 direct initialization



It is said that a converting constructor specifies an implicit conversion from the types of its arguments (if any) to the type of its class. Note that non-explicit [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) also specifies an implicit conversion.
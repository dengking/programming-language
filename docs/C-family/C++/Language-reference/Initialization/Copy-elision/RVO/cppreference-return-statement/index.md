# cppreference [`return` statement](https://en.cppreference.com/w/cpp/language/return)

## RVO

**Returning by value** may involve construction and copy/move of a **temporary object**, unless [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) is used. 


> NOTE: 上面这段话的意思是: 如果没有使用  [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) ，则"**Returning by value** may involve construction and copy/move of a **temporary object**, "

Specifically, the conditions for copy/move are as follows:

### Automatic move from local variables and parameters(since C++11)

If *`expression`* is a (possibly parenthesized) [id-expression](https://en.cppreference.com/w/cpp/language/identifiers) that names a variable whose type is either

> NOTE: "id-expression"意味着: 它是lvalue。

1、a non-volatile object type or

2、a non-volatile rvalue reference to object type(since C++20)

> NOTE: 2、暂时不懂

and that variable is declared

1、in the body or

2、as a parameter of the innermost enclosing function or lambda expression,

> NOTE: 2、暂时不懂

then [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) to select the constructor to use for initialization of the returned value or, for `co_return`, to select the overload of `promise.return_value()` (since C++20) is performed *twice*:

1、first as if *expression* were an rvalue expression (thus it may select the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor)), and

a、if the first overload resolution failed or

b、it succeeded, but did not select the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) (formally, the first parameter of the selected constructor was not an rvalue reference to the (possibly cv-qualified) type of *expression*)(until C++20)



2、then overload resolution is performed as usual, with *expression* considered as an lvalue (so it may select the [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor)).

### Guaranteed copy elision(since C++17)

If *expression* is a prvalue, the result object is initialized directly by that expression. This does not involve a copy or move constructor when the types match (see [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)).
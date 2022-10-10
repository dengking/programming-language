# C++ keywords: noexcept (since C++11)



## cppreference [C++ keywords: noexcept (since C++11)](https://en.cppreference.com/w/cpp/keyword/noexcept)

|                                                              |      |
| ------------------------------------------------------------ | ---- |
| [`noexcept` operator](https://en.cppreference.com/w/cpp/language/noexcept) (since C++11) |      |
| [`noexcept` specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) (since C++11) |      |





## cppreference [noexcept operator (since C++11)](https://en.cppreference.com/w/cpp/language/noexcept)

The `noexcept` operator performs a compile-time check that returns true if an expression is declared to not throw any exceptions.

It can be used within a function template's [`noexcept` specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) to declare that the function will throw exceptions for some types but not others.

> NOTE:
>
> 一、关于上面这段话的内容，在 "cppreference [noexcept specifier (since C++11)](https://en.cppreference.com/w/cpp/language/noexcept_spec) # Notes" 中也进行了论述:
>
> > One of the uses of the constant *expression* is (along with the [`noexcept` operator](https://en.cppreference.com/w/cpp/language/noexcept)) to define function templates that declare `noexcept` for some types but not others.
>
> 在cppreference [noexcept specifier (since C++11)](https://en.cppreference.com/w/cpp/language/noexcept_spec) 的example提供了对应上述的例子，下面是简化的例子:
>
> ```c++
> // whether foo is declared noexcept depends on if the expression
> // T() will throw any exceptions
> template <class T>
> void foo() noexcept(noexcept(T())) {}
> 
> int main()
> {
>     foo<int>(); // noexcept(noexcept(int())) => noexcept(true), so this is fine
> }
> 
> ```
>
> 
>
> 



## cppreference [noexcept specifier (since C++11)](https://en.cppreference.com/w/cpp/language/noexcept_spec)



Every function in C++ is either *non-throwing* or *potentially throwing*:

### *potentially-throwing* functions

一、functions declared with a non-empty [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) (until C++17)

> NOTE:
>
> 一、C++17已经移除了 [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) 

二、functions declared with `noexcept` specifier whose *expression* evaluates to `false` 

三、functions declared without `noexcept` specifier except for

1、[destructors](https://en.cppreference.com/w/cpp/language/destructor) unless the destructor of any potentially-constructed base or member is *potentially-throwing* (see below)

2、[default constructors](https://en.cppreference.com/w/cpp/language/default_constructor), [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor), [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) that are implicitly-declared or defaulted on their first declaration unless:

a、a constructor for a base or member that the implicit definition of the constructor would call is *potentially-throwing* (see below)

b、a subexpression of such an initialization, such as a default argument expression, is *potentially-throwing* (see below)

c、a default member initializer (for default constructor only) is *potentially-throwing* (see below)

3、[copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment) operators, [move assignment](https://en.cppreference.com/w/cpp/language/move_assignment) operators that are implicitly-declared or defaulted on their first declaration unless the invocation of any assignment operator in the implicit definition is *potentially-throwing* (see below)



### non-throwing functions

non-throwing functions are all others (

一、those with noexcept specifier whose *expression* evaluates to `true` 

二、destructors, 

三、defaulted special member functions, 

四、deallocation functions

)


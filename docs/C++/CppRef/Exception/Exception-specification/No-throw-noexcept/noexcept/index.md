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
> 一、 [dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec) 
>
> (deprecated in C++11)
> (removed in C++17)

二、functions declared with `noexcept` specifier whose *expression* evaluates to `false` 

三、functions declared without `noexcept` specifier except for

1、[destructors](https://en.cppreference.com/w/cpp/language/destructor) unless the destructor of any potentially-constructed base or member is *potentially-throwing* (see below)

> NOTE:
>
> 一、根据后面的"non-throwing functions"段、cnblogs [C++11 带来的新特性 （3）—— 关键字noexcept](https://www.cnblogs.com/sword03/p/10020344.html) 中的内容可知：
>
> > 析构函数是默认加上关键字`noexcept`的
>
> 在 cnblogs [C++11 带来的新特性 （3）—— 关键字noexcept](https://www.cnblogs.com/sword03/p/10020344.html) 中给出了验证的例子
>
> 二、上面这段话说明的是在何时destructor是 ***potentially-throwing*** 的

2、[default constructors](https://en.cppreference.com/w/cpp/language/default_constructor), [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor), [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) that are implicitly-declared or defaulted on their first declaration unless:

a、a constructor for a base or member that the implicit definition of the constructor would call is *potentially-throwing* (see below)

b、a subexpression of such an initialization, such as a default argument expression, is *potentially-throwing* (see below)

c、a default member initializer (for default constructor only) is *potentially-throwing* (see below)

> NOTE:
>
> 一、上面描述的是在何时"[default constructors](https://en.cppreference.com/w/cpp/language/default_constructor), [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor), [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) that are implicitly-declared or defaulted on their first declaration"是 ***potentially-throwing*** 的 

3、[copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment) operators, [move assignment](https://en.cppreference.com/w/cpp/language/move_assignment) operators that are implicitly-declared or defaulted on their first declaration unless the invocation of any assignment operator in the implicit definition is *potentially-throwing* (see below)

4、[deallocation functions](https://en.cppreference.com/w/cpp/memory/new/operator_delete)

> NOTE:
>
> 一、[deallocation functions](https://en.cppreference.com/w/cpp/memory/new/operator_delete) 虽然没有`noexcept`，但是它是"non-throwing function"



### non-throwing functions

non-throwing functions are all others (

一、those with noexcept specifier whose *expression* evaluates to `true` 

二、destructors, 

三、defaulted special member functions, 

四、deallocation functions

)



#### Exception specification and overload

TODO

#### Pointers (including pointers to member function) to non-throwing functions

TODO

#### virtual function non-throwing

TODO



#### Non-throwing functions call potentially-throwing functions

**Non-throwing functions** are permitted to call **potentially-throwing functions**. Whenever an exception is thrown and the search for a handler encounters the outermost block of a **non-throwing function**, the function [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) is called:

```c++
extern void f(); // potentially-throwing
 
void g() noexcept
{
    f();      // valid, even if f throws
    throw 42; // valid, effectively a call to std::terminate
}
```

### Exception specification of a function template specialization(since C++14)

TODO



### Formal definition of *potentially-throwing expression*

1、`e` is a function call to a function, pointer to function, or pointer to member function which is *potentially-throwing*, unless `e` is a [core constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) (until C++17)

2、`e` makes an implicit call to a *potentially-throwing* function (such as an overloaded operator, an allocation function in a `new`-expression, a constructor for a function argument, or a destructor if `e` is a full-expression)

3、`e` is a [`throw`-expression](https://en.cppreference.com/w/cpp/language/throw)

4、`e` is a [`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast) that casts a polymorphic reference type

5、`e` is a [`typeid`](https://en.cppreference.com/w/cpp/language/typeid) expression applied to a dereferenced pointer to a polymorphic type

6、`e` has an immediate subexpression that is potentially-throwing

### Notes

One of the uses of the constant *expression* is (along with the [`noexcept` operator](https://en.cppreference.com/w/cpp/language/noexcept)) to define function templates that declare `noexcept` for some types but not others.

Note that a `noexcept` specification on a function is not a **compile-time check**; it is merely a method for a programmer to inform the compiler whether or not a function should throw exceptions. The compiler can use this information to enable certain optimizations on **non-throwing functions** as well as enable the [`noexcept` operator](https://en.cppreference.com/w/cpp/language/noexcept), which can check at compile time if a particular expression is declared to throw any exceptions. For example, containers such as [std::vector](https://en.cppreference.com/w/cpp/container/vector) will move their elements if the elements' move constructor is `noexcept`, and copy otherwise (unless the copy constructor is not accessible, but a potentially throwing move constructor is, in which case the strong exception guarantee is waived).

> NOTE:
>
> 一、zero-cost-abstraction:
>
> 1、use-static-info-to-optimize
>
> 二、"waived"的意思是: 抛弃。
>
> 

## stackoverflow [When should I really use noexcept?](https://stackoverflow.com/questions/10787766/when-should-i-really-use-noexcept)

> NOTE:
>
> 一、其实按照CppCoreGuidelines中的规范即可解决很多问题
>





## TODO

stackoverflow [When should I really use noexcept?](https://stackoverflow.com/questions/10787766/when-should-i-really-use-noexcept)

cnblogs [C++11 带来的新特性 （3）—— 关键字noexcept ](https://www.cnblogs.com/sword03/p/10020344.html)

stackoverflow [Disabling C++ exceptions, how can I make any std:: throw() immediately terminate?](https://stackoverflow.com/questions/7249378/disabling-c-exceptions-how-can-i-make-any-std-throw-immediately-terminate)
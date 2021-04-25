# `std::function`

## cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)

Class template `std::function` is a general-purpose polymorphic function wrapper. Instances of `std::function` can store, copy, and invoke any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) *target* -- 

- functions
- [lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)
- [bind expressions](https://en.cppreference.com/w/cpp/utility/functional/bind)
- other function objects
- pointers to member functions 
- pointers to data members

> NOTE: 
>
> `std::function`是一种abstraction、wrapper；正如在文章drdobbs [Lambdas in C++11](https://www.drdobbs.com/cpp/lambdas-in-c11/240168241?pgno=2)中所说的：
>
> > Luckily, C++11 incorporates a **wrapper** for any of the following functions with `std::function` and makes it easy to pass around lambda expressions:
> >
> > - Lambda expression
> > - Function pointer
> > - Functor
>
> 上面这段话中的polymorphic 是非常重要的，它表明`std::function`是支持多态的，`std::function`所存储的应该是pointer to function，所以它是reference semantic，所以它能够支持polymorphism；关于reference semantic，参见`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics`。

The stored callable object is called the *target* of `std::function`. If a `std::function` contains no target, it is called *empty*. Invoking the *target* of an *empty* `std::function` results in [std::bad_function_call](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call) exception being thrown.

> NOTE: the stored callable object和`std::function`之间的关系就好比thread和`std::thread`之间的关系。

`std::function` satisfies the requirements of [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) and [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable).

> NOTE: `std::function`它满足  [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) 和[*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) ，这传达出的一个semantic是两个 `std::function` object能够表示相同的target；想想，`std::function`是否是 [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) 和 [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) 的呢？



### Notes

Care should be taken when a `std::function` whose **result type** is a **reference** is initialized from a lambda expression without a **trailing-return-type**. Due to the way auto deduction works, such **lambda expression** will always return a **prvalue**. Hence, the **resulting reference** will usually bind to a **temporary** whose lifetime ends when `std::function::operator()` returns.

> NOTE: 上面这段话的第一句的意思是：当使用一个“lambda expression”来构造`std::function`的时候，如果这个lambda expression所表示的function的返回值类型是reference，并且lambda expression没有使用**trailing-return-type**来说明返回值的，那么在这种情况下，compiler的auto deduction将会导致这个**lambda expression**的返回值是**prvalue**；那么这种情况下，就会导致dangling reference。下面这个例子就是最好的说明：

```c++
#include <functional>
#include <iostream>


int main()
{
	std::function<const int&()> F([]{ return 42; });
	int x = F(); // Undefined behavior: the result of F() is a dangling reference
	std::cout<<x<<std::endl;
}
// g++ --std=c++11 test.cpp
```

## `std::function` 是 polymorphic value type

> NOTE: 
>
> 1、"tag-std-function-polymorphic value type-融合OOP-和-GP"

`std::function` 是典型的polymorphic value type，它如何了OOP + GP 的优点:

一、OOP: 

1、提供了consistent interface，因此它可以被pass around

正如在 stackoverflow [What is ScopeGuard in C++?](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c) # [A](https://stackoverflow.com/a/31365171) 中所总结的: 

> The rôle of the `function` here is to avoid templating so that `Scope_guard` instances can be declared as such, and passed around. An alternative, slightly more complex and with slightly constrained usage, but possibly marginally more efficient, is to have a class templated on a functor type, and use C++11 `auto` for declarations, with the scope guard instance created by a factory function. 

2、polymorphic

二、GP: 抽象出了类型，能够表达callable concept

### `std::function` is generic and a good abstraction

`std::function` is generic and a good abstraction，所有callable都可以包装为`std::function`:

1、lambda

2、method

3、member function

此处需要添加一些例子来进行说明:

1、spdlog的thread_pool

2、`generic_thread_pool`



## `std::function` and member function

在 cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) example中给出了一个"`std::function` and member function" 的例子。

### stackoverflow [Using generic std::function objects with member functions in one class](https://stackoverflow.com/questions/7582546/using-generic-stdfunction-objects-with-member-functions-in-one-class)

> NOTE: 非常好的一篇文章

[A](https://stackoverflow.com/a/7582576)

A non-static member function must be called with an object. That is, it always implicitly passes "this" pointer as its argument.

Because your `std::function` signature specifies that your function doesn't take any arguments (`<void(void)>`), you must *bind* the first (and the only) argument.

```cpp
std::function<void(void)> f = std::bind(&Foo::doSomething, this);
```

If you want to bind a function with parameters, you need to specify placeholders:

```cpp
using namespace std::placeholders;
std::function<void(int,int)> f = std::bind(&Foo::doSomethingArgs, this, std::placeholders::_1, std::placeholders::_2);
```

Or, if your compiler supports C++11 lambdas:

```cpp
std::function<void(int,int)> f = [=](int a, int b) {
    this->doSomethingArgs(a, b);
}
```

(I don't have a C++11 capable compiler at hand *right now*, so I can't check this one.)

[A](https://stackoverflow.com/a/7582574)

Either you need

```cpp
std::function<void(Foo*)> f = &Foo::doSomething;
```

> NOTE: pointer to member function

so that you can call it on any instance, or you need to bind a specific instance, for example `this`

```cpp
std::function<void(void)> f = std::bind(&Foo::doSomething, this);
```



[A](https://stackoverflow.com/a/40394121)

If you need to store a member function **without** the class instance, you can do something like this:

```cpp
class MyClass
{
public:
    void MemberFunc(int value)
    {
      //do something
    }
};

// Store member function binding
auto callable = std::mem_fn(&MyClass::MemberFunc);

// Call with late supplied 'this'
MyClass myInst;
callable(&myInst, 123);
```

What would the storage type look like without *auto*? Something like this:

```cpp
std::_Mem_fn_wrap<void,void (__cdecl TestA::*)(int),TestA,int> callable
```

You can also pass this function storage to a standard function binding

```cpp
std::function<void(int)> binding = std::bind(callable, &testA, std::placeholders::_1);
binding(123); // Call
```

Past and future notes: An older interface *std::mem_func* existed, but has since been deprecated. A proposal exists, post C++17, to make [pointer to member functions callable](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0312r0.pdf). This would be most welcome.
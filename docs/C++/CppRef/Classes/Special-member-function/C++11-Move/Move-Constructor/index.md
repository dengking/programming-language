# Move Constructor



## cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)



### Notes

To make the [strong exception guarantee](https://en.cppreference.com/w/cpp/language/exceptions#Exception_safety) possible, user-defined move constructors should not throw exceptions. For example, [std::vector](https://en.cppreference.com/w/cpp/container/vector) relies on [std::move_if_noexcept](https://en.cppreference.com/w/cpp/utility/move_if_noexcept) to choose between move and copy when the elements need to be relocated.

> NOTE:
>
> 1、"Use-static-info-to-optimize"



## Move constructor on derived object

1、如何调用基类的move constructor？

### stackoverflow [Move constructor on derived object](https://stackoverflow.com/questions/4086800/move-constructor-on-derived-object)



```C++
Derived(Derived&& rval) : Base(std::move(rval))
 { }
```

> NOTE: 
>
> 1、正确写法



### [A](https://stackoverflow.com/a/4086826)

> NOTE:
>
> 1、"A named rvalue reference is an lvalue-just like any other variable"

`rval` is not a Rvalue. It is an Lvalue inside the body of the move constructor. That's why we have to explicitly invoke `std::move`.

Refer [this](http://www.artima.com/cppsource/rvalue.html). The important note is

> Note above that the argument x is treated as an lvalue internal to the move functions, even though it is declared as an rvalue reference parameter. That's why it is necessary to say move(x) instead of just x when passing down to the base class. This is a key safety feature of move semantics designed to prevent accidently moving twice from some named variable. All moves occur only from rvalues, or with an explicit cast to rvalue such as using std::move. If you have a name for the variable, it is an lvalue.
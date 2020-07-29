

# `std::move`

## cppreference [std::move](https://en.cppreference.com/w/cpp/utility/move)



## Implementation



> https://stackoverflow.com/a/11540204
>
> `std::move(some_lvalue)` casts an lvalue to an rvalue, thus enabling a subsequent move.
>
> 需要注意的是， 仅仅是rvalue，而不是prvalue，因为xvalue也是rvalue；



C++11给予programmer可以引用prvalue的权利，这就是rvalue reference，对于prvalue，programmer是可以安全地将其move走的，这是在C++语言级别支持的（compiler能够识别）。同时C++还给予了programmer将lvalue也move走的权利，为了支持这个，C++语言做了如下变动：

引入了xvalue的概念，xvalue既可以归入glvalue，也可以归入rvalue，通过`std::move`，programmer告诉compiler将其当做rvalue来使用，以充分发挥move semantic。

为了支持上述转换，C++添加了reference collapsing规则；

## [What is std::move(), and when should it be used?](https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used)

### [A](https://stackoverflow.com/a/3413547)

[Wikipedia Page on C++11 R-value references and move constructors](https://en.wikipedia.org/wiki/C%2B%2B11#Rvalue_references_and_move_constructors)

1. In C++11, in addition to copy constructors, objects can have move constructors.
   (And in addition to copy assignment operators, they have move assignment operators.)
2. The move constructor is used instead of the copy constructor, if the object has type "rvalue-reference" (`Type &&`).
3. `std::move()` is a cast that produces an **rvalue-reference** to an object, to enable moving from it.

It's a new `C++` way to **avoid copies**. For example, using a move constructor, a `std::vector` could just copy its internal pointer to data to the new object, leaving the moved object in an incorrect state, avoiding to copy all data. This would be C++-valid.

Try googling for move semantics, rvalue, perfect forwarding.



## 


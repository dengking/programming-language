# Class template argument deduction (CTAD) (since C++17)

## cppreference [Class template argument deduction (CTAD) (since C++17)](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)

In order to instantiate a [class template](https://en.cppreference.com/w/cpp/language/class_template), every template argument must be known, but not every template argument has to be specified. In the following contexts the compiler will deduce the template arguments from the type of the initializer:

### Deduction for class templates

#### Implicitly-generated deduction guides



#### User-defined deduction guides



### Deduction for alias templates(since C++20)



## CTAD is replacement for object generator pattern

在阅读stackoverflow [Current status of std::make_array](https://stackoverflow.com/questions/50942556/current-status-of-stdmake-array) # [A](https://stackoverflow.com/a/50943029) 

> C++17 includes [template argument deduction for classes](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0091r3.html), so you can now write:
>
> ```cpp
> std::pair p (foo, bar);
> std::array arr = { 1, 2, 3, 4, 5 };
> ```
>
> and so on. But there are some (somewhat subtle) remaining use cases where `make_pair` or `make_array` can be useful, and you can read about them in: [Usefulness of std::make_pair and std::make_tuple in C++1z](https://stackoverflow.com/questions/43875420/usefulness-of-stdmake-pair-and-stdmake-tuple-in-c1)

显然 [C++17 Class template argument deduction (CTAD)](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) 在一定程度上可以取代 object generator pattern。
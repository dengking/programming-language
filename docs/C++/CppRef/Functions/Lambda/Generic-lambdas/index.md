# Generic lambdas



## wikipedia [C++14 # Generic lambdas](https://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas)

In C++11, [lambda function](https://infogalactic.com/info/Anonymous_function) parameters need to be declared with concrete types. C++14 relaxes this requirement, allowing lambda function parameters to be declared with the `auto` type specifier.[[6\]](https://infogalactic.com/info/C%2B%2B14#cite_note-isocpp-6)

```c++
auto lambda = [](auto x, auto y) {return x + y;};
```

As for `auto` type deduction, generic lambdas follow the rules of template argument deduction (which are similar, but not identical in all respects). The above code is equivalent to this:[[11\]](https://infogalactic.com/info/C%2B%2B14#cite_note-11)

```c++
struct unnamed_lambda
{
  template<typename T, typename U>
    auto operator()(T x, U y) const {return x + y;}
};
auto lambda = unnamed_lambda{};
```

## cppreference [Lambda expressions (since C++11)](https://en.cppreference.com/w/cpp/language/lambda)

### since C++14

If [`auto`](https://en.cppreference.com/w/cpp/language/auto) is used as a type of a parameter or an explicit template parameter list is provided (since C++20), the lambda is a *generic lambda*. 

For each parameter in *params* whose type is specified as auto, an invented template parameter is added to *template-params*, in order of appearance. The invented template parameter may be a [parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack) if the corresponding function member of *params* is a function parameter pack.

```c++
// generic lambda, operator() is a template with two parameters
auto glambda = [](auto a, auto&& b) { return a < b; };
bool b = glambda(3, 3.14); // OK
 
// generic lambda, operator() is a template with one parameter
auto vglambda = [](auto printer)
{
    return [=](auto&&... ts) // generic lambda, ts is a parameter pack
    { 
        printer(std::forward<decltype(ts)>(ts)...);
        // nullary lambda (takes no parameters):
        return [=] { printer(ts...); };
    };
};
 
auto p = vglambda([](auto v1, auto v2, auto v3)
{
    std::cout << v1 << v2 << v3;
});
 
auto q = p(1, 'a', 3.14); // outputs 1a3.14
q();                      // outputs 1a3.14
```

### since C++20

If the lambda definition uses an explicit template parameter list, that template parameter list is used with operator(). For each parameter in *params* whose type is specified as `auto`, an additional invented template parameter is appended to the end of that template parameter list:

```c++
// generic lambda, operator() is a template with two parameters
auto glambda = []<class T>(T a, auto&& b) { return a < b; };
 
// generic lambda, operator() is a template with one parameter pack
auto f = []<typename... Ts>(Ts&&... ts)
{
    return foo(std::forward<Ts>(ts)...);
};
```



## cppstories [20 Smaller yet Handy C++20 Features](https://www.cppstories.com/2022/20-smaller-cpp20-features/)

In C++14, we got generic lambdas with `auto` as a lambda parameter. However, sometimes it was not good enough. That’s why in C++20, you can also use “real” template argument syntax, also with concepts!

```cpp
auto fn = []<typename T>(vector<T> const& vec) { 
    cout << size(vec) << “, “ << vec.capacity(); 
};
```

Ream more in [Lambda Week: Going Generic - C++ Stories](https://www.cppstories.com/2020/08/lambda-generic.html/) and in the proposal: [P0428r2](https://wg21.link/p0428r2).
# Abbreviated function template(since C++20)

function with auto parameter 相当于 function template



素材:

1、cppstories [20 Smaller yet Handy C++20 Features](https://www.cppstories.com/2022/20-smaller-cpp20-features/) 

2、microsoft [Abbreviated Function Templates and Constrained Auto](https://devblogs.microsoft.com/cppblog/abbreviated-function-templates-and-constrained-auto/) 



## cppreference [Function template](https://en.cppreference.com/w/cpp/language/function_template) # Abbreviated function template(since C++20)

> NOTE: 
>
> 一、目的是simplify C++

```C++
void f1(auto); // same as template<class T> void f(T)
void f2(C1 auto); // same as template<C1 T> void f2(T), if C1 is a concept
void f3(C2 auto...); // same as template<C2... Ts> void f3(Ts...), if C2 is a concept
void f4(const C3 auto*, C4 auto&); // same as template<C3 T, C4 U> void f4(const T*, U&);
template <class T, C U>
void g(T x, U y, C auto z); // same as template<class T, C U, C W> void g(T x, U y, W z);
```


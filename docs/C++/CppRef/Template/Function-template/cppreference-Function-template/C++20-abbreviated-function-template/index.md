# Abbreviated function template(since C++20)



## cppstories [20 Smaller yet Handy C++20 Features](https://www.cppstories.com/2022/20-smaller-cpp20-features/) 

Thanks to the terse concept syntax, you can also write templates without the `template<typename...>` part.

With unconstrained `auto`:

```cpp
void myTemplateFunc(auto param) { }
```

The code is equivalent to the following “regular” template style:

```cpp
template <typename T>
void myTemplateFunc(T param) { }
```

Or with constrained `auto` - this time we specify a concept name that the type has to comply with:

```cpp
template <class T>
concept SignedIntegral = std::is_signed_v<T> && std::is_integral_v<T>;

void signedIntsOnly(SignedIntegral auto val) { }

void floatsOnly(std::floating_point auto fp) { }
```

See at [@Compiler Explorer](https://godbolt.org/z/rnPvK6Wvj).

And then it’s equal to:

```cpp
template <class T>
concept SignedIntegral = std::is_signed_v<T> && std::is_integral_v<T>;

template <SignedIntegral T>
void signedIntsOnly(T val) { }

template <std::floating_point T>
void floatsOnly(T fp) { }
```

Additionally, `template <SignedIntegral T>` is also a short hand notation for:

```cpp
template <typename T>
requires SignedIntegral<T>
void signedIntsOnly(T val) { }

template <typename T>
requires std::floating_point<T>
void floatsOnly(T fp) { }
```

See a simple demo [@Compiler Explorer](https://godbolt.org/z/b6Pxshzn1).

Such syntax is similar to what you could use in generic lambdas from C++14:

```cpp
// C++14 lambda:
auto lambda = [](auto val) { };

// C++20 template function:
void myTemplateFunction(auto val) { }
```

See my separate blog post on Concepts: [C++20 Concepts - a Quick Introduction - C++ Stories](https://www.cppstories.com/2021/concepts-intro/).

And more in the proposal: [Yet another approach for constrained declarations - P1141R2](https://wg21.link/P1141R2).

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


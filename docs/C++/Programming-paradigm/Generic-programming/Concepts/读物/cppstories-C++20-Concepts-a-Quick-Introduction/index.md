# cppstories [C++20 Concepts - a Quick Introduction](https://www.cppstories.com/2021/concepts-intro/)



## What is a concept? [ ](https://www.cppstories.com/2021/concepts-intro/#what-is-a-concept) 

In short, a **concept is a set of constraints** on template parameters evaluated at compile time. You can use them for class templates and function templates to control function overloads and partial specialization.





We can also define another one using a **`requires` expression**:

```cpp
template <typename T>
concept ILabel = requires(T v)
{
    {v.buildHtml()} -> std::convertible_to<std::string>;
};
```

We defined a concept that requires that an object of type T has a member function called `buildHtml()`, which returns something convertible to `std::string`.



## How to use concepts [ ](https://www.cppstories.com/2021/concepts-intro/#how-to-use-concepts) 

In one of the most common case, for a small function template, you’ll see the following syntax:

```cpp
template <typename T>
requires CONDITION
void DoSomething(T param) { }
```

You can also use `requires clause` as the last part of a function declaration:

```cpp
template <typename T>
void DoSomething(T param) requires CONDITION
{ 
    
}
```

The key part is the `requires` clause. It allows us to specify various requirements on the input template parameters.

Let’s look at a simple function template that computes an average of an input container.

```cpp
#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>

template <typename T> 
requires std::integral<T> || std::floating_point<T>
constexpr double Average(std::vector<T> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / vec.size();
}

int main() {
    std::vector ints { 1, 2, 3, 4, 5};
    std::cout << Average(ints) << '\n';                                      
}
```

Play with code [@Compiler Explorer](https://godbolt.org/z/fefPT6b5r)

With the above source code, I used two concepts available in the standard library (`std::integral` and `std::floating_point`) and combined them together.



## Code Simplification [ ](https://www.cppstories.com/2021/concepts-intro/#code-simplification) 

As you could see, the syntax for concepts and constraints is relatively easy, but still, in C++20, we got much more!

There are various shortcuts and terse syntax that allow us to make template code super simple.

We have several things:

- Abbreviated Function Templates
- Constrained auto
- Terse syntax for concepts

### Constrained auto



For example:

```cpp
template <typename T>
void print(const std::vector<T>& vec) {
    for (size_t i = 0; auto& elem : vec)
        std::cout << elem << (++i == vec.size() ? "\n" : ", ");
}
```

We can “compress” it into:

```cpp
void print2(const std::vector<auto>& vec) {
    for (size_t i = 0; auto& elem : vec)
        std::cout << elem << (++i == vec.size() ? "\n" : ", ");
}
```

In the above case, I used unconstrained `auto`. In general, you can write:

```cpp
auto func(auto param) { }
```

And it expands into:

```cpp
template <typename T>
auto func(T param) { }
```

It looks similar to what we get with C++14 and generic lambdas ([Lambda Week: Going Generic](https://www.cppstories.com/2020/08/lambda-generic.html/)).

Additionally, we can also use constrained `auto`:

```cpp
void print3(const std::ranges::range auto& container) {
    for (size_t i = 0; auto && elem : container)
        std::cout << elem << (++i == container.size() ? "\n" : ", ");
};
```

With `print3`, I removed the need to pass a vector and restricted it for all ranges.

Play with the code [@Compiler Explorer](https://godbolt.org/z/9qhfY6d1e)

### Abbreviated Function Templates

Here we have:

```cpp
auto func(concept auto param) { }
```

Translates into:

```cpp
template <typename T>
requires concept<T>
auto func(T param) { }
```

What’s more, rather than specifying `template <typename T> requires...` you can write:

```cpp
template <std::integral T>
auto sum(const std::vector<T>& vec) {
    // return ...;
}
```



## The `requires` expression [ ](https://www.cppstories.com/2021/concepts-intro/#the-requires-expression) 

One of the most powerful items with concepts is the `requires` keyword. It has two forms:

1、the `requires` clause - like `requires std::integral<T>` or similar

2、the `requires` expression.

### Example 1

The last one is very flexible and allows to specify quite advanced constraints. In the introduction you’ve seen one case with a detection of `buildHtml()` member function. Here’s another example:

```cpp
template<typename T>
concept has_string_data_member = requires(T v) { // pretend we have a value of the type `T`
    { v.name_ } -> std::convertible_to<std::string>; 
};

struct Person {
    int age_ { 0 };
    std::string name_;
};

struct Box {
    double weight_ { 0.0 };
    double volume_ { 0.0 };
};

int main() {
    static_assert(has_string_data_member<Person>);
    static_assert(!has_string_data_member<Box>);
}
```

As you can see above, we can write `requires(T v)`, and from now on, we can pretend we have a value of the type `T`, and then we can list what operations we can use.



### Example 2

Another example:

```cpp
template <typename T>
concept Clock = requires(T c) { 
    c.start();  
    c.stop();
    c.getTime();
  };
```

The above concept restricts an “interface” for basic clocks. We require that it has the three member functions, but we don’t specify what type do they return.

From one perspective, we can say that the `requires` expression takes a type and tries to instantiate the specified requirements. If it fails, then a given class doesn’t comply with this concept. It’s like SFINAE but in a friendly and easy-to-express syntax.

I just showed some basic examples to give you a taste, but look at this article from A. Krzemienski: [Requires-expression | Andrzej’s C++ blog](https://akrzemi1.wordpress.com/2020/01/29/requires-expression/) which expends this topic in more depth.



## The updated Detection Idiom [ ](https://www.cppstories.com/2021/concepts-intro/#the-updated-detection-idiom) 

Thanks to Concepts we can now easily detect a function, a member function or even a particular overload. This is much simpler that with complicated SFINAE techniques that we had before.

See my other article on that topic: [How To Detect Function Overloads in C++17/20, std::from_chars Example - C++ Stories](https://www.cppstories.com/2019/07/detect-overload-from-chars/)
# `std::visit`

## cppreference [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)



## fluentcpp [On Design Patterns in C++](https://www.fluentcpp.com/2020/12/18/on-design-patterns-in-cpp/) # Visitor with `std::visit`

C++17’s `std::visit` gives another example of implementing the **Visitor design pattern**. `std::visit` is a way to apply a function on a `std::variant`. But since, by definition, a `std::variant` can hold values of different type, we may need various functions to operate on it.

> NOTE: 也是一种many-to-many

Consider the following example. This is one of the various techniques to create a function object that can operate on various types in C++:

```C++
struct Visitor
{
    std::string operator()(std::string const& s){ return s; }
    
    template<typename T>
    std::string operator()(T const& value) { return std::to_string(value);}
};
```

Let’s now assume that we have a function that allows to get a variant object:

```
std::variant<int, std::string, char> getNumber();
```

Then we can apply the visitor on the variant object with `std::visit`:

```
std::string s = std::visit(Visitor{}, getNumber());
```

Here the visited object (the variant) uses runtime polymorphism (even though without inheritance and virtual methods), and the visitor object (the `Visitor`) uses compile time polymorphism based on overload resolution.



## bitbashing [std::visit is everything wrong with modern C++](https://bitbashing.io/std-visit.html)
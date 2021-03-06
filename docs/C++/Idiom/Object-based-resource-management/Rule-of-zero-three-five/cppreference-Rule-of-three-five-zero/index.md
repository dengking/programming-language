# cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

> NOTE: 
>
> 原文给出的example非常一般

## Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

> NOTE: 三者只要有其一，就需要有其三

Because C++ copies and copy-assigns objects of user-defined types in various situations (passing/returning by value, manipulating a container, etc), these special member functions will be called, if accessible, and if they are not user-defined, they are implicitly-defined by the compiler.

> NOTE: 

The implicitly-defined special member functions are typically incorrect if the class manages a resource whose handle is an object of non-class type (raw pointer, POSIX file descriptor, etc), whose destructor does nothing and copy constructor/assignment operator performs a "shallow copy" (copy the value of the handle, without duplicating the underlying resource).

> NOTE: 
>
> 1、这就是问题的根源所在，一般，我们需要使用"single responsibility principle + rule of three five zero + RAII "来实现resource wrapper，这样能够有效地解决上述问题

```c++
#include <cstddef>
#include <cstring>
#include <iostream>
class rule_of_three
{
    char *cstring; // raw pointer used as a handle to a dynamically-allocated memory block

    void init(const char *s)
    {
        std::size_t n = std::strlen(s) + 1;
        cstring = new char[n];
        std::memcpy(cstring, s, n); // populate
    }

public:
    rule_of_three(const char *s = "")
    {
        std::cout << "construcotr" << std::endl;
        init(s);
    }

    ~rule_of_three()
    {
        delete[] cstring; // deallocate
    }

    rule_of_three(const rule_of_three &other) // copy constructor
    {
        std::cout << "copy constructor" << std::endl;
        init(other.cstring);
    }

    rule_of_three &operator=(const rule_of_three &other) // copy assignment
    {
        std::cout << "copy assignment" << std::endl;
        if (this != &other)
        {
            delete[] cstring; // deallocate
            init(other.cstring);
        }
        return *this;
    }
};

int main()
{

    rule_of_three a("rule of three"); // constructor
    rule_of_three a1();               // constructor
    rule_of_three b(a);               // copy constructor
    rule_of_three c = a;              // copy constructor
    c = b;                            // copy assignment
}
```

> NOTE: 上述代码中，copy assignment的实现是非常不好的，比它更好的实现方式是采用copy-and-swap idiom（在`C-family-language\C++\Idiom\OOP\Resource-management\Copy-and-swap`中对它进行了介绍）。

Classes that manage non-copyable resources through copyable handles may have to declare **copy assignment** and **copy constructor** private and not provide their definitions or define them as deleted. This is another application of the rule of three: deleting one and leaving the other to be implicitly-defined will most likely result in errors.

> NOTE: non-copyable resources 的典型例子包括：thread

## Rule of five

Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions:

```c++
class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
 public:
    rule_of_five(const char* s = "")
    : cstring(nullptr)
    { 
        if (s) {
            std::size_t n = std::strlen(s) + 1;
            cstring = new char[n];      // allocate
            std::memcpy(cstring, s, n); // populate 
        } 
    }
 
    ~rule_of_five()
    {
        delete[] cstring;  // deallocate
    }
 
    rule_of_five(const rule_of_five& other) // copy constructor
    : rule_of_five(other.cstring)
    {}
 
    rule_of_five(rule_of_five&& other) noexcept // move constructor
    : cstring(std::exchange(other.cstring, nullptr))
    {}
 
    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
         return *this = rule_of_five(other);
    }
 
    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
 
// alternatively, replace both assignment operators with 
//  rule_of_five& operator=(rule_of_five other) noexcept
//  {
//      std::swap(cstring, other.cstring);
//      return *this;
//  }
};
```

Unlike Rule of Three, failing to provide move constructor and move assignment is usually not an error, but a missed optimization opportunity.

## Rule of zero

Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership (which follows from the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)). Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.[[1\]](https://en.cppreference.com/w/cpp/language/rule_of_three#cite_note-1)

> NOTE: 
>

This rule also appears in the C++ Core Guidelines as [C.20: If you can avoid defining default operations, do](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero).

```c++
class rule_of_zero
{
    std::string cppstring;
 public:
    rule_of_zero(const std::string& arg) : cppstring(arg) {}
};
```



When a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. This blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted[[2\]](https://en.cppreference.com/w/cpp/language/rule_of_three#cite_note-2)

```c++
class base_of_five_defaults
{
 public:
    base_of_five_defaults(const base_of_five_defaults&) = default;
    base_of_five_defaults(base_of_five_defaults&&) = default;
    base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
    base_of_five_defaults& operator=(base_of_five_defaults&&) = default;
    virtual ~base_of_five_defaults() = default;
};
```




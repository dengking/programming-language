# CppCoreGuidelines [T: Templates and generic programming](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t-templates-and-generic-programming)

**Generic programming** is programming using types and algorithms parameterized by types, values, and algorithms. In C++, **generic programming** is supported by the `template` language mechanisms.

Arguments to generic functions are characterized by sets of requirements on the argument types and values involved. In C++, these requirements are expressed by compile-time predicates called **concepts**.

Templates can also be used for **meta-programming**; that is, programs that compose code at compile time.

A central notion in generic programming is “concepts”; that is, requirements on template arguments presented as compile-time predicates. “Concepts” were standardized in C++20, although they were first made available, in slightly older syntax, in GCC 6.1.

## Template use rule summary:

### [T.5: Combine generic and OO techniques to amplify(放大) their strengths, not their costs](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t5-combine-generic-and-oo-techniques-to-amplify-their-strengths-not-their-costs) 

Reason: Generic and OO techniques are complementary(相互补充的).

##### Example

 Static helps dynamic: Use static polymorphism to implement dynamically polymorphic interfaces.

```c++
class Command {
    // pure virtual functions
};

// implementations
template</*...*/>
class ConcreteCommand : public Command {
    // implement virtuals
};
```

> NOTE:
>
> 上述模式的tag-OOP-interface+template-implementation=virtual-base+template-derived-subclass，下面是它的use case:
>
> - type erasure

##### Example

Dynamic helps static: Offer a generic, comfortable, statically bound interface, but internally dispatch dynamically, so you offer a **uniform object layout**. Examples include **type erasure** as with `std::shared_ptr`’s deleter (but [don’t overuse type erasure](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-erasure)).

```c++
#include <memory>

class Object {
public:
    template<typename T>
    Object(T&& obj)
        : concept_(std::make_shared<ConcreteCommand<T>>(std::forward<T>(obj))) {}

    int get_id() const { return concept_->get_id(); }

private:
    struct Command {
        virtual ~Command() {}
        virtual int get_id() const = 0;
    };

    template<typename T>
    struct ConcreteCommand final : Command {
        ConcreteCommand(T&& obj) noexcept : object_(std::forward<T>(obj)) {}
        int get_id() const final { return object_.get_id(); }

    private:
        T object_;
    };

    std::shared_ptr<Command> concept_;
};

class Bar {
public:
    int get_id() const { return 1; }
};

struct Foo {
public:
    int get_id() const { return 2; }
};

Object o(Bar{});
Object o2(Foo{});
```



## [Template interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#template-interfaces)

Over the years, programming with templates have suffered from a weak distinction between the interface of a template and its implementation. Before concepts, that distinction had no direct language support. However, the interface to a template is a critical concept - a contract between a user and an implementer - and should be carefully designed.

> NOTE: template的interface和implementation指的是什么？

### [T.40: Use function objects to pass operations to algorithms](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t40-use-function-objects-to-pass-operations-to-algorithms)

##### Reason

Function objects can carry more information through an interface than a “plain” pointer to function. In general, passing function objects gives better performance than passing pointers to functions. 

##### Example

```c++
bool greater(double x, double y) { return x > y; }
sort(v, greater);                                    // pointer to function: potentially slow
sort(v, [](double x, double y) { return x > y; });   // function object
sort(v, std::greater{});                             // function object

bool greater_than_7(double x) { return x > 7; }
auto x = find_if(v, greater_than_7);                 // pointer to function: inflexible
auto y = find_if(v, [](double x) { return x > 7; }); // function object: carries the needed data
auto z = find_if(v, Greater_than<double>(7));        // function object: carries the needed data
```

You can, of course, generalize those functions using `auto` or concepts. For example:

```c++
auto y1 = find_if(v, [](totally_ordered auto x) { return x > 7; }); // require an ordered type
auto z1 = find_if(v, [](auto x) { return x > 7; });                 // hope that the type has a >
```

##### Note

Lambdas generate function objects.

##### Note

The performance argument depends on compiler and optimizer technology.

### [T.42: Use template aliases to simplify notation and hide implementation details](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t42-use-template-aliases-to-simplify-notation-and-hide-implementation-details)

##### Reason

Improved readability. Implementation hiding. Note that **template aliases** replace many uses of traits to compute a type. They can also be used to wrap a trait.

##### Example

```c++
template<typename T, size_t N>
class Matrix {
    // ...
    using Iterator = typename std::vector<T>::iterator;
    // ...
};
```

This saves the user of `Matrix` from having to know that its elements are stored in a `vector` and also saves the user from repeatedly typing `typename std::vector<T>::`.

##### Note

A simple, common use could be expressed: “Wrap traits!”

### [T.43: Prefer `using` over `typedef` for defining aliases](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t43-prefer-using-over-typedef-for-defining-aliases)

##### Reason

Improved readability: With `using`, the new name comes first rather than being embedded somewhere in a declaration. Generality: `using` can be used for template aliases, whereas `typedef`s can’t easily be templates. Uniformity: `using` is syntactically similar to `auto`.

##### Example

```c++
typedef int (*PFI)(int);   // OK, but convoluted

using PFI2 = int (*)(int);   // OK, preferred

template<typename T>
typedef int (*PFT)(T);      // error

template<typename T>
using PFT2 = int (*)(T);   // OK
```

### [T.44: Use function templates to deduce class template argument types (where feasible)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t44-use-function-templates-to-deduce-class-template-argument-types-where-feasible)



## Template and hierarchy rule summary:

### [T.84: Use a non-template core implementation to provide an ABI-stable interface](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t84-use-a-non-template-core-implementation-to-provide-an-abi-stable-interface)

> NOTE: 
>
> 1、这其实就是在 `Type-Erasure` 章节中，总结的: "OOP interface + template implementation"

#### Example

It could be a base class:

```c++
struct Link_base {   // stable
    Link_base* suc;
    Link_base* pre;
};

template<typename T>   // templated wrapper to add type safety
struct Link : Link_base {
    T val;
};

struct List_base {
    Link_base* first;   // first element (if any)
    int sz;             // number of elements
    void add_front(Link_base* p);
    // ...
};

template<typename T>
class List : List_base {
public:
    void put_front(const T& e) { add_front(new Link<T>{e}); }   // implicit cast to Link_base
    T& front() { static_cast<Link<T>*>(first).val; }   // explicit cast back to Link<T>
    // ...
};C

List<int> li;
List<string> ls;
```

## Other template rules summary:

### [T.150: Check that a class matches a concept using `static_assert`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t150-check-that-a-class-matches-a-concept-using-static_assert)

> NOTE: 
>
> 1、参见 `static_assert` 章节


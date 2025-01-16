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

> NOTE: 这其实是 [object generator idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Object_Generator) 

##### Reason

Writing the template argument types explicitly can be tedious and unnecessarily verbose.

##### Example

```c++
tuple<int, string, double> t1 = {1, "Hamlet", 3.14};   // explicit type
auto t2 = make_tuple(1, "Ophelia"s, 3.14);         // better; deduced type
```

Note the use of the `s` suffix to ensure that the string is a `std::string`, rather than a C-style string.



### [T.46: Require template arguments to be at least semiregular](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t46-require-template-arguments-to-be-at-least-semiregular)

> NOTE: [std::semiregular](https://en.cppreference.com/w/cpp/concepts/semiregular) 

##### Reason

 Readability. Preventing surprises and errors. Most uses support that anyway.

##### Example

```C++
class X {
public:
    explicit X(int);
    X(const X&);            // copy
    X operator=(const X&);
    X(X&&) noexcept;        // move
    X& operator=(X&&) noexcept;
    ~X();
    // ... no more constructors ...
};

X x {1};              // fine
X y = x;              // fine
std::vector<X> v(10); // error: no default constructor
```

##### Note

Semiregular requires default constructible.

### [T.48: If your compiler does not support concepts, fake them with `enable_if`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t48-if-your-compiler-does-not-support-concepts-fake-them-with-enable_if)

##### Reason

Because that’s the best we can do without direct concept support. `enable_if` can be used to conditionally define functions and to select among a set of functions.

##### Example

```c++
template<typename T>
enable_if_t<is_integral_v<T>>
f(T v)
{
    // ...
}

// Equivalent to:
template<Integral T>
void f(T v)
{
    // ...
}
```

##### Note

Beware of [complementary constraints](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-not). Faking concept overloading using `enable_if` sometimes forces us to use that error-prone design technique.

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

## [T.concepts.def: Concept definition rules](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#tconceptsdef-concept-definition-rules)

Defining good concepts is non-trivial. 

> 翻译: 定义好的概念是非常重要的

Concepts are meant to represent fundamental concepts in an application domain (hence the name “concepts”). Similarly throwing together a set of syntactic constraints to be used for the arguments for a single class or algorithm is not what concepts were designed for and will not give the full benefits of the mechanism.

Obviously, defining concepts is most useful for code that can use an implementation (e.g., C++20 or later) but defining concepts is in itself a useful design technique and help catch conceptual errors and clean up the concepts (sic!) of an implementation.

### [T.25: Avoid complementary constraints](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t25-avoid-complementary-constraints)

##### Reason

Clarity(清晰). Maintainability(可维护). Functions with complementary requirements expressed using negation are brittle(不牢固的、脆弱的).

##### Example

Initially, people will try to define functions with complementary requirements:

```c++
template<typename T>
    requires !C<T>    // bad
void f();

template<typename T>
    requires C<T>
void f();
```

This is better:

```c++
template<typename T>   // general template
    void f();

template<typename T>   // specialization by concept
    requires C<T>
void f();
```

The compiler will choose the unconstrained template only when `C<T>` is unsatisfied. If you do not want to (or cannot) define an unconstrained version of `f()`, then delete it.

```c++
template<typename T>
void f() = delete;
```

The compiler will select the overload, or emit an appropriate error.

##### Note

Complementary constraints are unfortunately common in `enable_if` code:

```c++
template<typename T>
enable_if<!C<T>, void>   // bad
f();

template<typename T>
enable_if<C<T>, void>
f();
```

##### Note

Complementary requirements on one requirement is sometimes (wrongly) considered manageable. However, for two or more requirements the number of definitions needs can go up exponentially (2,4,8,16,…):



```
C1<T> && C2<T>
!C1<T> && C2<T>
C1<T> && !C2<T>
!C1<T> && !C2<T>
```

Now the opportunities for errors multiply.

## Other template rules summary:

### [T.150: Check that a class matches a concept using `static_assert`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t150-check-that-a-class-matches-a-concept-using-static_assert)

> NOTE: 
>
> 1、参见 `static_assert` 章节


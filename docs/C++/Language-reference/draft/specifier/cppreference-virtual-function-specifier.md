

# [virtual function specifier](https://en.cppreference.com/w/cpp/language/virtual)

The **virtual specifier** specifies that a **non-static** [member function](https://en.cppreference.com/w/cpp/language/member_functions) is *virtual* and supports **dynamic dispatch**. It may only appear in the `decl-specifier-seq` of the initial declaration of a **non-static** member function (i.e., when it is declared in the class definition).

### Explanation

**Virtual functions** are member functions whose behavior can be overridden in **derived classes**. As opposed to non-virtual functions, the overridden behavior is preserved even if there is no compile-time information about the actual type of the class. If a **derived class** is handled using **pointer** or **reference** to the base class, a call to an overridden virtual function would invoke the behavior defined in the **derived class**. This behavior is suppressed(抑制) if the function is selected using [qualified name lookup](https://en.cppreference.com/w/cpp/language/lookup) (that is, if the function's name appears to the right of the scope resolution operator `::`).

Run this code

```cpp
#include <iostream>
struct Base {
   virtual void f() {
       std::cout << "base\n";
   }
};
struct Derived : Base {
    void f() override { // 'override' is optional
        std::cout << "derived\n";
    }
};
int main()
{
    Base b;
    Derived d;
 
    // virtual function call through reference
    Base& br = b; // the type of br is Base&
    Base& dr = d; // the type of dr is Base& as  well
    br.f(); // prints "base"
    dr.f(); // prints "derived"
 
    // virtual function call through pointer
    Base* bp = &b; // the type of bp is Base*
    Base* dp = &d; // the type of dp is Base* as  well
    bp->f(); // prints "base"
    dp->f(); // prints "derived"
 
    // non-virtual function call
    br.Base::f(); // prints "base"
    dr.Base::f(); // prints "base"
}
```



### In detail

If some member function `vf` is declared as `virtual` in a class `Base`, and some class `Derived`, which is derived, directly or indirectly, from `Base`, has a declaration for member function with the same

- name
- parameter type list (but not the return type)
- cv-qualifiers
- ref-qualifiers

Then this function in the class `Derived` is also *virtual* (whether or not the keyword `virtual` is used in its declaration) and *overrides* `Base::vf` (whether or not the word `override` is used in its declaration).

`Base::vf` does not need to be **visible** (can be declared `private`, or inherited using **private inheritance**) to be overridden.

```cpp
class B {
    virtual void do_f(); // private member
 public:
    void f() { do_f(); } // public interface
};
struct D : public B {
    void do_f() override; // overrides B::do_f
};
 
int main()
{
    D d;
    B* bp = &d;
    bp->f(); // internally calls D::do_f();
}
```

For every **virtual function**, there is the *final overrider*, which is executed when a virtual function call is made（显然这和[**overload resolution**](https://en.cppreference.com/w/cpp/language/overload_resolution)有关）. A virtual member function `vf` of a base class `Base` is the **final overrider** unless the **derived class** declares or inherits (through multiple inheritance) another function that overrides `vf`.

```cpp
struct A { virtual void f(); };     // A::f is virtual
struct B : A { void f(); };         // B::f overrides A::f in B
struct C : virtual B { void f(); }; // C::f overrides A::f in C
struct D : virtual B {}; // D does not introduce an overrider, B::f is final in D
struct E : C, D  {       // E does not introduce an overrider, C::f is final in E
    using A::f; // not a function declaration, just makes A::f visible to lookup
};
int main() {
   E e;
   e.f();    // virtual call calls C::f, the final overrider in e
   e.E::f(); // non-virtual call calls A::f, which is visible in E
}
```

思考：**TODO** 那么上面的代码可以编译通过吗？

If a function has more than one final overrider, the program is ill-formed:

```cpp
struct A {
    virtual void f();
};
struct VB1 : virtual A {
    void f(); // overrides A::f
};
struct VB2 : virtual A {
    void f(); // overrides A::f
};
// struct Error : VB1, VB2 {
//     // Error: A::f has two final overriders in Error
// };
struct Okay : VB1, VB2 {
    void f(); // OK: this is the final overrider for A::f
};
struct VB1a : virtual A {}; // does not declare an overrider
struct Da : VB1a, VB2 {
    // in Da, the final overrider of A::f is VB2::f
};
```

A function with the **same name** but **different parameter list** does not override the base function of the same name, but *hides* it: when [unqualified name lookup](https://en.cppreference.com/w/cpp/language/lookup) examines the scope of the **derived class**, the lookup finds the declaration and does not examine the **base class**.

```cpp
struct B {
    virtual void f();
};
struct D : B {
    void f(int); // D::f hides B::f (wrong parameter list)
};
struct D2 : D {
    void f(); // D2::f overrides B::f (doesn't matter that it's not visible)
};
 
int main()
{
    B b;   B& b_as_b   = b;
    D d;   B& d_as_b   = d;    D& d_as_d = d;
    D2 d2; B& d2_as_b  = d2;   D& d2_as_d = d2;
 
    b_as_b.f(); // calls B::f()
    d_as_b.f(); // calls B::f()
    d2_as_b.f(); // calls D2::f()
 
    d_as_d.f(); // Error: lookup in D finds only f(int)
    d2_as_d.f(); // Error: lookup in D finds only f(int)
}
```

If a function is declared with the specifier `override`, but does not override a virtual function, the program is ill-formed:

```CPP
struct B {     
virtual void f(int); 
}; 
struct D : B {     
    virtual void f(int) override; // OK, D::f(int) overrides B::f(int)                     virtual void f(long) override; // Error: f(long) does not override B::f(int) 
};
```

If a function is declared with the specifier `final`, and another function attempts to override it, the program is ill-formed:

```CPP
struct B {     
   virtual void f() const final; 
}; 
struct D : B {     
    void f() const; // Error: D::f attempts to override final B::f 
};
```

Non-member functions and static member functions cannot be virtual.

Functions templates cannot be declared `virtual`. This applies only to functions that are themselves templates - a regular member function of a class template can be declared virtual.

| Virtual functions (whether declared virtual or overriding one) cannot have any associated constraints.`struct A {     virtual void f() requires true; // Error: constrained virtual function };`If an overriding function specifies [contract conditions](https://en.cppreference.com/w/cpp/language/attributes/contract), it must specify the same list contract conditions as the functions it overrides; no diagnostic is required if corresponding conditions will always evaluate to the same value. Otherwise, it is considered to have the list of contract conditions from one of its overriden functions; the names in the contract conditions are bound and the semantic constraints are checked at the point where the contract conditions appear.`struct A {     virtual void f(int i) [[expects: i > x]];     virtual void g(int i) [[expects: i < x]];     virtual void h(int i) [[expects: i < 0]];     int x; };   struct B : A {     virtual void f(int i); // OK, 'x' in precondition means A::x     virtual void g(int i) [[expects: i < x]]; // error     virtual void h(int i) [[expects: i < 1]]; // error     std::string x; };   struct C : A {     virtual void f(int i) [[expects: i > x]]; // OK     virtual void g(int i) [[expects: i < A::x]]; // ill-formed, no diagnostic required };`If a contract condition of a virtual function `f` odr-uses *this, the class of which `f` is a direct member must be an unambiguous and accessible base class of any class in which `f` is overridden.`struct A {     virtual void g() [[expects: x == 0]]; // odr-uses *this     int x = 42; }; struct B : A { };   struct C : A, B {     virtual void g(); //error, A is an ambiguous base };`If a function overrides more than one function, all of the overridden functions must have the same list of contract conditions; no diagnostic is required if corresponding conditions will always evaluate to the same value.`struct A {     virtual void g() [[expects: x == 0]]; // x means A::x     int x = 42; };   int x = 42; struct B {   virtual void g() [[expects: x == 0]]; // x means ::x }   struct C : A, B {     virtual void g(); //error };` | (since C++20) |
| ------------------------------------------------------------ | ------------- |
|                                                              |               |

[Default arguments](https://en.cppreference.com/w/cpp/language/default_arguments) for virtual functions are substituted at the compile time.

#### Covariant return types

If the function `Derived::f` overrides a function `Base::f`, their return types must either be the same or be *covariant*. Two types are covariant if they satisfy all of the following requirements:

- both types are pointers or references (lvalue or rvalue) to classes. Multi-level pointers or references are not allowed.
- the referenced/pointed-to class in the return type of `Base::f()` must be a unambiguous and accessible direct or indirect base class of the referenced/pointed-to class of the return type of `Derived::f()`.
- the return type of `Derived::f()` must be equally or less [cv-qualified](https://en.cppreference.com/w/cpp/language/cv) than the return type of `Base::f()`.

The class in the return type of `Derived::f` must be either `Derived` itself, or must be a [complete type](https://en.cppreference.com/w/cpp/language/incomplete_type) at the point of declaration of `Derived::f`.

When a virtual function call is made, the type returned by the final overrider is [implicitly converted](https://en.cppreference.com/w/cpp/language/implicit_cast) to the return type of the overridden function that was called:

```
class B {};
 
struct Base {
    virtual void vf1();
    virtual void vf2();
    virtual void vf3();
    virtual B* vf4();
    virtual B* vf5();
};
 
class D : private B {
    friend struct Derived; // in Derived, B is an accessible base of D
};
 
class A; // forward-declared class is an incomplete type
 
struct Derived : public Base {
    void vf1();    // virtual, overrides Base::vf1()
    void vf2(int); // non-virtual, hides Base::vf2()
//  char vf3();    // Error: overrides Base::vf3, but has different
                   // and non-covariant return type
    D* vf4();      // overrides Base::vf4() and has covariant return type
//  A* vf5();      // Error: A is incomplete type
};
 
int main()
{
    Derived d;
    Base& br = d;
    Derived& dr = d;
 
    br.vf1(); // calls Derived::vf1()
    br.vf2(); // calls Base::vf2()
//  dr.vf2(); // Error: vf2(int) hides vf2()
 
    B* p = br.vf4(); // calls Derived::vf4() and converts the result to B*
    D* q = dr.vf4(); // calls Derived::vf4() and does not convert
                     //  the result to B*
 
}
```

#### Virtual destructor

Even though destructors are not inherited, if a base class declares its destructor `virtual`, the derived destructor always overrides it. This makes it possible to delete dynamically allocated objects of polymorphic type through pointers to base.

```cpp
class Base {
 public:
    virtual ~Base() { /* releases Base's resources */ }
};
 
class Derived : public Base {
    ~Derived() { /* releases Derived's resources */ }
};
 
int main()
{
    Base* b = new Derived;
    delete b; // Makes a virtual function call to Base::~Base()
              // since it is virtual, it calls Derived::~Derived() which can
              // release resources of the derived class, and then calls
              // Base::~Base() following the usual order of destruction
}
```

Moreover, if a class is *polymorphic* (declares or inherits at least one virtual function), and its destructor is not virtual, deleting it is *undefined behavior* regardless of whether there are resources that would be leaked if the derived destructor is not invoked.

A useful guideline is that the destructor of any base class must be [public and virtual or protected and non-virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#discussion-make-base-class-destructors-public-and-virtual-or-protected-and-nonvirtual).

### During construction and destruction

When a virtual function is called directly or indirectly from a constructor or from a destructor (including during the construction or destruction of the class’s non-static data members, e.g. in a member [initializer list](https://en.cppreference.com/w/cpp/language/initializer_list)), and the object to which the call applies is the object under construction or destruction, the function called is the final overrider in the constructor’s or destructor’s class and not one overriding it in a more-derived class. In other words, during construction or destruction, the more-derived classes do not exist.

When constructing a complex class with multiple branches, within a constructor that belongs to one branch, polymorphism is restricted to that class and its bases: if it obtains a pointer or reference to a base subobject outside this subhierarchy, and attempts to invoke a virtual function call (e.g. using explicit member access), the behavior is undefined:

```cpp
struct V {
    virtual void f();
    virtual void g();
};
 
struct A : virtual V {
    virtual void f(); // A::f is the final overrider of V::f in A
};
struct B : virtual V {
    virtual void g(); // B::g is the final overrider of V::g in B
    B(V*, A*);
};
struct D : A, B {
    virtual void f(); // D::f is the final overrider of V::f in D
    virtual void g(); // D::g is the final overrider of V::g in D
 
    // note: A is initialized before B
    D() : B((A*)this, this) 
    {
    }
};
 
// the constructor of B, called from the constructor of D 
B::B(V* v, A* a)
{
    f(); // virtual call to V::f (although D has the final overrider, D doesn't exist)
    g(); // virtual call to B::g, which is the final overrider in B 
 
    v->g(); // v's type V is base of B, virtual call calls B::g as before
 
    a->f(); // a’s type A is not a base of B. it belongs to a different branch of the
            // hierarchy. Attempting a virtual call through that branch causes
            // undefined behavior even though A was already fully constructed in this
            // case (it was constructed before B since it appears before B in the list
            // of the bases of D). In practice, the virtual call to A::f will be
            // attempted using B's virtual member function table, since that's what
            // is active during B's construction)
}
```

### See also

- [derived classes and modes of inheritance](https://en.cppreference.com/w/cpp/language/derived_class)
- [override specifier](https://en.cppreference.com/w/cpp/language/override) (since C++11)
- [final specifier](https://en.cppreference.com/w/cpp/language/final) (since C++11)




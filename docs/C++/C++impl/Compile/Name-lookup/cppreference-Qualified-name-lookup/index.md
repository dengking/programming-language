# cppreference [Qualified name lookup](https://en.cppreference.com/w/cpp/language/qualified_lookup)

A *qualified* name is a name that appears on the right hand side of the **scope resolution operator** `::` (see also [qualified identifiers](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers)). A qualified name may refer to a

1、class member (including static and non-static functions, types, templates, etc)

2、namespace member (including another namespace)

3、enumerator

If there is nothing on the left hand side of the `::`, the lookup considers only declarations made in the **global namespace scope** (or introduced into the global namespace by a [using declaration](https://en.cppreference.com/w/cpp/language/namespace)). This makes it possible to refer to such names even if they were hidden by a local declaration:

> NOTE: 上面仅仅提及了 [using declaration](https://en.cppreference.com/w/cpp/language/namespace) 而没有 [using-directives](https://en.cppreference.com/w/cpp/language/namespace)；刚刚看了[using-directives](https://en.cppreference.com/w/cpp/language/namespace)的介绍，我想到的一个问题是：visibility和name lookup的关系是什么？

> NOTE: 
>
> 上面第一段话所介绍的"If there is nothing on the left hand side of the `::`"，是"Use a name in global namespace"用法，参见`C++\Language-reference\Basic-concept\Organization\Scope`章节。下面的例子非常好的体现了这个用法: 

```cpp
#include <iostream>
int main() {
  struct std{};
  std::cout << "fail\n"; // Error: unqualified lookup for 'std' finds the struct
  ::std::cout << "ok\n"; // OK: ::std finds the namespace std
}
```



Before **name lookup** can be performed for the name on the right hand side of `::`, lookup must be completed for the name on its left hand side (unless a [decltype](https://en.cppreference.com/w/cpp/language/decltype) expression is used, or there is nothing on the left). This lookup, which may be **qualified** or **unqualified**, depending on whether there's another `::` to the left of that name, considers only namespaces, class types, enumerations, and templates whose specializations are types:

> NOTE：
>
> 从这段话可以推测出，对qualified name的lookup是从左到右的。

```cpp
struct A {
  static int n;
};
int main() {
  int A;
  A::n = 42;    // OK: unqualified lookup of A to the left of :: ignores the variable
  A b;          // Error: unqualified lookup of A finds the variable A
}
```

When a **qualified name** is used as a [declarator](https://en.cppreference.com/w/cpp/language/declarations), then [unqualified lookup](https://en.cppreference.com/w/cpp/language/unqualified_lookup) of the names used in the same declarator that follow that qualified name, but not the names that precede it, is performed in the scope of the member's class or namespace:

> NOTE：这段话描述的是下面的`C::X C::arr[number], brr[number];`。

```cpp
class X { };
constexpr int number = 100;
struct C {
  class X { };
  static const int number = 50;
  static X arr[number];
};
X C::arr[number], brr[number];    // Error: look up for X finds ::X, not C::X
C::X C::arr[number], brr[number]; // OK: size of arr is 50, size of brr is 100
```

If `::` is followed by the character `~` that is in turn followed by an identifier (that is, it specifies a destructor or pseudo-destructor), that identifier is looked up in the same scope as the name on the left hand side of `::`

```cpp
struct C { typedef int I; };
typedef int I1, I2;
extern int *p, *q;
struct A { ~A(); };
typedef A AB;
int main() {
  p->C::I::~I(); // the name I after ~ is looked up in the same scope as I before ::
                 // (that is, within the scope of C, so it finds C::I)
  q->I1::~I2();  // The name I2 is looked up in the same scope as I1
                 // that is, from the current scope, so it finds ::I2
  AB x;
  x.AB::~AB();   // The name AB after ~ is looked up in the same scope as AB before ::
                 // that is, from the current scope, so it finds ::AB
}
```

## Enumerators

If the lookup of the left-hand side name comes up with an [enumeration](https://en.cppreference.com/w/cpp/language/enum) (either scoped or unscoped), the lookup of the right-hand side must result in an enumerator that belongs that enumeration, otherwise the program is ill-formed.

## Class members

If the lookup of the **left** hand side name comes up with a `class`/`struct` or `union` name, the name on the **right** hand side of `::` is looked up in the scope of that class (and so may find a declaration of a member of that class or of its base), with the following exceptions

1、A destructor is looked up as described above (in the scope of the name to the left of ::)

2、A conversion-type-id in a [user-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator) function name is first looked up in the scope of the class. If not found, the name is then looked up in the **current scope**. The conversion-type-id must denote the same type in both scopes.

3、names used in **template arguments** are looked up in the **current scope** (not in the scope of the template name)

4、names in [using-declarations](https://en.cppreference.com/w/cpp/language/namespace) also consider `class`/`enum` names that are hidden by the name of a variable, data member, function, or enumerator declared in the same scope

If the **right** hand side of `::` names the same class as the **left** hand side, the name designates the [constructor](https://en.cppreference.com/w/cpp/language/constructor) of that class. Such **qualified name** can only be used in a declaration of a **constructor** and in the [using-declaration](https://en.cppreference.com/w/cpp/language/using_declaration) for an [inheriting constructor](https://en.cppreference.com/w/cpp/language/using_declaration#Inheriting_constructors). In those lookups where function names are ignored (that is, when looking up a name on the left of `::`, when looking up a name in [elaborated type specifier](https://en.cppreference.com/w/cpp/language/elaborated_type_specifier), or [base specifier](https://en.cppreference.com/w/cpp/language/derived_class)), the same syntax resolves to the injected-class-name:

```cpp
struct A { A(); };
struct B : A { B(); };
A::A() { } // A::A names a constructor, used in a declaration
B::B() { } // B::B names a constructor, used in a declaration
B::A ba;   // B::A names the type A (looked up in the scope of B)
A::A a;    // Error, A::A does not name a type
 
struct A::A a2; // OK: lookup in elaborated type specifier ignores functions
                // so A::A simply names the class A as seen from within the scope of A
                // (that is, the injected-class-name)
```

**Qualified name lookup** can be used to access a class member that is hidden by a nested declaration or by a derived class. A call to a qualified member function is never virtual

```cpp
struct B { virtual void foo(); };
struct D : B { void foo() override; };
int main()
{
    D x;
    B& b = x;
    b.foo(); // calls D::foo (virtual dispatch)
    b.B::foo(); // calls B::foo (static dispatch)
}
```

## Namespace members

If the name on the **left** of `::` refers to a namespace or if there is nothing on the left of `::` (in which case it refers to the **global namespace**), the name that appears on the **right** hand side of `::` is looked up in the scope of that namespace, except that

- names used in **template arguments** are looked up in the current scope

```cpp
namespace N {
   template<typename T> struct foo {};
   struct X {};
}
N::foo<X> x; // error: X is looked up as ::X, not as N::X
```

**Qualified lookup** within the scope of a [namespace](https://en.cppreference.com/w/cpp/language/namespace) `N` first considers all declarations that are located in `N` and all declarations that are located in the [inline namespace members](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces) of `N` (and, transitively, in their inline namespace members). If there are no declarations in that set then it considers declarations in all namespaces named by [using-directives](https://en.cppreference.com/w/cpp/language/namespace#Using-directives) found in `N` and in all transitive inline namespace members of `N`. The rules are applied recursively:

```cpp
int x;
namespace Y {
  void f(float);
  void h(int);
}
namespace Z {
  void h(double);
}
namespace A {
  using namespace Y;
  void f(int);
  void g(int);
  int i;
}
namespace B {
  using namespace Z;
  void f(char);
  int i;
}
namespace AB {
  using namespace A;
  using namespace B;
  void g();
}
void h()
{
  AB::g();  // AB is searched, AB::g found by lookup and is chosen AB::g(void)
            // (A and B are not searched)
  AB::f(1); // First, AB is searched, there is no f
            // Then, A, B are searched
            // A::f, B::f found by lookup (but Y is not searched so Y::f is not considered)
            // overload resolution picks A::f(int)
  AB::x++;    // First, AB is searched, there is no x
              // Then A, B are searched. There is no x
              // Then Y and Z are searched. There is still no x: this is an error
  AB::i++;  // AB is searched, there is no i
            // Then A, B are searched. A::i and B::i found by lookup: this is an error
  AB::h(16.8);  // First, AB is searched: there is no h
                // Then A, B are searched. There is no h
                // Then Y and Z are searched.
                // lookup finds Y::h and Z::h. Overload resolution picks Z::h(double)
}
```

It is allowed for the same declaration to be found more than once:

```cpp
namespace A { int a; }
namespace B { using namespace A; }
namespace D { using A::a; }
namespace BD {
  using namespace B;
  using namespace D;
}
void g()
{
  BD::a++; // OK: finds the same A::a through B and through D
}
```


# [Namespaces](https://en.cppreference.com/w/cpp/language/namespace) 

**Namespaces** provide a method for preventing name conflicts in large projects.

Symbols declared inside a **namespace** block are placed in a **named scope** that prevents them from being mistaken for identically-named symbols in other **scopes**.

Multiple namespace blocks with the same name are allowed. All declarations within those blocks are declared in the named scope.

Syntax

---

`namespace ns_name { declarations }`	(1)	

`inline namespace ns_name { declarations }`	(2)	(since C++11)

`namespace { declarations }`	(3)	

`ns_name::name`	(4)	

`using namespace ns_name;`	(5)	

`using ns_name::name;`	(6)	

`namespace name = qualified-namespace ;`	(7)	

`namespace ns_name::name`	(8)	(since C++17)

---

1) [Named namespace definition](https://en.cppreference.com/w/cpp/language/namespace#Namespaces) for the namespace `ns_name`.

2) [Inline namespace definition](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces) for the namespace `ns_name`. Declarations inside `ns_name` will be visible in its enclosing namespace(在包括`ns_name`的namespace中，`ns_name`中的所有declaration都可见).

3) [Unnamed namespace definition](https://en.cppreference.com/w/cpp/language/namespace#Unnamed_namespaces). Its members have potential scope from their point of declaration to the end of the translation unit, and have [internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration).

4) Namespace names (along with class names) can appear on the left hand side of the **scope resolution operator**, as part of [qualified name lookup](https://en.cppreference.com/w/cpp/language/lookup).

5) [using-directive](https://en.cppreference.com/w/cpp/language/namespace#Using-directives): From the point of view of unqualified [name lookup](https://en.cppreference.com/w/cpp/language/lookup) of any name after a using-directive and until the end of the scope in which it appears, every name from `ns_name` is visible as if it were declared in the nearest enclosing namespace which contains both the using-directive and `ns_name`.

6) [using-declaration](https://en.cppreference.com/w/cpp/language/namespace#Using-declarations): makes the symbol name from the namespace `ns_name` accessible for [unqualified lookup](https://en.cppreference.com/w/cpp/language/lookup) as if declared in the same class scope, block scope, or namespace as where this using-declaration appears.

7) namespace-alias-definition: makes name a synonym for another namespace: see [namespace alias](https://en.cppreference.com/w/cpp/language/namespace_alias)

8) nested namespace definition: `namespace A::B::C { ... }` is equivalent to `namespace A { namespace B { namespace C { ... } } }`.

总结：需要总结using-directive和using-declaration的异同所在。

- using-directive是visible，using-declaration是accessible
- using-declaration从名称来看，是declaration

### Explanation

#### Namespaces H4

```
inline(optional) namespace attr(optional) identifier { namespace-body }
```

**Namespace definitions** are only allowed at namespace scope, including the **global scope**.

总结：也就是不能够在function，class中定义namespace。看了下面的例子就知道global scope的含义了。

To reopen an existing namespace (formally, to be an ***extension-namespace-definition***), the lookup for the identifier used in the namespace definition must resolve to a **namespace name** (not a **namespace alias**), that was declared as a member of the **enclosing namespace** or of an **inline namespace** within an enclosing namespace.

总结：reopen一个已存在的namespace的正式说法是*extension-namespace-definition*，即扩展namespace定义。

The namespace-body defines a [namespace scope](https://en.cppreference.com/w/cpp/language/scope), which affects [name lookup](https://en.cppreference.com/w/cpp/language/lookup).

总结：scope的概念在任何语言中都是存在的，那c++中的scope是如何设计的呢？

All **names** introduced by the declarations that appear within namespace-body (including **nested namespace definitions**) become members of the namespace identifier, whether this **namespace definition** is the **original namespace definition** (which introduced identifier), or an **extension namespace definition** (which "reopened" the already defined namespace)

总结：参看下面的例子就可以完全理解 **extension namespace definition**的含义

A namespace member that was declared within a namespace body may be defined or redeclared outside of it using **explicit qualification**



```cpp
namespace Q {
  namespace V { // V is a member of Q, and is fully defined within Q
// namespace Q::V { // C++17 alternative to the above two lines
    class C { void m(); }; // C is a member of V and is fully defined within V
                           // C::m is only declared
    void f(); // f is a member of V, but is only declared here
  }
  void V::f() // definition of V's member f outside of V
              // f's enclosing namespaces are still the global namespace, Q, and Q::V
  {
      extern void h(); // This declares ::Q::V::h
  }
  void V::C::m() // definition of V::C::m outside of the namespace (and the class body)
                 // enclosing namespaces are the global namespace, Q, and Q::V
  {
  }
}
```

**Out-of-namespace definitions** and **redeclarations** are only allowed after the point of declaration, only at **namespace scope**, and only in namespaces that enclose the original namespace (including the **global namespace**) and they must use **qualified-id syntax** (since C++14)

```cpp
namespace Q {
  namespace V { // original-namespace-definition for V
    void f(); // declaration of Q::V::f
  }
  void V::f() {} // OK
  void V::g() {} // Error: g() is not yet a member of V
  namespace V { // extension-namespace-definition for V
    void g(); // declaration of Q::V::g
  }
}
namespace R { // not a enclosing namespace for Q
   void Q::V::g() {} // Error: cannot define Q::V::g inside R
}
void Q::V::g() {} // OK: global namespace encloses Q
```

Names introduced by [friend](https://en.cppreference.com/w/cpp/language/friend) declarations within a **non-local class** X become members of the innermost enclosing namespace of X, but they do not become visible to ordinary [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (neither [unqualified](https://en.cppreference.com/w/cpp/language/unqualified_lookup) nor [qualified](https://en.cppreference.com/w/cpp/language/qualified_lookup)) unless a matching declaration is provided at namespace scope, either before or after the class definition. Such name may be found through [ADL](https://en.cppreference.com/w/cpp/language/adl) which considers both **namespaces** and **classes**.

Only the innermost enclosing namespace is considered by such friend declaration when deciding whether the name would conflict with a previously declared name.

```cpp
void h(int);
namespace A {
  class X {
    friend void f(X); // A::f is a friend
    class Y {
        friend void g(); // A::g is a friend
        friend void h(int); // A::h is a friend, no conflict with ::h
    };
  };
  // A::f, A::g and A::h are not visible at namespace scope
  // even though they are members of the namespace A
  X x;
  void g() {  // definition of A::g
     f(x); // A::X::f is found through ADL
  }
  void f(X) {}       // definition of A::f
  void h(int) {}     // definition of A::h
  // A::f, A::g and A::h are now visible at namespace scope
  // and they are also friends of A::X and A::X::Y
}
```

#### Inline namespaces H4

未阅读

#### Unnamed namespaces H4

未阅读

#### Using-declarations H4

Introduces a name that is defined elsewhere into the **declarative region** where this **using-declaration** appears.

---

`using typename(optional) nested-name-specifier unqualified-id ;`		(until C++17)

---

`using declarator-list ;`		(since C++17)

---
| -                     | -    | -                                                            |
| --------------------- | ---- | ------------------------------------------------------------ |
| nested-name-specifier | -    | a sequence of names and scope resolution operators `::`, ending with a **scope resolution operator**. A single `::` refers to the **global namespace**. |
| unqualified-id        | -    | an [id-expression](https://en.cppreference.com/w/cpp/language/identifiers) |
| typename              | -    | the keyword `typename` may be used as necessary to resolve [dependent names](https://en.cppreference.com/w/cpp/language/dependent_name), when the using-declaration introduces a member type from a base class into a class template |
| declarator-list       | -    | comma-separated list of one or more **declarators** of the form `**typename**`(optional)nested-name-specifier unqualified-id. The last declarator may be an ellipsis, although that form is only meaningful in [derived class definitions](https://en.cppreference.com/w/cpp/language/using_declaration) |

**Using-declarations** can be used to introduce **namespace members** into other namespaces and block scopes, or to introduce **base class members** into **derived class definitions**.

思考：derived class肯定会继承base class members，那为什么还要通过**using-declaration**来将base class members引入到derived class。

A **using-declaration** with more than one **using-declarator** is equivalent to a corresponding sequence of using-declarations with one using-declarator. (since C++17)

For the use in **derived class definitions**, see [using declaration](https://en.cppreference.com/w/cpp/language/using_declaration).

Names introduced into a namespace scope by a **using-declaration** can be used just like any other names, including **qualified lookup** from other scopes:

```cpp
void f();
namespace A {
    void g();
}
namespace X {
    using ::f; // global f is now visible as ::X::f
    using A::g; // A::g is now visible as ::X::g
    using A::g, A::g; // (C++17) OK: double declaration allowed at namespace scope
}
void h()
{
    X::f(); // calls ::f
    X::g(); // calls A::g
}
```

If, after the **using-declaration** was used to take a member from a namespace, the namespace is extended and **additional declarations** for the same name are introduced, those **additional declarations** do not become visible through the **using-declaration** (in contrast with **using-directive**). One exception is when a **using-declaration** names a class template: partial specializations introduced later are effectively visible, because their [lookup](https://en.cppreference.com/w/cpp/language/lookup) proceeds through the primary template.

总结：结合下面的这个例子来理解上面这段话就可以了

```cpp
namespace A {
    void f(int);
}
using A::f; // ::f is now a synonym for A::f(int)
 
namespace A { // namespace extension
   void f(char); // does not change what ::f means
}
void foo() {
    f('a'); // calls f(int), even though f(char) exists.
} 
void bar() {
   using A::f; // this f is a synonym for both A::f(int) and A::f(char)
   f('a'); // calls f(char)
}
```

**Using-declarations** cannot name [template-id](https://en.cppreference.com/w/cpp/language/templates#template-id), namespace, or a scoped enumerator. Each declarator in a **using-declaration** introduces one and only one name, for example **using-declaration** for an [enumeration](https://en.cppreference.com/w/cpp/language/enum) does not introduce any of its enumerators.

All restrictions on regular **declarations** of the same names, hiding, and overloading rules apply to **using-declarations**:

```cpp
namespace A {
    int x;
}
namespace B {
    int i;
    struct g { };
    struct x { };
    void f(int);
    void f(double);
    void g(char); // OK: function name g hides struct g
}
void func() {
    int i;
    using B::i; // error: i declared twice
 
    void f(char);
    using B::f; // OK: f(char), f(int), f(double) are overloads
    f(3.5); // calls B::f(double)
 
    using B::g;
    g('a');      // calls B::g(char)
    struct g g1; // declares g1 to have type struct B::g
 
    using B::x;
    using A::x;  // OK: hides struct B::x
    x = 99;      // assigns to A::x
    struct x x1; // declares x1 to have type struct B::x
}
```

思考：什么时候hide什么时候error呢？

注意：上述`B::f`仅仅声明了，而没有定义。

If a function was introduced by a **using-declaration**, declaring a function with the same name and parameter list is **ill-formed** (unless the declaration is for the same function). If a function template was introduced by a **using-declaration**, declaring a function template with the same name, parameter type list, return type, and template parameter list is **ill-formed**. Two using-declarations can introduce functions with the same name and parameter list, but if a call to that function is attempted, the program is **ill-formed**.

总结：结合下面的这个例子来理解上面这段话就可以了

```cpp
namespace B {
    void f(int);
    void f(double);
}
namespace C {
    void f(int);
    void f(double);
    void f(char);
}
void h() {
    using B::f; // introduces B::f(int), B::f(double)
    using C::f; // introduces C::f(int), C::f(double), and C::f(char)
    f('h');      // calls C::f(char)
    f(1);        // error: B::f(int) or C::f(int)?
    void f(int); // error: f(int) conflicts with C::f(int) and B::f(int)
}
```

If an entity is declared, but not defined in some inner namespace, and then declared through **using-declaration** in the outer namespace, and then a definition appears in the outer namespace with the same unqualified name, that definition is a member of the outer namespace and conflicts with the using-declration:

```cpp
namespace X {
  namespace M {
     void g(); // declares, but doesn't define X::M::g()
  }
  using M::g;
  void g();   // Error: attempt to declare X::g which conflicts with X::M::g()
}
```

More generally, a declaration that appears in any namespace scope and introduces a name using an **unqualified** identifier always introduces a member into the namespace it's in and not to any other namespace. The exceptions are explicit instantiations and explicit specializations of a **primary template** that is defined in an inline namespace: because they do not introduce a new name, they may use unqualified-id in an enclosing namespace.

#### Using-directives H4

A *using-directive* is a [block-declaration](https://en.cppreference.com/w/cpp/language/declarations) with the following syntax:

---

`attr(optional) using namespace nested-name-specifier(optional) namespace-name ;	`

---
| -                     | -    | -                                                            |
| --------------------- | ---- | ------------------------------------------------------------ |
| attr(C++11)           | -    | any number of [attributes](https://en.cppreference.com/w/cpp/language/attributes) that apply to this using-directive |
| nested-name-specifier | -    | a sequence of names and scope resolution operators `::`, ending with a scope resolution operator. A single `::` refers to the **global namespace**. |
| namespace-name        | -    | a name of a namespace. When looking up this name, [lookup](https://en.cppreference.com/w/cpp/language/lookup) considers namespace declarations only |

**Using-directives** are allowed only in namespace [scope](https://en.cppreference.com/w/cpp/language/scope) and in **block scope**. From the point of view of unqualified [name lookup](https://en.cppreference.com/w/cpp/language/lookup) of any name after a **using-directive** and until the end of the scope in which it appears, every name from namespace-name is visible as if it were declared in the nearest enclosing namespace which contains both the **using-directive** and **namespace-name**.

**Using-directive** does not add any names to the declarative region in which it appears (unlike the **using-declaration**), and thus does not prevent identical names from being declared(**using-directive**不会向它出现的声明区域添加任何名称（与using-declaration不同），因此不会阻止声明相同的名称).

**Using-directives** are transitive for the purposes of [unqualified lookup](https://en.cppreference.com/w/cpp/language/lookup): if a scope contains a **using-directive** that nominates(提名) a namespace-name, which itself contains **using-directive** for some namespace-name-2, the effect is as if the **using directives** from the second namespace appear within the first. The order in which these transitive namespaces occur does not influence name lookup.

总结：此处关于transitive 的介绍需要结合下面的例子来进行理解

```cpp
namespace A {
    int i;
}
namespace B {
    int i;
    int j;
    namespace C {
        namespace D {
            using namespace A; // all names from A injected into global namespace
            int j;
            int k;
            int a = i; // i is B::i, because A::i is hidden by B::i
        }
        using namespace D; // names from D are injected into C
                           // names from A are injected into global namespace
        int k = 89; // OK to declare name identical to one introduced by a using
        int l = k;  // ambiguous: C::k or D::k
        int m = i;  // ok: B::i hides A::i
        int n = j;  // ok: D::j hides B::j
    }
}
```

If, after a **using-directive** was used to nominate some namespace, the namespace is extended and additional members and/or **using-directives** are added to it, those additional members and the additional namespaces are visible through the using-directive (in contrast with using-declaration)

```cpp
namespace D {
   int d1;
   void f(char);
}
using namespace D; // introduces D::d1, D::f, D::d2, D::f,
                   //  E::e, and E::f into global namespace!
 
int d1; // OK: no conflict with D::d1 when declaring
namespace E {
    int e;
    void f(int);
}
namespace D { // namespace extension
    int d2;
    using namespace E; // transitive using-directive
    void f(int);
}
void f() {
    d1++; // error: ambiguous ::d1 or D::d1?
    ::d1++; // OK
    D::d1++; // OK
    d2++; // OK, d2 is D::d2
    e++; // OK: e is E::e due to transitive using
    f(1); // error: ambiguous: D::f(int) or E::f(int)?
    f('a'); // OK: the only f(char) is D::f(char)
}
```

### Notes H3

The using-directive `using namespace std;` at any namespace scope introduces every name from the namespace `std`into the **global namespace** (since the **global namespace** is the nearest namespace that contains both `std` and any user-declared namespace), which may lead to undesirable name collisions. This, and other using directives are generally considered bad practice at file scope of a header file.

### Example

This example shows how to use a namespace to create a class that already has been named in the `std` namespace.

```cpp
#include <vector>
 
namespace vec {
 
    template< typename T >
    class vector {
        // ...
    };
 
} // of vec
 
int main()
{
    std::vector<int> v1; // Standard vector.
    vec::vector<int> v2; // User defined vector.
 
    v1 = v2; // Error: v1 and v2 are different object's type.
 
    {
        using namespace std;
        vector<int> v3; // Same as std::vector
        v1 = v3; // OK
    }
 
    {
        using vec::vector;
        vector<int> v4; // Same as vec::vector
        v2 = v4; // OK
    }
 
    return 0;
}
```

# c++编译错误 H1
## undefined reference to `B::f(double)' H2
代码如下：
```
namespace A {
    int x;
}

namespace B {
    int i;
    struct g { };
    struct x { };
    void f(int);
    void f(double);
    void g(char); 
}

void func() {
    int i;
    //using B::i; 
 
    void f(char);
    using B::f; 
    f(3.5); 
 
    using B::g;
    g('a');      
    struct g g1; 
 
    using B::x;
    using A::x;  
    x = 99;      
    struct x x1; 
}

int main()
{
    func();
}

```

报错信息如下：
```
/tmp/ccak91SA.o: In function `func()':
using-declaration.cpp:(.text+0x1c): undefined reference to `B::f(double)'
using-declaration.cpp:(.text+0x26): undefined reference to `B::g(char)'
collect2: error: ld returned 1 exit status
```
错误的原因是`B::f(double)`和`B::g(char)`只声明而没有定义，这就导致了函数`func`中对它的调用会失败。

引起我的好奇的是上述报错信息中的`.text+0x1c`和`.text+0x26`，显然它们是正文段。
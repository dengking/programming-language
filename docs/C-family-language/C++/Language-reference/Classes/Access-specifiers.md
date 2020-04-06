

# [access specifiers](https://en.cppreference.com/w/cpp/language/access)

In a **member-specification** of a [class/struct](https://en.cppreference.com/w/cpp/language/class) or [union](https://en.cppreference.com/w/cpp/language/union), define the accessibility of subsequent（后续的） members.

In a **base-specifier** of a [derived class](https://en.cppreference.com/w/cpp/language/derived_class) declaration, define the accessibility of inherited members of the subsequent **base class**.

思考：declaration和specification的区别和联系

### Syntax



1) The members declared after the specifier have **public member access**

2) The members declared after the specifier have **protected member access**

3) The members declared after the specifier have **private member access**

4) [Public inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Public_inheritance): the public and protected members of the [base class](https://en.cppreference.com/w/cpp/language/derived_class) listed after the specifier keep their member access in the derived class(基类的public和protected members在子类中保持其member access)

5) [Protected inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Protected_inheritance): the public and protected members of the [base class](https://en.cppreference.com/w/cpp/language/derived_class) listed after the specifier are **protected** members of the derived class

6) [Private inheritance](https://en.cppreference.com/w/cpp/language/derived_class#Private_inheritance): the public and protected members of the [base class](https://en.cppreference.com/w/cpp/language/derived_class) listed after the specifier are **private members** of the derived class

总结：显然 private member of base class在子类中是无法被访问的；显然，access specifier能够控制两个方面：

- 用户访问member的权限
- 基类成员在子类中的访问权限

思考：base class的private member在derived class中能否被访问？

### Explanation

The name of every [class](https://en.cppreference.com/w/cpp/language/class) member (static, non-static, function, type, etc) has an associated "member access". When a name of the member is used anywhere a program, its access is checked, and if it does not satisfy the access rules, the program does not compile:

总结：显然，member access check是 发生在编译期的

Run this code

```c++
#include <iostream>
class Example {
 public: // all declarations after this point are public
    void add(int x) { // member "add" has public access
        n += x; // OK: private Example::n can be accessed from Example::add
    }
 private: // all declarations after this point are private
    int n = 0; // member "n" has private access
};
int main()
{
    Example e;
    e.add(1); // OK: public Example::add can be accessed from main
//  e.n = 7;  // Error: private Example::n cannot be accessed from main
}
```



**Access specifiers** give the author of the class the ability to decide which class members are accessible to the users of the class (that is, the *interface*) and which members are for internal use of the class (the *implementation*)

总结：这段话给出了c++中提供access specifier的目的

### In detail

总结：下面这段主要讲解的是access rule，需要注意的是，下面的论述其实包括了class member access和non class两种情况。

All members of a class (bodies of [member functions](https://en.cppreference.com/w/cpp/language/member_functions), initializers of member objects, and the entire [nested class definitions](https://en.cppreference.com/w/cpp/language/nested_types)) have access to all the names to which a class can access（一个类的所有成员能够访问所有给类能够访问的names). A **local class** within a member function has access to all the names the **member function** itself can access.

A **class** defined with the keyword `class` has **private access** for its members and its **base classes** by default. A **class** defined with the keyword `struct` has **public access** for its members and its base classes by default. A [union](https://en.cppreference.com/w/cpp/language/union) has **public access** for its members by default.

总结：上面这段话是比较概括性的，它的含义是：无论是成员函数，还是继承，在不指定access specifier的时候，`class`的默认权限是`private`，`struct`的默认权限是`public`。

To grant access to additional functions or classes to `protected` or `private` members, a [friendship declaration](https://en.cppreference.com/w/cpp/language/friend) may be used.

总结：上面这段话的意思是：通过[friendship declaration](https://en.cppreference.com/w/cpp/language/friend) 可以将一个类的 `protected` or `private` members 的access授予另外的function和class。

Accessibility applies to all names with no regard to their origin, so a name introduced by a [typedef](https://en.cppreference.com/w/cpp/language/typedef) or [using declarations](https://en.cppreference.com/w/cpp/language/using_declaration) is checked, not the name it refers to.

上面这段话的意思是：Accessibility检查仅仅作用于names，而无需考虑这些names的来源，因此检查由`typedef`或 [using declarations](https://en.cppreference.com/w/cpp/language/using_declaration) 引入的名称，而不是它所引用的名称。

```c++
class A : X {
  class B { }; // B is private in A
public:
  typedef B BB; // BB is public
};
void f() {
  A::B y; // error, A::B is private
  A::BB x; // OK, A::BB is public
}
```

总结：上面这段代码中，需要注意的是class B是class A的member，由于author of class没有指定access specifier，所以它的access默认是private。

**Member access** does not affect **visibility**: 

- names of private and privately-inherited members are visible and considered by [**overload resolution**](https://en.cppreference.com/w/cpp/language/overload_resolution), 

- implicit conversions to inaccessible base classes are still considered, etc. 

总结：这一段描述了member access和visibility的关系。

**Member access check** is the last step after any given language construct is interpreted. The intent of this rule is that replacing any `private` with `public` never alters the behavior of the program.

**Access checking** for the names used in [default function arguments](https://en.cppreference.com/w/cpp/language/default_arguments)(函数默认参数) as well as in the default [template parameters](https://en.cppreference.com/w/cpp/language/template_parameters#Default_template_arguments) is performed at the point of declaration, not at the point of use.

Access rules for the names of [virtual functions](https://en.cppreference.com/w/cpp/language/virtual) are checked at the call point using the type of the expression used to denote the object for which the member function is called. The access of the **final overrider** is ignored.

总结：需要注意的是这三段话描述的是access check发生的时机。第一段话和第二段话分别描述了**Member access check**和**Access checking**，显然，前者是关于class的，而后者则为non class。

第三段话的理解需要结合下面的这个例子来，并且其中还包括一些不容易理解的名词，比如，final overrider，参见下面的virtual function specifier章节即可知。变量`b`的static type是`B`，其dynamic type是`D`，第三段话的意思是对于virtual functions的access check是通过其调用者的static type来进行的，所以对变量`b`而言，其成员`f()`是public的，`b`的成员`f()`的final overrider是定义在其dynamic type `D`中的成员`f()`，所以 `b.f()`最终调用的是其dynamic type `D`中的成员`f()`。显然在这种情况下，The access of the **final overrider** is ignored。

```c++
struct B { virtual int f(); }; // f is public in B
class D : public B { private: int f(); }; // f is private in D
void f() {
 D d;
 B& b = d;
 b.f(); // OK: B::f() is public, D::f() is invoked even though it's private
 d.f(); // error: D::f() is private
}
```

A name that is private according to unqualified [name lookup](https://en.cppreference.com/w/cpp/language/lookup), may be accessible through qualified name lookup:

```c++
class A { };
class B : private A { };
class C : public B {
   A* p; // error: unqualified name lookup finds A as the private base of B
   ::A* q; // OK, qualified name lookup finds the namespace-level declaration
};
```

A name that is accessible through multiple paths in the inheritance graph has the access of the path with the most access:

```c++
class W { public: void f(); };
class A : private virtual W { };
class B : public virtual W { };
class C : public A, public B {
void f() { W::f(); } // OK, W is accessible to C through B
};
```

Any number of access specifiers may appear within a class, in any order. Member access specifiers may affect class layout: the addresses of non-static [data members](https://en.cppreference.com/w/cpp/language/data_members#Layout) are only guaranteed to increase in order of declaration for the members with the same access. For [StandardLayoutType](https://en.cppreference.com/w/cpp/named_req/StandardLayoutType), all non-static data members must have the same access.

When a member is redeclared within the same class, it must do so under the same member access:

```c++
struct S {
  class A; // S::A is public
private:
  class A {}; // error: cannot change access
};
```

### Public member access

Public members form a part of the public interface of the class (other parts of the public interface are the non-member functions found by [ADL](https://en.cppreference.com/w/cpp/language/adl)).

A public member of a class is accessible everywhere.

```c++
class S {
 public: // n, f, E, A, B, C, U are public members
    int n;
    static void f() {}
    enum E {A, B, C};
    struct U {};
};
int main()
{
    S::f(); // S::f is accessible in main
    S s;
    s.n = S::B; // S::n and S::B are accesisble in main
    S::U x; // S::U is accessible in main
}
```

### Protected member access

Protected members form the interface for the derived classes (which is distinct from the public interface of the class).

A protected member of a class `Base` can only be accessed

1) by the members and friends of `Base`

2) by the members and friends (until C++17) of any class derived from `Base`, but only when operating on an object of a type that is derived from `Base` (including `this`)

```
struct Base {
 protected:
    int i;
 private:
    void g(Base& b, struct Derived& d);
};
 
struct Derived : Base {
    void f(Base& b, Derived& d) // member function of a derived class
    {
        ++d.i; // okay: the type of d is Derived
        ++i; // okay: the type of the implied '*this' is Derived
//      ++b.i; // error: can't access a protected member through Base
//                 (Otherwise it would be possible to change other derived classes, 
//                  like a hypothetical Derived2, base implementation)
    }
};
 
void Base::g(Base& b, Derived& d) // member function of Base
{
    ++i; // okay
    ++b.i; // okay
    ++d.i; // okay
}
 
void x(Base& b, Derived& d) // non-member non-friend
{
//    ++b.i; // error: no access from non-member
//    ++d.i; // error: no access from non-member
}
```

When a pointer to a protected member is formed, it must use a derived class in its declaration:

```
struct Base {
 protected:
    int i;
};
 
struct Derived : Base {
    void f()
    {
//      int Base::* ptr = &Base::i;    // error: must name using Derived
        int Base::* ptr = &Derived::i; // okay
    }
};
```

### Private member access

Private members form the implementation of a class, as well as the private interface for the other members of the class.

A private member of a class can only be accessed by the members and friends of that class, regardless of whether the members are on the same or different instances:

```
class S {
 private:
    int n; // S::n is private
 public:
    S() : n(10) {} // this->n is accessible in S::S
    S(const S& other) : n(other.n) {} // other.n is accessible in S::S
};
```

The [explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast) (C-style and function-style) allows casting from a derived lvalue to reference to its private base, or from a pointer to derived to a pointer to its private base.

### Inheritance

See [derived classes](https://en.cppreference.com/w/cpp/language/derived_class) for the meaning of public, protected, and private inheritance.

# [What are POD types in C++?](https://stackoverflow.com/questions/146452/what-are-pod-types-in-c)


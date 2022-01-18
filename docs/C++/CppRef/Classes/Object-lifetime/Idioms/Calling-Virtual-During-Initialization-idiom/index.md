# Calling Virtuals During Initialization

## stackoverflow [Calling virtual functions inside constructors](https://stackoverflow.com/questions/962132/calling-virtual-functions-inside-constructors)

### [A](https://stackoverflow.com/a/962148)

Calling virtual functions from a constructor or destructor is dangerous and should be avoided whenever possible. All C++ implementations should call the version of the function defined at the level of the hierarchy in the current constructor and no further.

The [C++ FAQ Lite](https://isocpp.org/wiki/faq/strange-inheritance#calling-virtuals-from-ctors) covers this in section 23.7 in pretty good detail. I suggest reading that (and the rest of the FAQ) for a followup.

Excerpt:

> [...] In a constructor, the virtual call mechanism is disabled because overriding from derived classes hasn’t yet happened. Objects are constructed from the base up, “base before derived”.
>
> [...]
>
> Destruction is done “derived class before base class”, so virtual functions behave as in constructors: Only the local definitions are used – and no calls are made to overriding functions to avoid touching the (now destroyed) derived class part of the object.

**EDIT** Corrected Most to All (thanks litb)



## 补充

在cppreference [Lifetime#Access outside of lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Access_outside_of_lifetime)章节中，对这种行为进行了专门说明: 

> [virtual function calls during construction and destruction](https://en.cppreference.com/w/cpp/language/virtual#During_construction_and_destruction)

C++标准中将其定义为undefined behavior。



## More C++ Idioms/[Calling Virtuals During Initialization](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Calling_Virtuals_During_Initialization)

### Intent

Simulate calling of virtual functions during object initialization.

### Motivation

Sometimes it is desirable to invoke **virtual functions** of **derived classes** while a derived object is being initialized. Language rules explicitly prohibit（禁止） this from happening because calling member functions of derived object before derived part of the object is initialized is dangerous. It is not a problem if the **virtual function** does not access data members of the object being constructed. But there is no general way of ensuring it.

```c++
#include<iostream>

class Base {
 public:
   Base()
   {
       this->foo(42);
       this->bar();
   // these will not use dynamic binding
   // goal: simulate dynamic binding in those calls
   }
   virtual void foo(int n) const // often pure virtual
   {
       std::cout<<"Base::foo"<<std::endl;
   }
   virtual double bar() const    // often pure virtual
   {
       std::cout<<"Base::bar"<<std::endl;       
   }
 };
 
 
 class Derived : public Base {
 public:
   virtual void foo(int n) const
   {
       std::cout<<"Derived::foo"<<std::endl;       
   }
   virtual double bar() const
   {
       std::cout<<"Derived::bar"<<std::endl;       
   }
 };

int main()
{
    Derived d;
}
```

> NOTE: 上述程序运行如下：
>
> ```c++
> Base::foo
> Base::bar
> ```
>
> 显然没有发生polymorphism。

### Solution and Sample Code

There are multiple ways of achieving the desired effect. Each has its own pros and cons. In general the approaches can be divided into two categories. One using **two phase initialization** and other one using only **single phase initialization**.



#### Two phase initialization

**Two phase initialization** technique separates object construction from initializing its state. Such a separation may not be always possible. Initialization of object's state is clubbed together in a separate function, which could be a member function or a free standing function.

```c++
class Base {
 public:
   void init()  // may or may not be virtual
   {
       this->foo(42);
       this->bar();
   }
   virtual void foo(int n) const // often pure virtual
   {
       std::cout<<"Base::foo"<<std::endl;
   }
   virtual double bar() const    // often pure virtual
   {
       std::cout<<"Base::bar"<<std::endl;       
   }
 };
 
 
 class Derived : public Base {
 public:
   Derived (const char *)
   {
       
   }
   virtual void foo(int n) const
   {
       std::cout<<"Derived::foo"<<std::endl;       
   }
   virtual double bar() const
   {
       std::cout<<"Derived::bar"<<std::endl;       
   }
 };
```

##### using non-member function

```c++
template <class Derived, class Parameter>
std::unique_ptr <Base> factory (Parameter p)
{
  std::unique_ptr <Base> ptr (new Derived (p));
  ptr->init (); 
  return ptr;
}
```

A non-template version of this approach can be found here. The factory function can be moved inside base class but it has to be static.

```c++
class Base {
  public:
    template <class D, class Parameter>
    static std::unique_ptr <Base> Create (Parameter p)
    {
       std::unique_ptr <Base> ptr (new D (p));       
       ptr->init (); 
       return ptr;
    }
};
int main ()
{
  std::unique_ptr <Base> b = Base::Create <Derived> ("para");
}
```

Constructors of class `Derived` should be made private to prevent users from accidentally using them. Interfaces should be easy to use correctly and hard to use incorrectly - remember? The factory function should then be friend of the derived class. In case of member create function, Base class can be friend of Derived.

> NOTE: 下面是完整程序

```c++
#include<iostream>
#include<memory>

class Base {
 public:
   void init()  // may or may not be virtual
   {
       this->foo(42);
       this->bar();
   }
   virtual void foo(int n) const // often pure virtual
   {
       std::cout<<"Base::foo"<<std::endl;
   }
   virtual double bar() const    // often pure virtual
   {
       std::cout<<"Base::bar"<<std::endl;       
   }
 };
 
 
 class Derived : public Base {
 public:
   Derived (const char *)
   {
       
   }
   virtual void foo(int n) const
   {
       std::cout<<"Derived::foo"<<std::endl;       
   }
   virtual double bar() const
   {
       std::cout<<"Derived::bar"<<std::endl;       
   }
 };
 
template <class Derived, class Parameter>
std::unique_ptr <Base> factory (Parameter p)
{
  std::unique_ptr <Base> ptr (new Derived (p));
  ptr->init (); 
  return ptr;
}
int main ()
{
  std::unique_ptr <Base> b = factory<Derived> ("para");
}
```

> NOTE: 上诉程序输出如下：
>
> ```
> Derived::foo
> Derived::bar
> ```
>
> 显然发生了polymorphism。

#### Without using two phase initialization

Achieving desired effect using a helper hierarchy is described here but an extra class hierarchy has to be maintained, which is undesirable. Passing pointers to static member functions is C'ish. Curiously Recurring Template Pattern idiom can be useful in this situation.

```c++
#include<iostream>
using namespace std;
class Base {
};
template <class D>
class InitTimeCaller : public Base {
  protected:
    InitTimeCaller () {
       D::foo ();
       D::bar ();
    }
};
class Derived : public InitTimeCaller<Derived> 
{
  public:
    Derived () : InitTimeCaller <Derived> () {
		cout << "Derived::Derived()\n";
	}
    static void foo () {
		cout << "Derived::foo()\n";
	}
    static void bar () {
		cout << "Derived::bar()\n";
	}
};
int main()
{
    Derived d;
}
```

> NOTE: 上述程序的输出如下：
>
> ```
> Derived::foo()
> Derived::bar()
> Derived::Derived()
> ```
>
> 

Using [Base-from-member idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Base-from-Member) more complex variations of this idiom can be created.




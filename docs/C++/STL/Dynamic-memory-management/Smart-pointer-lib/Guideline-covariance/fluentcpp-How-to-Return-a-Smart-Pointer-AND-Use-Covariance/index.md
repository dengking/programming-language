# fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

That solution is simple and targeted to that situation. But here I want to expand on this and tackle the more general problem: in C++, it seems that we can have covariant return, or smart pointer return, but not both. Or can we?

## The problem: Covariant return type vs. smart pointers

C++ has support for [covariant return type](https://en.wikipedia.org/wiki/Covariant_return_type). That is, you can have the following code:

```c++
struct Base {};
struct Derived : Base {};

struct Parent
{
   virtual Base * foo();
} ;

struct Child : Parent
{
   virtual Derived * foo() override ;
} ;
```

Here, we expect the `foo` method from Child to return `Base *` for a successful overriding (and compilation!). With the covariant return type, we can actually replace `Base *` by any of its derived types. For example, `Derived *`.

This works for pointers, and for references… But the moment you try to use smart pointers:

```c++
#include <memory>

struct Base {};
struct Derived : Base {};

struct Parent
{
   virtual std::unique_ptr<Base> foo();
} ;

struct Child : Parent
{
   virtual std::unique_ptr<Derived> foo() override ;
} ;
```

… the compiler generates an error.

## Use cases

Since the problem is general, let’s take a wide panel of use cases with increasing complexity:

### Simple hierarchy:

![img](https://www.fluentcpp.com/wp-content/uploads/2017/07/fluentcpp-simple-hierarchy.png)

### Multiple inheritance:

![img](https://www.fluentcpp.com/wp-content/uploads/2017/07/fluentcpp-multiple-inheritance.png)

### Deep hierarchy:

![img](https://www.fluentcpp.com/wp-content/uploads/2017/07/fluentcpp-deep-hierarchy.png)

### Diamond inheritance:

![img](https://www.fluentcpp.com/wp-content/uploads/2017/07/fluentcpp-diamond-inheritance.png)

By handling all those cases in a natural way, the solution should be usable for most production problems.

## Preamble(序言): Separation of concerns + private virtual function

Instead of having one `clone` member function handling everything, we will separate it into two member functions. In the following piece of code:

```c++
class some_class
{
public:
   std::unique_ptr<some_class> clone() const
   {
      return std::unique_ptr<some_class>(this->clone_impl());
   }

private:
   virtual some_class * clone_impl() const
   {
      return new some_class(*this) ;
   }
};
```

The first function, `clone_impl()`, does the actual work of cloning using the **copy-constructor**. It offers a strong guarantee (as long as the copy-constructor offers it), and transfers the ownership of the pointer to the newly created object. While this is usually unsafe, we assume that in this case no one can call this function except the `clone()` function, which is enforced by the private access of `clone_impl()`.

> NOTE: 
> 1、对于unsafe的，通过make it private来减小的负面影响

The second function, `clone()`, retrieves the pointer, and gives its ownership to a `unique_ptr`. This function cannot fail by itself, so it offers the same strong guarantee as `clone_impl()`.

## Simple Hierarchy: Covariance + Name hiding

Using the technique above, we can now produce a simple OO hierarchy:

![img](https://www.fluentcpp.com/wp-content/uploads/2017/07/fluentcpp-simple-hierarchy-2-1.png)

```c++
class cloneable
{
public:
   virtual ~cloneable() {}

   std::unique_ptr<cloneable> clone() const
   {
      return std::unique_ptr<cloneable>(this->clone_impl());
   }

private:
   virtual cloneable * clone_impl() const = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
class concrete : public cloneable
{
public:
   std::unique_ptr<concrete> clone() const
   {
      return std::unique_ptr<concrete>(this->clone_impl());
   }

private:
   virtual concrete * clone_impl() const override
   {
      return new concrete(*this);
   }
};
int main()
{
   std::unique_ptr<concrete> c = std::make_unique<concrete>();
   std::unique_ptr<concrete> cc = c->clone();
 
   cloneable * p = c.get();
   std::unique_ptr<clonable> pp = p->clone();
}
```

> NOTE: 上述代码中使用到了`std::make_unique  `, 这是c++14的，所以我将原文代码进行了修改，使它在c++11的compiler中能够编译通过。
>
> ```C++
> #include <memory>
> #include <utility>
> 
> 
> // note: this implementation does not disable this overload for array types
> template<typename T, typename... Args>
> std::unique_ptr<T> make_unique(Args&&... args)
> {
>     return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
> }
> 
> class cloneable
> {
> public:
> 	virtual ~cloneable()
> 	{
> 	}
> 
> 	std::unique_ptr<cloneable> clone() const
> 	{
> 		return std::unique_ptr<cloneable>(this->clone_impl());
> 	}
> 
> private:
> 	virtual cloneable * clone_impl() const = 0;
> };
> 
> ///////////////////////////////////////////////////////////////////////////////
> 
> class concrete: public cloneable
> {
> public:
> 	std::unique_ptr<concrete> clone() const
> 	{
> 		return std::unique_ptr<concrete>(this->clone_impl());
> 	}
> 
> private:
> 	virtual concrete * clone_impl() const override
> 	{
> 		return new concrete(*this);
> 	}
> };
> int main()
> {
> 	std::unique_ptr<concrete> c = make_unique<concrete>();
> 	std::unique_ptr<concrete> cc = c->clone();
> 
> 	cloneable * p = c.get();
> 	std::unique_ptr<cloneable> pp = p->clone();
> }
> // g++ --std=c++11 -Wall -pedantic -test.cpp
> 
> ```
>
> 

Do you see what we did, here?

By separating the concerns, we were able to use covariance at each level of the hierarchy to produce a `clone_impl` member function returning the exact type of pointer we wanted.

And using a little (usually) annoying feature in C++, name hiding (i.e. when declaring a name in a derived class, this name hides all the symbols with the same name in the base class), we hide (not override) the clone() member function to return a smart pointer of the exact type we wanted.

> NOTE: name hiding这个概念非常重要，参见`C++\Language-reference\Classes\Overload-VS-override-VS-name-hiding.md`

When cloning from a concrete, we obtain a `unique_ptr<concrete>`, and when cloning from a `cloneable`, we obtain a `unique_ptr<cloneable>`.

One could get uneasy at the idea of having a `clone_impl` member function using a RAII-unsafe transfer of ownership, but the problem is mitigated as the member function is private, and is called only by `clone`. This limits the risk as the user of the class can’t call it by mistake.

> NOTE: 这些考虑是非常值得学习的

This solves the problem but adds some amount of boilerplate code.

## Simple Hierarchy, v2: Enter the CRTP

```C++
template <typename Derived, typename Base>
class clone_inherit<Derived, Base> : public Base
{
public:
   std::unique_ptr<Derived> clone() const
   {
      return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }

private:
   virtual clone_inherit * clone_impl() const override
   {
      return new Derived(*this);
   }
};
```

`clone_inherit` is a CRTP that knows its derived class, but also all its direct base class. It implements the covariant `clone_impl()` and hiding `clone()` member functions as usual, but they use casts to move through the hierarchy of types.

This enables us to change the concrete class defined above into:

```c++
class concrete
   : public clone_inherit<concrete, cloneable>
{
};
int main()
{
   std::unique_ptr<concrete> c = std::make_unique<concrete>();
   std::unique_ptr<concrete> cc = b->clone();
 
   cloneable * p = c.get();
   std::unique_ptr<clonable> pp = p->clone();
}
```

As you can see, the concrete class is now free of clutter.

This effectively adds a polymorphic and covariant clone() to a hierarchy of class.

This CRTP is the foundation of our general solution: Every next step will build upon it.



> NOTE: 原文给出的代码是无法编译通过的，下面是修正过后的版本
>
> ```c++
> #include <memory>
> 
> // note: this implementation does not disable this overload for array types
> template<typename T, typename ... Args>
> std::unique_ptr<T> make_unique(Args&&... args)
> {
> 	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
> }
> class cloneable
> {
> public:
> 	virtual ~cloneable()
> 	{
> 	}
> 
> 	std::unique_ptr<cloneable> clone() const
> 	{
> 		return std::unique_ptr<cloneable>(this->clone_impl());
> 	}
> 
> private:
> 	virtual cloneable * clone_impl() const = 0;
> };
> 
> template<typename Derived, typename Base>
> class clone_inherit: public Base
> {
> public:
> 	std::unique_ptr<Derived> clone() const
> 	{
> 		return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
> 	}
> 
> private:
> 	virtual clone_inherit * clone_impl() const override
> 	{
> 		return new Derived(static_cast<const Derived&>(*this));
> 	}
> };
> 
> class concrete
> : public clone_inherit<concrete, cloneable>
> {
> 
> };
> 
> int main()
> {
> 	std::unique_ptr<concrete> c = make_unique<concrete>();
> 	std::unique_ptr<concrete> cc = c->clone();
> 
> 	cloneable * p = c.get();
> 	std::unique_ptr<cloneable> pp = p->clone();
> }
> 
> ```
>
> 
>

## Multiple Inheritance: Variadic templates to the rescue
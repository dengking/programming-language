# Coercion by Member Template



## More C++ Idioms/[Coercion by Member Template](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Coercion_by_Member_Template)

### Intent

To increase the flexibility of a class template's interface by allowing the class template to participate in the same **implicit type conversions** (coercion) as its parameterizing types enjoy.

### Motivation

It is often useful to extend a relationship between two types to class templates specialized with those types. For example, suppose that class `D` derives from class `B`. A pointer to an object of type `D` can be assigned to a pointer to `B;` C++ supports that implicitly. However, types composed of these types do not share the relationship of the composed types. That applies to class templates as well, so a `Helper<D>` object normally cannot be assigned to a `Helper<B>` object.

```c++
class B {};
class D : public B {};
template <class T>
class Helper {};

int main()
{
B *bptr;
D *dptr;
bptr = dptr; // OK; permitted by C++

Helper<B> hb;
Helper<D> hd; 
hb = hd; // Not allowed but could be very useful 
}

```

> NOTE: 上述程序编译报错如下：
>
> ```
> test.cpp:14:4: error: no match for ‘operator=’ (operand types are ‘Helper<B>’ and ‘Helper<D>’)
> ```
>
> 即没有`operator=`，则无法进行conversion。

There are cases where such conversions are useful, such as allowing conversion from `std::unique_ptr<D>` to `std::unique_ptr<B>`. That is quite intuitive, but isn't supported without using the Coercion by Member Template Idiom.

### Solution and Sample Code

Define **member template functions**, in a class template, which rely on the **implicit type conversions** supported by the parameter types. In the following example, the templated constructor and assignment operator work for any type `U`, for which initialization or assignment of a `T *` from a `U *` is allowed.

```c++
#include<iostream>
using namespace std;

class B {};
class D : public B {};

template <class T>
class Ptr
{
  public:
    Ptr () {}

    Ptr (Ptr const & p)
      : ptr (p.ptr)
    {
      std::cout << "Copy constructor\n";
    }

    // Supporting coercion using member template constructor.
    // This is not a copy constructor, but behaves similarly.
    template <class U>
    Ptr (Ptr <U> const & p)
      : ptr (p.ptr) // Implicit conversion from U to T required
    {
      std::cout << "Coercing member template constructor\n";
    }

    // Copy assignment operator.
    Ptr & operator = (Ptr const & p)
    {
      ptr = p.ptr;
      std::cout << "Copy assignment operator\n";
      return *this;
    }

    // Supporting coercion using member template assignment operator.
    // This is not the copy assignment operator, but works similarly.
    template <class U>
    Ptr & operator = (Ptr <U> const & p)
    {
      ptr = p.ptr; // Implicit conversion from U to T required
      std::cout << "Coercing member template assignment operator\n";
      return *this;
    } 

    T *ptr;
};

int main (void)
{
   Ptr <D> d_ptr;
   Ptr <B> b_ptr (d_ptr); // Now supported
   b_ptr = d_ptr;         // Now supported
}
```

Another use for this idiom is to permit assigning an array of pointers to a class to an array of pointers to that class' base. Given that `D` derives from `B`, a `D` object **is-a** `B` object. However, an array of `D` objects **is-not-an** array of `B` objects. This is prohibited in C++ because of [slicing](https://en.wikipedia.org/wiki/Object_slicing). Relaxing this rule for an array of pointers can be helpful. For example, an array of pointers to `D` should be assignable to an array of pointers to `B` (assuming `B`'s destructor is virtual). Applying this idiom can achieve that, but extra care is needed to prevent copying arrays of pointers to one type to arrays of pointers to a derived type. Specializations of the member function templates or [SFINAE](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/SFINAE) can be used to achieve that.

The following example uses a templated constructor and assignment operator expecting `Array<U *>` to only allow copying Arrays of pointers when the element types differ.

```c++
#include<iostream>
#include<algorithm>
using namespace std;

class B {};
class D : public B {};

template <class T>
class Array
{
  public:
    Array () {}
    Array (Array const & a)
    {
      std::copy (a.array_, a.array_ + SIZE, array_);
    }

    template <class U>
    Array (Array <U *> const & a)
    {
      std::copy (a.array_, a.array_ + SIZE, array_);
    }

    template <class U>
    Array & operator = (Array <U *> const & a)
    {
      std::copy (a.array_, a.array_ + SIZE, array_);
    }

    enum { SIZE = 10 };
    T array_[SIZE];
};


```

Many smart pointers such as [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) , [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr)  employ this idiom.



### Caveats

A typical mistake in implementing the Coercion by Member Template Idiom is failing to provide the non-template copy constructor or copy assignment operator when introducing the templated copy constructor and assignment operator. A compiler will automatically declare a copy constructor and a copy assignment operator if a class does not declare them, which can cause hidden and non-obvious faults when using this idiom.

### Known Uses

- [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)
- [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)
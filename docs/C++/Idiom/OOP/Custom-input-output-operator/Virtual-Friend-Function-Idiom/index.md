# Virtual Friend Function

## More C++ Idioms/[Virtual Friend Function](https://en.wikibooks.org/wiki/More_C++_Idioms/Virtual_Friend_Function)

> NOTE: 

### Intent

Simulate a virtual friend function.

### Also Known As

### Motivation

**Friend functions** are often needed in `C++`. A canonical example is that of types that can be printed to output streams (*e.g.,* `std::cout`). An overloaded left-shift operator function, which is often a friend, is needed to achieve seamless streaming capabilities. Friend functions are really an *extension* of the class's interface. However, friend functions in C++ can not be declared virtual and therefore no **dynamic binding** of friend functions is possible. Applying a friend function to an entire hierarchy of classes becomes awkward if an overloaded friend function is needed for every class in the hierarchy. This lack of support for **dynamic binding** makes it hard to justify that friend functions are in fact an *extension* of the class's interface. **Virtual friend function idiom** addresses this concern elegantly.

### Solution and Sample Code

**Virtual friend function idiom** makes use of an extra indirection to achieve the desired effect of **dynamic binding** for friend functions. In this idiom, usually there is only one function that is a friend of the **base class** of the hierarchy and the **friend function** simply **delegates** the work to a **helper member function** that is virtual. The helper function is overridden in every derived class, which does the real job and the **friend function** just serves as a facade(门面).

```c++
#include<iostream>
using namespace std;
class Base {
  public:
    friend ostream& operator << (ostream& o, const Base& b);
    // ...
  protected:
    virtual void print(ostream& o) const
    { 
        o<<"Base";
    }
};
/* make sure to put this function into the header file */
inline std::ostream& operator<< (std::ostream& o, const Base& b)
{
  b.print(o); // delegate the work to a polymorphic member function.
  return o;
}

class Derived : public Base {
  protected:
    virtual void print(ostream& o) const
    { 
        o<<"Derived";
    }
};

int main()
{
    Base b;
    std::cout<<b<<std::endl;
    Derived d;
    std::cout<<d<<std::endl;
}
```

> NOTE: 上述程序的输出如下：
>
> ```
> Base
> Derived
> ```
>
> 



### References

- [Virtual friend function idiom](http://www.parashift.com/c++-faq-lite/friends.html#faq-14.3), Marshall Cline



## 我的实践

### `My-library/Protocol-parsing-library`

1、使用virtual friend function idiom，overload `<<` operator，能够避免书写大量的重复code。

2、使用 "OOP interface +  template implementation" 能够非常灵活。
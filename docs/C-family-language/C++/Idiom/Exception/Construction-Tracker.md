# Construction Tracker



## More C++ Idioms/[Construction Tracker](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Construction_Tracker)

> NOTE: 这个idiom使用的场景是比较狭隘的：when initialization of two or more objects in the constructor's **initialization list** can throw the same exception type；它使用一个**counter** 来track construction of objects。

### Intent

To identify the data member that throws an exception when initialization of two or more objects in the constructor's **initialization list** can throw the same exception type

### Motivation

When two or more objects are initialized in a constructor's **initialization list** and all of them can throw the same exception (`std::exception`), tracking which one of them failed become a tricky issue as there can be only one try block surrounding the initialization list. Such a try block has a special name called '**constructor try block**', which is nothing but a '**function-try block**'.

### Solution and Sample Code

**Construction Tracker idiom** uses a simple technique to track successful construction on objects in the **initialization list**. A **counter** is simply incremented as（as的意思是：当） constructors of objects finish successfully one-by-one. It cleverly uses **bracket operator** to inject the **counter increments** in between calls to the constructors all being invisible to the user of the class.

> NOTE: 上面这段话总结了这个idiom的核心。上面这段话中的**counter**所对应的是下面程序中的`TrackerType tracker = NONE`，起初`tracker = NONE`；当构造`b_`时，它increment了：`tracker = ONE`；当构造`c_`，它increment了：`tracker = TWO`；

```c++
#include <iostream>
#include <stdexcept>
#include <cassert>

struct B {
    B (char const *) { throw std::runtime_error("B Error"); }
};
struct C {
    C (char const *) { throw std::runtime_error("C Error"); }
};
class A {
   B b_;
   C c_;
   enum TrackerType { NONE, ONE, TWO };
public:
   A( TrackerType tracker = NONE)
   try    // A constructor try block.
     : b_((tracker = ONE, "hello")) // Can throw std::exception
     , c_((tracker = TWO, "world")) // Can throw std::exception
     {
        assert(tracker == TWO);
        // ... constructor body ...
     }
   catch (std::exception const & e)
     {
        if (tracker == ONE) {
           std::cout << "B threw: " << e.what() << std::endl;
        }
        else if (tracker == TWO) {
           std::cout << "C threw: " << e.what() << std::endl;
        }
        throw;
     }
};

int main (void) 
{
    try {
        A a;
    }
    catch (std::exception const & e) {
          std::cout << "Caught: " << e.what() << std::endl;
    }
    return 0;       
}
```

The double parentheses is how the bracket operator is used to place in the assignment to the tracker. This idiom critically depends upon the constructor of `B` and `C` taking at least one parameter. If class `B` and `C` does not take parameters, then an adapter class needs to be written such that it the adapter class will accept a dummy parameter and calling the default parameters of `B` and `C`. Such an adapter can be written using [More C++ Idioms/Parameterized Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class) idiom using mixin-from-below technique. The adapter class can also be completely encapsulated inside class `A`. In the consturctor of class `A`, the tracker parameter has a default value and therefore it does no bother the user.

### References

[Smart Pointers Reloaded (III): Constructor Tracking](http://erdani.org/publications/cuj-2004-02.pdf)
# Empty base optimization



## cppreference [Empty base optimization](https://en.cppreference.com/w/cpp/language/ebo)

Allows the size of an empty base subobject to be zero.

```C++
#include <cassert>
 
struct Base {}; // empty class
 
struct Derived1 : Base {
    int i;
};
 
int main()
{
    // the size of any object of empty class type is at least 1
    assert(sizeof(Base) >= 1);
 
    // empty base optimization applies
    assert(sizeof(Derived1) == sizeof(int));
}
// g++ test.cpp
```



## Empty Base Optimization



### 22.0.3 Motivation

Empty classes come up from time to time in C++. C++ requires empty classes to have non-zero size to ensure object identity. For instance, an array of `EmptyClass` below has to have non-zero size because each object identified by the array subscript must be unique. Pointer arithmetic will fall apart if `sizeof(EmptyClass)` is zero. Often the size of such a class is one.

```C++
class EmptyClass {};
EmptyClass arr[10]; // Size of this array can’t be zero.
```

When the same empty class shows up as a data member of other classes, it consumes more than a single byte. Compilers often align data on 4-byte boundaries to avoid splitting. The four bytes taken by the **empty class object** are just placeholders and serve no useful purpose. Avoiding wastage of space is desirable to save memory and help fit more objects in the cpu cache lines.

### 22.0.4 Solution and Sample Code

C++ makes special exemption(豁免) for empty classes when they are inherited from. The compiler is allowed to flatten the inheritance hierarchy in a way that the **empty base class** does not consume space. For instance, in the following example, `sizeof(AnInt)` is 4 on 32 bit architectures and `sizeof(AnotherEmpty)` is 1 byte even though both of them inherit from the `EmptyClass`

```C++
#include <iostream>
class EmptyClass
{
};
class AnInt: public EmptyClass
{
	int data;
};
// size = sizeof(int)
class AnotherEmpty: public EmptyClass
{
};
// size = 1
int main()
{
	std::cout << sizeof(EmptyClass) << std::endl;
	std::cout << sizeof(AnInt) << std::endl;
	std::cout << sizeof(AnotherEmpty) << std::endl;
}

```

EBCO makes use of this exemption(豁免) in a **systematic**(系统的) way. It may not be desirable to naively move the **empty classes** from **member-list** to **base-class-list** because that may expose interfaces that are otherwise hidden from the users. For instance, the following way of applying EBCO will apply the optimization but may have undesirable side-effects: The signatures of the functions (if any in `E1`, `E2`) are now visible to the users of `class Foo` (although they can’t call them because of private inheritance).

```C++
#include <iostream>
class E1
{
};
class E2
{
};
// **** before EBCO ****
class Foo
{
	E1 e1;
	E2 e2;
	int data;
};
// sizeof(Foo) = 8
// **** after EBCO ****
class Foo1: private E1, private E2
{
	int data;
};
// sizeof(Foo1) = 4
int main()
{
	std::cout << sizeof(Foo) << std::endl;
	std::cout << sizeof(Foo1) << std::endl;
}
// g++ test.cpp

```

A practical way of using EBCO is to combine the empty members into a single member that flattens the storage. The following template `BaseOptimization` applies EBCO on its first two type parameter. The `Foo` class above has been rewritten to use it.

```C++
#include <iostream>
class E1
{
};
class E2
{
};

template<class Base1, class Base2, class Member>
struct BaseOptimization: Base1, Base2
{
	Member member;
	BaseOptimization()
	{
	}
	BaseOptimization(Base1 const &b1, Base2 const &b2, Member const &mem) :
					Base1(b1), Base2(b2), member(mem)
	{
	}
};
class Foo
{
	BaseOptimization<E1, E2, int> data;
};
// sizeof(Foo) = 4
int main()
{
	std::cout << sizeof(Foo) << std::endl;
}
// g++ test.cpp

```

With this technique, there is no change in the inheritance relationship of the `Foo` class. It also avoids the problem of accidentally overriding a function from the base classes. Note that in the approach shown above it is critical that the base classes do not conflict with each other. That is, `Base1` and `Base2` are part of independent hierarchies.


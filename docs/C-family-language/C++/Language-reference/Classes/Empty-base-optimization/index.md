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



## MoreCppIdiom#[Empty Base Optimization](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Empty_Base_Optimization)

### 22.0.1 Intent

Optimize storage for data members of empty class types

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

EBCO makes use of this exemption(豁免) in a **systematic**(系统的) way. 

It may not be desirable to naively move the **empty classes** from **member-list** to **base-class-list** because that may expose interfaces that are otherwise hidden from the users. For instance, the following way of applying EBCO will apply the optimization but may have undesirable side-effects: The signatures of the functions (if any in `E1`, `E2`) are now visible to the users of `class Foo` (although they can’t call them because of private inheritance).

> NOTE: 这个例子所阐述的是"naively move the **empty classes** from **member-list** to **base-class-list**"的弊端: 基类(`class E1`、`class E2`)的function被暴露给了子类`class Foo`。

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

> NOTE: 这个例子所展示的更好的做法: "combine the empty members into a single member that flattens the storage"，其实这是符合"Composition-over-inheritance"的。

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

With this technique, there is no change in the **inheritance relationship** of the `Foo` class. It also avoids the problem of accidentally overriding a function from the base classes. Note that in the approach shown above it is critical that the base classes do not conflict with each other. That is, `Base1` and `Base2` are part of independent hierarchies.

#### Caveat

**Object identity issues** do not appear to be consistent across compilers. The addresses of the empty objects may or may not be the same. For instance, the pointer returned by `first` and `second` member methods of `BaseOptimization` class may be the same on some compilers and different on others. See more discussion on stackoverflow [boost compressed_pair and addresses of empty objects](https://stackoverflow.com/questions/7694158/boost-compressed-pair-and-addresses-of-empty-objects)

### 22.0.5 Known Uses

• [boost::compressed_pair](http://www.boost.org/doc/libs/1_47_0/libs/utility/compressed_pair.htm) makes use of this technique to optimize the size of the pair.
• A C++03 emulation of `unique_ptr` also uses this idiom.

## stackoverflow [When do programmers use Empty Base Optimization (EBO)](https://stackoverflow.com/questions/4325144/when-do-programmers-use-empty-base-optimization-ebo)



[A](https://stackoverflow.com/a/4325624)

EBO is important in the context of [policy based design](http://en.wikipedia.org/wiki/Policy-based_design), where you generally inherit *privately* from multiple policy classes. If we take the example of a thread safety policy, one could imagine the pseudo-code :

```cpp
class MTSafePolicy
{
public:
  void lock() { mutex_.lock(); }
  void unlock() { mutex_.unlock(); }

private:
  Mutex mutex_;
};

class MTUnsafePolicy
{
public:
  void lock() { /* no-op */ }
  void unlock() { /* no-op */ }
};
```

Given a policy based-design class such as :

```cpp
template<class ThreadSafetyPolicy>
class Test : ThreadSafetyPolicy
{
  /* ... */
};
```

Using the class with a `MTUnsafePolicy` simply add no size overhead the class `Test` : it's a perfect example of *don't pay for what you don't use*.



## csdn [C++ EBO 空基类最优化](https://blog.csdn.net/zhangxiao93/article/details/76038001)



```C++
class Empty
{
public:
    typedef int TYPENAME;//typedef
    enum color{red,green,yellow};//enum
    void hello(){ cout << "hello" << endl; }//non-virtual 函数
    static int xx;//static 成员变量

};
class HoldAnInt : private Empty
{
public:
    void newFunc()
    {
        hello();
        cout << "new Func" << endl;
    }
private:
    int x;
};


int main()
{
    cout << sizeof(Empty) << endl;//1.
    cout << sizeof(HoldAnInt) << endl;//4
    HoldAnInt a;
    a.newFunc();// hello newFunc
    getchar();
    return 0;
}
```


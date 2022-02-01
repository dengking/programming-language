# Member initializer lists

## cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/constructor)





## geeksforgeeks [When do we use Initializer List in C++?](https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/)

**Initializer List** is used to initialize data members of a class. The list of members to be initialized is indicated with constructor as a comma separated list followed by a colon(冒号). Following is an example that uses initializer list to initialize x and y of Point class.

```c++
#include<iostream> 
using namespace std; 
  
class Point { 
private: 
    int x; 
    int y; 
public: 
    Point(int i = 0, int j = 0):x(i), y(j) {}  
    /*  The above use of Initializer list is optional as the  
        constructor can also be written as: 
        Point(int i = 0, int j = 0) { 
            x = i; 
            y = j; 
        } 
    */    
      
    int getX() const {return x;} 
    int getY() const {return y;} 
}; 
  
int main() { 
  Point t1(10, 15); 
  cout<<"x = "<<t1.getX()<<", "; 
  cout<<"y = "<<t1.getY(); 
  return 0; 
} 
```

The above code is just an example for **syntax** of Initializer list. In the above code, `x` and `y` can also be easily initialed inside the constructor. But there are situations where initialization of data members inside constructor doesn’t work and **Initializer List** must be used. Following are such cases:

### 1) For initialization of non-static const data members:

**const data members** must be initialized using Initializer List. In the following example, “t” is a const data member of Test class and is initialized using Initializer List.

```c++
#include<iostream> 
using namespace std; 
  
class Test { 
    const int t; 
public: 
    Test(int t):t(t) {}  //Initializer list must be used 
    int getT() { return t; } 
}; 
  
int main() { 
    Test t1(10); 
    cout<<t1.getT(); 
    return 0; 
} 
  
/* OUTPUT: 
   10  
*/
```

### 2) For initialization of reference members:

**Reference members** must be initialized using **Initializer List**. In the following example, “t” is a reference member of Test class and is initialized using **Initializer List**.

```c++
// Initialization of reference data members 
#include<iostream> 
using namespace std; 
  
class Test { 
    int &t; 
public: 
    Test(int &t):t(t) {}  //Initializer list must be used 
    int getT() { return t; } 
}; 
  
int main() { 
    int x = 20; 
    Test t1(x); 
    cout<<t1.getT()<<endl; 
    x = 30; 
    cout<<t1.getT()<<endl; 
    return 0; 
} 
/* OUTPUT: 
    20 
    30 
 */
```

### 3) For initialization of member objects which do not have default constructor:

In the following example, an object “a” of class “A” is data member of class “B”, and “A” doesn’t have **default constructor**. Initializer List must be used to initialize “a”.

```c++
#include <iostream> 
using namespace std; 
  
class A { 
    int i; 
public: 
    A(int ); 
}; 
  
A::A(int arg) { 
    i = arg; 
    cout << "A's Constructor called: Value of i: " << i << endl; 
} 
  
// Class B contains object of A 
class B { 
    A a; 
public: 
    B(int ); 
}; 
  
B::B(int x):a(x) {  //Initializer list must be used 
    cout << "B's Constructor called"; 
} 
  
int main() { 
    B obj(10); 
    return 0; 
} 
/* OUTPUT: 
    A's Constructor called: Value of i: 10 
    B's Constructor called 
*/
```

If class A had both default and parameterized constructors, then Initializer List is not must if we want to initialize “a” using default constructor, but it is must to initialize “a” using parameterized constructor.

### 4) For initialization of base class members : 

 Like point 3, **parameterized constructor** of base class can only be called using **Initializer List**.

```c++
#include <iostream> 
using namespace std; 
  
class A { 
    int i; 
public: 
    A(int ); 
}; 
  
A::A(int arg) { 
    i = arg; 
    cout << "A's Constructor called: Value of i: " << i << endl; 
} 
  
// Class B is derived from A 
class B: A { 
public: 
    B(int ); 
}; 
  
B::B(int x):A(x) { //Initializer list must be used 
    cout << "B's Constructor called"; 
} 
  
int main() { 
    B obj(10); 
    return 0; 
} 
```

### 5) When constructor’s parameter name is same as data member

If constructor’s parameter name is same as data member name then the data member must be initialized either using [this pointer ](http://msdn.microsoft.com/en-us/library/y0dddwwd.aspx)or Initializer List. In the following example, both member name and parameter name for A() is “i”.

```c++
#include <iostream> 
using namespace std; 
  
class A { 
    int i; 
public: 
    A(int ); 
    int getI() const { return i; } 
}; 
  
A::A(int i):i(i) { }  // Either Initializer list or this pointer must be used 
/* The above constructor can also be written as  
A::A(int i) {  
    this->i = i; 
} 
*/
  
int main() { 
    A a(10); 
    cout<<a.getI(); 
    return 0; 
} 
/* OUTPUT: 
    10 
*/
```

### 6) For Performance reasons:

It is better to initialize all class variables in **Initializer List** instead of assigning values inside body. Consider the following example:

```c++
// Without Initializer List 
class MyClass { 
    Type variable; 
public: 
    MyClass(Type a) {  // Assume that Type is an already 
                     // declared class and it has appropriate  
                     // constructors and operators 
      variable = a; 
    } 
}; 
```

Here compiler follows following steps to create an object of type `MyClass`

\1. Type’s constructor is called first for “a”.

\2. The assignment operator of “Type” is called inside body of `MyClass()` constructor to assign

```c++
    variable = a; 
```

\3. And then finally destructor of “Type” is called for “a” since it goes out of scope.

Now consider the same code with `MyClass()` constructor with Initializer List

```c++
// With Initializer List 
class MyClass { 
    Type variable; 
public: 
    MyClass(Type a):variable(a) {   // Assume that Type is an already 
                     // declared class and it has appropriate 
                     // constructors and operators 
    } 
}; 
```

With the Initializer List, following steps are followed by compiler:

\1. Copy constructor of “Type” class is called to initialize : variable(a). The arguments in initializer list are used to copy construct “variable” directly.

\2. Destructor of “Type” is called for “a” since it goes out of scope.

As we can see from this example if we use assignment inside constructor body there are three function calls: constructor + destructor + one addition assignment operator call. And if we use Initializer List there are only two function calls: copy constructor + destructor call. See [this ](https://www.geeksforgeeks.org/output-of-c-program/)post for a running example on this point.

This assignment penalty will be much more in “real” applications where there will be many such variables. Thanks to *ptr* for adding this point.

Please write comments if you find anything incorrect, or you want to share more information about the topic discussed above.



## Prefer to use member initializer list

### [LeetBook C++ 面试突击](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/) # [为什么用成员初始化列表会快一些？](https://leetcode-cn.com/leetbook/read/cpp-interview-highlights/eft937/)

说明：数据类型可分为内置类型和用户自定义类型（类类型），对于用户自定义类型，利用成员初始化列表效率高。

原因：用户自定义类型如果使用类初始化列表，直接调用该成员变量对应的构造函数即完成初始化；如果在构造函数中初始化，因为 C++ 规定，对象的成员变量的初始化动作发生在进入构造函数本体之前，那么在执行构造函数的函数体之前首先调用默认的构造函数为成员变量设初值，在进入函数体之后，调用该成员变量对应的构造函数。因此，使用列表初始化会减少调用默认的构造函数的过程，效率高。

### stackoverflow [Why should I prefer to use member initialization lists?](https://stackoverflow.com/questions/926752/why-should-i-prefer-to-use-member-initialization-lists)

#### [A](https://stackoverflow.com/a/926795/10173843)

For [POD](https://stackoverflow.com/a/146454/626796) class members, it makes no difference, it's just a matter of style. For class members which are classes, then it avoids an unnecessary call to a **default constructor**. Consider:

```C++
#include<iostream>
class A
{
public:
    A() { 
        x = 0; 
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    A(int x_) { 
        x = x_; 
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    int x;
};

class B
{
public:
    B()
    {
        a.x = 3;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
private:
    A a;
};
int main()
{
    B b;
}
// g++ test.cpp -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、输出如下
>
> ```C++
> A::A()
> 
> B::B()
> ```
>
> 从上面可以看到，它是会首先调用 `A` 的default constructor `A::A()` 的

In this case, the constructor for `B` will call the default constructor for `A`, and then initialize `a.x` to 3. A better way would be for `B`'s constructor to directly call `A`'s constructor in the initializer list:

```C++
#include<iostream>
class A
{
public:
    A() { 
        x = 0; 
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    A(int x_) { 
        x = x_; 
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    int x;
};

class B
{
public:
    B() : a(3)
    {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
private:
    A a;
};
int main()
{
    B b;
}
// g++ test.cpp -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、输出如下:
>
> ```C++
> A::A(int)
> 
> B::B()
> ```
>
> 

This would only call `A`'s `A(int)` constructor and not its default constructor. In this example, the difference is negligible(微不足道的), but imagine if you will that `A`'s default constructor did more, such as allocating memory or opening files. You wouldn't want to do that unnecessarily.

```C++
class A
{
public:
    A(int x_) { x = x_; }
    int x;
};

class B
{
public:
    B() : a(3), y(2)  // 'a' and 'y' MUST be initialized in an initializer list;
    {                 // it is an error not to do so
    }
private:
    A a;
    const int y;
};
```

## Member initialization order

这在 `Initialization&deinitialization-order` 章节进行了讨论。


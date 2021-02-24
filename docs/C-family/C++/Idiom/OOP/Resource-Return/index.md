# Resource Return



## More C++ Idioms/[Resource Return](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Resource_Return)



### Intent

To convey ownership transfer (of a resource) explicitly in the return type of a factory function.

> NOTE: convey ownership给caller

### Motivation

Many times, interfaces are developed where the caller is **implicitly** responsible for resource release. If the caller is not aware of this fact or simply forgets to take correct steps, it gives rise to an **easy-to-use-incorrectly** kind of interface. Following code snippet shows an example.

```c++
struct X
{
  void foo() {}
};
X * Xfactory() // Resource ownership implicitly transferred to the caller.
{
  return new X; // Dynamically allocated instance
}
int main (void)
{
  Xfactory()->foo(); // Dynamically allocated instance of X leaks here
}
```

Resource Return Idiom provides different alternatives to rectify the situation and results into (somewhat) hard-to-use-incorrectly interfaces.

### Solution and Sample Code

```c++
#include <memory>
#include <iostream>

struct X
{
  X()
  {
    std::cout<<"Ctor"<<std::endl;
  }
  ~X()
  {
    std::cout<<"Dctor"<<std::endl;
  }
  void foo() {}
};
std::unique_ptr<X> Xfactory() // Resource ownership explicitly transferred to the caller.
{
  return std::unique_ptr<X> (new X); // Dynamically allocated instance
}
int main (void)
{
  Xfactory()->foo(); // Dynamically allocated instance of X does not leak here
}
```

> NOTE: 编译`g++ test.cpp --std=c++11`，输出如下：
>
> ```
> Ctor
> Dctor
> ```
>
> 

There are several issues to be considered while determining the type of resource-management smart pointer to use to return a resource. Possible options are:

- s`td::auto_ptr` （已经废弃了）
- boost::shared_ptr / [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) (as of C++11)
- [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) in C++0x
- User defined [Handle/Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom

An excellent discussion of pros and cons of choosing one over the other are discussed at length by Scott Meyers in his article [The Resource Return Problem](http://www.aristeia.com/Papers/resourceReturnProblem.txt). 



When exclusive ownership is involved in case of Resource Return idiom, [Move Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Move_Constructor) idiom is often useful while transferring ownership of resources.


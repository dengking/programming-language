# Execute-Around Pointer



## More c++ idiom [Execute-Around Pointer](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Execute-Around_Pointer)

> NOTE: 总结：
>
> 使用一个proxy class来描述aspect，aspect的[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)行为放到proxy class的constructor、destructor中来进行实现。primary class中涉及到[*cross-cutting concerns*](https://en.wikipedia.org/wiki/Aspect-oriented_programming)的行为，需要由proxy class object来forward到primary class object来实现。

### Intent

Provide a smart pointer object that transparently executes actions before and after each function call on an object, given that the actions performed are the same for all functions.[[1\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Execute-Around_Pointer#cite_note-1) This can be regarded as a special form of [aspect oriented programming(AOP)](https://en.wikipedia.org/wiki/Aspect-oriented_programming).

> NOTE: 

### Also Known As

Double application of smart pointer.

> NOTE: 上述double application of smart pointer让我想到了double dispatch。

### Motivation

Often times it is necessary to execute a functionality before and after every member function call of a class. For example, in a multi-threaded application it is necessary to lock before modifying the data structure and unlock it afterwards. In a data structure visualization application might be interested in the size of the data structure after every insert/delete operation.

```c++
#include <vector>
#include <iostream>

class Visualizer {
    std::vector <int> & vect;
  public:
    Visualizer (std::vector<int> &v) : vect(v) {}
    void data_changed () {
       std::cout << "Now size is: " << vect.size() << std::endl;
    }
};
int main () // A data visualization application.
{
  std::vector <int> vector;
  Visualizer visu (vector);
  //...
  vector.push_back (10);
  visu.data_changed ();
  vector.push_back (20);
  visu.data_changed ();
  // Many more insert/remove calls here
  // and corresponding calls to visualizer.
}
```

Such a repetition of function calls is error-prone and tedious. It would be ideal if calls to visualizer could be automated. Visualizer could be used for `std::list <int>` as well. Such funcitonality which is not a part of single class but rather cross cuts multiple classes is commonly known as **aspects**. This particular idiom is useful for designing and implementing simple **aspects**.

### Solution and Sample Code



```c++
#include <vector>
#include <iostream>

class VisualizableVector {
  public:
    class proxy {
      public:
        proxy (std::vector<int> *v) : vect (v) {
          std::cout << "Before size is: " << vect->size () << std::endl;
        }
        std::vector<int> * operator -> () {
          return vect;
        }
        ~proxy () {
          std::cout << "After size is: " << vect->size () << std::endl;
        }
      private:
        std::vector <int> * vect;
    };
    VisualizableVector (std::vector<int> *v) : vect(v) {}    
    proxy operator -> () {
       return proxy (vect);
    }
  private:
    std::vector <int> * vect;
};
int main()
{
  std::vector<int> vec;
  VisualizableVector vecc(&vec);
  //...
  vecc->push_back(10); // Note use of -> operator instead of . operator
  vecc->push_back(20);
}
```

> NOTE: 使用proxy class来实现aspect

Overloaded `->` operator of `visualizableVector` creates a **temporary proxy object** and it is returned. Constructor of proxy object logs size of the vector. The overloaded `->` operator of proxy is then called and it simply forwards the call to the underlying vector object by returning a raw pointer to it. After the real call to the vector finishes, destructor of proxy logs the size again. Thus the logging for visualization is transparent and the main function becomes free from clutter. This idiom is a special case of **Execute Around Proxy**, which is more general and powerful.

> NOTE: execute around proxy是什么？

The real power of the idiom can be derived if we combine it judiciously with templates and chain the overloaded `->` operators.

```c++
#include <vector>
#include <iostream>

template <class NextAspect, class Para>
class Aspect
{
  protected:
    Aspect (Para p): para_(p) {}
    Para  para_;
  public:
    NextAspect operator -> () 
    {
      return NextAspect (para_);
    }
};

template <class NextAspect, class Para>
struct Visualizing : Aspect<NextAspect, Para>
{
  public:
    Visualizing (Para p) 
       : Aspect<NextAspect, Para> (p) 
    {
	std::cout << "Before Visualization aspect" << std::endl;
    }
    ~Visualizing () 
    {
	std::cout << "After Visualization aspect" << std::endl;
    }
};
template <class NextAspect, class Para>
struct Locking : Aspect<NextAspect, Para>
{
  public:
    Locking (Para p) 
       : Aspect<NextAspect, Para> (p) 
    {
		std::cout << "Before Lock aspect" << std::endl;
    }
    ~Locking () 
    {
	std::cout << "After Lock aspect" << std::endl;
    }
};
template <class NextAspect, class Para>
struct Logging : Aspect<NextAspect, Para>
{
  public:
    Logging (Para p) 
        : Aspect <NextAspect, Para> (p) 
    {
		std::cout << "Before Log aspect" << std::endl;
    }
    ~Logging () 
    {
	std::cout << "After Log aspect" << std::endl;
    }
};
template <class Aspect, class Para>
class AspectWeaver 
{
public:
    AspectWeaver (Para p) : para_(p) {}    
    Aspect operator -> () 
    {
       return Aspect (para_);
    }
private:
	Para para_;
};

#define AW1(T,U) AspectWeaver<T<U, U>, U>
#define AW2(T,U,V) AspectWeaver<T<U<V, V>, V>, V>
#define AW3(T,U,V,X) AspectWeaver<T<U<V<X, X>, X>, X>, X>

int main()
{
  std::vector<int> vec;
  AW3(Visualizing, Locking, Logging, std::vector <int> *) X(&vec);
  //...
  X->push_back(10); // Note use of -> operator instead of . operator
  X->push_back(20);
}
```

> NOTE: 
>
> ```c++
> #define AW1(T,U) AspectWeaver<T<U, U>, U>
> #define AW2(T,U,V) AspectWeaver<T<U<V, V>, V>, V>
> #define AW3(T,U,V,X) AspectWeaver<T<U<V<X, X>, X>, X>, X>
> ```
>
> 上面的代码是比较难以理解的，需要把握的是最后一个模板参数`Para`是，前面的模板参数是指定`Aspect`的，`Aspect`的设计是nesting的，从而实现`Aspect`的nesting。


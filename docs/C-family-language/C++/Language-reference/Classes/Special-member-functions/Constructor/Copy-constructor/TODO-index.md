# Copy constructor



## cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor)



## cplusplus [When a copy constructor is called](http://www.cplusplus.com/forum/general/100395/)

The following are the cases when **copy constructor** is called.
- When instantiating one object and initializing it with values from another object. 

- When passing an object by value. 

- When an object is returned from a function by value.

  

demo:

```c++
#include <iostream>

struct A
{
    A() {}
    A( const A& ) { std::cout << "A::copy_constructor is called to make a copy\n" ; }
    // other stuff
    A( A&& ) { std::cout << "A::move_constructor is called to make a copy\n" ; }
};

void foo( A object ) 
{ 
    std::cout << "foo has recieved a copy of an object\n";
}

A bar() 
{ 
    std::cout << "bar - about to return an object by value\n" ; 
    static A a ; 
    return a ; 
    
}

int main()
{
    std::cout << "about to call bar\n" ;
    A object = bar() ;
    std::cout << "bar has returned\n" ;

    std::cout << "\n\nabout to call foo\n" ;
    foo(object) ;
}
```
上面两个函数演示了2和3；下面这段解释了为什么`A object = bar() `没有触发move constructor。

```c++
A( A&& ) 
{ 
	std::cout << "A::move_constructor is called to make a copy\n" ; 
}
A object = bar() ;
```

In practice, the move constructor will never be used in this particular program.

`bar()` can't use the move constructor to make a copy. And RVO enables copy-elision of the anonymous return value.



<http://ideone.com/iWp77h>  


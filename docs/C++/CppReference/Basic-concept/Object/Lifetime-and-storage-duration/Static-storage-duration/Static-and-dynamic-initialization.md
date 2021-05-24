# Static initialization and dynamic initialization in c++

1、static initialization和dynamic initialization是两个非常重要的概念，对于static storage object的initialization往往会涉及到此；

2、在 pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/) 中，对这个问题有着非常好的描述

## stackoverflow [What is dynamic initialization of object in c++?](https://stackoverflow.com/questions/5945897/what-is-dynamic-initialization-of-object-in-c)

[A](https://stackoverflow.com/a/5945936)

> NOTE: 
>
> 1、这个回答是较好的

Dynamic initialization is that in which initialization value isn't known at compile-time. It's computed at runtime to initialize the variable.

Example,

```cpp
int factorial(int n)
{
     if ( n < 0 )       return -1; //indicates input error
     else if ( n == 0 ) return 1;
     else               return n * factorial(n-1);
}

int const a = 10 ; //static initialization 
             //10 is known at compile time. Its 10!

int const b = factorial(8); //dynamic initialization 
                      //factorial(8) isn't known at compile time,
                      //rather it's computed at runtime.
```

That is, static-initialization *usually* involves constant-expression (which is known at compile-time), while dynamic-initialization involves non-constant expression.

So there are two kind of initializations:

- Static initialization : Its either zero-initialization or initialization with a constant expression
- Any other initialization is dynamic initialization.

> NOTE: 
>
> 1、总结的非常好

## TODO



http://cplusplus.bordoon.com/static_initialization.html

http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html

http://cplusplus.bordoon.com/static_initialization.html

https://stackoverflow.com/questions/42386329/c-non-local-static-object-vs-local-static-object

http://the-algo-blog.blogspot.com/2011/07/c-non-local-static-object.html

https://www.geeksforgeeks.org/static-objects-destroyed/

http://www.devx.com/tips/Tip/13634
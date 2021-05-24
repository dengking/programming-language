# Pointer to function and function type



## 典型例子

下面的典型的例子来源自: 

1、c-faq [The Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)

### Pointer to function

```C++
                 +--------------------+
                 | +---+              |
                 | |+-+|              |
                 | |^ ||              |
            char *(*fp)( int, float *);
             ^   ^ ^  ||              |
             |   | +--+|              |
             |   +-----+              |
             +------------------------+
```

`fp`就是典型的pointer to function



### Function type

```C++
                      +-----------------------------+
                      |                  +---+      |
                      |  +---+           |+-+|      |
                      |  ^   |           |^ ||      |
                void (*signal(int, void (*fp)(int)))(int);
                 ^    ^      |      ^    ^  ||      |
                 |    +------+      |    +--+|      |
                 |                  +--------+      |
                 +----------------------------------+
```

`signal`就是典型的function type。



### 对比凸显差异的例子

```C++
typedef int* Function(int*); // function pointer
typedef int* (*PointerToFunction)(int*); // pointer to function
```



## `void` in function type and function pointer

本节收录了一些使用`void`的例子。

### stackoverflow [What does C expression ((void(*)(void))0)(); mean?](https://stackoverflow.com/questions/20357106/what-does-c-expression-voidvoid0-mean)



```C
((void(*)(void))0)();
```

> NOTE:
>
> 1、结合了pointer to function 、cast 、function  invokation

So we have integer 0 type casting to this tricky type `(void(*))(void)` and then executing it. Source claims that this should work, but what does it actually?

This must be one of those C jokes like `#define TRUE FALSE`, I suppose.

#### [A](https://stackoverflow.com/a/20357286)

This is a function expecting no arguments and returning no value:

```c
void f(void)
```

This is a **pointer** to a function expecting no arguments and returning no value:

```c
void (*p)(void)
```

This is the *type* of that pointer:

```c
void (*)(void) /* just remove the p! */
```

This is that type in parentheses:

```c
(void (*)(void))
```

This is a **cast** to that type (the type in parentheses, followed by a value):

```c
(void (*)(void))0
```

Still with me? so far we have the integer value 0 cast to a pointer-to-function-that-takes-no-arguments-and-returns-nothing.

The cast is an expression with pointer-to-function type. When you have one of those you can call it like this:

```c
(your expression here)(arguments to the function)
```

The first set of parentheses are just for precedence, and sometimes might not be needed (but this time they are). The end result:

```c
((void (*)(void))0)(/* no args */);
```

Takes the value 0, casts it to pointer-to-function-expecting-no-arguments-and-returning-nothing, and calls it, supply no arguments.



#### [A](https://stackoverflow.com/a/20357227)

The syntax to cast address to a function pointer and then call it would look like this:

```c
((void (*)(void))address)();
```

It might be clearer to do something like this though:

```c
void (*fptr)(void) = (void (*)(void))address;
fptr();
```

Said that `((void(*)(void))0)();` instruction is used to jump to 0 in firmwares usually. It is a bit improper because it actually calls in 0 instead of jumping to 0, but practically it won't make any difference (a fw hot reboot will be performed)

### stackoverflow [What is `void(*)(void *)` [duplicate]](https://stackoverflow.com/questions/12830860/what-is-voidvoid)



#### [A](https://stackoverflow.com/a/12830911)

It's a function pointer.

```c++
void (*destroy)(void *data)
```

`destroy` is a pointer to a function which returns `void` and takes a `void*` as an argument.

[cdecl.org](http://cdecl.org/) is a useful tool for discerning complex C declarations. Also, take a look at [the spiral rule](https://stackoverflow.com/questions/3707096/spiral-rule-and-declaration-follows-usage-for-parsing-c-expressions).





### `void(void)`

在[sizeof operator](https://en.cppreference.com/w/c/language/sizeof)的例子中看到了这个声明，其中的注释说它声明的是一个function

```c
    printf("sizeof(void(*)(void))  = %zu\n", sizeof(void(*)(void)));
    printf("sizeof(char[10])       = %zu\n", sizeof(char[10]));
//  printf("sizeof(void(void))     = %zu\n", sizeof(void(void))); // Error: function type
```

它是声明的function type，可以参看[Typedef Function Type](http://www.iso-9899.info/wiki/Typedef_Function_Type)



## iso-9899 [Typedef Function Type](http://www.iso-9899.info/wiki/Typedef_Function_Type)

There's no need to typedef pointers to function types, typedefing a function type makes things clearer.

'function' is a pointer to a function type:

```c
typedef int (*function)(int a, int b);
function pointer;
```

The alternative of a non-pointer function type is:

```c
typedef int function(int a, int b);
function *pointer;
```



#### Code Sample

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

typedef int function(int a, int b);

int call_function(function *p, int a, int b) {
    return p(a, b);
}

int main(void) {
    int sum;
    
    sum = call_function(&add, 10, 5);
    printf("add: %d\n", sum);
    
    sum = call_function(&sub, 10, 5);
    printf("sub: %d\n", sum);
    
    return 0;
}
```



## Function pointer and type aliasing

Function pointer and type aliasing有时候是比较容易混淆的，下面是一些例子:

### csdn [详解virtual table](https://blog.csdn.net/wangweixaut061/article/details/7019994) 

```C++
#include <iostream>
using namespace std;
class Base
{

public:

	virtual void f()
	{
		cout << "Base::f" << endl;
	}

	virtual void g()
	{
		cout << "Base::g" << endl;
	}

	virtual void h()
	{
		cout << "Base::h" << endl;
	}

};
int main()
{
	typedef void (*Fun)(void);

	Base b;

	Fun pFun = NULL;

	cout << "虚函数表地址：" << (int*) (&b) << endl;

	cout << "虚函数表的第一个函数地址:" << (int*) *(int*) (&b) << endl;

	// Invoke the first virtual function

	pFun = (Fun) *((int*) *(int*) (&b));

	pFun();
}
// g++ test.cpp -Wall -pendatic
```

1、上述例子就是混合了pointer to function、type aliasing的，如果熟练掌握，非常可能会理解错误

2、第一次看到: `(int*) *(int*) (&b)` 时，我以为 `(int*) *(int*)` 是pointer to function passing pointer to int and return pointer to int，这是错误的，，正确的pointer to function的写法如下:

```C++
int* (*PointerToFunction)(int*) 
```

它的准确含义是:

a、`(int*) (&b)` 转换为 `int*` 类型

b、读取值: `*(int*) (&b)`

c、`(int*) *(int*) (&b)` 将读取的值，再转换为 `int*` 类型


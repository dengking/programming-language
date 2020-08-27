# Declarations

总结一些平时遇到的比较难以理解的declaration。

对于type declaration，参见：

- `C-family-language\C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Type naming`

## [What does C expression ((void(*)(void))0)(); mean?](https://stackoverflow.com/questions/20357106/what-does-c-expression-voidvoid0-mean)

```C
((void(*)(void))0)();
```

So we have integer 0 type casting to this tricky type `(void(*))(void)` and then executing it. Source claims that this should work, but what does it actually?

This must be one of those C jokes like `#define TRUE FALSE`, I suppose.

### [A](https://stackoverflow.com/a/20357286)

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



### [A](https://stackoverflow.com/a/20357227)

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

## [What is `void(*)(void *)` [duplicate]](https://stackoverflow.com/questions/12830860/what-is-voidvoid)



### [A](https://stackoverflow.com/a/12830911)

It's a function pointer.

```c++
void (*destroy)(void *data)
```

`destroy` is a pointer to a function which returns `void` and takes a `void*` as an argument.

[cdecl.org](http://cdecl.org/) is a useful tool for discerning complex C declarations. Also, take a look at [the spiral rule](https://stackoverflow.com/questions/3707096/spiral-rule-and-declaration-follows-usage-for-parsing-c-expressions).





## `void(void)`

在[sizeof operator](https://en.cppreference.com/w/c/language/sizeof)的例子中看到了这个声明，其中的注释说它声明的是一个function

```c
    printf("sizeof(void(*)(void))  = %zu\n", sizeof(void(*)(void)));
    printf("sizeof(char[10])       = %zu\n", sizeof(char[10]));
//  printf("sizeof(void(void))     = %zu\n", sizeof(void(void))); // Error: function type
```

它是声明的function type，可以参看[Typedef Function Type](http://www.iso-9899.info/wiki/Typedef_Function_Type)



## [Typedef Function Type](http://www.iso-9899.info/wiki/Typedef_Function_Type)

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





### Code Sample

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
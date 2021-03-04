# sizeof 



## creference [sizeof operator](https://en.cppreference.com/w/c/language/sizeof)

Queries size of the object or type

Used when actual size of the object must be known

> NOTE: 在[这篇文章](https://stackoverflow.com/a/4162948)中提及，**`sizeof()` is a compile-time operator**. 所以，它的实现依赖于编译信息；



### Syntax

|                     |
| ------------------- |
| `sizeof( type )`    |
| `sizeof expression` |

Both versions return a value of type [size_t](https://en.cppreference.com/w/c/types/size_t).

1) Returns the size, in bytes, of the [object representation](https://en.cppreference.com/w/c/language/object#Object_representation) of *type*

2) Returns the size, in bytes, of the object representation of the type of *expression*. No implicit conversions are applied to *expression*.

### Notes

Depending on the computer architecture, a [byte](https://en.wikipedia.org/wiki/Byte) may consist of 8 or more bits, the exact number provided as [CHAR_BIT](https://en.cppreference.com/w/c/types/limits).

`sizeof(char)`, `sizeof(signed char)`, and `sizeof(unsigned char)` always return 1.

`sizeof` cannot be used with function types, incomplete types (including `void`), or [bit field](https://en.cppreference.com/w/c/language/bit_field) lvalues.

> NOTE:
>
> 下面演示了`sizeof` function type的例子:
>
> ```c++
> #include <stdio.h>
> 
> int main(void)
> {
> 	printf("sizeof(void(void))     = %zu\n", sizeof(void(void))); // Error: function type
> 	printf("sizeof(char[])         = %zu\n", sizeof(char[])); // Error: incomplete type
> 
> 	printf("sizeof main            = %zu\n", sizeof main); // Error: Function type
> }
> 
> ```
>
> 

When applied to an operand that has [structure](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) type, the result is the total number of bytes in such an object, including internal and trailing padding. The trailing padding is such that if the object were an element of an array, the alignment requirement of the next element of this array would be satisfied, in other words, `sizeof(T)` returns the size of an element of a `T[]` array.



If *type* is a [VLA](https://en.cppreference.com/w/c/language/array) type and changing the value of its size expression would not affect the result of `sizeof`, it is unspecified whether or not the size expression is evaluated.

Except if the type of *expression* is a [VLA](https://en.cppreference.com/w/c/language/array), (since C99)*expression* is not evaluated and the `sizeof` operator may be used in an integer [constant expression](https://en.cppreference.com/w/c/language/constant_expression).

> NOTE: 上面对`sizeof`和VLA进行了描述，上面的描述是非常不直接的，在[Why does sizeof(x++) not increment x?](https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x)的回答[A](https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x/8225813#8225813)中有较好的描述：
>
> > `sizeof` is a ***compile-time operator\***, so at the time of compilation `sizeof` and its operand get replaced by the result value. The *operand is **not evaluated** (except when it is a variable length array) at all; only the **type** of the result matters.*
>
> 需要注意的是，C++不支持VLA

Number of elements in any [array](https://en.cppreference.com/w/c/language/array) a including VLA (since C99) may be determined with the expression `sizeof a / sizeof a[0]`. Note that if `a` has pointer type (such as after array-to-pointer conversion of function parameter type adjustment), this expression would simply divide the number of bytes in a pointer type by the number of bytes in the pointed type.

> NOTE: 关于"`a` has pointer type"的情况，在文章stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)`#`5.3 Pitfall: Using the C idiom to get number of elements.中进行了详细的分析。

```C
#include <stdio.h>
 
int main(void)
{
    short x;
    // type argument:
    printf("sizeof(float)          = %zu\n", sizeof(float));
    printf("sizeof(void(*)(void))  = %zu\n", sizeof(void(*)(void)));
    printf("sizeof(char[10])       = %zu\n", sizeof(char[10]));
//  printf("sizeof(void(void))     = %zu\n", sizeof(void(void))); // Error: function type
//  printf("sizeof(char[])         = %zu\n", sizeof(char[])); // Error: incomplete type
 
    // expression argument:
    printf("sizeof 'a'             = %zu\n", sizeof 'a'); // type of 'a' is int
//  printf("sizeof main            = %zu\n", sizeof main); // Error: Function type
    printf("sizeof &main           = %zu\n", sizeof &main);
    printf("sizeof \"hello\"         = %zu\n", sizeof "hello"); // type is char[6]
    printf("sizeof x               = %zu\n", sizeof x);   // type of x is short
    printf("sizeof (x+1)           = %zu\n", sizeof(x+1)); // type of x+1 is int
}
```

> NOTE:上述程序的输出如下：
>
> ```c++
> sizeof(float)          = 4
> sizeof(void(*)(void))  = 8
> sizeof(char[10])       = 10
> sizeof 'a'             = 4
> sizeof &main           = 8
> sizeof "hello"         = 6
> sizeof x               = 2
> sizeof (x+1)           = 4
> ```
>
> 

> NOTE : `&main`中的`&`是[Member access operators](https://en.cppreference.com/w/c/language/operator_member_access)，它返回的是一个pointer；

> NOTE : 关于`void(*)(void))`和`void(void)`的理解，参看这篇文章：[What does C expression ((void(*)(void))0)(); mean?](https://stackoverflow.com/questions/20357106/what-does-c-expression-voidvoid0-mean)

## `sizeof` and unevaluated expressions

在cppreference [Expressions#Unevaluated expressions](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions)章节中，介绍了unevaluated expressions 的概念：

对于C++而言，由于它不支持VLA，所以在C++中，"the operands of `sizeof` operator are expressions that are not evaluated "

对于C而言，需要分情况进行讨论：

- 如果expression是VLA，不是
- 如果expression不是VLA，则和C++相同

### [Why does sizeof(x++) not increment x?](https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x)

Here is the code compiled in dev c++ windows:

```c
#include <stdio.h>

int main() {
    int x = 5;
    printf("%d and ", sizeof(x++)); // note 1
    printf("%d\n", x); // note 2
    return 0;
}
```

I expect `x` to be 6 after executing **note 1**. However, the output is:

```c
4 and 5
```

Can anyone explain why `x` does not increment after **note 1**?

#### [A](https://stackoverflow.com/a/8225835)

From the [C99 Standard](http://www.open-std.org/JTC1/sc22/wg14/www/docs/n1256.pdf) (the emphasis is mine)

> 6.5.3.4/2
>
> The sizeof operator yields the size (in bytes) of its operand, which may be an expression or the parenthesized name of a type. The size is determined from the type of the operand. The result is an integer. If the type of the operand is a variable length array type, the operand is evaluated; otherwise, **the operand is not evaluated** and the result is an integer constant.



 ### Examples



 ```c++
 #include <cstddef
 #include <iostream
 
 int main()
 {
 	std::size_t n = sizeof(std::cout << 42);
 	std::cout << n << std::endl;
 }
 // g++ test.cpp
 ```

 上述程序的输出为:

 ```
 272
 ```

 [Why does sizeof(x++) not increment x?](https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x):

 ```c
 #include <stdio.h
 
 int main()
 {
 	int x = 5;
 	printf("%d and ", sizeof(x++)); // note 1
 	printf("%d\n", x); // note 2
 	return 0;
 }
 // gcc test.c
 ```

 上述程序的输入：

 ```
 4 and 5
 ```

 https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x/8225813#8225813

 ```c
 #include <stdio.h
 
 short func(short x)
 {  // this function never gets called !!
 	printf("%d", x);    // this print never happens
 	return x;
 }
 
 int main()
 {
 	printf("%d", sizeof(func(3))); // all that matters to sizeof is the
 								   // return type of the function.
 	return 0;
 }
 // gcc test.c
 ```

 




## [Why is sizeof considered an operator?](https://stackoverflow.com/questions/1393582/why-is-sizeof-considered-an-operator)



### [A](https://stackoverflow.com/a/1393592)

Because the C standard says so, and it gets the only vote.

As consequences:

- The operand of `sizeof` can be a **parenthesised type**, `sizeof (int)`, instead of an object expression.

  ***SUMMARY*** : 用于类型必须要要加上parentheses

- The parentheses are unnecessary: `int a; printf("%d\n", sizeof a);` is perfectly fine. They're often seen, firstly because they're needed as part of a **type cast expression**, and secondly because `sizeof` has very high precedence, so `sizeof a + b` isn't the same as `sizeof (a+b)`. But they aren't part of the invocation of `sizeof`, they're part of the operand.

- You can't take the address of `sizeof`.

- The expression which is the operand of `sizeof` is not evaluated at runtime (`sizeof a++` does not modify `a`).

  ***SUMMARY*** : 关于这一点，参见下面的[Why does sizeof(x++) not increment x?](https://stackoverflow.com/questions/8225776/why-does-sizeofx-not-increment-x)

- The expression which is the operand of sizeof can have any type except void, or function types. Indeed, that's kind of the point of sizeof.

A function would differ on all those points. There are probably other differences between a function and a unary operator, but I think that's enough to show why sizeof could not be a function even if there was a reason to want it to be.

### [A](https://stackoverflow.com/a/1393604)

It can be used as a compile-time constant, which is only possible if it's an operator rather than a function. For instance:

```c
union foo {
    int i;
    char c[sizeof(int)];
};
```

Syntactically if it weren't an operator then it would have to be a preprocessor macro since functions can't take types as arguments. That would be a difficult macro to implement since `sizeof` can take both types and variables as an argument.

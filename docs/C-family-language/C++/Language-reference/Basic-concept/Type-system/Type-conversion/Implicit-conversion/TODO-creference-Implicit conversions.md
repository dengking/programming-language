# [Implicit conversions](https://en.cppreference.com/w/c/language/conversion)

When an expression is used in the context where a value of a different type is expected, *conversion* may occur:

```C
int n = 1L; // expression 1L has type long, int is expected
n = 2.1; // expression 2.1 has type double, int is expected
char *p = malloc(10); // expression malloc(10) has type void*, char* is expected
```

**Conversions** take place in the following situations:

***SUMMARY*** : 以下是**Conversions**发生的几种场景

- [Conversion as if by assignment](#Conversion as if by assignment)
- [Default argument promotions](#Default argument promotions)
- [Usual arithmetic conversions](#Usual arithmetic conversions)
- [Value transformations](#Value transformations)

### Conversion as if by assignment

- In the [assignment](https://en.cppreference.com/w/c/language/operator_assignment) operator, the value of the right-hand operand is converted to the unqualified type of the left-hand operand.
- In [scalar initialization](https://en.cppreference.com/w/c/language/scalar_initialization), the value of the initializer expression is converted to the unqualified type of the object being initialized
- In a [function-call expression](https://en.cppreference.com/w/c/language/operator_other), to a function that has a prototype, the value of each argument expression is converted to the type of the unqualified declared types of the corresponding parameter
- In a [return statement](https://en.cppreference.com/w/c/language/return), the value of the operand of `return` is converted to an object having the return type of the function

***SUMMARY*** : 后两种并没有显式地`=`，前两种是有显式的`=`，其实感觉 [assignment](https://en.cppreference.com/w/c/language/operator_assignment) 和  [scalar initialization](https://en.cppreference.com/w/c/language/scalar_initialization)是非常类似的；

### Default argument promotions

In a [function call expression](https://en.cppreference.com/w/c/language/operator_other#Function_call) when the call is made to

1) a [function without a prototype](https://en.cppreference.com/w/c/language/function_declaration)

2) a [variadic function](https://en.cppreference.com/w/c/language/variadic), where the **argument expression** is one of the trailing arguments that are matched against the ellipsis parameter

Each argument of **integer type** undergoes *integer promotion* (see below), and each argument of type `float` is implicitly converted to the type `double`

```c
int add_nums(int count, ...);
int sum = add_nums(2, 'c', true); // add_nums is called with three ints: (2, 99, 1)
```

***SUMMARY*** : 关于**integer type**，参见[Type](https://en.cppreference.com/w/c/language/type)。

Note that float [complex](http://en.cppreference.com/w/c/numeric/complex/complex) and float [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary) are not promoted to double [complex](http://en.cppreference.com/w/c/numeric/complex/complex) and double [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary) in this context.







### Usual arithmetic conversions

The arguments of the following arithmetic operators undergo **implicit conversions** for the purpose of obtaining the *common real type*, which is the type in which the calculation is performed:

***SUMMARY*** : 查看下面的转换规则可以发现上述的*common real type*其实就是更高精度的type，相当于ceiling了一把，非常类似于下面的[integer promotion](#Integer promotions)；

- [binary arithmetic](https://en.cppreference.com/w/c/language/operator_arithmetic) `*, /, %, +, -`
- [relational operators](https://en.cppreference.com/w/c/language/operator_comparison) `<, >, <=, >=, ==, !=`
- [binary bitwise arithmetic](https://en.cppreference.com/w/c/language/operator_arithmetic) `&, ^, |,`
- the [conditional operator](https://en.cppreference.com/w/c/language/operator_other) `?:`



1) If one operand is `long double`, `long double` [complex](http://en.cppreference.com/w/c/numeric/complex/complex), or `long double` [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary), the other operand is implicitly converted as follows:

- integer or real floating type to `long double`
- complex type to `long double` [complex](http://en.cppreference.com/w/c/numeric/complex/complex)
- imaginary type to `long double` [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary)

2) Otherwise, if one operand is `double`, `double` [complex](http://en.cppreference.com/w/c/numeric/complex/complex), or `double` [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary), the other operand is implicitly converted as follows:

- integer or real floating type to `double`
- complex type to `double` [complex](http://en.cppreference.com/w/c/numeric/complex/complex)
- imaginary type to `double` [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary)

3) Otherwise, if one operand is float, float [complex](http://en.cppreference.com/w/c/numeric/complex/complex), or float [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary), the other operand is implicitly converted as follows:

- integer type to float (the only real type possible is float, which remains as-is)
- complex type remains float [complex](http://en.cppreference.com/w/c/numeric/complex/complex)
- imaginary type remains float [imaginary](http://en.cppreference.com/w/c/numeric/complex/imaginary)

***SUMMARY*** : 1) 2) 3)描述的是浮点数的转换规则

4) Otherwise, both operands are integers. In that case,

First of all, both operands undergo [*integer promotions*](#Integer promotions) (see below). Then

- If the types after promotion are the same, that type is the common type
- Otherwise, if both operands after promotion have the same signedness (both signed or both unsigned), the operand with the lesser *conversion rank* (see below) is implicitly converted to the type of the operand with the greater *conversion rank*

- 















### Value transformations





#### Array to pointer conversion





#### Function to pointer conversion







### Implicit conversion semantics

***SUMMARY*** : 本节描述的implicit conversion的semantic，即语义

**Implicit conversion**, whether *as if by assignment* or a *usual arithmetic conversion*, consists of two stages:



1) value transformation (if applicable)

2) one of the conversions listed below (if it can produce the target type)



#### Compatible types

Conversion of a value of any type to any [compatible type](https://en.cppreference.com/w/c/language/types#Compatible_types) is always a no-op and does not change the **representation**.

***SUMMARY*** : 有些conversion是涉及到representation的变更的；

```c
uint8_t (*a)[10];         // if uint8_t is a typedef to unsigned char
unsigned char (*b)[] = a; // then these pointer types are compatible
```





#### Integer promotions

***SUMMARY*** : 注意这是promotion，而不是demotion。因为compiler需要保证不要截断而导致数值的改变；

**Integer promotion** is the **implicit conversion** of a value of any integer type with *rank* less or equal to *rank* of int or of a [bit field](https://en.cppreference.com/w/c/language/bit_field) of type `_Bool`, `int`, `signed int`, `unsigned int`, to the value of type int or `unsigned int`

If `int` can represent the entire range of values of the original type (or the range of values of the original bit field), the value is converted to type `int`. Otherwise the value is converted to `unsigned int`.

**Integer promotions** preserve the value, including the sign:





#### Boolean conversion





#### Integer conversions





#### Real floating-integer conversions





#### Real floating point conversions





#### Complex type conversions







#### Imaginary type conversions





#### Real-complex conversions





#### Real-imaginary conversions





#### Complex-imaginary conversions





#### Pointer conversions

A pointer to `void` can be implicitly converted to and from any **pointer** to object type with the following semantics:

- If a pointer to object is converted to a pointer to `void` and back, its value compares equal to the original pointer.
- No other guarantees are offered

***SUMMARY*** : `void *`显然只是用于保存address；显然保存于`void *`类型的address所执行的object的type的alignment requirement和`void *`类型的alignment requirement是不同的；

***SUMMARY*** : 此处仅仅描述了C中`void *`类型和其他类型的的转换，关于`void *`类型在C中的应用，参见《void in C》

```c
int* p = malloc(10 * sizeof(int)); // malloc returns void*
```

A pointer to an **unqualified type** may be implicitly converted to the pointer to qualified version of that type (in other words, [const](https://en.cppreference.com/w/c/language/const), [volatile](https://en.cppreference.com/w/c/language/volatile), and [restrict](https://en.cppreference.com/w/c/language/restrict) qualifiers can be added. The original pointer and the result compare equal.

```c
int n;
const int* p = &n; // &n has type int*
```

Any integer [constant expression](https://en.cppreference.com/w/c/language/constant_expression) with value 0 as well as **integer pointer expression** with value zero cast to the type `void*` can be implicitly converted to any pointer type (both pointer to object and pointer to function). The result is the **null pointer value** of its type, guaranteed to compare unequal to any non-null pointer value of that type. This integer or `void*` expression is known as *null pointer constant* and the standard library provides one definition of this constant as the macro [NULL](https://en.cppreference.com/w/c/types/NULL) .

```c
int* p = 0;
double* q = NULL;
```



***ATTENTION*** : 

```C
#include <stdio.h>
#include <string.h>

int main(void) {
    int i = 1;
    void* p = i;

}
```

整型是可以cast到pointer类型的，在[cast operator](https://en.cppreference.com/w/c/language/cast)中有如下描述：

> Any integer can be cast to any pointer type. Except for the null pointer constants such as [NULL](https://en.cppreference.com/w/c/types/NULL) (which [doesn't need a cast](https://en.cppreference.com/w/c/language/conversion)), the result is implementation-defined, may not be correctly aligned, may not point to an object of the referenced type, and may be a [trap representation](https://en.cppreference.com/w/c/language/object).

所以上面的程序是可以编译通过的，但是会有如下的warning：

```
test.c: 在函数‘main’中:
test.c:6:15: 警告：将一个整数转换为大小不同的指针 [-Wint-to-pointer-cast]
     void* p = (void*)i;
```



下面的程序就无法编译通过

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    double i = 1;
    void* p = (void*)i;
    
}

```

```
test.c: 在函数‘main’中:
test.c:6:5: 错误：无法转换为指针类型
     void* p = (void*)i;
```



### Notes
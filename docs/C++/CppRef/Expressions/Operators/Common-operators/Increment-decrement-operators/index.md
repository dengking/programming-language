# Increment-decrement operators



## cppreference [Increment/decrement operators](Increment/decrement operators)



| Operator name  | Syntax | [Overloadable](https://en.cppreference.com/w/cpp/language/operators) |                         |                          |
| :------------: | :----: | :----------------------------------------------------------: | :---------------------: | ------------------------ |
|                |        |                                                              | Inside class definition | Outside class definition |
| pre-increment  | `++a`  |                             Yes                              |   T& T::operator++();   | T& operator++(T& a);     |
| pre-decrement  | `--a`  |                             Yes                              |   T& T::operator--();   | T& operator--(T& a);     |
| post-increment | `a++`  |                             Yes                              |  T T::operator++(int);  | T operator++(T& a, int); |
| post-decrement | `a--`  |                             Yes                              |  T T::operator--(int);  | T operator--(T& a, int); |

一、Prefix versions of the built-in operators return *references* and postfix versions return *values*, and typical [user-defined overloads](https://en.cppreference.com/w/cpp/language/operators) follow the pattern so that the user-defined operators can be used in the same manner as the built-ins. However, in a user-defined operator overload, any type can be used as return type (including void).

> NOTE:
>
> 一、对于最后一段话的验证如下:
>
> Example 1: return object
>
> ```c++
> #include <algorithm>
> #include <vector>
> #include <iostream>
> 
> class Foo
> {
> public:
>     int i = 0;
>     Foo &operator++()
>     {
>         std::cout << "prefix-increment-operator" << std::endl;
>         ++i;
>         return *this;
>     }
>     Foo operator++(int)
>     {
>         std::cout << "postfix-increment-operator" << std::endl;
>         ++i;
>         return *this;
>     }
> };
> int main()
> {
>     Foo f;
>     ++f;
>     std::cout << f.i << std::endl;
>     f++;
>     std::cout << f.i << std::endl;
> }
> ```
>
> Example 2: return void
>
> ```c++
> #include <algorithm>
> #include <vector>
> #include <iostream>
> 
> class Foo
> {
> public:
>     int i = 0;
>     void operator++()
>     {
>         std::cout << "prefix-increment-operator" << std::endl;
>         ++i;
>     }
>     void operator++(int)
>     {
>         std::cout << "postfix-increment-operator" << std::endl;
>         ++i;
>     }
> };
> int main()
> {
>     Foo f;
>     ++f;
>     std::cout << f.i << std::endl;
>     f++;
>     std::cout << f.i << std::endl;
> }
> ```
>
> Example 2: return int
>
> ```c++
> #include <algorithm>
> #include <vector>
> #include <iostream>
> 
> class Foo
> {
> public:
>     int i = 0;
>     int &operator++()
>     {
>         std::cout << "prefix-increment-operator" << std::endl;
>         return ++i;
>     }
>     int operator++(int)
>     {
>         std::cout << "postfix-increment-operator" << std::endl;
>         return ++i;
>     }
> };
> int main()
> {
>     Foo f;
>     ++f;
>     std::cout << f.i << std::endl;
>     f++;
>     std::cout << f.i << std::endl;
> }
> ```
>
> 

二、The int parameter is a **dummy parameter** used to differentiate between prefix and postfix versions of the operators. When the user-defined postfix operator is called, the value passed in that parameter is always zero, although it may be changed by calling the operator using function call notation (e.g., a.operator++(2) or operator++(a, 2)).

### Notes

Because of the side-effects involved, built-in increment and decrement operators must be used with care to avoid **undefined behavior** due to violations of [sequencing rules](https://en.cppreference.com/w/cpp/language/eval_order).

> NOTE:
>
> 一、关于上面这段话，在 stackoverflow [Post-increment and Pre-increment concept?](https://stackoverflow.com/questions/4445706/post-increment-and-pre-increment-concept) # [A](https://stackoverflow.com/a/4445841) 中有着详细的说明:
>
> Believing that "urban legend" has led many a novice (and professional) astray, to wit, the endless stream of questions about Undefined Behavior in expressions.
>
> So.
>
> For the built-in C++ prefix operator,
>
> ```cpp
> ++x
> ```
>
> increments `x` and produces (as the expression's result) `x` as an lvalue, while
>
> ```cpp
> x++
> ```
>
> increments `x` and produces (as the expression's result) the original value of `x`.
>
> In particular, for `x++` there is no *no time ordering* implied for the increment and production of original value of `x`. The compiler is free to emit machine code that produces the original value of `x`, e.g. it might be present in some register, and that delays the increment until the end of the expression (next sequence point).
>
> Folks who incorrectly believe the increment must come first, and they are many, often conclude from that certain expressions must have well defined effect, when they actually have Undefined Behavior.

Because a **temporary copy** of the object is constructed during post-increment and post-decrement, *pre-increment* or *pre-decrement* operators are usually more efficient in contexts where the returned value is not used.

> NOTE: 
>
> 一、上面这段话是从efficiency的角度来比较两种operator

## creference [Increment/decrement operators](Increment/decrement operators)

Increment/decrement operators are unary operators that increment/decrement the value of a variable by 1.

They can have **postfix** form:

```C
expr ++		
expr --		

```

As well as the **prefix** form:

```
++ expr		
-- expr		

```

The operand *expr* of both prefix and postfix increment or decrement must be a [modifiable lvalue](https://en.cppreference.com/w/c/language/value_category) of [integer type](https://en.cppreference.com/w/c/language/type)(including `_Bool` and enums), real floating type, or a pointer type. It may be cvr-qualified, unqualified, or [atomic](https://en.cppreference.com/w/c/language/atomic).

The result of the **postfix** increment and decrement operators is the value of *expr*.

The result of the **prefix** increment operator is the result of adding the value `1` to the value of *expr*: the expression ++e is equivalent to e+=1. The result of the prefix decrement operator is the result of subtracting the value `1` from the value of *expr*: the expression --e is equivalent to e-=1.

> NOTE: 需要注意的是：`++ expr`、`expr ++`、`-- expr`、`expr --`都是expression，它们有自己的value；

> NOTE: 上面的这段话已经总结了postfix和prefix之间的差异所在了；

**Increment operators** initiate the **side-effect** of adding the value `1` of appropriate type to the operand. Decrement operators initiate the **side-effect** of subtracting the value `1` of appropriate type from the operand. As with any other side-effects, these operations complete at or before the next [sequence point](https://en.cppreference.com/w/c/language/eval_order). 

```c
int a = 1;
int b = a++; // stores 1+a (which is 2) to a
             // returns the value of a (which is 1)
             // After this line, b == 1 and a == 2
a = 1;
int c = ++a; // stores 1+a (which is 2) to a
             // returns 1+a (which is 2)
             // after this line, c == 2 and a == 2
```

Post-increment or post-decrement on any [atomic variable](https://en.cppreference.com/w/c/language/atomic) is an atomic read-modify-write operation with memory order [memory_order_seq_cst](https://en.cppreference.com/w/c/atomic/memory_order).



See [arithmetic operators](https://en.cppreference.com/w/c/language/operator_arithmetic) for limitations on pointer arithmetic, as well as for implicit conversions applied to the operands.

> NOTE: [C: What is the difference between ++i and i++?](https://stackoverflow.com/questions/24853/c-what-is-the-difference-between-i-and-i) 中的回答 [A](https://stackoverflow.com/a/24874)

> The reason `++i` *can* be slightly faster than `i++` is that `i++` can require a local copy of the value of i before it gets incremented, while `++i` never does. In some cases, some compilers will optimize it away if possible... but it's not always possible, and not all compilers do this.

> I try not to rely too much on compilers optimizations, so I'd follow Ryan Fox's advice: when I can use both, I use `++i`.

### Notes

Because of the **side-effects** involved, increment and decrement operators must be used with care to avoid **undefined behavior** due to violations of [sequencing rules](https://en.cppreference.com/w/c/language/eval_order).

Increment/decrement operators are not defined for complex or imaginary types: the usual definition of adding/subtracting the real number 1 would have no effect on imaginary types, and making it add/subtract `i` for imaginaries but `1` for complex numbers would have made it handle `0+yi` different from `yi`.

Unlike `C++` (and some implementations of C), the increment/decrement expressions are never themselves lvalues: `&++a` is invalid.





## stackoverflow [C: What is the difference between ++i and i++?](https://stackoverflow.com/questions/24853/c-what-is-the-difference-between-i-and-i)



### [A](https://stackoverflow.com/a/24858)

1、`++i` will increment the value of `i`, and then return the incremented value.

```c
 i = 1;
 j = ++i;
 (i is 2, j is 2)
```

2、`i++` will increment the value of `i`, but return the original value that `i` held before being incremented.

```c
 i = 1;
 j = i++;
 (i is 2, j is 1)
```

For a `for` loop, either works. `++i` seems more common, perhaps because that is what is used in [K&R](https://en.wikipedia.org/wiki/The_C_Programming_Language).

In any case, follow the guideline "prefer `++i` over `i++`" and you won't go wrong.

There's a couple of comments regarding the efficiency of `++i` and `i++`. In any non-student-project compiler, there will be no performance difference. You can verify this by looking at the generated code, which will be identical.

The efficiency question is interesting... here's my attempt at an answer: [Is there a performance difference between i++ and ++i in C?](https://stackoverflow.com/questions/24886/is-there-a-performance-difference-between-i-and-i)

As *On Freund* notes, it's different for a C++ object, since `operator++()` is a function and the compiler can't know to optimize away the creation of a temporary object to hold the intermediate value.



### [A](https://stackoverflow.com/a/24874)

The reason `++i` *can* be slightly faster than `i++` is that `i++` can require a local copy of the value of i before it gets incremented, while `++i` never does. In some cases, some compilers will optimize it away if possible... but it's not always possible, and not all compilers do this.

I try not to rely too much on compilers optimizations, so I'd follow Ryan Fox's advice: when I can use both, I use `++i`.


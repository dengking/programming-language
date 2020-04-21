# Increment/decrement operators



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

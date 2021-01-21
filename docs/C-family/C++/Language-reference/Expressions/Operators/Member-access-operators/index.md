# Member access operators



## cppreference [Member access operators](https://en.cppreference.com/w/cpp/language/operator_member_access)

Accesses a member of its operand.



### Explanation

> NOTE:  下面两类是比较陌生的

| **Operator name**            | **Syntax** | [Overloadable](https://en.cppreference.com/w/cpp/language/operators) | **Inside class definition** | **Outside class definition** |
| ---------------------------- | ---------- | ------------------------------------------------------------ | --------------------------- | ---------------------------- |
| member of object             | `a.b`      | No                                                           | N/A                         | N/A                          |
| member of pointer            | `a->b`     | Yes                                                          | `R* T::operator->();`       | N/A                          |
| pointer to member of object  | `a.*b`     | No                                                           | N/A                         | N/A                          |
| pointer to member of pointer | `a->*b`    | Yes                                                          | `R& T::operator->*(S b);`   | `R& operator->*(T a, S b);`  |

Built-in *member of pointer* and *pointer to member of pointer* operators provide access to a **data member** or **member function** of the class pointed-to by the pointer operand.

### Built-in member access operators



### Built-in pointer-to-member access operators





# draft

# [Member access operators](https://en.cppreference.com/w/c/language/operator_member_access)



**Member access operators** allow access to the members of their operands.

| Operator |         Operator name         | Example |                         Description                          |
| :------: | :---------------------------: | :-----: | :----------------------------------------------------------: |
|    []    |        array subscript        |  a[b]   |          access the **b**th element of array **a**           |
|    *     |      pointer dereference      |   *a    | dereference the pointer **a** to access the object or function it refers to |
|    &     |          address of           |   &a    | create a pointer that refers to the object or function **a** |
|    .     |         member access         |   a.b   | access member **b** of [struct](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) **a** |
|    ->    | member access through pointer |  a->b   | access member **b** of [struct](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) pointed to by **a** |



### Subscript

The **array subscrpt expression** has the form

```

pointer-expression [ integer-expression ]	(1)	
integer-expression [ pointer-expression ]	(2)	

```

where

| *pointer-expression* | -    | an [expression](https://en.cppreference.com/w/c/language/expressions) of type pointer to complete object |
| -------------------- | ---- | ------------------------------------------------------------ |
| *integer-expression* | -    | an [expression](https://en.cppreference.com/w/c/language/expressions) of integer type |

The **subscript operator expression** is an [lvalue expression](https://en.cppreference.com/w/c/language/value_category) whose type is the type of the object pointed to by *pointer-expression*.

***SUMMARY*** : 这句话的意思是`(1)`个`(2)`的结果是一个 [lvalue expression](https://en.cppreference.com/w/c/language/value_category) 。

By definition, the subscript operator `E1[E2]` is exactly identical to `*((E1)+(E2))`. If *pointer-expression* is an array expression, it undergoes [lvalue-to-rvalue conversion](https://en.cppreference.com/w/c/language/conversion) and becomes a **pointer to the first element of the array**.

***SUMMARY*** : 上述解释了`[]`的真实含义；

Due to the definition of the [addition between a pointer and an integer](https://en.cppreference.com/w/c/language/operator_arithmetic), the result is the element of the array with the index equal to the result of *`integer-expression`* (or, if *`pointer-expression`* was pointing at ith element of some array, the index of the result is i plus the result of *`integer-expression`*)

Note: see [array](https://en.cppreference.com/w/c/language/array) for the details on multidimensional arrays.

```c
#include <stdio.h>
int main(void)
{
    int a[3] = {1,2,3};
    printf("%d %d\n", a[2],  // n == 3
                      2[a]); // same, n == 3
    a[2] = 7; // subscripts are lvalues
 
    int n[2][3] = {1,2,3,4,5,6};
    int (*p)[3] = &n[1]; // elements of n are arrays
    int x = n[1][2]; // applying [] again to the array n[1]
 
    printf("%c %c\n", "abc"[2], 2["abc"]); // string literals are arrays too
}
```



***SUMMARY*** : [Subscript](#Subscript)其实就是[Dereference](#Dereference)。

### Dereference

The *dereference* or *indirection* expression has the form

```

* pointer-expression		

```

where

| *pointer-expression* | -    | an [expression](https://en.cppreference.com/w/c/language/expressions) of any pointer type |
| -------------------- | ---- | ------------------------------------------------------------ |
|                      |      |                                                              |

If *pointer-expression* is a pointer to function, the result of the dereference operator is a function designator for that function.

If *pointer-expression* is a pointer to object, the result is an [lvalue expression](https://en.cppreference.com/w/c/language/value_category) that designates the pointed-to object.

Dereferencing a **null pointer**, a pointer to an object outside of its lifetime (a **dangling pointer**), a **misaligned pointer**, or **a pointer with indeterminate value** is **undefined behavior**, except when the **dereference operator** is nullified by applying the address-of operator to its result, as in `&*E`

***SUMMARY*** : 关于为什么 `&*E`可以，参见[Address of](#Address of)中的解释；

```c
#include <stdio.h>
int main(void)
{
    int n = 1;
    int* p = &n;
    printf("*p = %d\n", *p); // the value of *p is what's stored in n
    *p = 7; // *p is lvalue
    printf("*p = %d\n", *p);
}
```



### Address of

The **address-of expression** has the form

```

& function	(1)	
& lvalue-expression	(2)	
& * expression	(3)	
& expression [ expression ]	(4)	

```

1) address of a function

2) address of an object

3) special case: `&` and `*` cancel each other, neither one is evaluated

4) special case: `&` and the `*` that is implied in `[]` cancel each other, only the **addition** implied in `[]` is evaluated

***SUMMARY*** : 最终返回的是`a+N`

***SUMMARY*** : 4)的一个例子就是[Subscript](#Subscript)中；

| *lvalue-expression* | -    | an [lvalue](https://en.cppreference.com/w/c/language/value_category) expression of any type that is not a [bit field](https://en.cppreference.com/w/c/language/bit_field) and does not have [register](https://en.cppreference.com/w/c/language/storage_duration)storage class |
| ------------------- | ---- | ------------------------------------------------------------ |
|                     |      |                                                              |

The address-of operator produces the [non-lvalue](https://en.cppreference.com/w/c/language/value_category) address of its operand, suitable for initializing a pointer to the type of the operand. If the operand is a function designator ((1)), the result is a pointer to function. If the operand is an object ((2)), the result is a pointer to object.

If the operand is the **dereference operator**, no action is taken (so it's okay to apply `&*` to a **null pointer**), except that the result is not an lvalue.

If the operand is an **array index expression**, no action is taken other than the **array-to-pointer conversion** and the **addition**, so `&a[N]` is valid for an array of size N (obtaining a pointer one past the end is okay, dereferencing it is not, but dereference cancels out in this expression).



### Member access

The member access expression has the form

```
expression . member-name
```

where

| *expression*  | -    | an expression of [struct](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) type |
| ------------- | ---- | ------------------------------------------------------------ |
| *member-name* | -    | an [identifier](https://en.cppreference.com/w/c/language/identifier) that names a member of the struct or union designated by *expression* |

The **member access expression** designates the named member of the [struct](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) designated by its **left operand**. It has the same [value category](https://en.cppreference.com/w/c/language/value_category) as its left operand.

If the **left operand** is [const](https://en.cppreference.com/w/c/language/const) or [volatile](https://en.cppreference.com/w/c/language/volatile) qualified, the result is also qualified. If the left operand is [atomic](https://en.cppreference.com/w/c/language/atomic), the behavior is undefined.

***SUMMARY*** : 并没有搞懂最后一句话的意思

Note: besides identifiers that name objects of `struct` or union type, the following expressions may have `struct` or union types: [assignment](https://en.cppreference.com/w/c/language/operator_assignment), [function call](https://en.cppreference.com/w/c/language/operator_other#Function_call), [comma operator](https://en.cppreference.com/w/c/language/operator_other#Comma_operator), [conditional operator](https://en.cppreference.com/w/c/language/operator_other#Conditional_operator), and [compound literal](https://en.cppreference.com/w/c/language/compound_literal).

```c
#include <stdio.h>
struct s {int x;};
struct s f(void) { return (struct s){1}; }
int main(void)
{
    struct s s;
    s.x = 1; // ok, changes the member of s
    int n = f().x; // f() is an expression of type struct s
//  f().x = 1; // Error: this member access expression is not an lvalue
 
    const struct s sc;
//  sc.x = 3;  // Error: sc.x is const, can't be assigned
 
    union { int x; double d; } u = {1};
    u.d = 0.1; // changes the active member of the union
}
```
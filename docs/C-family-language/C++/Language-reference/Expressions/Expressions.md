[TOC]

# [Expressions](https://en.cppreference.com/w/cpp/language/expressions)

An expression is a sequence of *operators* and their *operands*, that specifies a **computation**.

**Expression evaluation** may produce a **result** (e.g., evaluation of 2+2 produces the result 4) and may generate **side-effects** (e.g. evaluation of [std::printf](http://en.cppreference.com/w/cpp/io/c/fprintf)("%d",4) prints the character '4' on the standard output).

#### General

- [value categories](https://en.cppreference.com/w/cpp/language/value_category) (lvalue, rvalue, glvalue, prvalue, xvalue) classify expressions by their values
- [order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) of arguments and subexpressions specify the order in which intermediate results are obtained

***SUMMARY*** : python中没有value categories一说

### Operators

#### **Common operators**

| [assignment](https://en.cppreference.com/w/cpp/language/operator_assignment) | [increment decrement](https://en.cppreference.com/w/cpp/language/operator_incdec) | [arithmetic](https://en.cppreference.com/w/cpp/language/operator_arithmetic) | [logical](https://en.cppreference.com/w/cpp/language/operator_logical) | [comparison](https://en.cppreference.com/w/cpp/language/operator_comparison) | [member access](https://en.cppreference.com/w/cpp/language/operator_member_access) | [other](https://en.cppreference.com/w/cpp/language/operator_other) |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| ```a = b a += b a -= b a *= b a /= b a %= b a &= b a |= b a ^= b a <<= b a >>= b``` | `++a --a a++ a--`                                            | `+a -a a + b a - b a * b a / b a % b ~a a & b a | b a ^ b a << b a >> b` | `!a a && b a || b`                                           | `a == b a != b a < b a > b a <= b a >= b a <=> b`            | a[b], *a &a a->b a.b a->*b a.*b``                            | `a(...) a, b ? :`                                            |

***SUMMARY*** ： python中assignment的含义是bind

***SUMMARY*** : python中叫做Attribute references[¶](https://docs.python.org/3.3/reference/expressions.html#attribute-references)，`c++`中叫做member access。

#### **Special operators**

[`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast) converts one type to another **related type** 
[`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast) converts within inheritance hierarchies 

***SUMMARY*** : 让我想起来python中的[type() and isinstance()](https://stackoverflow.com/questions/1549801/what-are-the-differences-between-type-and-isinstance)

[`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) adds or removes [cv](https://en.cppreference.com/w/cpp/language/cv) qualifiers
[`reinterpret_cast`](https://en.cppreference.com/w/cpp/language/reinterpret_cast) converts type to unrelated type
[C-style cast](https://en.cppreference.com/w/cpp/language/explicit_cast) converts one type to another by a mix of `static_cast`, `const_cast`, and `reinterpret_cast` 

***SUMMARY*** : 上述为与type conversion相关的函数，在下面的[Conversions](#Conversions)章节也进行了重复

[`new`](https://en.cppreference.com/w/cpp/language/new) creates objects with dynamic storage duration
[`delete`](https://en.cppreference.com/w/cpp/language/delete) destructs objects previously created by the new expression and releases obtained memory area

***SUMMARY*** : [Memory allocation](#Memory allocation)

[`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof) queries the size of a type
[`sizeof...`](https://en.cppreference.com/w/cpp/language/sizeof...) queries the size of a [parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack) (since C++11)
[`typeid`](https://en.cppreference.com/w/cpp/language/typeid) queries the type information of a type
[`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept) checks if an expression can throw an exception (since C++11)
[`alignof`](https://en.cppreference.com/w/cpp/language/alignof) queries alignment requirements of a type (since C++11)



- [operator precedence](https://en.cppreference.com/w/cpp/language/operator_precedence) defines the order in which operators are bound to their arguments
- [alternative representations](https://en.cppreference.com/w/cpp/language/operator_alternative) are alternative spellings for some operators
- [operator overloading](https://en.cppreference.com/w/cpp/language/operators) makes it possible to specify the behavior of the operators with user-defined classes.

#### Conversions

- [standard conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) implicit(隐式) conversions from one type to another
- [`const_cast` conversion](https://en.cppreference.com/w/cpp/language/const_cast)
- [`static_cast` conversion](https://en.cppreference.com/w/cpp/language/static_cast)
- [`dynamic_cast` conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)
- [`reinterpret_cast` conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)
- [explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast) conversion using C-style cast notation and functional notation
- [user-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator) makes it possible to specify conversion from user-defined classes



#### Memory allocation

- [new expression](https://en.cppreference.com/w/cpp/language/new) allocates memory dynamically
- [delete expression](https://en.cppreference.com/w/cpp/language/delete) deallocates memory dynamically



#### Other

- [constant expressions ](https://en.cppreference.com/w/cpp/language/constant_expression)can be evaluated at compile time and used in compile-time context (template arguments, array sizes, etc)
- [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof)
- [`alignof`](https://en.cppreference.com/w/cpp/language/alignof)
- [`typeid`](https://en.cppreference.com/w/cpp/language/typeid)
- [throw-expression](https://en.cppreference.com/w/cpp/language/throw)



### Primary expressions

The operands of any operator may be other **expressions** or **primary expressions** (e.g. in `1+2*3`, the operands of operator `+` are the subexpression `2*3` and the **primary expression** `1`).

***SUMMARY*** : python expression中也有Primaries[¶](https://docs.python.org/3.3/reference/expressions.html#primaries)

Primary expressions are any of the following:

1) Literals (e.g. 2 or "Hello, world")

2) Suitably declared [unqualified identifiers](https://en.cppreference.com/w/cpp/language/identifiers#Unqualified_identifiers) (e.g. `n` or `cout`)

3) Suitably declared [qualified identifiers](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers) (e.g. [std::string::npos](https://en.cppreference.com/w/cpp/string/basic_string/npos))

4) [Lambda-expressions](https://en.cppreference.com/w/cpp/language/lambda) (C++11)

5) [Fold-expressions](https://en.cppreference.com/w/cpp/language/fold) (C++17)

6) [Requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) (C++20)

Any expression in parentheses is also classified as a **primary expression**: this guarantees that the parentheses have higher precedence than any **operator**. Parentheses preserve **value**, **type**, and **value category**.

#### Literals

**Literals** are the tokens of a C++ program that represent constant values embedded in the source code.

- [integer literals](https://en.cppreference.com/w/cpp/language/integer_literal) are decimal, octal, hexadecimal or binary numbers of integer type.
- [character literals](https://en.cppreference.com/w/cpp/language/character_literal) are individual characters of type



- [floating-point literals](https://en.cppreference.com/w/cpp/language/floating_literal) are values of type float, double, or long double
- [string literals](https://en.cppreference.com/w/cpp/language/string_literal) are sequences of characters of type



- [boolean literals](https://en.cppreference.com/w/cpp/language/bool_literal) are values of type bool, that is true and false
- [`nullptr`](https://en.cppreference.com/w/cpp/language/nullptr) is the pointer literal which specifies a null pointer value (since C++11)
- [user-defined literals](https://en.cppreference.com/w/cpp/language/user_literal) are constant values of user-specified type (since C++11)

***SUMMARY*** : python expression中也有Literals[¶](https://docs.python.org/3.3/reference/expressions.html#literals)



### Unevaluated expressions

The **operands** of the operators [`typeid`](https://en.cppreference.com/w/cpp/language/typeid), [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof), [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept), and [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) (since C++11) are expressions that are not evaluated (unless they are **polymorphic glvalues** and are the operands of `typeid`), since these operators only query the **compile-time** properties of their operands. Thus, `std::size_t n = sizeof(std::cout << 42);` does not perform console output.

The unevaluated operands are considered to be *full expressions* even though they are syntactically operands in a larger expression (for example, this means that `sizeof(T())` requires an accessible `T::~T`)

The [requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) are also unevaluated expressions.An invocation of an [immediate function](https://en.cppreference.com/w/cpp/language/consteval) is always evaluated, even in an unevaluated operand.

### Discarded-value expressions

A *discarded-value expression* is an expression that is used for its side-effects only. The value calculated from such expression is discarded. Such expressions include the full expression of any [expression statement](https://en.cppreference.com/w/cpp/language/statements#Expression_statements), the left-hand argument of the built-in comma operator, or the argument of a cast-expression that casts to the type void.

Array-to-pointer and function-to-pointer **conversions** are never applied to the **value** calculated by a **discarded-value expression**. The **lvalue-to-rvalue conversion** is applied if and only if the expression is a [volatile-qualified](https://en.cppreference.com/w/cpp/language/cv) glvalue and has one of the following forms (built-in meaning required, possibly parenthesized)

- id-expression
- array subscript expression
- class member access expression
- indirection
- pointer-to-member operation
- conditional expression where both the second and the third operands are one of these expressions,
- comma expression where the right operand is one of these expressions.

In addition, if the lvalue is of volatile-qualified class type, a volatile copy-constructor is required to initialize the resulting **rvalue temporary**.

***SUMMARY*** : 上面这段话中的几个conversion，都是standard conversion。


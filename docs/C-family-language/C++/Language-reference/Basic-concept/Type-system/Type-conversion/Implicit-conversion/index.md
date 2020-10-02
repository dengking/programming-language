# Implicit conversions

在阅读本文前，建议先阅读accu [Overload Resolution - Selecting the Function](https://accu.org/journals/overload/13/66/kilpelainen_268/)，在`C++\Language-reference\Functions\Overload-resolution\accu-Overload-Resolution.md`中收录了这篇文章，并对其进行了详细注解。

## cppreference [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion)

Implicit conversions are performed whenever an expression of some type `T1` is used in context that does not accept that type, but accepts some other type `T2`; in particular:

> NOTE: 下面罗列了各种可能的场景

| situation                                                    | explanation                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| when the expression is used as the argument when calling a function that is declared with `T2` as parameter |                                                              |
| when the expression is used as an operand with an operator that expects `T2` | operator其实和前面所述的function是非常类似的（参见`C++\Language-reference\Expressions\Operators\index.md`中总结的“Operator is a kind of function”思想） |
| when initializing a new object of type `T2`, including `return` statement in a function returning `T2` | `return` statement应该说的是这种情况:<br> `T2 Foo(){ return T1 }` |
| when the expression is used in a `switch` statement (`T2` is integral type) |                                                              |
| when the expression is used in an `if` statement or a loop (`T2` is bool) |                                                              |



### Order of the conversions

> NOTE:
>
> | conversion sequence          | 注解 |
> | ---------------------------- | ---- |
> | Implicit conversion sequence |      |
> | Standard conversion sequence |      |
>
> 从下面的描述来看，Implicit conversion sequence 包含: 
>
> - Standard conversion sequence
> - User-defined conversion

#### Implicit conversion sequence

**Implicit conversion sequence** consists of the following, in this order:

| order | explanation                                 | 注解                                    |
| ----- | ------------------------------------------- | --------------------------------------- |
| 1)    | zero or one *standard conversion sequence*; |                                         |
| 2)    | zero or one *user-defined conversion*;      | 参见下面的“User-defined conversion”章节 |
| 3）   | zero or one *standard conversion sequence*. |                                         |

When considering the **argument** to a **constructor** or to a **user-defined conversion function**, only a **standard conversion sequence** is allowed (otherwise **user-defined conversions** could be effectively chained). 

> NOTE: 上述argument，说明是调用函数的时候；上述constructor、user-defined conversion function，说明是user-defined conversion。
>
> 上面这段话，没有具体的例子，难以准确弄清楚作者想要表达的含义。

When converting from one built-in type to another built-in type, only a **standard conversion sequence** is allowed.

#### Standard conversion sequence 

A **standard conversion sequence** consists of the following, in this order:

| order | explanation                                                  | 注解          |
| ----- | ------------------------------------------------------------ | ------------- |
| 1)    | zero or one conversion from the following set: <br>- *lvalue-to-rvalue conversion*<br/>- *array-to-pointer conversion*<br/>- *function-to-pointer conversion*; |               |
| 2)    | zero or one:<br>- *numeric promotion* <br/>- *numeric conversion*; |               |
| 3)    | zero or one *function pointer conversion*;                   | (since C++17) |
| 4)    | zero or one *qualification conversion*.                      |               |

#### User-defined conversion

| conversion                                                   | 注解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| **non-explicit** single-argument [converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) | 参见: `C++\Language-reference\Classes\Special-member-functions\Constructor\Converting-constructor`章节 |
| **non-explicit** [conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) call |                                                              |

> NOTE: 需要注意的是: 本文描述的是implicit conversion，因此它是不允许:
>
> - explicit single-argument converting function
> - explicit conversion function

#### Implicitly convertible

An expression `e` is said to be *implicitly convertible to `T2`* if and only if `T2` can be [copy-initialized](https://en.cppreference.com/w/cpp/language/copy_initialization) from `e`, that is the declaration `T2 t = e;` is well-formed (can be compiled), for some invented temporary `t`. Note that this is different from [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (`T2 t(e)`), where **explicit** constructors and conversion functions would additionally be considered.

| expression  | initialization                                               | conversion              |
| ----------- | ------------------------------------------------------------ | ----------------------- |
| `T2 t = e;` | [copy-initialized](https://en.cppreference.com/w/cpp/language/copy_initialization) | **implicit** conversion |
| `T2 t(e)`   | [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) | **explicit** conversion |



### Value transformations

#### Lvalue to rvalue conversion

#### Array to pointer conversion

#### Temporary materialization

#### Function to pointer

### Numeric promotions
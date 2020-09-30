# Implicit conversions

在阅读本文前，建议先阅读accu [Overload Resolution - Selecting the Function](https://accu.org/journals/overload/13/66/kilpelainen_268/)，在`C++\Language-reference\Functions\Overload-resolution\accu-Overload-Resolution.md`中收录了这篇文章，并对其进行了详细注解。

## cppreference [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion)

Implicit conversions are performed whenever an expression of some type `T1` is used in context that does not accept that type, but accepts some other type `T2`; in particular:

> NOTE: 下面罗列了各种可能的场景

| situation                                                    | explanation                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| when the expression is used as the argument when calling a function that is declared with `T2` as parameter |                                                              |
| when the expression is used as an operand with an operator that expects `T2` | operator其实和前面所述的function是非常类似的（参见`C++\Language-reference\Expressions\Operators\index.md`中总结的“Operator is a kind of function”思想） |
| when initializing a new object of type `T2`, including `return` statement in a function returning `T2` |                                                              |
| when the expression is used in a `switch` statement (`T2` is integral type) |                                                              |
| when the expression is used in an `if` statement or a loop (`T2` is bool) |                                                              |


# cppreference Expressions # [Discarded-value expressions](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions)

> NOTE: 
>
> 1、discarded-value expressions即“弃值表达式”。
>
> 2、需要注意的是：discarded-value expressions，不是unevaluated expressions，discarded-value expressions会被evaluated，但是它的value会被discard。

A *discarded-value expression* is an expression that is used for its **side-effects** only. The **value** calculated from such expression is discarded. Such expressions include 

1、the full expression of any [expression statement](https://en.cppreference.com/w/cpp/language/statements#Expression_statements), 

2、the left-hand argument of the [built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator), or 

> NOTE: 
>
> 1、后面有专门关于[built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)的描述；

3、the argument of a cast-expression that casts to the type `void`.

> NOTE: 
>
> 1、参见 `Cast-to-type-void` 章节

Array-to-pointer and function-to-pointer **conversions** are never applied to the **value** calculated by a **discarded-value expression**. The **lvalue-to-rvalue conversion** is applied if and only if the expression is a [volatile-qualified](https://en.cppreference.com/w/cpp/language/cv) glvalue and has one of the following forms (built-in meaning required, possibly parenthesized)

- [id-expression](https://en.cppreference.com/w/cpp/language/identifiers)
- array subscript expression
- class member access expression
- [indirection](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_indirection_operator)
- pointer-to-member operation
- conditional expression where both the second and the third operands are one of these expressions,
- comma expression where the right operand is one of these expressions.

In addition, if the lvalue is of volatile-qualified class type, a volatile copy-constructor is required to initialize the resulting **rvalue temporary**.

> NOTE: 
>
> 1、没有读懂


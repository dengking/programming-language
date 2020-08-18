# Expressions



## cppreference [Expressions](https://en.cppreference.com/w/cpp/language/expressions)

An expression is a sequence of *operators* and their *operands*, that specifies a **computation**.

> NOTE: 上述定义是从数学expression的角度出发来给出的定义，关于此可以参见`Theory\Programming-language-construct\Basic-Language-construct\Operator-expression-statement.md`，其中对这个话题进行了详细分析。
>
> 那c++语言中的expression是否仅限于此呢？不是的，在cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category)中给出了详细的说明：
>
> Each C++ [expression](https://en.cppreference.com/w/cpp/language/expressions) (an operator with its operands, a literal, a variable name, etc.) 
>
> 显然，在c++中，我们可以将literal、variable name都看做是expression。



### General

- [value categories](https://en.cppreference.com/w/cpp/language/value_category) (lvalue, rvalue, glvalue, prvalue, xvalue) classify expressions by their values
- [order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) of arguments and subexpressions specify the order in which intermediate results are obtained
- [operator precedence](https://en.cppreference.com/w/cpp/language/operator_precedence) defines the order in which operators are bound to their arguments
- [alternative representations](https://en.cppreference.com/w/cpp/language/operator_alternative) are alternative spellings for some operators
- [operator overloading](https://en.cppreference.com/w/cpp/language/operators) makes it possible to specify the behavior of the operators with user-defined classes.

> NOTE: python中没有value categories一说，相比之下，python的data model是简单地多的。其他的相关问题，python中都有。

> NOTE: 上述后三条在原文中和前两条不在一起，我觉得它们是所有的operator都会涉及到的问题，所以我将它们放到了一起。

### Operators

> NOTE: 
>
> 原文对c++语言中的operator的分类方法如下：
>
> operator可以分为两大类：
>
> - Common operators
> - Special operators
>
> Special operators可以进一步细分为：
>
> - Conversions
> - Memory allocation
> - Other
>
> 对c++语言中的operator的还可以采用另外的分类方法：
>
> 根据compile-time或run-time来对operator进行分类：
>
> compile-time operator：
>
> - `sizeof`
>
> - `decltype`
>
>   ......
>
>   在原文的[Unevaluated expressions](https://en.cppreference.com/w/cpp/language/expressions)章节对此进行了说明。

#### Common operators

> NOTE： python中assignment的含义是bind。

> NOTE : python中叫做Attribute references[¶](https://docs.python.org/3.3/reference/expressions.html#attribute-references)，`c++`中叫做member access。

#### Special operators

> NOTE: 下面是对special operator的分类。

##### Conversions

- [standard conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) implicit(隐式) conversions from one type to another
- [`const_cast` conversion](https://en.cppreference.com/w/cpp/language/const_cast)
- [`static_cast` conversion](https://en.cppreference.com/w/cpp/language/static_cast)
- [`dynamic_cast` conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)
- [`reinterpret_cast` conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)
- [explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast) conversion using C-style cast notation and functional notation
- [user-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator) makes it possible to specify conversion from user-defined classes

> NOTE: 放到了type system章节

##### Memory allocation

- [new expression](https://en.cppreference.com/w/cpp/language/new) allocates memory dynamically
- [delete expression](https://en.cppreference.com/w/cpp/language/delete) deallocates memory dynamically



##### Other

- [constant expressions ](https://en.cppreference.com/w/cpp/language/constant_expression)can be evaluated at compile time and used in compile-time context (template arguments, array sizes, etc)

  > NOTE: 非常重要的概念

- [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof)

- [`alignof`](https://en.cppreference.com/w/cpp/language/alignof)

- [`typeid`](https://en.cppreference.com/w/cpp/language/typeid)

- [throw-expression](https://en.cppreference.com/w/cpp/language/throw)



### Primary expressions

The operands of any operator may be other **expressions** or **primary expressions** (e.g. in `1+2*3`, the operands of operator `+` are the subexpression `2*3` and the **primary expression** `1`).

> NOTE: python expression中也有Primaries[¶](https://docs.python.org/3.3/reference/expressions.html#primaries)的概念

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

> NOTE: python expression中也有Literals[¶](https://docs.python.org/3.3/reference/expressions.html#literals)



### Unevaluated expressions

The **operands** of the operators [`typeid`](https://en.cppreference.com/w/cpp/language/typeid), [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof), [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept), and [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) (since C++11) are expressions that are not evaluated (unless they are **polymorphic glvalues** and are the operands of `typeid`), since these operators only query the **compile-time** properties of their operands. Thus, `std::size_t n = sizeof(std::cout << 42);` does not perform console output.

> NOTE: 
>
> 由于`typeid`、`sizeof`、`noexcept`、`decltype`都是compile-time operator（ `typeid`比较特殊，上面这段话对此进行了补充说明），所以当它们的operand是expression的时候，这些operator仅仅会使用expression的compile-time property，所以这些expression是不会被evaluate的，这就是本节所描述的unevaluated expressions。
>
> 我们可以从对立面来理解它，以function-call operator为例：
>
> ```c++
> #include <iostream>
> int i = 1;
> void F(int i)
> {
> std::cout<<i<<std::endl;
> }
> int main()
> {
> int i = 1;
> F(i+1);
> }
> ```
>
> 上述程序的输出为`2`，显然function-call operator的operand `i+1`被evaluated了，它是run-time的。
>
> #### Example: `sizeof`
>
> 在文章`C++\Language-reference\Expressions\Operators\sizeof.md`中给出了`sizeof`和Unevaluated expressions之间的例子。

> NOTE: 
>
> #### Unevaluated expressions and built-in comma operator
>
> `decltype` 的 operand 可以是build-in command operator构成的expression，build-in command operator的第一个operand是discard value的，说明它是被evaluated的，那这要如何来进行理解呢？

> NOTE: 上面这一段提示了我们：有些operator在compile-time进行计算的，而有些是在run-time进行计算的，可以将此作为对operator的分类方法；上面这一段对[`typeid`](https://en.cppreference.com/w/cpp/language/typeid)进行了特殊说明，它表示`typeid`也可能是run-time。

The unevaluated operands are considered to be *full expressions* even though they are syntactically operands in a larger expression (for example, this means that `sizeof(T())` requires an accessible `T::~T`). (since C++14)

> NOTE: “for example, this means that `sizeof(T())` requires an accessible `T::~T`”没有搞懂。

The [requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) are also unevaluated expressions. An invocation of an [immediate function](https://en.cppreference.com/w/cpp/language/consteval) is always evaluated, even in an unevaluated operand.(since C++20)

> NOTE: 各种特殊的规则，使得c++语言比较复杂。

### Discarded-value expressions

> NOTE: discarded-value expressions即“弃值表达式”。需要注意的是：discarded-value expressions，不是unevaluated expressions，discarded-value expressions会被evaluated，但是它的value会被discard。

A *discarded-value expression* is an expression that is used for its **side-effects** only. The **value** calculated from such expression is discarded. Such expressions include the full expression of any [expression statement](https://en.cppreference.com/w/cpp/language/statements#Expression_statements), the left-hand argument of the [built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator), or the argument of a cast-expression that casts to the type `void`.

> NOTE: 
>
> #### built-in comma operator
>
> 后面有专门关于[built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)的描述；
>
> #### cast-expression that casts to the type `void`
>
> 可以利用“the argument of a cast-expression that casts to the type `void` is a discard-value expression”来实现一些效果，下面是一些案例：
>
> - [Expression SFINAE on the return type](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)
>
>   > A common idiom is to use expression SFINAE on the return type, where the expression uses the comma operator, whose left subexpression is the one that is being examined (cast to `void` to ensure the user-defined operator comma on the returned type is not selected), and the right subexpression has the type that the function is supposed to return.
>
> - https://stackoverflow.com/a/1486931 silence a warning about unused variables
>
>   ```c
>   // silence a warning about unused variables，https://stackoverflow.com/a/1486931
>   #define UNUSED(expr) do { (void)(expr); } while (0)
>   ```
>
>
> Example: 在cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)的`4)`就是说明的discard value expression，Example也对此进行了说明。

Array-to-pointer and function-to-pointer **conversions** are never applied to the **value** calculated by a **discarded-value expression**. The **lvalue-to-rvalue conversion** is applied if and only if the expression is a [volatile-qualified](https://en.cppreference.com/w/cpp/language/cv) glvalue and has one of the following forms (built-in meaning required, possibly parenthesized)

- [id-expression](https://en.cppreference.com/w/cpp/language/identifiers)
- array subscript expression
- class member access expression
- [indirection](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_indirection_operator)
- pointer-to-member operation
- conditional expression where both the second and the third operands are one of these expressions,
- comma expression where the right operand is one of these expressions.

In addition, if the lvalue is of volatile-qualified class type, a volatile copy-constructor is required to initialize the resulting **rvalue temporary**.





## Full list of c++ operator

c++中有哪些operator？下面对此进行枚举：

在cppreference [C++ Operator Precedence](https://en.cppreference.com/w/cpp/language/operator_precedence)中枚举了几乎所有的c++ operator。

## Supplement

下面是在阅读cppreference时遇到的，需要进行补充的：

- [id-expression](https://en.cppreference.com/w/cpp/language/identifiers)，此处的id的含义是identifier


# cppreference [Expressions](https://en.cppreference.com/w/cpp/language/expressions)

An expression is a sequence of *operators* and their *operands*, that specifies a **computation**.

> NOTE: 上述定义是从数学expression的角度出发来给出的定义，关于此可以参见`Theory\Programming-language-construct\Basic-Language-construct\Operator-expression-statement`，其中对这个话题进行了详细分析。
>
> 那c++语言中的expression是否仅限于此呢？不是的，在cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category)中给出了详细的说明：
>
> Each C++ [expression](https://en.cppreference.com/w/cpp/language/expressions) (an operator with its operands, a literal, a variable name, etc.) 
>
> 显然，在c++中，我们可以将literal、variable name都看做是expression。



## General

- [value categories](https://en.cppreference.com/w/cpp/language/value_category) (lvalue, rvalue, glvalue, prvalue, xvalue) classify expressions by their values
- [order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) of arguments and subexpressions specify the order in which intermediate results are obtained
- [operator precedence](https://en.cppreference.com/w/cpp/language/operator_precedence) defines the order in which operators are bound to their arguments
- [alternative representations](https://en.cppreference.com/w/cpp/language/operator_alternative) are alternative spellings for some operators
- [operator overloading](https://en.cppreference.com/w/cpp/language/operators) makes it possible to specify the behavior of the operators with user-defined classes.

> NOTE: python中没有value categories一说，相比之下，python的data model是简单地多的。其他的相关问题，python中都有。

> NOTE: 上述后三条在原文中和前两条不在一起，我觉得它们是所有的operator都会涉及到的问题，所以我将它们放到了一起。



## Operators

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



### Common operators

> NOTE: python中assignment的含义是bind。

> NOTE: python中叫做Attribute references[¶](https://docs.python.org/3.3/reference/expressions.html#attribute-references)，`c++`中叫做member access。



### Special operators

> NOTE: 下面是对special operator的分类。



#### Conversions

> NOTE: 放到了type system章节，参见`C++\Language-reference\Basic-concept\Type-system\Type-conversion`。



#### Memory allocation

1、[new expression](https://en.cppreference.com/w/cpp/language/new) allocates memory dynamically

2、[delete expression](https://en.cppreference.com/w/cpp/language/delete) deallocates memory dynamically



#### Other

| operator                                                     | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [constant expressions ](https://en.cppreference.com/w/cpp/language/constant_expression) | can be evaluated at compile time and used in compile-time context (template arguments, array sizes, etc), 非常重要的概念，在`C++\Language-reference\Expressions\Constant-expressions`中进行了介绍 |
| [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof) | 参见: <br>- “Unevaluated expressions”                        |
| [`alignof`](https://en.cppreference.com/w/cpp/language/alignof) | 参见: <br/>- “Unevaluated expressions”                       |
| [`typeid`](https://en.cppreference.com/w/cpp/language/typeid) | 参见: <br/>- “Unevaluated expressions”                       |
| [throw-expression](https://en.cppreference.com/w/cpp/language/throw) |                                                              |



## Primary expressions

> NOTE: primary expression的概念是比较重要的，因为有的时候，compiler的报错中，直接就使用primary expression。

The operands of any operator may be other **expressions** or **primary expressions** (e.g. in `1+2*3`, the operands of operator `+` are the subexpression `2*3` and the **primary expression** `1`).

> NOTE: python expression中也有Primaries[¶](https://docs.python.org/3.3/reference/expressions.html#primaries)的概念

Primary expressions are any of the following:

1) Literals (e.g. `2` or `"Hello, world"`)

2) Id-expressions

> NOTE: 参见下面的“ID-expression”章节

3) [Lambda-expressions](https://en.cppreference.com/w/cpp/language/lambda) (C++11)

4) [Fold-expressions](https://en.cppreference.com/w/cpp/language/fold) (C++17)

5) [Requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) (C++20)

Any expression in parentheses is also classified as a **primary expression**: this guarantees that the parentheses have higher precedence than any **operator**. Parentheses preserve **value**, **type**, and **value category**.

> NOTE: 如何理解primary expression呢？我觉得应该从operand来理解，operand需要是primary expression；如果不满足这个条件，则compiler就会complain类似如下error:
>
> [expected primary-expression before ‘>’ token](https://stackoverflow.com/questions/3505713/c-template-compilation-error-expected-primary-expression-before-token)
>
> 在`C++\Language-reference\Basic-concept\Organization\Name-lookup\Dependent-name-lookup`中讲述了上述错误的深层原因。
>
> 通过上述错误，能够加深我们对primary expression的理解。



### Literals

**Literals** are the tokens of a C++ program that represent constant values embedded in the source code.

- [integer literals](https://en.cppreference.com/w/cpp/language/integer_literal) are decimal, octal, hexadecimal or binary numbers of integer type.
- [character literals](https://en.cppreference.com/w/cpp/language/character_literal) are individual characters of type



- [floating-point literals](https://en.cppreference.com/w/cpp/language/floating_literal) are values of type float, double, or long double
- [string literals](https://en.cppreference.com/w/cpp/language/string_literal) are sequences of characters of type



- [boolean literals](https://en.cppreference.com/w/cpp/language/bool_literal) are values of type bool, that is true and false
- [`nullptr`](https://en.cppreference.com/w/cpp/language/nullptr) is the pointer literal which specifies a null pointer value (since C++11)
- [user-defined literals](https://en.cppreference.com/w/cpp/language/user_literal) are constant values of user-specified type (since C++11)

> NOTE: python expression中也有Literals[¶](https://docs.python.org/3.3/reference/expressions.html#literals)



## Unevaluated expressions

The **operands** of the operators [`typeid`](https://en.cppreference.com/w/cpp/language/typeid), [`sizeof`](https://en.cppreference.com/w/cpp/language/sizeof), [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept), and [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) (since C++11) are expressions that are not evaluated (unless they are **polymorphic glvalues** and are the operands of `typeid`), since these operators only query the **compile-time** properties of their operands. Thus, `std::size_t n = sizeof(std::cout << 42);` does not perform console output.

> NOTE: 
>
> 由于`typeid`、`sizeof`、`noexcept`、`decltype`都是compile-time operator（ `typeid`比较特殊，上面这段话对此进行了补充说明），所以当它们的operand是expression的时候，这些operator仅仅会使用expression的compile-time property，所以这些expression是不会被evaluate的，这就是本节所描述的unevaluated expressions。
>
> 我们可以从对立面来理解它，以function-call operator为例：
>
> ```c++
> #include <iostream>
> 
> int i = 1;
> 
> void F(int i) {
>     std::cout << i << std::endl;
> }
> 
> int main() {
>     int i = 1;
>     F(i + 1);
> }
> 
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
> `decltype` 的 operand 可以是build-in comma operator构成的expression，build-in comma operator的第一个operand是discard value的，说明它是被evaluated的，那这要如何来进行理解呢？

> NOTE: 上面这一段提示了我们：有些operator在compile-time进行计算的，而有些是在run-time进行计算的，可以将此作为对operator的分类方法；上面这一段对[`typeid`](https://en.cppreference.com/w/cpp/language/typeid)进行了特殊说明，它表示`typeid`也可能是run-time。



### Unevaluated operands are *full expressions*  (since C++14)

The unevaluated operands are considered to be *full expressions* even though they are syntactically operands in a larger expression (for example, this means that `sizeof(T())` requires an accessible `T::~T`). (since C++14)

> NOTE: “for example, this means that `sizeof(T())` requires an accessible `T::~T`”没有搞懂。

### [Requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) (since C++20)

The [requires-expressions](https://en.cppreference.com/w/cpp/language/constraints) are also unevaluated expressions. An invocation of an [immediate function](https://en.cppreference.com/w/cpp/language/consteval) is always evaluated, even in an unevaluated operand.(since C++20)

> NOTE: 各种特殊的规则，使得c++语言比较复杂。



## Discarded-value expressions

> NOTE: 参见`Discarded-value-expressions`章节



## Full list of c++ operator

c++中有哪些operator？下面对此进行枚举：

在cppreference [C++ Operator Precedence](https://en.cppreference.com/w/cpp/language/operator_precedence)中枚举了几乎所有的c++ operator。



## Supplement: id-expression

原文没有对id-expression进行深入说明，下面是对它的补充说明:

id-expression中的id的含义是[identifier](https://en.cppreference.com/w/cpp/language/identifiers)，在cppreference [identifier](https://en.cppreference.com/w/cpp/language/identifiers)的“In expressions”段中对它进行了详细的说明，它包含:

1) Suitably declared [unqualified identifiers](https://en.cppreference.com/w/cpp/language/identifiers#Unqualified_identifiers) (e.g. `n` or `cout`)

2) Suitably declared [qualified identifiers](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers) (e.g. [std::string::npos](https://en.cppreference.com/w/cpp/string/basic_string/npos))

在下面的章节中提及了id-expression:

- cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions)



### Example 1

stackoverflow [Why user-defined conversion is not implicitly taking place on the calling object](https://stackoverflow.com/questions/44699176/why-user-defined-conversion-is-not-implicitly-taking-place-on-the-calling-object)

```c++
#include <iostream>

class A {
public:
    void func() const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};

class B {
public:
    // user-defined conversion operator to A
    operator A() const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return a_;
    }

private:
    A a_;
};

int main() {
    B b;
    static_cast<A>(b).func(); // call func() on temporary instance of A
    // b.func(); // <-- error: 'class B' has no member named 'func'
}
// g++  test.cpp

```

[A](https://stackoverflow.com/a/44699334)

**Conversion** isn't considered for **member access** (§5.2.5/2 [expr.ref]).

> In either case, the *id-expression* shall name a member of the class or of one of its base classes

Here the *id-expression* is `func()`

So the compiler considers that `func` must be a member of `B` or a class that `B` derives from. Implicit conversion to other types that might have a `func` member is not considered.



### Example 2

thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) :

compiler默认情况下是将dependent name作为identifier，通过keyword `typename`来告诉compiler，这个name表示的是type，通过keyword `template`来告诉compiler，这个name表示的是template；
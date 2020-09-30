# cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category)

Each C++ [expression](https://en.cppreference.com/w/cpp/language/expressions) (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a *type* and a *value category*. Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories: *prvalue*, *xvalue*, and *lvalue*. 

> NOTE: 始终牢记stroustrup [“New” Value Terminology](http://www.stroustrup.com/terminology.pdf)中给出的"W"图
>
> ```
> iM      im      Im
>  \     /  \     /
>     \   /    \   /
>      \ /      \ /
>       i        m
> ```
>
> 
>
> ```
> lvalue       xvalue           prvalue
>  \           /    \           /
>     \         /      \         /
>      \       /        \       /
>       \     /          \     /
>        \   /            \   /
>        glvalue           rvalue
> ```
>
> 需要按照stroustrup [“New” Value Terminology](http://www.stroustrup.com/terminology.pdf) 中的思路来理解本文。

## Primary categories

### lvalue

The following expressions are *lvalue expressions*:

#### Named variable

> NOTE: 这种是最容易理解的，它们是典型的`iM`

the name of a variable, a function, a [template parameter object](https://en.cppreference.com/w/cpp/language/template_parameters#Non-type_template_parameter) (since C++20), or a data member, regardless of type, such as [std::cin](http://en.cppreference.com/w/cpp/io/cin) or [std::endl](http://en.cppreference.com/w/cpp/io/manip/endl). Even if the variable's type is rvalue reference, the expression consisting of its name is an lvalue expression;

> NOTE: 上面这段话后半句的意思是: 虽然 [std::cin](http://en.cppreference.com/w/cpp/io/cin) or [std::endl](http://en.cppreference.com/w/cpp/io/manip/endl) 的类型是rvalue reference，但是它们满足`iM`，因此它们是rvalue，这就是stackoverflow [What are move semantics?](https://stackoverflow.com/questions/3106110/what-are-move-semantics) `#` [part two](https://stackoverflow.com/a/11540204) 中总结的：
>
> > A **named rvalue reference** is an lvalue, just like any other variable.
>
> 在`C++\Language-reference\Reference\Move-semantic\stackoverflow-What-is-move-semantics.md`中收录了这篇文章。

#### Lvalue function call expression

> NOTE: 本节标题中的lvalue function是我创造的一个概念，与它相对的一个概念是rvalue expression，下面是它们的定义:
>
> | 概念              | 说明                                                   |
> | ----------------- | ------------------------------------------------------ |
> | lvalue function   | function whose **return type** is **lvalue reference** |
> | rvalue expression | function whose **return type** is non-reference        |
>
> 上述 两个概念是基于value category来对function进行分类的，这样做是为了便于描述value category，因为在C++中，operator可以使用function来进行描述，所以operator expression（表示的是包含operator的expression） 都可以使用function call expression来进行描述，所以使用function call expression，可以描述所有的operator expression。
>
> Lvalue function call expression的value category是lvalue，这种情况是非常具有代表性的，后面的很多operator expressin都可以归入这个范畴。

a function call or an overloaded operator expression, whose return type is **lvalue reference**, such as [std::getline](http://en.cppreference.com/w/cpp/string/basic_string/getline)([std::cin](http://en.cppreference.com/w/cpp/io/cin), str), [std::cout](http://en.cppreference.com/w/cpp/io/cout) `<< 1`, `str1 = str2`, or `++it`;

> NOTE: 需要对上面列举的例子进行详细的说明:
>
> `std::getline`的原型如下，显然返回值的类型是lvalue reference: 
>
> ```C++
> template< class CharT, class Traits, class Allocator >
> std::basic_istream<CharT,Traits>& getline( std::basic_istream<CharT,Traits>& input,
>                                         std::basic_string<CharT,Traits,Allocator>& str,
>                                         CharT delim );
> ```
>
> [std::cout](http://en.cppreference.com/w/cpp/io/cout) `<< 1` ，它实际调用的是[std::basic_ostream<CharT,Traits>::operator<<](https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt)，这个函数的原型是:
>
> ```C++
> basic_ostream& operator<<( int value );
> ```
>
> 
>
> `str1 = str2` 是 assignment expression，下面会进行介绍；
>
> `++it` 是 increment expression，下面会进行介绍；

#### Assignment expression

`a = b`, `a += b`, `a %= b`, and all other built-in [assignment and compound assignment](https://en.cppreference.com/w/cpp/language/operator_assignment) expressions;

> NOTE: 在 [assignment and compound assignment](https://en.cppreference.com/w/cpp/language/operator_assignment) 中，给出了[Assignment operators](https://en.cppreference.com/w/cpp/language/operator_assignment)的函数原型，可以看到，它们都是前面描述的: a function call or an overloaded operator expression, whose return type is **lvalue reference**，即它是lvalue function，所以包它的expression就是lvalue expression。

#### Pre-increment and pre-decrement expression

`++a` and `--a`, the built-in [pre-increment and pre-decrement](https://en.cppreference.com/w/cpp/language/operator_incdec#Built-in_prefix_operators) expressions;

> NOTE: 相当于 Lvalue function call expression。

#### indirection expression

`*p`, the built-in [indirection](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_indirection_operator) expression;

> NOTE: 相当于 Lvalue function call expression。

#### Built-in subscript

`a[n]` and `p[n]`, the built-in [subscript](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator) expressions, where one operand in `a[n]` is an array lvalue (since C++11);

> NOTE: 相当于 Lvalue function call expression。

#### Member access expression

> NOTE:  本段标题中的“member access”是取自cppreference [Member access operators](https://en.cppreference.com/w/cpp/language/operator_member_access)，下面这些operator，cppreference中将它们都归入了Member access operators。

`a.m`, the [member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) expression, except where `m` is a **member enumerator** or a non-static member function, or where `a` is an rvalue and `m` is a non-static data member of non-reference type;

> NOTE: 需要注意的是，“member of object”的意思是“object的member”。下面对否定条件进行说明:
>
> | 否定条件                   | 分析                |
> | -------------------------- | ------------------- |
> | member enumerator          | enumerator是prvalue |
> | non-static member function | function肯定是右值  |
> |                            |                     |
>
> 

`p->m`, the built-in [member of pointer](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) expression, except where `m` is a member enumerator or a non-static member function;

> NOTE: 和前面的情况类似

`a.*mp`, the [pointer to member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators) expression, where `a` is an lvalue and `mp` is a pointer to data member;

`p->*mp`, the built-in [pointer to member of pointer](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators) expression, where `mp` is a pointer to data member;

#### Comma expression

`a, b`, the built-in [comma](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator) expression, where `b` is an lvalue;

`a ? b : c`, the [ternary conditional](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) expression for some `b` and `c` (e.g., when both are lvalues of the same type, but see [definition](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) for detail);

#### String literal

a [string literal](https://en.cppreference.com/w/cpp/language/string_literal), such as "Hello, world!";

#### Cast expression

a cast expression to lvalue reference type, such as `static_cast<int&>(x)`;

Properties:

- Same as glvalue (below).
- Address of an lvalue may be taken: &++i[[1\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-1) and &[std::endl](http://en.cppreference.com/w/cpp/io/manip/endl) are valid expressions.
- A modifiable lvalue may be used as the left-hand operand of the built-in assignment and compound assignment operators.
- An lvalue may be used to [initialize an lvalue reference](https://en.cppreference.com/w/cpp/language/reference_initialization); this associates a new name with the object identified by the expression.

### prvalue

The following expressions are *prvalue expressions*:

#### Literal类prvalue

> NOTE: 本段标题“Literal类prvalue”是我创造的一个概念，它表示类似于literal的prvalue，包含如下: 

**literal**: 

a [literal](https://en.cppreference.com/w/cpp/language/expressions#Literals) (except for [string literal](https://en.cppreference.com/w/cpp/language/string_literal)), such as `42`, `true` or `nullptr`;

> NOTE: literal是非常典型的一类prvalue，后面的很多都可以归入这一类，literal是典型的与named variable不同的；我们可以使用传统的rvalue来理解它: 显然它是无法处于assignment左侧的。

**`this`** :

the [`this`](https://en.cppreference.com/w/cpp/language/this) pointer;

> NOTE: 需要注意的是，`this` pointer不是一个variable（虽然`this`是有name的，我们不需要declare），它相当于literal（可以使用`nullptr`来进行类比），显然它是无法处于assignment左侧的；
>
> 如果我们从`this`的实现来思考的话，那么`this` pointer是rvalue就非常容易理解: compiler让`this`的值为object的地址，因此，`this`仅仅是一个地址值而已，和literal非常类似，所以`this`是prvalue。

**enumerator** :

an [enumerator](https://en.cppreference.com/w/cpp/language/enum)

> NOTE: enumerator和`this`、literal是非常类似的，enumerator的值在compile阶段就已经确定了，所以所有的enumerator都会被替换为它的对应的value；显然它是无法位于assignment左侧的。

**Non-type template parameter** :

non-type [template parameter](https://en.cppreference.com/w/cpp/language/template_parameters) unless its type was a class or (since C++20) an lvalue reference type;

> NOTE: non-type template parameter对应的argument一般是literal

#### Rvalue function call expression

> NOTE: Rvalue function 的概念在前面的Lvalue function call expression章节中已经介绍了

a function call or an overloaded operator expression, whose return type is non-reference, such as `str.substr(1, 2)`, `str1 + str2`, or `it++`;

> NOTE: 下面是cnblogs [Lvalues and Rvalues](https://www.cnblogs.com/areliang/archive/2011/11/16/2251480.html)中给出的一个例子: 
>
> ```C++
> int a;  
> a = 10; // 10是rvalue，它没有地址，&10就是错误的表达式。从汇编语言的角度来看，10是直接存在于MOV指令中的立即数。   
> 10 = a; // 错误，10是rvalue，不可赋值。   
> //函数返回值属于rvalue，因为返回值通常用CPU寄存器传递，没有地址。   
> int foo()  
> {  
>     return 0;  
> }  
> int b = foo(); //没问题，函数返回值是rvalue。   
> int* p = &foo(); //错误，rvalue没有地址。   
> void bar(int& i)  
> {  
> }  
> bar(foo()); //错误，bar函数参数需要的是lvalue。 
> ```
>
> 

#### Post-increment and post-decrement expression

`a++` and `a--`, the built-in [post-increment and post-decrement](https://en.cppreference.com/w/cpp/language/operator_incdec#Built-in_postfix_operators) expressions;

> NOTE: 相当于 Rvalue function call expression。

#### Arithmetic expression

`a + b`, `a % b`, `a & b`, `a << b`, and all other built-in [arithmetic](https://en.cppreference.com/w/cpp/language/operator_arithmetic) expressions;

#### Logical expression

`a && b`, `a || b`, `!a`, the built-in [logical](https://en.cppreference.com/w/cpp/language/operator_logical) expressions;

#### Comparison expression

`a < b`, `a == b`, `a >= b`, and all other built-in [comparison](https://en.cppreference.com/w/cpp/language/operator_comparison) expressions;

#### Address-of

`&a`, the built-in [address-of](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_address-of_operator) expression;

#### Member of object expression

`a.m`, the [member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) expression, where `m` is a member enumerator or a non-static member function[[2\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-pmfc-2), or where `a` is an rvalue and `m` is a non-static data member of non-reference type (until C++11);



`p->m`, the built-in [member of pointer](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) expression, where `m` is a member enumerator or a non-static member function[[2\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-pmfc-2);

- a.*mp, the [pointer to member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators) expression, where `mp` is a pointer to member function[[2\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-pmfc-2), or where `a` is an rvalue and `mp` is a pointer to data member (until C++11);
- p->*mp, the built-in [pointer to member of pointer](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators) expression, where `mp` is a pointer to member function[[2\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-pmfc-2);
- a, b, the built-in [comma](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator) expression, where `b` is an rvalue;
- a ? b : c, the [ternary conditional](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) expression for some `b` and `c` (see [definition](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) for detail);
- a cast expression to non-reference type, such as `static_cast<double>(x)`, [std::string](http://en.cppreference.com/w/cpp/string/basic_string){}, or `(int)42`;

  

Properties:

- Same as rvalue (below).
- A prvalue cannot be [polymorphic](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects): the [dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type) of the object it identifies is always the type of the expression.
- A non-class non-array prvalue cannot be [cv-qualified](https://en.cppreference.com/w/cpp/language/cv). (Note: a function call or cast expression may result in a prvalue of non-class cv-qualified type, but the cv-qualifier is immediately stripped out.)
- A prvalue cannot have [incomplete type](https://en.cppreference.com/w/cpp/language/type#Incomplete_type) (except for type void, see below, or when used in [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype))
- A prvalue cannot have [abstract class type](https://en.cppreference.com/w/cpp/language/abstract_class) or an array thereof.

### xvalue

The following expressions are *xvalue expressions*:

- a function call or an overloaded operator expression, whose return type is rvalue reference to object, such as std::move(x);
- a[n], the built-in [subscript](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator) expression, where one operand is an array rvalue;
- a.m, the [member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) expression, where `a` is an rvalue and `m` is a non-static data member of non-reference type;
- a.*mp, the [pointer to member of object](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators) expression, where `a` is an rvalue and `mp` is a pointer to data member;
- a ? b : c, the [ternary conditional](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) expression for some `b` and `c` (see [definition](https://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator) for detail);
- a cast expression to rvalue reference to object type, such as static_cast<char&&>(x);

| any expression that designates a temporary object, after [temporary materialization](https://en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization). | (since C++17) |
| ------------------------------------------------------------ | ------------- |
|                                                              |               |

Properties:

- Same as rvalue (below).
- Same as glvalue (below).

In particular, like all rvalues, xvalues bind to rvalue references, and like all glvalues, xvalues may be [polymorphic](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects), and non-class xvalues may be [cv-qualified](https://en.cppreference.com/w/cpp/language/cv).

## Mixed categories

### glvalue

A *glvalue expression* is either lvalue or xvalue.

Properties:

- A glvalue may be implicitly converted to a prvalue with lvalue-to-rvalue, array-to-pointer, or function-to-pointer [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast).
- A glvalue may be [polymorphic](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects): the [dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type) of the object it identifies is not necessarily the static type of the expression.
- A glvalue can have [incomplete type](https://en.cppreference.com/w/cpp/language/type#Incomplete_type), where permitted by the expression.

### rvalue

An *rvalue expression* is either prvalue or xvalue.

Properties:

- Address of an rvalue may not be taken: &int(), &i++[[3\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-3), &42, and &std::move(x) are invalid.
- An rvalue can't be used as the left-hand operand of the built-in assignment or compound assignment operators.
- An rvalue may be used to [initialize a const lvalue reference](https://en.cppreference.com/w/cpp/language/reference_initialization), in which case the lifetime of the object identified by the rvalue is [extended](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary) until the scope of the reference ends.

| An rvalue may be used to [initialize an rvalue reference](https://en.cppreference.com/w/cpp/language/reference_initialization), in which case the lifetime of the object identified by the rvalue is [extended](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary) until the scope of the reference ends.When used as a function argument and when [two overloads](https://en.cppreference.com/w/cpp/language/overload_resolution) of the function are available, one taking rvalue reference parameter and the other taking lvalue reference to const parameter, an rvalue binds to the rvalue reference overload (thus, if both copy and move constructors are available, an rvalue argument invokes the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor), and likewise with copy and move assignment operators). | (since C++11) |
| ------------------------------------------------------------ | ------------- |
|                                                              |               |

## Special categories

### Pending member function call

The expressions a.mf and p->mf, where `mf` is a [non-static member function](https://en.cppreference.com/w/cpp/language/member_functions), and the expressions a.*pmf and p->*pmf, where `pmf` is a [pointer to member function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_member_functions), are classified as prvalue expressions, but they cannot be used to initialize references, as function arguments, or for any purpose at all, except as the left-hand argument of the function call operator, e.g. (p->*pmf)(args).

### Void expressions

Function call expressions returning void, cast expressions to void, and [throw-expressions](https://en.cppreference.com/w/cpp/language/throw) are classified as prvalue expressions, but they cannot be used to initialize references or as function arguments. They can be used in discarded-value contexts (e.g. on a line of its own, as the left-hand operand of the comma operator, etc.) and in the returnstatement in a function returning void. In addition, throw-expressions may be used as the second and the third operands of the [conditional operator ?:](https://en.cppreference.com/w/cpp/language/operator_other).

| Void expressions have no *result object*. | (since C++17) |
| ----------------------------------------- | ------------- |
|                                           |               |

### Bit fields

An expression that designates a [bit field](https://en.cppreference.com/w/cpp/language/bit_field) (e.g. a.m, where `a` is an lvalue of type struct A { int m: 3; }) is an lvalue expression: it may be used as the left-hand operand of the assignment operator, but its address cannot be taken and a non-const lvalue reference cannot be bound to it. A const lvalue reference can be initialized from a bit-field lvalue, but a temporary copy of the bit-field will be made: it won't bind to the bit field directly.

## History

### CPL

The programming language [CPL](https://en.wikipedia.org/wiki/CPL_(programming_language)) was first to introduce value categories for expressions: all CPL expressions can be evaluated in "right-hand mode", but only certain kinds of expression are meaningful in "left-hand mode". When evaluated in right-hand mode, an expression is regarded as being a rule for the computation of a value (the right-hand value, or *rvalue*). When evaluated in left-hand mode an expression effectively gives an address (the left-hand value, or *lvalue*). "Left" and "Right" here stood for "left of assignment" and "right of assignment".

### C

The C programming language followed a similar taxonomy, except that the role of assignment was no longer significant: C expressions are categorized between "lvalue expressions" and others (functions and non-object values), where "lvalue" means an expression that identifies an object, a "locator value"[[4\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-4).

### C++98

Pre-2011 C++ followed the C model, but restored the name "rvalue" to non-lvalue expressions, made functions into lvalues, and added the rule that references can bind to lvalues, but only references to const can bind to rvalues. Several non-lvalue C expressions became lvalue expressions in C++.

### C++11

With the introduction of move semantics in C++11, value categories were redefined to characterize two independent properties of expressions[[5\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-5):

- *has identity*: it's possible to determine whether the expression refers to the same entity as another expression, such as by comparing addresses of the objects or the functions they identify (obtained directly or indirectly);
- *can be moved from*: [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor), [move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment), or another function overload that implements move semantics can bind to the expression.

In C++11, expressions that:

- have identity and cannot be moved from are called *lvalue* expressions;
- have identity and can be moved from are called *xvalue* expressions;
- do not have identity and can be moved from are called *prvalue* ("pure rvalue") expressions;
- do not have identity and cannot be moved from are not used[[6\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-6).

The expressions that have identity are called "glvalue expressions" (glvalue stands for "generalized lvalue"). Both lvalues and xvalues are glvalue expressions.

The expressions that can be moved from are called "rvalue expressions". Both prvalues and xvalues are rvalue expressions.

### C++17

In C++17, [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) was made mandatory in some situations, and that required separation of prvalue expressions from the temporary objects initialized by them, resulting in the system we have today. Note that, in contrast with the C++11 scheme, prvalues are no longer moved from.

## Footnotes

1. [↑](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-1) Assuming i has built-in type or the pre-increment operator is [overloaded](https://en.cppreference.com/w/cpp/language/operators) to return by lvalue reference.
2. ↑ [2.0](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-pmfc_2-0) [2.1](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-pmfc_2-1) [2.2](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-pmfc_2-2) [2.3](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-pmfc_2-3) Special rvalue category, see pending member function call.
3. [↑](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-3) Assuming i has built-in type or the postincrement operator is not [overloaded](https://en.cppreference.com/w/cpp/language/operators) to return by lvalue reference.
4. [↑](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-4) "A difference of opinion within the C community centered around the meaning of lvalue, one group considering an lvalue to be any kind of object locator, another group holding that an lvalue is meaningful on the left side of an assigning operator. The C89 Committee adopted the definition of lvalue as an object locator." -- ANSI C Rationale, 6.3.2.1/10.
5. [↑](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-5) ["New" Value Terminology](http://www.stroustrup.com/terminology.pdf) by Bjarne Stroustrup, 2010.
6. [↑](https://en.cppreference.com/w/cpp/language/value_category#cite_ref-6) const prvalues (only allowed for class types) and const xvalues do not bind to T&& overloads, but they bind to the const T&& overloads, which are also classified as "move constructor" and "move assignment operator" by the standard, satisfying the definition of "can be moved from" for the purpose of this classification. However, such overloads cannot modify their arguments and are not used in practice; in their absence const prvalues and const xvalues bind to const T& overloads.

### Defect reports
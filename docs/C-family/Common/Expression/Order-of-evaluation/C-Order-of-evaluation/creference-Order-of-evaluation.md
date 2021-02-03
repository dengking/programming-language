# cppreference [Order of evaluation](https://en.cppreference.com/w/c/language/eval_order)



Order of evaluation of the **operands** of any C operator, including the order of **evaluation of function arguments** in a function-call expression, and the order of evaluation of the subexpressions within any expression is **unspecified** (except where noted below). The compiler will evaluate them in any order, and may choose another order when the same expression is evaluated again.

There is no concept of left-to-right or right-to-left evaluation in C, which is not to be confused with left-to-right and right-to-left **associativity** of operators: the expression `f1() + f2() + f3()` is parsed as `(f1() + f2()) + f3()` due to left-to-right associativity of operator`+`, but the function call to `f3` may be evaluated first, last, or between `f1()` or `f2()` at run time.

> NOTE : expression中还嵌套有多个expression，比如`f1() + f2() + f3()`，`f1()` 是`+`的**operands**，同时它自己也是也expression，它也需要evaluation；



### Definitions

There are two kinds of **evaluations** performed by the compiler for each expression or subexpression (both of which are optional):

- *value computation*: calculation of the value that is returned by the expression. This may involve determination of the identity of the object ([lvalue evaluation](https://en.cppreference.com/w/c/language/value_category)) or reading the value previously assigned to an object (rvalue evaluation)
- *side effect*: access (read or write) to an object designated by a [volatile](https://en.cppreference.com/w/c/language/volatile) lvalue, modification (writing) to an object, atomic synchronization (since C11), modifying a file, modifying the floating-point environment (if supported), or calling a function that does any of those operations.

If no **side effects** are produced by an expression and the compiler can determine that the value is not used, the expression [may not be evaluated](https://en.cppreference.com/mwiki/index.php?title=c/language/as-if&action=edit&redlink=1).

#### Ordering

"sequenced-before" is an asymmetric, transitive, pair-wise relationship between evaluations within the same thread (it may extend across threads if atomic types and memory barriers are involved).

- If a [*sequence point*](https://en.wikipedia.org/wiki/Sequence_point) is present between the subexpressions E1 and E2, then both value computation and side effects of E1 are *sequenced-before* every value computation and side effect of E2
- If evaluation A is **sequenced before** evaluation B, then evaluation of A will be complete before evaluation of B begins.
- If A is not sequenced before B and B is sequenced before A, then evaluation of B will be complete before evaluation of A begins.
- If A is not sequenced before B and B is not sequenced before A, then two possibilities exist:
  - evaluations of A and B are unsequenced: they may be performed in any order and may overlap (within a single thread of execution, the compiler may interleave the CPU instructions that comprise A and B)
  - evaluations of A and B are indeterminably-sequenced: they may be performed in any order but may not overlap: either A will be complete before B, or B will be complete before A. The order may be the opposite the next time the same expression is evaluated.

### Rules

1) There is a **sequence point ** after the evaluation of all **function arguments** and of the function designator, and before the actual **function call**.

> NOTE : 在函数调用之前，先evaluate它的argument，这是非常容易理解的；但是多个argument的order并没有被规定；上面这段话所描述的evaluation order是：function arguments，function designator，function call；

2) There is a **sequence point** after evaluation of the first (left) operand and before evaluation of the second (right) operand of the following binary operators: `&&` (logical AND), `||` (logical OR), and `,` (comma).

> NOTE : 对于`&&`、`||`而言，这种安排便于实现逻辑短路；

3) There is a **sequence point** after evaluation of the first (left) operand and before evaluation of the second or third operand (whichever is evaluated) of the conditional operator `?:`

> NOTE : 这种安排也是符合流程的；

4) There is a **sequence point** after the evaluation of a **full expression** (an expression that is not a subexpression: typically something that ends with a semicolon（分号） or a **controlling statement**（条件判断部分） of `if`/`switch`/`while`/`do`) and before the **next** full expression.


5) There is a **sequence point** at the end of a **full declarator**.

6) There is a **sequence point** immediately before the return of a library function.

> NOTE : 这是什么意思？

7) There is a sequence point after the action associated with each conversion specifier in formatted I/O (in particular, it is well-formed for [scanf](https://en.cppreference.com/w/c/io/fscanf) to write different fields into the same variable and for [printf](https://en.cppreference.com/w/c/io/fprintf) to read and modify or modify the same variable more than once using %n)

8) There are sequence points before and immediately after each call to a comparison function made by the library functions [qsort](https://en.cppreference.com/w/c/algorithm/qsort) and [bsearch](https://en.cppreference.com/w/c/algorithm/bsearch), as well as between any call to the comparison function and the movement of the associated objects made by [qsort](https://en.cppreference.com/w/c/algorithm/qsort)


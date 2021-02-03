# cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order)

Order of evaluation of any part of any expression, including order of evaluation of function arguments is *unspecified* (with some exceptions listed below). The compiler can evaluate operands and other subexpressions in any order, and may choose another order when the same expression is evaluated again.

> NOTE: 
>
> 1、为什么C++标准没有对此进行规定？

## No left-to-right or right-to-left evaluation 

> NOTE: 
>
> 1、下面这段的描述，是和直觉相悖的。
>
> 2、通过这段描述可以看出，C++对Order of evaluation并没有进行严格的限制，它的限制是非常松散的，那C++的限制有哪些呢？C++是基于Sequenced-before relation而建立的，参见下面的"Sequenced-before rules (since C++11)"章节
>
> > NOTE: 主要描述C++11，对C++11之前，并不考虑
>
> 3、正是这种松散，可能带来一些问题，后面会进行总结

There is no concept of **left-to-right** or **right-to-left** evaluation in C++. This is not to be confused with left-to-right and right-to-left associativity of operators: the expression `a() + b() + c()` is parsed as `(a() + b()) + c()` due to left-to-right associativity of operator`+`, but the function call to `c` may be evaluated first, last, or between `a()` or `b()` at run time:



```C++
#include <cstdio>
int a()
{
	return std::puts("a");
}
int b()
{
	return std::puts("b");
}
int c()
{
	return std::puts("c");
}
void z(int, int, int)
{
}
int main()
{
	z(a(), b(), c());       // all 6 permutations of output are allowed
	return a() + b() + c(); // all 6 permutations of output are allowed
}
// gcc test.cpp

```

> NOTE: 下面是在我的计算机上运行的结果
>
> ```C++
> c
> b
> a
> a
> b
> c
> ```
>
> 显然是和原网页中的输出不同的

## Sequenced-before rules (since C++11)

> NOTE: 
>
> 1、wikipedia [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) 中关于Sequenced-before的描述:
>
> The term [sequence point](https://en.wikipedia.org/wiki/Sequence_point) was removed, being replaced by specifying that either one operation is sequenced before another, or that two operations are unsequenced.[[25\]](https://en.wikipedia.org/wiki/C%2B%2B11#cite_note-25)
>
> 通过上述描述可以看出，C++11对于Order of evaluation的限制还是比较松散的: 存在"unsequenced"的情况
>
> 2、典型的order and relation，参见 工程discrete的"Relation"章节

### Evaluation of Expressions

> NOTE: 
>
> 1、"Evaluation of Expressions"是language designer规定的，language implementation需要遵循它来翻译我们的code

Evaluation of each expression includes:

1、*value computations*: calculation of the value that is returned by the expression. This may involve determination of the identity of the object (glvalue evaluation, e.g. if the expression returns a reference to some object) or reading the value previously assigned to an object (prvalue evaluation, e.g. if the expression returns a number, or some other value)

2、Initiation of *side effects*: access (read or write) to an object designated by a volatile glvalue, modification (writing) to an object, calling a library I/O function, or calling a function that does any of those operations.

### Ordering

"sequenced-before" is an asymmetric(非对称), transitive(传递), pair-wise relationship between evaluations within the same thread.

> NOTE: 
>
> 1、"sequenced-before"描述的是"within the same thread"，那inter-thread的ordering呢？有inter-thread sequenced before

1、If A is sequenced before B, then evaluation of A will be complete before evaluation of B begins.

2、If A is not sequenced before B and B is sequenced before A, then evaluation of B will be complete before evaluation of A begins.

> NOTE: asymmetric(非对称)

3、If A is not sequenced before B and B is not sequenced before A, then two possibilities exist:

- evaluations of A and B are unsequenced: they may be performed in any order and may **overlap** (within a single thread of execution, the compiler may interleave the CPU instructions that comprise A and B)

- evaluations of A and B are indeterminately(不确定的) sequenced: they may be performed in any order but may not overlap: either A will be complete before B, or B will be complete before A. The order may be the opposite the next time the same expression is evaluated.

    > NOTE: 关于 "indeterminately sequenced"，参见下面的 "Rules # 11) "



### Rules

1)

> NOTE: 其实描述的是两个*full expression*之间的order

2)

> NOTE: 描述的是

3)

> NOTE: 其实描述的是function argument 和 function body 之间的关系

When calling a function (whether or not the function is inline, and whether or not explicit function call syntax is used), every value computation and side effect associated with any **argument expression**, or with the postfix expression designating the called function, is *sequenced before* execution of every expression or statement in the **body of the called function**.



11) A function call that is not *sequenced before* or *sequenced after* another function call is *indeterminately sequenced* (the program must behave [as if](https://en.cppreference.com/w/cpp/language/as_if) the CPU instructions that constitute different function calls were not interleaved, even if the functions were inlined).

(since C++17) The rule 11 has one exception: function calls made by a standard library algorithm executing under [`std::execution::par_unseq`](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t) execution policy are unsequenced and may be arbitrarily interleaved.



12) The call to the allocation function ([`operator new`](https://en.cppreference.com/w/cpp/memory/new/operator_new)) is 

1、(until C++17) **indeterminately sequenced** with respect to the evaluation of the constructor arguments in a [new-expression](https://en.cppreference.com/w/cpp/language/new)；

2、(since C++17) **sequenced before** the evaluation of the constructor arguments in a [new-expression](https://en.cppreference.com/w/cpp/language/new)；

> NOTE: 这段话要如何理解？

## Sequence point rules (until C++11)

> NOTE: 未阅读
# josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html)

> NOTE: 
>
> 1、这篇文章介绍了一种基于AST来分析expression evaluation，进而判断undefined behavior的方法，这种方法能够让programmer分析出expression是否存在undefined behavior，其实这种方法应该就是compiler检查expression的方法；
>
> 2、本文中给出的方法是典型的: relation structure computation。
>
> 3、这篇文章对Lvalue-to-rvalue conversion的解释也是比较好的

```C++
i = i++
```

This expression, and many like it, demonstrate the **sequencing rules** of C++ and how they can cause your program to behave in ways you might not expect. What should the result of this expression be given some initial value for `i`? Questions like this are extremely frequent on [Stack Overflow](https://stackoverflow.com/questions/tagged/c++). In this case, the answer is that this code is completely undefined — it could do anything. Literally anything (at least formally speaking).

## Unspecified behavior and undefined behavior and implementation-defined behavior

The C++ standard defines an **execution path** according to your code. Given particular inputs, the program will always follow this **execution path**. Sometimes, the standard allows multiple possible execution paths for your program. This gives compilers extra freedom to optimize in various ways. When the standard allows a particular set of possible paths, this is known as ***unspecified behavior***. In other cases, the standard gives absolutely no requirements about the behavior of your program, and this is known as ***undefined behavior***. Undefined behavior is certainly not something you want in your code.

**Implementation-defined behavior** is a subset of **unspecified behavior**, for which the implementation is required to document its choice of behavior.

The sequencing rules of C++, which describe how the evaluations of expressions and their subexpressions are ordered, may determine that an expression is undefined. C++11 introduced a smarter but slightly more complex approach to specifying these rules, which is still used in C++14. There’s a great [overview of the rules on the cppreference.com wiki](http://en.cppreference.com/w/cpp/language/eval_order).

## Evaluation 

In simple terms, evaluations are ordered by a **sequenced-before relationship**. That is, evaluation of one part of an expression may be **sequenced-before** the evaluation of another part. Evaluations can be one of two things: 

1、value computations, which work out the result of an expression; and 

2、side effects, which are modifications of objects. 

When two evaluations are not sequenced-before each other, they are unsequenced (we cannot say which will occur first and they may even overlap).

## Sequencing rules

Informally, the basic sequencing rules are as follows:

1、The **value computation** of an operator’s operands are sequenced before the **value computation** of its result — else how would we compute the result? Note, however, that the **side effects** of the operands are not necessarily **sequenced-before**.

2、In general, the evaluation of an operator's operands are unsequenced with respect to each other. For example, in `x + y`, the evaluation of `x` and `y` are unsequenced.

3、For `&&`, `||`, and `,`, however, evaluation of the left operand is sequenced before the right operand.

4、The value computation of postfix `++` and `--` is sequenced before their side effects.

> NOTE: 也就是它们会先返回return value，然后将修改后的value写入到memory中

5、The side effects of prefix `++` and `--` are sequenced before their value computation.

> NOTE: 显然，它是先将value写入到memory中，然后返回return value的

6、The value computations of the operands of any assignment operator (`=`, `+=`, `-=`, etc.) are sequenced before the side effect of the assignment, which is itself **sequenced before** value computation of the assignment expression.

> NOTE: 上面这段话要如何理解？
>
> 1、"side effect of the assignment"指的是什么？是指将value写入到assignment的左操作数？
>
> 2、"which is itself **sequenced before** value computation of the assignment expression"修饰的是？应该修饰的是"The value computations of the operands of any assignment operator"
>
> 这样设计的最终目的是保证: 将左操作数的value赋值给右操作数。

## Acyclic directed graph

> NOTE: 非常符合在工程discrete中章节的"Relation-computation-structure"章节中总结的思想。

The rules that define when the sequenced-before relationship exists between two evaluations inherently(天生的) form an **acyclic directed graph structure**. As an example, let’s consider the earlier line of code once again:

```C++
i = i++
```

Let's first split it up into its subexpressions. The full expression is the assignment, which has two operands. The left operand is just `i`, while the right operand is `i++` which itself has `i` as an operand. The following tree represents this structure, where the arrows represent the **sequenced-before** relationship of the **value computations** of those subexpressions:

![The subexpression tree of i = i++.](https://josephmansfield.uk/media/images/sequencing-graphs/subexpressions.png)

Both the assignment and the increment have side effects (i.e. they modify the value of an object). We know how they are sequenced with respect to the **value computations** by looking at the above sequencing rules. The **assignment** comes after the value computation of its operands and before its own value computation. The **increment** comes after its value computation, but is not sequenced-before the assignment. If we add them as red nodes, we have:

> NOTE: 最后一句话是理解的关键:
>
> 1、" The **assignment** comes after the value computation of its operands and before its own value computation"，这的指定是 assignment operator的evaluation rule:
>
> a、其中的"assignment"指的是assignment operator的side effect，按照规定，它是先于它的value computation的
>
> "If we add them as red nodes, we have"，下面这张图要如何理解？red node分别指的是**assignment**、**increment**。
>
> 

![The almost-complete sequenced-before graph for i = i++.](https://josephmansfield.uk/media/images/sequencing-graphs/side-effects.png)

### Value computations that *use the value of an object*

We can read this graph as flowing chronologically(按照时间顺序) upwards. For the purposes of determining undefined behavior, we also need to identify any **value computations** that *use the value of an object*. This is quite a subtle(难以捉摸的) point, but only value computations of expressions denoting(表示) objects (like `i`) that are being used where an rvalue is expected are value computations that use the value of an object. When such an expression is used where an lvalue is expected instead, its value is not important, as in the left operand of `=`.

> NOTE: 翻译如下: 
>
> "我们可以把这张图表看成是按时间顺序向上流动的。为了确定未定义的行为，我们还需要识别使用对象值的任何值计算。这是非常微妙的一点，但是只有表示对象(如i)的表达式的值计算才会使用对象的值计算。当在需要左值的地方使用这样的表达式时，其值并不重要，如在=的左操作数中。"
>
> "This is quite a subtle(难以捉摸的) point, but only value computations of expressions denoting(表示) objects (like `i`) that are being used where an rvalue is expected are value computations that use the value of an object" 这段话是不容易理解的，其中
>
> 1、 "value computations of expressions denoting(表示) objects (like `i`) that are being used where an rvalue is expected"是主语，
>
> 2、它的核心意思是: 当需要rvalue的时候，才会使用它的value，在期待lvalue的时候，一般是不需要value的，因此也不需要关注它的value；显然`i=++i`就是非常典型的例子



> For those who want to read more, look up [value categories](http://en.cppreference.com/w/cpp/language/value_category). The left operand of `=` is an lvalue, which means that we don't care about its value. **Lvalue-to-rvalue conversion** can be thought of as reading the value from an object.





![An annotated sequenced-before graph for i = i++.](https://josephmansfield.uk/media/images/sequencing-graphs/postincrement.png)

### Herein(在此处) lies the undefined behaviour

Notice the interesting placement of the `i` increment — nothing else is sequenced after it, so it could even occur after the assignment. Herein(在此处) lies the undefined behaviour. Depending on whether the **increment** occurs before or after the **assignment**, we could get different results.

### 方法总结

To determine whether an expression might be undefined, we can simply look at its graph. Two evaluations in different branches are completely unsequenced (that is, if there is no directed path between them). The standard states that we have undefined behaviour if: 

1、we have **two side effects on the same scalar object that are unsequenced**; or 

> NOTE: 上面的例子就是典型的这种情况，简单而言: two unsequenced side effects

2、we have **a side effect on a scalar object and a value computation using the value of the same object that are unsequenced**. 

> NOTE: 上述两条规则都要去是同一个scalar object，对于不同的object，是不适用的

In the above graph, I have connected the problematic pair of evaluations with a dotted line — two unsequenced side effects.

As you can see, this gives a great way to visualize the sequencing rules as applied to a particular expression and makes it much easier to see why certain expressions might result in undefined behaviour.

Let’s take a look at some more examples of both undefined and well-defined expressions:

## Example: `i = ++i`

```C++
i = ++i
```





![An sequenced-before graph for i = ++i.](https://josephmansfield.uk/media/images/sequencing-graphs/preincrement.png)

> NOTE: 上述自底向上的第一个红色框`i`，表示的是`++i`的side effect，它是sequenced-before它的value computation的；

It is interesting to see that by merely switching the postfix increment to a prefix increment, this expression has become well-defined. That’s because the increment of `i` has to occur before the value computation of the increment expression and, therefore, before the assignment to `i`.

> Before C++11, this was actually considered undefined, despite having only one possible result.

## Example: `i = i++ + i++`

```C++
i = i++ + i++
```



![An sequenced-before graph for i = i++ + i++.](https://josephmansfield.uk/media/images/sequencing-graphs/complex.png)



This is a slightly more complex adaptation of the expression we first looked at. It seems to be quite a popular example. It has undefined behaviour for the same reasons, but also exhibits unsequenced side effects and value computations between the two increments. In the same way, `i++ + i++` alone is undefined.

## Example: `i = v[i++]`

```C++
i = v[i++]
```

![An sequenced-before graph for i = v[i++].](https://josephmansfield.uk/media/images/sequencing-graphs/subscript.png)

This one tends to be hard for many to grasp, although its problem is exactly the same as the previous examples. I suspect that most people think of `i++` as an operation that “returns the value of `i`, then increments `i`.” The problem is that the increment can happen at any point, and might happen after the assignment to `i`. The fact that we're using it as a subscript doesn't change anything.



## Example: `i++ || i++`

```C++
i++ || i++
```

![An sequenced-before graph for i++ || i++.](https://josephmansfield.uk/media/images/sequencing-graphs/logical-or.png)

This demonstrates the ability of some operators (namely `||`, `&&`, and `,`) to enforce sequencing between their operands. The standard states that every value computation and side effect associated with the left operand is sequenced before those of the right operand.



## Example: `f(i = 1, i = 2)`

```C++
f(i = 1, i = 2)
```

![An sequenced-before graph for f(i = 1, i = 2).](https://josephmansfield.uk/media/images/sequencing-graphs/function.png)

Here we’re calling a function and in each argument we’re assigning a different value to `i`. The evaluations of function arguments are also unsequenced, so we cannot say what the value of `i` will be after this expression has been evaluated.

------

It’s worth noting that function calls are always indeterminately sequenced. This means that one always occurs before the other, but we cannot say which way around. This is to prevent function calls from interleaving. However, the undefined behaviour rule only applies with unsequenced evaluations. Therefore, if you have two function calls in different branches that modify the same scalar object, you don’t have undefined behaviour — instead you have unspecified behaviour.

Also remember that overloaded operators are actually treated as function calls. That is, if you want to know how `x + y` is sequenced, and `operator+` is overloaded for `x`, you need to treat the expression as `x.operator+(y)` or `operator+(x, y)` — whichever is defined.

Finally, it is not always enough to look at the identifiers in expressions. Previously, we noted that an object `i` was modified in two parts of an expression. However, it’s possible to have two different identifiers, say `i` and `j`, that either refer to the same scalar object or are more complex data types that have a shared scalar object that they modify. Keep this in mind.

Next time you come across a complex expression (which is hopefully not very often), you now have a great way to analyse the sequencing of evaluations and determine the possible execution paths of that expression. Perhaps this might reduce the number of questions on Stack Overflow about this topic, but if not, at least it gives a nice visual way to answer them.
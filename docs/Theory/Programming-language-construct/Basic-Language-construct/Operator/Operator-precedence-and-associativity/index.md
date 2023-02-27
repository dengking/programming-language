# Operator precedence and associativity

素材:

一、http://www.cs.ecu.edu/karl/5220/spr16/Notes/CFG/precedence.html

二、

对一个我们平常手写的math expression（infix format），[operator associativity](https://en.wikipedia.org/wiki/Operator_associativity)、operator precedence、括号三者对expression的计算顺序进行规定从而决定了expression的含义:

1、operator precedence按照precedence对计算顺序进行排序

2、operator associativity对具备相同precedence对operator的计算顺序进行指定，它其实是对括号的简化

3、括号，前面的都是隐式的、inherent，而括号则是显示的，它提供了供programmer对计算顺序的控制



## Operator precedence

这是比较好理解的。

### wikipedia [Order of operations](https://en.wikipedia.org/wiki/Order_of_operations)

In [mathematics](https://en.wikipedia.org/wiki/Mathematics) and [computer programming](https://en.wikipedia.org/wiki/Computer_programming), the **order of operations** (or **operator precedence**) is a collection of rules that reflect conventions about which procedures to perform first in order to evaluate a given [mathematical expression](https://en.wikipedia.org/wiki/Expression_(mathematics)).

For example, in mathematics and most computer languages, multiplication is granted a higher precedence than addition, and it has been this way since the introduction of modern algebraic notation.[[1\]](https://en.wikipedia.org/wiki/Order_of_operations#cite_note-Bronstein_1987-1)[[2\]](https://en.wikipedia.org/wiki/Order_of_operations#cite_note-2) Thus, the expression $2 + 3 × 4$ is interpreted to have the value $2 + (3 × 4) = 14$, not $(2 + 3) × 4 = 20.$ With the introduction of exponents in the 16th and 17th centuries, they were given precedence over both addition and multiplication and could be placed only as a superscript to the right of their base.[[1\]](https://en.wikipedia.org/wiki/Order_of_operations#cite_note-Bronstein_1987-1) Thus $3 + 5^2 = 28 $and $3 × 5^2 = 75$.

These conventions exist to eliminate ambiguity while allowing notation to be as brief as possible. Where it is desired to override the precedence conventions, or even simply to emphasize them, parentheses `( )` (sometimes replaced by brackets `[ ]` or braces `{ }` for readability) can indicate an alternate order or reinforce the default order to avoid confusion. For example, $(2 + 3) × 4 = 20$ forces addition to precede multiplication, and $(3 + 5)^2 = 64$ forces addition to precede exponentiation.





## Operator associativity

对比喻precedence，它稍微难一点，下面是我的一些总结:

我们知道`-`是left associative的，所以`1-2-3`就需要先计算`1-2`，而不是先计算`2-3`，显然，expression（infix format） `1-2-3`只对应了一棵binary expression tree。

我们知道`=`是right associative的，所以`a=b=c`就需要先计算`b=c`，而不是先计算`a=b`，显然，expression（infix format） `a=b=c`只对应了一棵binary expression tree。

需要注意的是，有些运算符不止一种associativity，比如：乘法运算`*`和加法运算`+` are both left and right associative，所以包含它们的expression（infix format）可能对应不止一棵binary expression tree，每一棵binary expression tree对应了一种计算次序，需要注意的是不同的计算次序，即不同的binary expression tree，所对应的计算成本是不同的，关于这的一个典型问题就是[matrix chain multiplication](https://en.wikipedia.org/wiki/Matrix_chain_multiplication)问题；

需要注意的是，在上述表述中，我使用了这样的描述：

> 每一棵**binary expression tree**对应了一种**计算次序**

显然，对于一个expression（infix format），它的计算次序可能是不固定的，但是一旦将它转换为一个binary expression tree，则它的计算次序就固定了，这是因为binary expression tree的结构决定的：每个内节点对应了一个operator，每个叶子节点对应了一个operand，通过内节点将各个叶子节点给连接起来，其实就决定了expression的解释了，这就是binary expression tree的构造过程。

需要注意的是，expression（prefix format）和expression（postfix format）不存在expression（prefix format）的问题，它们的计算次序是固定的；

那如何实现binary expression tree的构造呢？其实这个过程是复杂的；



### wikipedia [Operator associativity](https://en.wikipedia.org/wiki/Operator_associativity)

In [programming languages](https://en.wikipedia.org/wiki/Programming_languages), the **associativity** of an [operator](https://en.wikipedia.org/wiki/Operator_(programming)) is a property that determines how operators of the same [precedence](https://en.wikipedia.org/wiki/Order_of_operations) are grouped in the absence of [parentheses](https://en.wikipedia.org/wiki/Bracket_(mathematics)). If an [operand](https://en.wikipedia.org/wiki/Operand) is both preceded and followed by operators (for example, `^ 3 ^`), and those operators have equal precedence, then the operand may be used as input to two different operations (i.e. the two operations indicated by the two operators). The choice of which operations to apply the operand to, is determined by the "**associativity**" of the operators. **Operators** may be **associative** (meaning the operations can be grouped arbitrarily（任意的）), **left-associative** (meaning the operations are grouped from the left), **right-associative** (meaning the operations are grouped from the right) or **non-associative** (meaning operations cannot be chained, often because the output type is incompatible with the input types). The associativity and precedence of an operator is a part of the definition of the programming language; different programming languages may have different associativity and precedence for the same type of operator.

> NOTE: 
>
> 一、在编程语言中，运算符的关联性是一种属性，它确定在没有括号的情况下如何对具有相同优先级的运算符进行分组。如果操作数在运算符之前和之后（例如^ 3 ^），并且那些运算符具有相同的优先级，则该操作数可以用作两个不同操作（即两个运算符指示的两个操作）的输入。将操作数应用于哪个运算的选择，取决于运算符的“关联性”。运算符可以是关联的（表示操作可以任意分组），左关联的（表示操作从左开始分组），右关联的（表示操作从右开始分组）或非关联的（表示操作不能链接，通常是因为输出类型与输入类型不兼容）。运算符的关联性和优先级是编程语言定义的一部分；对于相同类型的运算符，不同的编程语言可能具有不同的关联性和优先级。
>
> 二、上面这段话中所谓的"grouped"其实就可以理解为是加上括号，显然通过associativity我们能够省略 [parentheses](https://en.wikipedia.org/wiki/Bracket_(mathematics)) ，它是一种简化，所以反过来，associativity可以指导我们如何加括号，left-associative则从左往右加括号、right-associative则从右往左加括号

Consider the expression `a ~ b ~ c`. If the operator `~` has left associativity, this expression would be interpreted as `(a ~ b) ~ c`. If the operator has right associativity, the expression would be interpreted as `a ~ (b ~ c)`. If the operator is non-associative, the expression might be a [syntax error](https://en.wikipedia.org/wiki/Syntax_error), or it might have some special meaning. Some mathematical operators have inherent（固有的） **associativity**. For example, subtraction and division, as used in conventional math notation, are **inherently left-associative**. Addition and multiplication, by contrast, are both left and right associative. (e.g. `(a * b) * c = a * (b * c)`).

Many programming language manuals provide a table of operator precedence and associativity; see, for example, [the table for C and C++](https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B#Operator_precedence).

The concept of notational associativity described here is related to, but different from the mathematical [associativity](https://en.wikipedia.org/wiki/Associativity). An operation that is mathematically associative, by definition requires no notational associativity. (For example, addition has the associative property, therefore it does not have to be either left associative or right associative.) An operation that is not mathematically associative, however, must be notationally left-, right-, or non-associative. (For example, subtraction does not have the associative property, therefore it must have notational associativity.)

### Examples

Associativity is only needed when the operators in an expression have **the same precedence**. Usually `+` and `-` have the same precedence. Consider the expression `7 − 4 + 2`. The result could be either `(7 − 4) + 2 = 5` or `7 − (4 + 2) = 1`. The former result corresponds to the case when `+` and `−` are left-associative, the latter to when `+` and `-` are right-associative.

In order to reflect normal usage, [addition](https://en.wikipedia.org/wiki/Addition), [subtraction](https://en.wikipedia.org/wiki/Subtraction), [multiplication](https://en.wikipedia.org/wiki/Multiplication), and [division](https://en.wikipedia.org/wiki/Division_(mathematics)) operators are usually **left-associative**,[[1\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-Bronstein_1987-1)[[2\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-5) while for an [exponentiation](https://en.wikipedia.org/wiki/Exponentiation) operator (if present)[[6\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-Codeplea_2016-6) and [Knuth's up-arrow operators](https://en.wikipedia.org/wiki/Knuth's_up-arrow_notation) there is no general agreement. Any [assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science)) operators are typically **right-associative**. To prevent cases where operands would be associated with two operators, or no operator at all, operators with the same precedence must have the same associativity.




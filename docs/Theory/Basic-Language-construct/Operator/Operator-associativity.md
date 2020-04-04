# [Operator associativity](https://en.wikipedia.org/wiki/Operator_associativity)

In [programming languages](https://en.wikipedia.org/wiki/Programming_languages), the **associativity** of an [operator](https://en.wikipedia.org/wiki/Operator_(programming)) is a property that determines how operators of the same [precedence](https://en.wikipedia.org/wiki/Order_of_operations) are grouped in the absence of [parentheses](https://en.wikipedia.org/wiki/Bracket_(mathematics)). If an [operand](https://en.wikipedia.org/wiki/Operand) is both preceded and followed by operators (for example, `^ 3 ^`), and those operators have equal precedence, then the operand may be used as input to two different operations (i.e. the two operations indicated by the two operators). The choice of which operations to apply the operand to, is determined by the "**associativity**" of the operators. **Operators** may be **associative** (meaning the operations can be grouped arbitrarily（任意的）), **left-associative** (meaning the operations are grouped from the left), **right-associative** (meaning the operations are grouped from the right) or **non-associative** (meaning operations cannot be chained, often because the output type is incompatible with the input types). The associativity and precedence of an operator is a part of the definition of the programming language; different programming languages may have different associativity and precedence for the same type of operator.

在编程语言中，运算符的关联性是一种属性，它确定在没有括号的情况下如何对具有相同优先级的运算符进行分组。如果操作数在运算符之前和之后（例如^ 3 ^），并且那些运算符具有相同的优先级，则该操作数可以用作两个不同操作（即两个运算符指示的两个操作）的输入。将操作数应用于哪个运算的选择，取决于运算符的“关联性”。运算符可以是关联的（表示操作可以任意分组），左关联的（表示操作从左开始分组），右关联的（表示操作从右开始分组）或非关联的（表示操作不能链接，通常是因为输出类型与输入类型不兼容）。运算符的关联性和优先级是编程语言定义的一部分；对于相同类型的运算符，不同的编程语言可能具有不同的关联性和优先级。

Consider the expression `a ~ b ~ c`. If the operator `~` has left associativity, this expression would be interpreted as `(a ~ b) ~ c`. If the operator has right associativity, the expression would be interpreted as `a ~ (b ~ c)`. If the operator is non-associative, the expression might be a [syntax error](https://en.wikipedia.org/wiki/Syntax_error), or it might have some special meaning. Some mathematical operators have inherent（固有的） **associativity**. For example, subtraction and division, as used in conventional math notation, are **inherently left-associative**. Addition and multiplication, by contrast, are both left and right associative. (e.g. `(a * b) * c = a * (b * c)`).

Many programming language manuals provide a table of operator precedence and associativity; see, for example, [the table for C and C++](https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B#Operator_precedence).

The concept of notational associativity described here is related to, but different from the mathematical [associativity](https://en.wikipedia.org/wiki/Associativity). An operation that is mathematically associative, by definition requires no notational associativity. (For example, addition has the associative property, therefore it does not have to be either left associative or right associative.) An operation that is not mathematically associative, however, must be notationally left-, right-, or non-associative. (For example, subtraction does not have the associative property, therefore it must have notational associativity.)

## Examples

Associativity is only needed when the operators in an expression have **the same precedence**. Usually `+` and `-` have the same precedence. Consider the expression `7 − 4 + 2`. The result could be either `(7 − 4) + 2 = 5` or `7 − (4 + 2) = 1`. The former result corresponds to the case when `+` and `−` are left-associative, the latter to when `+` and `-` are right-associative.

In order to reflect normal usage, [addition](https://en.wikipedia.org/wiki/Addition), [subtraction](https://en.wikipedia.org/wiki/Subtraction), [multiplication](https://en.wikipedia.org/wiki/Multiplication), and [division](https://en.wikipedia.org/wiki/Division_(mathematics)) operators are usually **left-associative**,[[1\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-Bronstein_1987-1)[[2\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-5) while for an [exponentiation](https://en.wikipedia.org/wiki/Exponentiation) operator (if present)[[6\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-Codeplea_2016-6) and [Knuth's up-arrow operators](https://en.wikipedia.org/wiki/Knuth's_up-arrow_notation) there is no general agreement. Any [assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science)) operators are typically **right-associative**. To prevent cases where operands would be associated with two operators, or no operator at all, operators with the same precedence must have the same associativity.

### A detailed example

Consider the expression `5^4^3^2`, in which `^` is taken to be a **right-associative** exponentiation operator. A **parser** reading the tokens from left to right would apply the associativity rule to a branch, because of the right-associativity of `^`, in the following way:

1. Term `5` is read.
2. Nonterminal `^` is read. Node: "`5^`".
3. Term `4` is read. Node: "`5^4`".
4. **Nonterminal** `^` is read, triggering the **right-associativity rule**. **Associativity** decides node: "`5^(4^`".
5. Term `3` is read. Node: "`5^(4^3`".
6. Nonterminal `^` is read, triggering the re-application of the right-associativity rule. Node "`5^(4^(3^`".
7. Term `2` is read. Node "`5^(4^(3^2`".
8. No tokens to read. Apply **associativity** to produce **parse tree** "`5^(4^(3^2))`".

This can then be evaluated depth-first, starting at the top node (the first `^`):

1. The evaluator walks down the tree, from the first, over the second, to the third `^` expression.
2. It evaluates as: $3^2 = 9$. The result replaces the expression branch as the second operand of the second `^`.
3. Evaluation continues one level up the [parse tree](https://en.wikipedia.org/wiki/Parse_tree) as: $4^9 = 262144$. Again, the result replaces the expression branch as the second operand of the first `^`.
4. Again, the evaluator steps up the tree to the root expression and evaluates as: $5^262144 ≈ 6.2060699 × 10183230$. The last remaining branch collapses and the result becomes the overall result, therefore completing overall evaluation.

A left-associative evaluation would have resulted in the parse tree `((5^4)^3)^2` and the completely different results 625, 244140625 and finally ~5.9604645 × 1016.

## Right-associativity of assignment operators

In many [imperative programming languages](https://en.wikipedia.org/wiki/Imperative_programming_language), the [assignment operator](https://en.wikipedia.org/wiki/Assignment_operator) is defined to be right-associative, and assignment is defined to be an **expression** (with a value), not just a **statement**. This allows [chained assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science)#Chained_assignment) by using the value of one **assignment expression** as the input (right operand) of the next.

***SUMMARY*** : 上面这段话就表达了expression和statement之间的差异，需要将它添加到《`expression-statement-declaration.md`》中

For example, in [C](https://en.wikipedia.org/wiki/C_(programming_language)), the assignment `a = b` is an expression that returns a value (namely, `b` converted to the type of `a`) with the side effect of setting `a` to this value.[[a\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-7) An assignment can be performed in the middle of an expression. The right-associativity of the `=` operator allows expressions such as `a = b = c` to be interpreted as `a = (b = c)`, thereby setting both `a` and `b` to the value of `c`. In C, the alternative `(a = b) = c` does not make sense because `a = b` is not an [l-Value](https://en.wikipedia.org/wiki/Value_(computer_science)), just an r-value. However, in [C++](https://en.wikipedia.org/wiki/C%2B%2B) an assignment `a = b` returns a value referring to the left term in the assignment. Therefore, `(a = b) = c` can be interpreted as `a = b; a = c;`.

## Non-associative operators

Non-associative operators are operators that have no defined behavior when used in sequence in an expression. In Prolog the infix operator `:-` is **non-associative** because constructs such as "`a :- b :- c`" constitute syntax errors.

Another possibility is that sequences of certain operators are interpreted in some other way, which cannot be expressed as associativity. This generally means that syntactically, there is a special rule for sequences of these operations, and semantically the behavior is different. A good example is in [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), which has several such constructs.[[7\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-8) Since assignments are statements, not operations, the assignment operator does not have a value and is not associative. [Chained assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science)#Chained_assignment) is instead implemented by having a grammar rule for sequences of assignments `a = b = c`, which are then assigned left-to-right. Further, combinations of assignment and augmented assignment, like `a = b += c` are not legal in Python, though they are legal C. Another example are comparison operators, such as `>`, `==`, and `<=`. A chained comparison like `a < b < c` is interpreted as `(a < b) and (b < c)`, not equivalent to either `(a < b) < c` or `a < (b < c)`.[[8\]](https://en.wikipedia.org/wiki/Operator_associativity#cite_note-9)
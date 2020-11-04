# Operator expression and statement

大多数programming language都会涉及到这三个概念，本文对它们进行分析，我觉得从expression作为切入点是最好的，一是因为它是我们最最熟悉的（我们从小就学习数学，数学表达式的概念我们早已根深蒂固了），二是因为expression由operator构成，且expression也可以构成statement。

## Expression and operator

cppreference中对c++语言的[Expressions](https://en.cppreference.com/w/cpp/language/expressions)的介绍如下：

> An expression is a sequence of *operators* and their *operands*, that specifies a computation.
>
> **Expression evaluation** may produce a result (e.g., evaluation of 2+2 produces the result 4) and may generate side-effects (e.g. evaluation of `std::printf("%d",4)` prints the character '4' on the standard output).

`std::printf("%d",4)`涉及到的*operators*是`()`，在`c++`中，它表示的是函数调用；

显然expression是涉及到***operators***，提及***operators***，我们的第一反应就是数学中的一些常见的operator，`+-*/`，它们是一门programming language中最最基础的*operator*；其实这也揭示了programming language其实是深深的扎根于math的，programming language中的很多expression就是直接的[math expression](https://en.wikipedia.org/wiki/Expression_(mathematics))，programming language中的expression遵循着[math expression](https://en.wikipedia.org/wiki/Expression_(mathematics))系统的规律，这是因为programming language是起源自math，或者按照在文章[Language](https://dengking.github.io/Post/Language/Language/) 中所述的观点：使用programming language来描述计算。

但是programming language毕竟不是math，它还有很多其他的需求，因此，programming language扩展出了非常多的***operator***，如在`c`和`c++`中非常常见的[type conversion](https://en.cppreference.com/w/cpp/language/expressions)，如所有的programming language中都是支持的function call（[c++ function call](https://en.cppreference.com/w/cpp/language/operator_other)，[python function call](https://docs.python.org/3/reference/expressions.html#calls) ），如class member的access（[python Attribute references](https://docs.python.org/3/reference/expressions.html#attribute-references) ,[c++ member access](https://en.cppreference.com/w/cpp/language/operator_member_access)）等；

其实，programming language和math都是在使用***operator***，***operator***往往都是一些符号，programming language的能够使用的符号仅仅局限于ASCII符号，而math中能够使用的符号则是非常广泛的，它不仅仅局限于ASCII；所有programming language为了摆脱这个限制，只能够使用一些字母组合来表示一些operator，比如[c++ new](https://en.cppreference.com/w/cpp/language/new)，[python await](https://docs.python.org/3/reference/expressions.html#await-expression) ，这些表示operator的字母组合往往被programming language作为key word来使用。对于这些使用字母组合来表示的operator，是programmer非常任意忽视的；

> NOTE: [c++ new](https://en.cppreference.com/w/cpp/language/new)是一个operator，但是[c malloc](https://en.cppreference.com/w/c/memory/malloc)是一个function；

提及*operators*，就涉及到Operator precedence，[Wikipedia Order of operations](https://en.wikipedia.org/wiki/Order_of_operations) ，[c++ operator precedence](https://en.cppreference.com/w/cpp/language/operator_precedence)，[python precedence](https://docs.python.org/3/reference/expressions.html#operator-precedence)。

和[math expression](https://en.wikipedia.org/wiki/Expression_(mathematics))一样，programming language中的expression也是可以进行evaluation，这一点在[Expression (computer science)](https://en.wikipedia.org/wiki/Expression_(computer_science))中进行了介绍；如第一段所属，**Expression evaluation** may produce a result and may generate side-effects。

提及operator，就会涉及到operator overloading，[c++ operator overloading](https://en.cppreference.com/w/cpp/language/operators) ，[python Emulating numeric types](https://docs.python.org/3/reference/datamodel.html#special-method-names) 等；



总的来说，programming language的expression其实非常类似于math expression，programmer使用它们来表达如何进行运算；

关于expression，无论是c，`c++`还是python，本质上都遵循上面这段话所总结的；



### 参见

维基百科[Expression (computer science)](https://en.wikipedia.org/wiki/Expression_(computer_science))



## Statements

维基百科[Statement (computer science)](https://en.wikipedia.org/wiki/Statement_(computer_science))的介绍是比较好的：

> In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **statement** is a syntactic unit of an [imperative](https://en.wikipedia.org/wiki/Imperative_programming) [programming language](https://en.wikipedia.org/wiki/Programming_language) that expresses some action to be carried out. A program written in such a language is formed by a sequence of one or more statements. A statement may have internal components (e.g., [expressions](https://en.wikipedia.org/wiki/Expression_(computer_science))).

显然，上述观点和我在文章[Unit](https://dengking.github.io/Post/Unit/)中所总结的：

显然，如果将我们的program比作是一篇文章的话，statement就相当于“句子”。

### syntax

在维基百科[Statement (computer science)](https://en.wikipedia.org/wiki/Statement_(computer_science))的[Syntax](https://en.wikipedia.org/wiki/Statement_(computer_science)#Syntax)段介绍了描述（定义）statement的语法，比如python中使用indentation，c和`c++`中使用`;`。

### Classification

在维基百科[Statement (computer science)](https://en.wikipedia.org/wiki/Statement_(computer_science))的[Kinds of statements](https://en.wikipedia.org/wiki/Statement_(computer_science)#Kinds_of_statements)段介绍了描述（定义）statement的分类，大多数programming language都将statement分为：

- Simple statements
- Compound statements

### python statement

在python中将***statement***分为[simple statement](https://docs.python.org/3/reference/simple_stmts.html)和[Compound statements](https://docs.python.org/3/reference/compound_stmts.html) 。

python中对[simple statement](https://docs.python.org/3/reference/simple_stmts.html)的定义如下：

> A **simple statement** is comprised within a single **logical line**. Several **simple statements** may occur on a single line separated by semicolons. 

python中对[Compound statements](https://docs.python.org/3/reference/compound_stmts.html)的定义如下：

> **Compound statements** contain (groups of) other statements; they affect or control the execution of those other statements in some way. In general, **compound statements** span multiple lines, although in simple incarnations a whole compound statement may be contained in one line.

其实python中的[simple statement](https://docs.python.org/3/reference/simple_stmts.html)和[Compound statements](https://docs.python.org/3/reference/compound_stmts.html)的界定是非常简单的，

需要注意的是，上面我对statement的理解是：***statement***控制着程序执行的**流程**，其实这是不完备的，如[python  Function definitions](https://docs.python.org/3/reference/compound_stmts.html#function-definitions)和[python Class definitions](https://docs.python.org/3/reference/compound_stmts.html#grammar-token-classdef)都是statement，所以在python中statement还具备**定义**的功能。

那应该如何理解python中的**函数定义语句**呢？在python中，并没有declaration，函数定义语句在python中是name bind参看 [python Function definitions](https://docs.python.org/3/reference/compound_stmts.html#function-definitions) ，其中的解释是非常清楚的：

> A **function definition** is an **executable statement**. Its execution **binds** the function name in the current local namespace to a function object (a wrapper around the executable code for the function). This function object contains a reference to the current global namespace as the global namespace to be used when the function is called.

两者之间的本质差别在于python program最终是由python interpreter来解释执行的，并且python中everything is an object；而c++ program则需要由compiler进行compile，它的declaration都是向compiler进行declare；



***TODO*** ： 关于python lack declaration，需要再去查阅查阅资料，以下是一些重要资料：

[Python variable declaration](https://stackoverflow.com/questions/11007627/python-variable-declaration)

[python lacks declaration](https://docs.python.org/3/reference/executionmodel.html#execution-model)

### `c++` statement

在[`c++` statement](https://en.cppreference.com/w/cpp/language/statements)的分类与界定比python要复杂地多，如下是`c++`中的statement的分类 ：

C++ includes the following types of statements:

1) expression statements;

2) compound statements;

3) selection statements;

4) iteration statements;

5) jump statements;

6) declaration statements;

7) try blocks;

8) atomic and synchronized blocks (TM TS).



在`c++`中有[declaration statements](https://en.cppreference.com/w/cpp/language/declarations)，如`int n = 1;// declaration statement`，而python中却没有，[python lack declaration](https://docs.python.org/3/reference/executionmodel.html#execution-model) ，我觉得python中的declaration statement其实对应这python中的name bind；显然这是python和`c++`的显著差别；

总的来说：statement有着如下的功能：

- ***statement***控制着程序执行的**流程**（显然**expression**并不具备这样的功能），如顺序执行，循环执行（`for`，`while`），`return`，条件分支执行（`if-else`）；
- 定义与声明




## 从`=`来看待expression和statement之间的差异

在[Operator associativity](https://en.wikipedia.org/wiki/Operator_associativity)中有这样的描述：

> In many [imperative programming languages](https://en.wikipedia.org/wiki/Imperative_programming_language), the [assignment operator](https://en.wikipedia.org/wiki/Assignment_operator) is defined to be right-associative, and assignment is defined to be an **expression** (with a **value**), not just a **statement**. This allows [chained assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science)#Chained_assignment) by using the value of one assignment expression as the input (right operand) of the next.
>
> For example, in [C](https://en.wikipedia.org/wiki/C_(programming_language)), the assignment `a = b` is an **expression** that returns a value (namely, `b` converted to the type of `a`) with the side effect of setting `a` to this value. An assignment can be performed in the middle of an expression. The right-associativity of the `=` operator allows expressions such as `a = b = c` to be interpreted as `a = (b = c)`, thereby setting both `a` and `b` to the value of `c`. In C, the alternative `(a = b) = c` does not make sense because `a = b` is not an [l-Value](https://en.wikipedia.org/wiki/Value_(computer_science)), just an r-value. However, in [C++](https://en.wikipedia.org/wiki/C%2B%2B) an assignment `a = b` returns a value referring to the left term in the assignment. Therefore, `(a = b) = c` can be interpreted as `a = b; a = c;`.




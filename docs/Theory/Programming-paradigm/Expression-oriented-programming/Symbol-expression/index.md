# Symbol expression

在computer science、mathematics中 ，symbol expression是一种非常有效的、广泛采用的描述方式/语言，本章将对它进行专门描述。

Symbol使我们能够方便的创造、实现DSL；

Symbolic programming将symbol暴露给programmer，或者说允许programmer构建symbol expression，并且对symbolic expression进行操作。这样programmer就能够使用DSL进行描述，这大大增加了灵活性，一种典型的例子就是math language。

## What is symbol?

"symbol"即"符号"。

### wikipedia [Symbol (formal)](https://en.wikipedia.org/wiki/Symbol_(formal))



### symbol是一种抽象、指代

Symbol是一种抽象、指代，它无处不在:

1) 函数的入参可以看做是symbol，它可以指代多种类型的入参

2) polymorphism其实是依赖于symbol的

它使用一个symbol进行指代:

- single interface to entities of different types
- use of a single symbol to represent multiple different types

关于此参见wikipedia [**Polymorphism**](http://en.wikipedia.org/wiki/Polymorphism_(computer_science))。

2) formal grammar中，就是 使用symbol expression来描述language的abstract structure

从wikipedia [Formal grammar](https://en.wikipedia.org/wiki/Formal_grammar)中，可以看到，其中大量的使用symbol。

3) Computer algebra就是使用symbol expression来描述algebra的abstract structure

wikipedia [Computer algebra](https://en.wikipedia.org/wiki/Computer_algebra)





## Symbol expression in computer science

在computer science中，使用symbol expression来描述abstract structure(这其实是一次抽象过程)，然后依据此structure来生成computation。比如formal language、computational graph。理解语义。



本文是在computer algebra、formal language之上的，以一个更高的角度来描述它们，或者说，它们都可以看做是symbol的一种application 。



## Symbolic computation

### stackoverflow [What is symbolic computation?](https://stackoverflow.com/questions/16395704/what-is-symbolic-computation)

According to [wiki](https://en.wikipedia.org/wiki/Symbolic_computation):

> In mathematics and computer science, computer algebra, also called symbolic computation or algebraic computation is a scientific area that refers to the study and development of algorithms and software for manipulating mathematical expressions and other mathematical objects

Does symbolic computation focus on symbol manipulation and computation? Lisp program is written in the form of an AST with atoms as leaves. Lisp is said to be language for symbolic computing. Does it mean that in symbolic computation, it is:

- all about symbols (symbols are atoms or non-atom expressions in Lisp)
- every symbol is assigned a semantic
- symbolic computation is a paradigm that orients programmers to focus on working with symbols and semantics (a semantic can be an atom or expression that does something) and the relationships between symbols, as opposed to think that data structure and code are two separated entities.
- program design is language design, based on symbol composition/manipulation and semantic assignment.

According to [this question](https://stackoverflow.com/questions/5588962/what-is-the-opposite-to-symbolic-computation), the opposite of symbolic computation is numeric computation. What's the primary difference between these two? When I work with Octave (I'm studying it), I have to work with numbers a lot and have to guess the meaning of those magic numbers many times. Is this a numerical computation focus?



### risc [Studying Symbolic Computation](https://risc.jku.at/studying-symbolic-computation/)

> NOTE: 介绍地非常好，总结的非常好。

**Symbolic computation** is the sub-area of **mathematics** and **computer science** which solves problems on ***symbolic objects*** representable on a computer. Typical examples of such objects are

- algebraic expressions,
- logical propositions, and
- programs themselves.

The problem solutions are integrated in many advanced software systems for ***computer algebra, computer aided design and manufacturing, computer supported reasoning, knowledge management, and formal system specification and verification.*** Besides playing a fundamental role within mathematics itself, symbolic computation is thus a key technology in many scientific and technical areas today.

More than any other area, symbolic computation depends on the integration of the
![Symbolic Computation](https://risc.jku.at/wp-content/uploads/2018/03/SC-600x318.png)

- theoretical foundations (mathematics, logics, algorithms),
- the implementation in software systems, and the
- practical applications.

It is a main objective of the ***symbolic computation curriculum***(课程) at RISC to unite these three aspects. Graduates of this program are not only experts in symbolic computation but, due to their work in highly **abstract models** with the goal to develop effective software solutions, also professionals in developing innovative computer-based solutions for challenging problems arising in theoretical investigations and real-world applications. Therefore they are highly qualified to find their place in ***research and development***, in ***academia and industry***.

Within the realm(领域) of symbolic computation, research at RISC mainly falls into three general categories:

- **![Symbolic Computation at RISC](https://risc.jku.at/wp-content/uploads/2018/03/SC2.png)Computer Algebra**

  We design and implement algorithms that operate on algebraic expressions; typical application areas are e.g. (algebraic) geometry and (algorithmic) combinatorics.

- **Computational Logic**

  We work on the specification, management, and derivation of knowledge expressed in the language of symbolic logic (resulting in software systems for supporting mathematical proving) and on the theory of computation.

- **Mathematical Software**

  We develop various symbolic computation software such as it occurs in computer algebra systems and theorem provers and study the logical foundations of software for the purpose of formal system specification and verification.

These categories (which present different views to the same field with strong overlappings and interrelationships) are also reflected in the RISC curriculum. See this [list of applications](https://risc.jku.at/applications/) for sample outcomes of this research.





### [Symbolic Computation (An Editorial)](http://www3.risc.jku.at/publications/download/risc_2749/1985-03-00-C.pdf), [Bruno Buchberger](https://en.wikipedia.org/wiki/Bruno_Buchberger), [Journal of Symbolic Computation](https://en.wikipedia.org/wiki/Journal_of_Symbolic_Computation) (1985) 1, pp. 1–6.

> NOTE: 已经下载到了本地



## Symbol expression and tree

在computer science中，symbol expression 都可以使用tree、graph来进行描述。



## Formal semantic

我不知道本节标题的formal semantic是否准确，这是上周彭总提出的一个概念。其实对symbol expression的evaluation就是对它的语义的理解，关于此，我首先想到的是基于AST的evaluation，如果都是基于AST的话，那么operator的定义、含义的解释就非常重要了。

我的一个问题是: 是否都是基于AST的evaluation来实现semantic的理解？关于这个问题，可以阅读: 

1) `programming-language\docs\Theory\Programming-paradigm\Symbolic-programming\utexas-cs378.pdf`

2) 龙书中的描述

3) Google "tree and formal semantic"

4) [Formal Semantics and Formal Pragmatics, Lecture 1](http://people.umass.edu/partee/MGU_2009/materials/MGU091_2-up.pdf)  这篇文章不错，已经将他保存到了本地



## Implementation: symbolic programming

参见`./Symbolic-programming`章节。
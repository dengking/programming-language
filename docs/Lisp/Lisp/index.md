# Lisp



## wikipedia [Lisp (programming language)](https://en.wikipedia.org/wiki/Lisp_(programming_language))

**Lisp** (historically **LISP**) is a family of [programming languages](https://en.wikipedia.org/wiki/Programming_language) with a long history and a distinctive, fully [parenthesized](https://en.wikipedia.org/wiki/Parenthesized) [prefix notation](https://en.wikipedia.org/wiki/Polish_notation#Computer_programming).[[3\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-3) Originally specified in 1958, Lisp is the second-oldest [high-level programming language](https://en.wikipedia.org/wiki/High-level_programming_language) in widespread use today. Only [Fortran](https://en.wikipedia.org/wiki/Fortran) is older, by one year.[[4\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-5) Lisp has changed since its early days, and many [dialects](https://en.wikipedia.org/wiki/Programming_language_dialect) have existed over its history. Today, the best-known general-purpose Lisp dialects are [Racket](https://en.wikipedia.org/wiki/Racket_(programming_language)), [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp), [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)) and [Clojure](https://en.wikipedia.org/wiki/Clojure).

> NOTE: 前缀表达式

Lisp was originally created as a practical [mathematical notation](https://en.wikipedia.org/wiki/Mathematical_notation) (数学符号) for [computer programs](https://en.wikipedia.org/wiki/Computer_program), influenced by (though not originally derived from [[6\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-Steele,_Guy_Lewis,_Jr.;_Sussman,_Gerald_Jay-6)) the notation of [Alonzo Church](https://en.wikipedia.org/wiki/Alonzo_Church)'s [lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus). It quickly became the favored programming language for [artificial intelligence](https://en.wikipedia.org/wiki/Artificial_intelligence) (AI) research. As one of the earliest programming languages, Lisp pioneered many ideas in [computer science](https://en.wikipedia.org/wiki/Computer_science), including [tree data structures](https://en.wikipedia.org/wiki/Tree_(data_structure)), [automatic storage management](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)), [dynamic typing](https://en.wikipedia.org/wiki/Dynamic_typing), [conditionals](https://en.wikipedia.org/wiki/Conditional_(computer_programming)), [higher-order functions](https://en.wikipedia.org/wiki/Higher-order_function), [recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science)), the [self-hosting](https://en.wikipedia.org/wiki/Self-hosting_(compilers)) [compiler](https://en.wikipedia.org/wiki/Compiler),[[7\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-Graham-7) and the [read–eval–print loop](https://en.wikipedia.org/wiki/Read–eval–print_loop).[[8\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-8)

> NOTE: 通过Lisp也可以看出programming language的发展史。
>
> 关于lambda calculus，参见工程discrete的`Relation-structure-computation\Computation\Theory-of-computation\Model-of-computation\Lambda-calculus`章节。
>
> [mathematical notation](https://en.wikipedia.org/wiki/Mathematical_notation) 的意思是 "数学符号"，其实就是symbol。

The name *LISP* derives from "LISt Processor"(列表处理器).[[9\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-ArtOfLisp-9) [Linked lists](https://en.wikipedia.org/wiki/Linked_list) are one of Lisp's major [data structures](https://en.wikipedia.org/wiki/Data_structure), and Lisp [source code](https://en.wikipedia.org/wiki/Source_code) is made of lists. Thus, Lisp programs can manipulate source code as a data structure, giving rise to the [macro](https://en.wikipedia.org/wiki/Macro_(computer_science)) systems that allow programmers to create new syntax or new [domain-specific languages](https://en.wikipedia.org/wiki/Domain-specific_language) embedded in Lisp.

> NOTE: 最后一段所描述的其实就是symbolic programming，它让programmer实现创建DSL。在现代programming language中，有很多时候，都需要这种能力。



The interchangeability of code and data gives Lisp its instantly recognizable syntax. All program code is written as *[s-expressions](https://en.wikipedia.org/wiki/S-expression)*, or parenthesized lists. A function call or syntactic form is written as a list with the function or operator's name first, and the arguments following; for instance, a function `f` that takes three arguments would be called as `(f arg1 arg2 arg3)`.

> NOTE: s-expression就是symbolic expression，它是Lisp系列语言的核心。

### History

[John McCarthy](https://en.wikipedia.org/wiki/John_McCarthy_(computer_scientist)) developed Lisp in 1958 while he was at the [Massachusetts Institute of Technology](https://en.wikipedia.org/wiki/Massachusetts_Institute_of_Technology) (MIT). McCarthy published its design in a paper in *[Communications of the ACM](https://en.wikipedia.org/wiki/Communications_of_the_ACM)* in 1960, entitled "**Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I**".[[10\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-MCCARTHY-10) He showed that with a few simple operators and a notation for **anonymous functions** borrowed from Church, one can build a [Turing-complete](https://en.wikipedia.org/wiki/Turing_completeness) language for algorithms.

> NOTE: 上面这段话中的**anonymous functions**，其实就是lambda function。

#### Genealogy and variants

> NOTE: 家系和变体

Over its sixty-year history, Lisp has spawned many variations on the core theme of an **S-expression language**. 

> NOTE: Lisp系列语言的核心是: **S-expression language**

### Syntax and semantics

#### Symbolic expressions (S-expressions)

Lisp is an [expression oriented language](https://en.wikipedia.org/wiki/Expression_oriented_language). Unlike most other languages, no distinction is made between "expressions" and ["statements"](https://en.wikipedia.org/wiki/Statement_(programming));[*[dubious](https://en.wikipedia.org/wiki/Wikipedia:Accuracy_dispute#Disputed_statement) – [discuss](https://en.wikipedia.org/wiki/Talk:Lisp_(programming_language)#Dubious)*] all code and data are written as expressions. When an expression is *evaluated*, it produces a value (in Common Lisp, possibly multiple values), which can then be embedded into other expressions. Each value can be any data type.

McCarthy's 1958 paper introduced two types of syntax: *Symbolic expressions* ([S-expressions](https://en.wikipedia.org/wiki/S-expression), sexps), which mirror the internal representation of code and data; and *Meta expressions* ([M-expressions](https://en.wikipedia.org/wiki/M-expression)), which express functions of S-expressions. M-expressions never found favor, and almost all Lisps today use S-expressions to manipulate both code and data.

#### Lists

> NOTE: 使用List来表示expression tree

Expressions are written as lists, using [prefix notation](https://en.wikipedia.org/wiki/Polish_notation). 

#### Operators



#### Lambda expressions and function definition



#### Conses and lists

*Main article:* [Cons](https://en.wikipedia.org/wiki/Cons)

A Lisp list is implemented as a [singly linked list](https://en.wikipedia.org/wiki/Singly_linked_list).[[55\]](https://en.wikipedia.org/wiki/Lisp_(programming_language)#cite_note-SebestaLanguages-55) Each cell of this list is called a *cons* (in Scheme, a *pair*), and is composed of two [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)), called the *car* and *cdr*. These are respectively equivalent to the `data` and `next` fields discussed in the article *[linked list](https://en.wikipedia.org/wiki/Linked_list)*.
# 关于本章

## Expression

既然本章讨论expression-oriented language，那么首先需要对expression的内容进行梳理，在下面章节中涉及到了expression:

1) `Theory\Programming-language-construct\Basic-Language-construct`

2) `C-family-language\C-and-C++\Expression`

3) 工程compiler-principle的`Chapter-2-A-Simple-Syntax-Directed-Translator\2.2-Syntax-Definition`节

4) 工程language的`Language\Language-in-discipline`章节

## Expression-oriented language and lambda-calculus

lambda-calculus是一个非常强大的abstraction model，现代很多programming language的内容都是源自于它。本章描述的expression-oriented programming language其实和它有着非常深刻的渊源。

## Symbol expression

在computer science、mathematics中 ，symbol expression是一种非常有效的、广泛采用的描述方式/语言，我们将在`./Symbol-expression`章节对它进行描述。

## Expression is function

本节标题的含义是: 我们可以将Expression看做是function，这就像是C++中的unified function model。我们可以看到functional programming其实就是一堆的expression。

## Symbolic programming and functional programming

它们其实都是基于lambda-calculus的。



## Expression-oriented language and formal grammar

Formal grammar所描述的是nesting关系，它用于描述formal language的structure，formal grammar构成了axiom schema；

Expression其实也有着nesting关系，典型的例子就是使用**括号**；

由于它们都有着nesting关系，所以都可以使用tree来结构化，从而实现它们的evaluation。


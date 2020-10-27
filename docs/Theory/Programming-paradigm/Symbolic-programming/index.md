# Symbolic programming

在学习deep learning的时候，其中描述了symbolic programing，因此，启发了我需要对symbolic programming进行总结。

wolfram是采用symbolic programming paradigm的一种programming language，所以，我们结合wolfram [Symbolic Programming: Computationally Active Language](https://blog.wolfram.com/2007/06/15/symbolic-programming-computationally-active-language/) 来理解symbolic programming。

## wolfram [Symbolic Programming: Computationally Active Language](https://blog.wolfram.com/2007/06/15/symbolic-programming-computationally-active-language/)

> NOTE: 这篇文章的作者从一个非常高、抽象的层次对symbolic programming进行了描述。wolfram是一种programming language，它是支持symbolic programming的。

In this blog and elsewhere, you’ll often see the statement that some advanced [Mathematica](http://www.wolfram.com/mathematica/) feature is just another application of [symbolic programming](http://reference.wolfram.com/mathematica/guide/LanguageOverview.html). It’s the kind of idea that seems too powerful to explain in a single blog post, yet simple enough that I am tempted to try. So, here goes.

> 翻译: 在本博客和其他地方，您将经常看到这样的说法:某些高级Mathematica特性只是symbolic programming的另一个应用。这种想法似乎过于强大以至于无法在一篇博客文章中解释清楚，但却足够简单，让我忍不住去尝试一下。

**Symbolic programming** is based on the concept of recasting(重铸) core features of **human language** into a **computationally active form**.

> 翻译: 符号编程是基于如下概念: 将人类语言的核心特性重铸为计算活动的形式。在后面将它称为"human-language-oriented programming language"即"面向人类语言的编程语言"。
>
> NOTE: 那上述**computationally active form**是什么form呢？在"Computation: The Transformation of Meaning"章节中会对此进行说明。

What does it mean to have a human-language-oriented programming language?

> 翻译: 拥有一种面向人类语言的编程语言意味着什么?

Our cognitive model of computation is typically a three-stage process: 

1) describing the computation; 

2) executing that description; and 

3) outputting the results.

The “language” part of most programming languages begins and ends with stage one. **Linguistic structures** are erected(建立) to describe the program. But the execution of the program is typically oriented around an entirely different system of types and objects; and likewise, the program’s output structure tends to resemble nothing particularly language-like.

**Symbolic programming** uses **linguistic structures** as the foundation of all aspects of **computation**. From a computation’s description, to how the computation executes, to how humans interface with the results, the exact same basic **tree structure** is used throughout.

This is a powerful **unification**(统一), making possible many useful computations that in other systems range from cumbersome to practically impossible. We’ll see examples along the way, but let me first describe what these linguistic structures actually are.

> 翻译: 这是一个强大的统一，使得许多有用的计算成为可能，而这些计算在其他系统中从繁琐到几乎不可能。
> 我们会看到一些例子，但让我先描述一下这些语言结构到底是什么。
>
> 下面的岁上面三段话的内容的理解、分析: 
>
> 第一段话和第二段话对比了other programming language和programming language using symbolic programming: 作者认为它们之间主要差异是: 
>
> **Symbolic programming** 的 model of computation 都是基于它的  **linguistic structures**，它采用的是**unification**(统一)的方式；**Symbolic programming** 的 model of computation 其实也非常强大，也能够实现非常多的computation，在第三段话中，对此进行了描述。
>
> other programming language则不是采用的**unification**(统一)的方式，它们的execution of the program不是基于它们的**Linguistic structures**。结合C来理解上面这段话: C program是需要符合一定的**linguistic structure**的，C program需要经过compiler的翻译，转换为instruction的方式来由CPU执行，显然C program的执行与C program的linguistic structure是不一致的。
>
> 在 stackoverflow [Mathematica: what is symbolic programming?](https://stackoverflow.com/questions/4430998/mathematica-what-is-symbolic-programming) # [A](https://stackoverflow.com/a/4435720) 中对**Symbolic programming** 的 model of computation 也有着相关的解释: 
>
> > I would characterize a symbolic programming environment as one in which the expressions used to represent program text also happen to be the primary data structure. As a result, it becomes very easy to build abstractions upon abstractions since data can easily be transformed into code and vice versa.
>
> 当今，programming language的  **linguistic structure** 都是采用的tree structure。

### Symbols: The Atoms of Language

> 翻译: 符号:语言的原子

**Symbols** are the basic **atoms** of language and also of **symbolic programming**. The point of **symbols** is to have distinct pieces of notation—be they words or mathematical functions or musical notes—which are then assigned an **interpretation** within the overall system.

> 翻译: 符号是语言的基本原子，也是符号编程的基本原子。符号的意义在它能够有不同的notation——无论是单词、数学函数还是音符——然后在整个系统中被分配解释。
>
> 直白的意思是: symbol其实是一种指代，它能够指代的事物的范围是非常广泛的，比如这段话中所列举的: word、mathematical function，至于symbol所指代的内容，需要后续进行赋值，这就是最后一段话的含义。在deep learning中，一般将symbol称为placeholder，比如:
>
> 1) mxnet [Deep Learning Programming Paradigm](https://mxnet.apache.org/versions/master/api/architecture/program_model):
>
> > We define the abstract function in terms of **placeholder** values
>
> 2) TensorFlow
>
> 后面会对symbol的含义、作用进行更加全面的描述。

#### symbol VS variable

> NOTE: 两者的核心差别在于: variable需要提前定义才能够使用，而symbol不需要定义就可以直接使用。

In programming, the defining feature of **symbols** is that they can stand for themselves. This is in contrast to **variables**, which must stand for some other value or be considered an error. For instance, JavaScript cannot perform the operation `x + x` unless `x` has a value:

![>>> x | x is not defined | >>> x+x | x is not defined](https://blog.wolfram.com/images/kovas/CompLang1a.gif)

while **Mathematica** has no trouble:

![In[1]:= x + x | Out[1]= 2 x](https://blog.wolfram.com/images/kovas/CompLang2.gif)



By existing simply as distinct entities, **symbols** serve as lightweight carriers of meaning. They are inert（惰性的、lazy）, but can acquire meaning through combination with other symbols. For example the symbol for [`Pi`](https://reference.wolfram.com/language/ref/Pi.html) is generically just another symbol, but when it appears “within” the symbol [`N`](https://reference.wolfram.com/language/ref/N.html), it takes on its special numerical meaning:

![pic](https://blog.wolfram.com/images/kovas/CompLang3.gif)

Because Mathematica manipulates things abstractly, it is able to do more-precise computation than some other systems. As the above example makes clear, symbols are only the building blocks: the real action is with the arrangements of symbols.

> NOTE: arrangements of symbols其实就是前面一直强调的structure，就是下面这段话中描述的Symbolic Expression。

### Symbolic Expressions: Combining Symbols into Meaning

> NOTE: 本节标题中，所谓的"meaning"，其实是semantic。

An English phrase is built of subphrases. A mathematical formula is built of subformulas. A musical score is built of bars, which contain notes. A web page layout is built from sublayouts. When humans want to communicate nontrivial structures, they use hierarchical nesting.

> NOTE: 关于nesting关系，参见工程discrete的`Relation-structure-computation\Model\Nesting-relation-model`章节

Across different languages and domains, these structures, or trees, take many syntactic forms. However, they can always be reduced to very simple data structures known as **symbolic expressions**. **Symbolic expressions** simply capture the concept of arranging symbols into **trees**. Here are a few trees built from the symbol “e”:

![pic](https://blog.wolfram.com/images/kovas/CompLang4.gif)

Like the symbols themselves, trees of symbols may simply exist, without a need to correspond to something else:

![In[4]:= e[e] | Out[4]= e[e]](https://blog.wolfram.com/images/kovas/CompLang5.gif)

**Symbolic expressions** are the fabric from which we construct meaning. Different tree structures correspond to different meanings.

> NOTE: 这段话其实是对本节标题的解释，上面这段话的意思是:
>
> 在symbolic programming中，我们会使用 **symbolic expression** ，通过 **symbolic expression** ，我们实现了 "construct meaning"。不同的 **symbolic expression** 有着不同的 tree structure，从而表达不同的 meaning。

A tree with symbols like [`Plus`](https://reference.wolfram.com/language/ref/Plus.html) and [`Power`](https://reference.wolfram.com/language/ref/Power.html) may represent an arithmetic expression, while the symbol [`SetterBar`](https://reference.wolfram.com/language/ref/SetterBar.html) can be combined with a subtree indicating the current selection and a list of possible alternatives:

![pic](https://blog.wolfram.com/images/kovas/CompLang6b.gif)

Everything in **Mathematica** is ultimately represented using this **uniform simple structure**. At the formal level, it ensures a degree of **structural compatibility** across all parts of the far-flung(广泛的) system. It also provides a system for introducing your own lightweight yet sophisticated structures.

[Pure functions](https://reference.wolfram.com/language/tutorial/PureFunctions.html), for example, are the original killer apps for lightweight structures of this vein. In Mathematica, they can be represented with [`Function`](https://reference.wolfram.com/language/ref/Function.html), but let’s roll our own using other generic primitives:

```mathematica
MyPureFunction[var_, expr_][arg_] := expr /. var -> arg
```

Let’s describe a function that adds 1 to its argument, and attach it to the symbol `f`. Notice the structure stays inert(惰性), and therefore can be passed around as data:

![In[6]:= f = MyPureFunction[x, x + 1] | Out[6]= MyPureFunction[x, 1 + x]](https://blog.wolfram.com/images/kovas/CompLang6c.gif)

When brought into contact with an **argument**, the definition fires.

![In[7]:= f[a] | Out[7]= 1 + a](https://blog.wolfram.com/images/kovas/CompLang6d.gif)

In **symbolic programming**, it is commonplace（司空见惯的） for a programmer to create **abstractions** that in another language would have required a modification of the core specification. This is the beauty of language: that creating new concepts does not require inventing new atomic “types” of things—only new arrangements of structure.

> NOTE: 上面这段话的含义是: 在其它的programming language中，创建新的abstraction需要modification of the core specification，但是在**symbolic programming**中并不需要，它只需要new arrangement of structure。

### Computation: The Transformation of Meaning

The most important and unique aspect of **symbolic programming** is that these tree structures can be made **computationally active**. They don’t just describe things—they actually execute.

The basic idea is that because **meaning** is encoded by **structure**, **computation**—the transformation of one **meaning** into another—corresponds to the transformation of one structure into another.

For instance, position 1 in the `SetterBar` expression represents the current selection. By modifying the tree at that position, we modify the meaning of the expression as a whole:

![In[8]:= ReplacePart[a b  c, 1->b] | Out[8]= a b c | In[9]:= a b c // InputForm | Out[9]/InputForm= SetterBar[b, {a, b, c}]](https://blog.wolfram.com/images/kovas/CompLang7.gif)

A computation begins and ends with unique, domain-specific structures. The process of computation, however, is executed using extremely general methods that are exactly the same across domains:



![](https://blog.wolfram.com/images/kovas/CompLang8.gif)

So, **computation** proceeds by very generic **tree transformations**. Rather conveniently, trees are also how we specify the computations themselves.

> NOTE: computation、representation都是基于tree的，这就是在本文前面所提出的核心思想。

This reduces the process of computation into a linguistic puzzle: using trees to describe how other trees should transform and intersect. Not only does this allow computational primitives to have an immediate and clear meaning, it also allows the process of language design to proceed organically as regularities in these tree structures are discovered and abstracted.

### Representing and Interacting with Meaning

Symbolic programming’s comprehensive emphasis on meaning allows a unique perspective on input, output and the whole interaction cycle between human and machine computation.

The basic idea is that symbolic expressions are the canonical, abstract containers of meaning. Given a symbolic expression, its meaning can then be embodied in a number of useful ways.

For example, the expression `Plus[2,2]` is rendered as `2+2`. And `Graphics[Rectangle[]]` renders as:

![Rectangle](https://blog.wolfram.com/images/kovas/CompLang9.gif)

And it works similarly for all sorts of domain-specific mathematics notation, documents and even [sound](https://reference.wolfram.com/language/ref/Sound.html) and [music](https://reference.wolfram.com/language/Music/tutorial/Music.html) primitives.

On their own, these are useful representations. But the fact that they are still really the same underlying expression leads to a powerful consequence: human computation and programmatic computation become structurally equivalent.

If I interactively create a [`Disk`](https://reference.wolfram.com/language/ref/Disk.html) using a drawing palette:

![Disk with drawing palette](https://blog.wolfram.com/images/kovas/CompLang10.gif)

its underlying data structure is the same as it would be if I had typed in the code, or if it had been generated by a program:

![InputForm for disk](https://blog.wolfram.com/images/kovas/CompLang11.gif)
`Graphics[Disk[{0.49, 0.49}, {0.35, 0.35}],ImageSize -> {36.335, Automatic},PlotRange -> {{0, 1}, {0, 1}}]`

Completely different representations—in this case text and graphics—are immediately and transparently compatible, because in the end they are all just [expressions](https://reference.wolfram.com/language/guide/Expressions.html). Here we have some “zero-overhead” code to color just the disks of a [graphic](https://reference.wolfram.com/language/ref/Graphics.html):

![ColorDisks[x_] := x /. {y_Disk -> {Blue, y}}](https://blog.wolfram.com/images/kovas/CompLang12.gif)

![ColorDisks function using graphics of Rectangle and Disk](https://blog.wolfram.com/images/kovas/CompLang13.gif)

The equivalence between these representations, and between human and programmatically generated structures, opens the door to exceptionally powerful ways of interacting with computation. It is the basis of Mathematica’s [dynamic interactivity](http://www.wolfram.com/mathematica/newin6/content/DynamicInteractivity/) framework.

The implications, however, go much further than that. Symbolic programming is the first abstraction that can uniformly span all three stages of computation, from specification to execution to output, and as a result it can leverage each aspect against the others.

Programs can be created by traditional programming, by the programs themselves or by interaction with what is considered output. The process of computation can be equivalently executed by the user or by the machine. Output is simply exposing a window into a piece of a (possibly running) program.

It’s pretty impressive that so many useful consequences can flow from the simple idea of **symbolic programming**: fitting computation to language, rather than the other way around.



## TODO

1) utexas [CS 378: Symbolic Programming](https://www.cs.utexas.edu/~novak/cs378.html)

已经将**Lecture Note**下载到本地了。

2) ybrikman [Six programming paradigms that will change how you think about coding](https://www.ybrikman.com/writing/2014/04/09/six-programming-paradigms-that-will/)#Symbolic programming

3) stackoverflow [Mathematica: what is symbolic programming?](https://stackoverflow.com/questions/4430998/mathematica-what-is-symbolic-programming)

4) wikipedia [Symbolic programming](https://en.wikipedia.org/wiki/Symbolic_programming) and wikipedia [Symbolic language (programming)](https://en.wikipedia.org/wiki/Symbolic_language_(programming))

## Symbolic programming and formal grammar

Symbolic programming 和 formal grammar都是基于nesting关系构建的。

本质上它们都是expression。

## Application: Computer algebra

Symbolic programming的一个重要application就是mathematics，本节对此进行梳理。

关于Computer algebra，参见工程discrete的`Relation-structure-computation\Computation\Computer-algebra`章节，或者wikipedia [Computer algebra](https://en.wikipedia.org/wiki/Computer_algebra)。

在software engineering中，我们常常需要以通用的方式来描述一个数学计算，比如: 

1) 如何描述computation graph

2) 在量化交易中，自定义**基差**计算公式

3) ......

在这类应用中，往往存在着两个阶段:

1) 构建阶段

使用symbol来构建math expression，往往需要使用domain-specifier language，比如直接使用math language。

2) Evaluation阶段

然后对这些symbol进行赋值，然后对math expression进行evaluate，从而得到最终的结果。

### Implementation

下面是一些具体的实现: 

1) [SymPy](https://www.sympy.org/en/index.html)

**SymPy** is a Python library for **symbolic mathematics**. It aims to become a full-featured **computer algebra system** (CAS) while keeping the code as simple as possible in order to be comprehensible and easily extensible. SymPy is written entirely in Python.

2) ArashPartow [exprtk](https://github.com/ArashPartow/exprtk)

3) Deep learning

Deep learning中，我们需要构建computational graph，computational graph就是典型的symbolic mathematics。目前大多数deep learning implementation，都支持symbolic programming。

4) Expression template

Expression template是C++ TMP中的一种重要trick，参见`C-family-language\C++\Idiom\TMP\Expression-Template`。

5) Wolfram Language

参见wikipedia [Wolfram Language](https://en.wikipedia.org/wiki/Wolfram_Language): 

> It emphasizes [symbolic computation](https://en.wikipedia.org/wiki/Symbolic_computation), [functional programming](https://en.wikipedia.org/wiki/Functional_programming), and [rule-based programming](https://en.wikipedia.org/wiki/Rule-based_programming)[[8\]](https://en.wikipedia.org/wiki/Wolfram_Language#cite_note-LangName-8) and can employ arbitrary [structures](https://en.wikipedia.org/wiki/Data_structure) and data.[[8\]](https://en.wikipedia.org/wiki/Wolfram_Language#cite_note-LangName-8) It is the programming language of the mathematical symbolic computation program [Mathematica](https://en.wikipedia.org/wiki/Mathematica).[[9\]](https://en.wikipedia.org/wiki/Wolfram_Language#cite_note-25anv-9)



### Symbolic programming VS imperative programming in Symbolic mathematics

关于这个主题，参见工程machine-learning的`Programming\Programming-paradigm\Symbolic-and-imperative`章节。
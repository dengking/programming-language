# [PEG Parsers](https://medium.com/@gvanrossum_83706/peg-parsers-7ed72462f97c)

Some years ago someone asked whether it would make sense to switch Python to a **PEG parser**. (Or a PEG grammar; I don’t recall exactly what was said by whom, or when.) I looked into it a bit and wasn’t sure what to think, so I dropped the subject. Recently I’ve learned more about PEG ([Parsing Expression Grammars](https://en.wikipedia.org/wiki/Parsing_expression_grammar)), and I now think it’s an interesting alternative to the home-grown parser generator that I developed 30 years ago when I started working on Python. (That parser generator, dubbed “pgen”, was just about the first piece of code I wrote for Python.)

[This is part 1 of my PEG series. See the [Series Overview](https://medium.com/@gvanrossum_83706/peg-parsing-series-de5d41b2ed60?sk=0a7ce9003b13aae8126a4a23812eb035) for the rest.]

The reason I’m now interested in PEG parsing is that I’ve become somewhat annoyed with pgen’s limitations. It uses a variant of LL(1) parsing that I cooked up myself — I didn’t like grammar rules that could produce the empty string, so I disallowed that, thereby simplifying the algorithm for producing parsing tables somewhat. I also invented my own EBNF-like grammar notation, which I still like very much.

Here are some of the issues with pgen that annoy me. The “1” in the LL(1) moniker implies that it uses only a single token lookahead, and this limits our ability of writing nice grammar rules. For example, a Python statement can be an expression or an assignment (or other things, but those all start with a dedicated keyword like `if` or `def`). We’d like to write this syntax as follows using the pgen notation. (Note that this example describes a toy language that is a tiny subset of Python, as is traditional in writing about language design.)

```
statement: assignment | expr | if_statement
expr: expr '+' term | expr '-' term | term
term: term '*' atom | term '/' atom | atom
atom: NAME | NUMBER | '(' expr ')'
assignment: target '=' expr
target: NAME
if_statement: 'if' expr ':' statement
```

A few words about the notation: `NAME` and `NUMBER` are tokens and predefined outside the grammar. Strings in quotes like `'+'` or `'if'` are are also tokens. (I should talk about tokens some other time.) Grammar rules start with a rule name followed by `:`, followed by one or more alternatives separated by `|`.

The problem is that if you write the grammar like this, the parser does not work, and pgen will complain. One of the issues is that some rules (`expr` and `term`) are **left-recursive**, and pgen isn’t smart enough to do the right thing here. This is typically solved by rewriting those rules, for example (leaving the other rules unchanged):

```
expr: term ('+' term | '-' term)*
term: atom ('*' atom | '/' atom)*
```

This reveals a few bits of pgen’s EBNF capabilities: you can nest alternatives inside parentheses, and you can create repetitions by placing `*` after an item, so the rule for `expr` here means “it’s a term, followed by zero or more repetitions of plus followed by a term or minus followed by a term”. This grammar accepts the same language as the first version, but it doesn’t reflect the intent of the language designer as well — in particular, it doesn’t show that the operators are **left-binding**, which is important when you are trying to generate code.

But there’s another annoying problem in this toy language (and in Python). Because of the single-token lookahead, the parser cannot determine whether it is looking at the start of an expression or an assignment. At the beginning of a statement, the parser needs to decide what alternative for `statement` it is seeing from the first token it sees. (Why? This is how pgen’s parsing automation works.) Let’s say our program is this:

```
answer = 42
```

This program is tokenized into three tokens:`NAME` (with value `answer`), `'='`, and `NUMBER` (with value `42`). The only lookahead we have at the start of the program is the first token, `NAME`. The rule we are trying to satisfy at this point is `statement` (the grammar’s start symbol). This rule has three alternatives: `expr`, `assignment`, and `if_statement`. We can rule out `if_statement`, because the lookahead token isn’t `'if'`. But both `expr` and `assignment` (can) start with a `NAME` token, and because of this pgen rejects our grammar as being ambiguous.

(That’s not entirely correct, since technically the *grammar* isn’t ambiguous per se; but we’ll ignore this because I don’t know if there’s a better word. And how does pgen decide this? It computes something called the FIRST set for each grammar rule, and it complains if the FIRST sets of the choices at any given point overlap.)

So couldn’t we solve this annoyance by giving the parser a larger lookahead buffer? For our toy example, a second lookahead token would be enough, since in this grammar the second token of an assignment must be `'='`. But in a more realistic language like Python, you may need an unlimited lookahead buffer, since the stuff to the left of the `'='` token may be arbitrarily complex, for example:

```
table[index + 1].name.first = 'Steven'
```

That’s already ten tokens before we encounter the `'='` token, and I could cook up arbitrary long examples if challenged. What we’ve done to solve this in pgen is to change the grammar to accept some illegal programs, adding an extra check in a later pass that generates a `SyntaxError` if it finds an invalid left-hand side for an assignment. For our toy language, this comes down to writing the following:

```
statement: assignment_or_expr | if_statement
assignment_or_expr: expr ['=' expr]
```

(The square brackets indicate an optional part.) And then in a subsequent compiler pass (say, when generating bytecode) we check whether there’s an `'='` or not, and if there is, we check that the left-hand side follows the syntax for `target`.

There’s a similar annoyance around keyword arguments in function calls. We would *like* to write something like this (again, a simplified version of Python’s call syntax):

```
call: atom '(' arguments ')'
arguments: arg (',' arg)*
arg: posarg | kwarg
posarg: expr
kwarg: NAME '=' expr
```

But the single-token lookahead can’t to tell the parser whether a `NAME` at the start of an argument is the beginning of `posarg` (since `expr` may start with `NAME`) or the beginning of `kwarg`. Again, Python’s current parser solves this by essentially stating

```
arg: expr ['=' expr]
```

and then sorting out the cases in a subsequent compiler pass. (We even got this slightly wrong and allowed things like `foo((a)=1)`, giving it the same meaning as `foo(a=1)`, until we fixed it in Python 3.8.)

So how does a **PEG parser** solve these annoyances? By using an **infinite lookahead buffer**! The typical implementation of a PEG parser uses something called “packrat parsing”, which not only loads the entire program in memory before parsing it, but also allows the parser to backtrack arbitrarily. While the term PEG primarily refers to the *grammar* notation, the *parsers* generated from PEG grammars are typically recursive-descent parsers with unlimited backtracking, and packrat parsing makes this efficient by memoizing the rules already matched for each position.

This makes everything easy, but of course there’s a cost: memory. Thirty years ago, I had a good reason to use a parsing technology with a single token lookahead: memory was expensive. LL(1) parsing (and other technologies like LALR(1), made famous by YACC) uses a state machine and a stack (a “push-down automaton”) to construct a parse tree efficiently.

Fortunately the computers on which CPython runs have a lot more memory than 30 years ago, and keeping the entire file in memory really isn’t much of a burden any more. For example, the largest non-test file in the stdlib that I could find is `_pydecimal.py`, which clocks in at around 223 kilobytes. In a Gigabyte world, that’s essentially nothing. And that’s what led me to have another look at parsing technology.

But there’s another thing about CPython’s current parser that bugs me. Compilers are complicated things, and CPython’s is no exception: while the output of the pgen-generated parser is a **parse tree**, this **parse tree** is not directly used as the input to the **code generator**: first it is transformed to an *abstract syntax tree* (AST), and then that **AST** is compiled into **bytecode**. (There’s more to it, but that’s not my focus here.)

Why not compile from the **parse tree**? That is how it originally worked, but about 15 years ago we found that the compiler was complicated by the structure of the **parse tree**, and we introduced a separate AST, and a separate translation phase from **parse tree** to AST. As Python evolves, the **AST** is more stable than the **parse tree**, so this reduces the opportunity for bugs in the compiler.

The AST is also easier to work with for third-party code that wants to inspect Python code, and is exposed through the popular `ast` module. This module also lets you construct AST nodes from scratch and modify existing AST nodes, and you can compile the new nodes to bytecode. The latter has enabled an entire cottage industry of language extensions for Python. (The parse tree is *also* exposed to Python users, via the `parser` module, but it is much more cumbersome to work with; therefore it has gone out of style in favor of the `ast` module.)

My idea now, putting these things together, is to see if we can create a new parser for CPython that uses PEG and packrat parsing to construct the AST directly during parsing, thereby skipping the intermediate parse tree construction, possibly *saving* memory despite using an infinite lookahead buffer. I’m not there yet, but I have a prototype that can compile a subset of Python into an AST at about the same speed as CPython’s current parser. It uses more memory, however, and I expect that extending the subset to the full language will slow down the PEG parser. But I also haven’t done anything to optimize it, so I am hopeful.

A final advantage of switching to PEG is that it provides more flexibility for future evolution of the language. In the past it’s been said that pgen’s LL(1) restrictions have helped Python’s grammar stay simple. That may well be so, but we have plenty of other processes in place to prevent unchecked growth of the language (mainly the PEP process, aided by very strict backwards compatibility requirements and the new governance structure). So I’m not worried.

I have a lot more to write about PEG parsing and my specific implementation, but I’ll write about that in a [later post](https://medium.com/@gvanrossum_83706/building-a-peg-parser-d4869b5958fb), after I’ve cleaned up the code.

License for this article and the code shown: [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)
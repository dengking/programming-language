# [Generating a PEG Parser](https://medium.com/@gvanrossum_83706/generating-a-peg-parser-520057d642a9)

Now that I’ve sketched the infrastructure for a parser and a simple hand-written parser in [part 2](https://medium.com/@gvanrossum_83706/building-a-peg-parser-d4869b5958fb), let’s turn to generating a parser from a grammar, as promised. I’ll also show how to implement packrat parsing using a `@memoize` decorator.

[This is part 3 of my PEG series. See the [Series Overview](https://medium.com/@gvanrossum_83706/peg-parsing-series-de5d41b2ed60?sk=0a7ce9003b13aae8126a4a23812eb035) for the rest.]

Last time we ended with a **hand-written parser**. With some limitations to the grammar, it’s easy to generate such parsers automatically from the grammar. (We’ll lift those limitations later.)

We need two things: something that reads the grammar, constructing a **data structure** representing the **grammar rules**; and something that takes that **data structure** and generates the **parser**. We also need boring glue that I’ll omit.

So what we’re creating here is a simple compiler-compiler. I’m simplifying the grammar notation a bit to the point where we just have rules and alternatives; this is actually sufficient for the toy grammar I’ve been using in the previous parts of the series:

```
statement: assignment | expr | if_statement
expr: expr '+' term | expr '-' term | term
term: term '*' atom | term '/' atom | atom
atom: NAME | NUMBER | '(' expr ')'
assignment: target '=' expr
target: NAME
if_statement: 'if' expr ':' statement
```

Using the full notation we can write up the grammar for grammar files:
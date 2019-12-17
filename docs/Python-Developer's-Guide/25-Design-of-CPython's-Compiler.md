# 25. Design of CPython’s Compiler[¶](https://devguide.python.org/compiler/#design-of-cpython-s-compiler)

## 25.1. Abstract[¶](https://devguide.python.org/compiler/#abstract)

In CPython, the compilation from source code to bytecode involves several steps:

1. Parse source code into a parse tree (`Parser/pgen.c`)
2. Transform parse tree into an Abstract Syntax Tree (`Python/ast.c`)
3. Transform AST into a Control Flow Graph (`Python/compile.c`)
4. Emit bytecode based on the Control Flow Graph (`Python/compile.c`)

The purpose of this document is to outline how these steps of the process work.

This document does not touch on how parsing works beyond what is needed to explain what is needed for compilation. It is also not exhaustive in terms of the how the entire system works. You will most likely need to read some source to have an exact understanding of all details.

## 25.2. Parse Trees[¶](https://devguide.python.org/compiler/#parse-trees)

Python’s parser is an [LL(1) parser](https://en.wikipedia.org/wiki/LL_parser) mostly based off of the implementation laid out in the Dragon Book [[Aho86\]](https://devguide.python.org/compiler/#aho86).

The grammar file for Python can be found in `Grammar/Grammar` with the numeric value of grammar rules stored in `Include/graminit.h`. The list of types of tokens (literal tokens, such as `:`, numbers, etc.) can be found in `Grammar/Tokens` with the numeric value stored in `Include/token.h`. The parse tree is made up of `node *` structs (as defined in `Include/node.h`).

Querying data from the node structs can be done with the following macros (which are all defined in `Include/node.h`):

- `CHILD(node *, int)`

  Returns the nth child of the node using zero-offset indexing

- `RCHILD(node *, int)`

  Returns the nth child of the node from the right side; use negative numbers!

- `NCH(node *)`

  Number of children the node has

- `STR(node *)`

  String representation of the node; e.g., will return `:` for a `COLON` token

- `TYPE(node *)`

  The type of node as specified in `Include/graminit.h`

- `REQ(node *, TYPE)`

  Assert that the node is the type that is expected

- `LINENO(node *)`

  Retrieve the line number of the source code that led to the creation of the parse rule; defined in `Python/ast.c`

For example, consider the rule for ‘while’:

```
while_stmt ::=  "while" expression ":" suite : ["else" ":" suite]
```

The node representing this will have `TYPE(node) == while_stmt` and the number of children can be 4 or 7 depending on whether there is an ‘else’ statement. `REQ(CHILD(node, 2), COLON)` can be used to access what should be the first `:` and require it be an actual `:` token.

## 25.3. Abstract Syntax Trees (AST)[¶](https://devguide.python.org/compiler/#abstract-syntax-trees-ast)

The abstract syntax tree (AST) is a high-level representation of the program structure without the necessity of containing the source code; it can be thought of as an abstract representation of the source code. The specification of the AST nodes is specified using the Zephyr Abstract Syntax Definition Language (ASDL) [[Wang97\]](https://devguide.python.org/compiler/#wang97).

> Green Tree Snakes
>
> See also [Green Tree Snakes - the missing Python AST docs](https://greentreesnakes.readthedocs.io/en/latest/) by Thomas Kluyver.

The definition of the AST nodes for Python is found in the file `Parser/Python.asdl`.



## 25.12. References[¶](https://devguide.python.org/compiler/#references)

| [[Aho86\]](https://devguide.python.org/compiler/#id1)  | Alfred V. Aho, Ravi Sethi, Jeffrey D. Ullman. Compilers: Principles, Techniques, and Tools, https://www.amazon.com/exec/obidos/tg/detail/-/0201100886/104-0162389-6419108 |
| ------------------------------------------------------ | ------------------------------------------------------------ |
| [[Wang97\]](https://devguide.python.org/compiler/#id2) | Daniel C. Wang, Andrew W. Appel, Jeff L. Korn, and Chris S. Serra. [The Zephyr Abstract Syntax Description Language.](https://www.cs.princeton.edu/research/techreps/TR-554-97) In Proceedings of the Conference on Domain-Specific Languages, pp. 213–227, 1997. |
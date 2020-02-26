# [Using ASDL to describe ASTs in compilers](https://eli.thegreenplace.net/2014/06/04/using-asdl-to-describe-asts-in-compilers)

ASTs (Abstract Syntax Trees) are an [important data structure](http://eli.thegreenplace.net/2009/02/16/abstract-vs-concrete-syntax-trees/) in compiler front-ends. If you've written a few parsers, you almost definitely ran into the need to describe the result of the parsing in terms of an AST. While the kinds of nodes such ASTs have and their structure is very specific to the source language, many commonalities come up. In other words, coding "yet another AST" gets really old after you've done it a few times.

Worry not, as you'd expect from the programmer crowd, this problem was "solved" by adding another level of abstraction. Yes, an **abstraction** over **Abstract** Syntax Trees, oh my! The abstraction here is some textual format (let's call it a DSL to sound smart) that describes what the AST looks like, along with machinery to auto-generate the code that implements this AST.

Most solutions in this domain are ad-hoc, but one that I've seen used more than once is [ASDL](http://asdl.sourceforge.net/) - Abstract Syntax Definition Language. The self-description from the website sounds about right:

> The Zephyr Abstract Syntax Description Lanuguage (ASDL) is a language designed to describe the tree-like data structures in compilers. Its main goal is to provide a method for compiler components written in different languages to interoperate. ASDL makes it easier for applications written in a variety of programming languages to communicate complex recursive data structures.

To given an example, here's a short snippet from an ASDL definition of a simple programming language:

```
program = Program(class* classes)

class = Class(identifier name, identifier? parent, feature* features)

[...]

expression = Assign(identifier name, expression expr)
           | StaticDispatch(expression expr, identifier type_name,
                            identifier name, expression* actual)
           | Dispatch(expression expr, identifier name, expression* actual)

[...]
```
# utexas CS 378, Symbolic Programming

## Symbolic Programming

***Symbols*** are words that refer to, or denote, other things: their ***referents***:

- Variable names in programming languages
- Variables in equations
- English words

Symbolic Programming manipulates stuctures of symbols, typically represented as trees:

- solve equations

- translate a program to another language

- specialize a generic program

- convert units of measurement

- translate English into a database query

- compose program components

## Functional Programming

A functional program is one with no side effects:

- changing a global variable
- updating a database
- printing

If we call `sin(x)`, it will just return a value, but will have no side effects.

Functional programming does everything by ***composition*** of functions:

```C++
guacamole:
season(mash(slice(peel(wash(avocado)))))
```



Functions are composed so that the output of one function is the input of the next function(pipelined). Functional programming works well with distributed cloud computing: the function can be replicated on many servers and executed in parallel on massive amounts of data.
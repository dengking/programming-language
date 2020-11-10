# 关于本章

本章讨论错误处理。





## wikipedia [Exception handling](https://en.wikipedia.org/wiki/Exception_handling)



Alternative approaches to exception handling in software are error checking, which maintains normal program flow with later explicit checks for contingencies reported using special return values or some auxiliary global variable such as C's [errno](https://en.wikipedia.org/wiki/Errno) or floating point status flags; or input validation to preemptively filter exceptional cases.

> NOTE: 思考：exception机制相比于error checking的优势是什么？在下面的文章中点出来了。



## Exception VS errono

exception是现代programming language提供的error handling机制，传统的做法是errono，那exception有什么优势呢？在wikipedia [Exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax)中有着解释:

> **Exception handling syntax** is the set of keywords and/or structures provided by a computer [programming language](https://en.wikipedia.org/wiki/Programming_language) to allow [exception handling](https://en.wikipedia.org/wiki/Exception_handling), which separates the handling of errors that arise during a program's operation from its **ordinary processes**.


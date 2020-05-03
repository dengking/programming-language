# Exception handling

“exception”是software engineer领域非常常见的一个概念：

现代大多数programming language都会提供的一种机制。

在工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Kernel\Book-Understanding-the-Linux-Kernel\Chapter-4-Interrupts-and-Exceptions\Chapter-4-Interrupts-and-Exceptions`中讨论OS层的exception概念。

工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Kernel\Book-Understanding-the-Linux-Kernel\Chapter-4-Interrupts-and-Exceptions\Chapter-4-Interrupts-and-Exceptions`中详细讨论了exception的具体分类、exception的实现机制等。

我们可以看到，无论是在programming language层，还是在OS层，exception概念的基本含义是相同的，正如维基百科[Exception handling](https://en.wikipedia.org/wiki/Exception_handling)中所总结的：

- an exception breaks the normal flow of execution and executes a pre-registered *exception handler*

## [oracle javase What Is an Exception?](https://docs.oracle.com/javase/tutorial/essential/exceptions/definition.html)

The term *exception* is shorthand for the phrase "exceptional event."

------

**Definition:** An *exception* is an event, which occurs during the execution of a program, that disrupts the normal flow of the program's instructions.

> NOTE: exception是一种event，这个认知是非常重要的。

## 维基百科[Exception handling](https://en.wikipedia.org/wiki/Exception_handling)



Alternative approaches to exception handling in software are error checking, which maintains normal program flow with later explicit checks for contingencies reported using special return values or some auxiliary global variable such as C's [errno](https://en.wikipedia.org/wiki/Errno) or floating point status flags; or input validation to preemptively filter exceptional cases.

> NOTE: 思考：exception机制相比于error checking的优势是什么？在下面的文章中点出来了。

## 维基百科[Exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax)

**Exception handling syntax** is the set of keywords and/or structures provided by a computer [programming language](https://en.wikipedia.org/wiki/Programming_language) to allow [exception handling](https://en.wikipedia.org/wiki/Exception_handling), which separates the handling of errors that arise during a program's operation from its ordinary processes.

> NOTE: 后面一段话说明了exception handling的优势所在。



## 维基百科[Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

> NOTE: 在c++语言中，exception safety的概念非常重要，并且c++语言对它进行了formal definition。这个话题放到了c++中进行了具体讨论。



思考：
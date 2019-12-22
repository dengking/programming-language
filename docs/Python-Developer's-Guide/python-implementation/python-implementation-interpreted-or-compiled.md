[TOC]



# [Is Python interpreted or compiled? Yes.](https://nedbatchelder.com/blog/201803/is_python_interpreted_or_compiled_yes.html)

A common question: “Is Python interpreted or compiled?” Usually, the asker has a simple model of the world in mind, and as is typical, the world is more complicated.

In the simple model of the world, “compile” means to convert a program in a high-level language into a **binary executable** full of machine code (CPU instructions). When you compile a C program, this is what happens. The result is a file that your operating system can run for you.

In the simple definition of “interpreted”, executing a program means reading the source file a line at a time, and doing what it says. This is the way some shells operate.

But the real world is not so limited. Making real programming languages useful and powerful involves a wider range of possibilities about how they work. [Compiling](https://en.wikipedia.org/wiki/Compiler) is a more general idea: take a program in one language (or form), and convert it into another language or form. Usually the source form is a higher-level language than the destination form, such as when converting from C to machine code. But converting from JavaScript 8 to JavaScript 5 is also a kind of compiling.

In Python, the source is compiled into a much simpler form called [bytecode](https://en.wikipedia.org/wiki/Bytecode). These are instructions similar in spirit to CPU instructions, but instead of being executed by the CPU, they are executed by software called a virtual machine. (These are not VM’s that emulate entire operating systems, just a simplified CPU execution environment.)

Here’s an example of a short Python function, and its bytecode:

> ```python
> >>> import dis
> >>> def example(x):
> ...     for i in range(x):
> ...         print(2 * i)
> ...
> >>> dis.dis(example)
>   2           0 SETUP_LOOP              28 (to 30)
>               2 LOAD_GLOBAL              0 (range)
>               4 LOAD_FAST                0 (x)
>               6 CALL_FUNCTION            1
>               8 GET_ITER
>         >>   10 FOR_ITER                16 (to 28)
>              12 STORE_FAST               1 (i)
> 
>   3          14 LOAD_GLOBAL              1 (print)
>              16 LOAD_CONST               1 (2)
>              18 LOAD_FAST                1 (i)
>              20 BINARY_MULTIPLY
>              22 CALL_FUNCTION            1
>              24 POP_TOP
>              26 JUMP_ABSOLUTE           10
>         >>   28 POP_BLOCK
>         >>   30 LOAD_CONST               0 (None)
>              32 RETURN_VALUE
> >>>
> ```

The [dis](https://docs.python.org/3/library/dis.html) module in the Python standard library is the disassembler that can show you **Python bytecode**. It’s also the best (but not great) documentation for the bytecode itself. If you want to know more about how Python’s bytecode works, there are [lots of conference talks about bytecode](https://www.youtube.com/results?search_query=exploring+cpython+bytecode). The software that executes bytecode can be written in any language: [byterun](https://github.com/nedbat/byterun) is an implementation in Python (!), which is useful only as an educational exercise.

An important aspect of Python’s compilation to bytecode is that it’s entirely implicit(完全是隐式的，即用户感知不到). You never invoke a compiler, you simply run a .py file. The Python implementation compiles the files as needed. This is different than Java, for example, where you have to run the Java compiler to turn Java source into compiled class files. For this reason, Java is often called a **compiled language**, while Python is called an **interpreted language**. But both compile to **bytecode**, and then both execute the **bytecode** with a software implementation of a **virtual machine**.

Another important Python feature is its interactive prompt. You can type Python statements and have them immediately executed. This interactivity is usually missing in “compiled” languages, but even at the Python interactive prompt, your Python is compiled to bytecode, and then the bytecode is executed. This immediate execution, and Python’s lack of an explicit compile step, are why people call the Python executable “the Python interpreter.”

By the way, even this is a simplified description of how these languages can work. “Compiled” languages like Java and C can have interactive prompts, but they are not at the center of those worlds in the same way that Python’s is. Java originally always compiled to **bytecode**, but then it pioneered(倡导) just-in-time (JIT) techniques for compiling to **machine code** at runtime, and now Java is sometimes compiled entirely to machine code, in the C style.

This shows just how flimsy the words “interpreted” and “compiled” can be. Like most adjectives applied to programming languages, they are thrown around as if they were black-and-white distinctions, but the reality is much subtler and complex.

Finally, how your program gets executed isn’t a characteristic of the language at all: it’s about the **language implementation**. I’ve been talking here about Python, but this has really been a description of CPython, the usual implementation of Python, so-named because it is written in C. [PyPy](https://pypy.org/) is another implementation, using a **JIT compiler** to run code much faster than CPython can.

So: is Python compiled? Yes. Is Python interpreted? Yes. Sorry, the world is complicated...


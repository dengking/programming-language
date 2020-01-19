## Part 1: Introduction to CPython

CPython is one of the many Python **runtimes**, maintained and written by different teams of developers. Some other runtimes you may have heard are [PyPy](https://pypy.org/), [Cython](https://cython.org/), and [Jython](https://www.jython.org/).

The unique thing about CPython is that it contains both a **runtime** and the shared **language specification** that all Python runtimes use. CPython is the “official,” or reference implementation of Python.

The Python language specification is the document that the description of the Python language. For example, it says that `assert` is a reserved keyword, and that `[]` is used for indexing, slicing, and creating empty lists.

Think about what you expect to be inside the Python distribution on your computer:

- When you type `python` without a file or module, it gives an interactive prompt.
- You can import built-in modules from the standard library like `json`.
- You can install packages from the internet using `pip`.
- You can test your applications using the built-in `unittest` library.

These are all part of the CPython distribution. There’s a lot more than just a **compiler**.

**Note:** This article is written against version [3.8.0b4](https://github.com/python/cpython/tree/v3.8.0b4) of the CPython source code.

### What’s in the Source Code?

The CPython source distribution comes with a whole range of tools, libraries, and components. We’ll explore those in this article. First we are going to focus on the compiler.

Inside of the newly downloaded `cpython` directory, you will find the following subdirectories:

```
cpython/
│
├── Doc      ← Source for the documentation
├── Grammar  ← The computer-readable language definition
├── Include  ← The C header files
├── Lib      ← Standard library modules written in Python
├── Mac      ← macOS support files
├── Misc     ← Miscellaneous files
├── Modules  ← Standard Library Modules written in C
├── Objects  ← Core types and the object model
├── Parser   ← The Python parser source code
├── PC       ← Windows build support files
├── PCbuild  ← Windows build support files for older Windows versions
├── Programs ← Source code for the python executable and other binaries
├── Python   ← The CPython interpreter source code
└── Tools    ← Standalone tools useful for building or extending Python
```

Next, we’ll compile CPython from the source code. This step requires a C compiler, and some build tools, which depend on the operating system you’re using.

### Compiling CPython (Linux)

For Linux, the first step is to download and install `make`, `gcc`, `configure`, and `pkgconfig`.

### What Does a Compiler Do?

The purpose of a compiler is to convert one language into another. Think of a compiler like a translator. You would hire a translator to listen to you speaking in English and then speak in Japanese:

[![Translating from English to Japanese](https://files.realpython.com/media/t.38be306a7e83.png)](https://files.realpython.com/media/t.38be306a7e83.png)

Some compilers will compile into a low-level machine code which can be executed directly on a system. Other compilers will compile into an intermediary language, to be executed by a virtual machine.

One important decision to make when choosing a compiler is the system portability requirements. [Java](https://en.wikipedia.org/wiki/Java_bytecode) and [.NET CLR](https://en.wikipedia.org/wiki/Common_Language_Runtime) will compile into an Intermediary Language so that the compiled code is portable across multiple systems architectures. C, Go, C++, and Pascal will compile into a low-level executable that will only work on systems similar to the one it was compiled.

Because Python applications are typically distributed as source code, the role of the Python runtime is to convert the Python source code and execute it in one step. Internally, the CPython runtime does compile your code. A popular misconception is that Python is an interpreted language. It is actually compiled.

Python code is not compiled into machine-code. It is compiled into a special low-level intermediary language called **bytecode** that only CPython understands. This code is stored in `.pyc` files in a hidden directory and cached for execution. If you run the same Python application twice without changing the source code, it’ll always be much faster the second time. This is because it loads the compiled bytecode and executes it directly.

### Why Is CPython Written in C and Not Python?

The **C** in CPython is a reference to the C programming language, implying that this Python distribution is written in the C language.

This statement is largely true: the compiler in CPython is written in pure C. However, many of the standard library modules are written in pure Python or a combination of C and Python.

**So why is CPython written in C and not Python?**

The answer is located in how compilers work. There are two types of compiler:

1. **[Self-hosted compilers](https://en.wikipedia.org/wiki/Self-hosting)** are compilers written in the language they compile, such as the Go compiler.
2. **[Source-to-source compilers](https://en.wikipedia.org/wiki/Source-to-source_compiler)** are compilers written in another language that already have a compiler.

If you’re writing a new programming language from scratch, you need an executable application to compile your compiler! You need a compiler to execute anything, so when new languages are developed, they’re often written first in an older, more established language.

A good example would be the Go programming language. The first Go compiler was written in C, then once Go could be compiled, the compiler was rewritten in Go.

CPython kept its C heritage: many of the standard library modules, like the `ssl` module or the `sockets` module, are written in C to access low-level operating system APIs. The APIs in the Windows and Linux kernels for [creating network sockets](https://realpython.com/python-sockets/), [working with the filesystem](https://realpython.com/working-with-files-in-python/) or [interacting with the display](https://realpython.com/python-gui-with-wxpython/) are all written in C. It made sense for Python’s extensibility layer to be focused on the C language. Later in this article, we will cover the Python Standard Library and the C modules.

There is a Python compiler written in Python called [PyPy](https://pypy.org/). PyPy’s logo is an [Ouroboros](https://en.wikipedia.org/wiki/Ouroboros) to represent the self-hosting nature of the compiler.

Another example of a cross-compiler for Python is [Jython](https://www.jython.org/). Jython is written in Java and compiles from Python source code into Java bytecode. In the same way that CPython makes it easy to import C libraries and use them from Python, Jython makes it easy to import and reference Java modules and classes.

### The Python Language Specification

Contained within the CPython source code is the definition of the Python language. This is the reference specification used by all the Python interpreters.

The specification is in both **human-readable** and **machine-readable** format. Inside the documentation is a detailed explanation of the Python language, what is allowed, and how each statement should behave.

#### Documentation

Located inside the `Doc/reference` directory are [reStructuredText](http://docutils.sourceforge.net/rst.html) explanations of each of the features in the Python language. This forms the official Python reference guide on [docs.python.org](https://docs.python.org/3/reference/).

Inside the directory are the files you need to understand the whole language, structure, and keywords:

```
cpython/Doc/reference
|
├── compound_stmts.rst
├── datamodel.rst
├── executionmodel.rst
├── expressions.rst
├── grammar.rst
├── import.rst
├── index.rst
├── introduction.rst
├── lexical_analysis.rst
├── simple_stmts.rst
└── toplevel_components.rst
```

Inside `compound_stmts.rst`, the documentation for compound statements, you can see a simple example defining the `with` statement.

The `with` statement can be used in multiple ways in Python, the simplest being the [instantiation of a context-manager](https://dbader.org/blog/python-context-managers-and-with-statement) and a nested block of code:

```python
with x():
   ...
```

You can assign the result to a variable using the `as` keyword:

```python
with x() as y:
   ...
```

You can also chain context managers together with a comma:

```python
with x() as y, z() as jk:
   ...
```

Next, we’ll explore the **computer-readable** documentation of the Python language.

#### Grammar

The documentation contains the human-readable specification of the language, and the machine-readable specification is housed in a single file, [`Grammar/Grammar`](https://github.com/python/cpython/blob/master/Grammar/Grammar).

The Grammar file is written in a context-notation called [Backus-Naur Form (BNF)](https://en.m.wikipedia.org/wiki/Backus–Naur_form). BNF is not specific to Python and is often used as the notation for grammars in many other languages.

The concept of grammatical structure in a programming language is inspired by [Noam Chomsky’s work on Syntactic Structures](https://en.wikipedia.org/wiki/Syntactic_Structures) in the 1950s!

Python’s grammar file uses the Extended-BNF (EBNF) specification with regular-expression syntax. So, in the grammar file you can use:

- **`\*`** for repetition
- **`+`** for at-least-once repetition
- **`[]`** for optional parts
- **`|`** for alternatives
- **`()`** for grouping

If you search for the `with` statement in the grammar file, at around line 80 you’ll see the definitions for the `with` statement:

```
with_stmt: 'with' with_item (',' with_item)*  ':' suite
with_item: test ['as' expr]
```

Anything in quotes is a string literal, which is how keywords are defined. So the `with_stmt` is specified as:

1. Starting with the word `with`
2. Followed by a `with_item`, which is a `test` and (optionally), the word `as`, and an expression
3. Following one or many items, each separated by a comma
4. Ending with a `:`
5. Followed by a `suite`

There are references to some other definitions in these two lines:

- **`suite`** refers to a block of code with one or multiple statements
- **`test`** refers to a simple statement that is evaluated
- **`expr`** refers to a simple expression

If you want to explore those in detail, the whole of the Python grammar is defined in this single file.

If you want to see a recent example of how grammar is used, in PEP 572 the **colon equals** operator was added to the grammar file in [this Git commit](https://github.com/python/cpython/commit/8f59ee01be3d83d5513a9a3f654a237d77d80d9a#diff-cb0b9d6312c0d67f6d4aa1966766cedd).

#### Using `pgen`

The grammar file itself is never used by the **Python compiler**. Instead, a **parser table** created by a tool called `pgen` is used. `pgen` reads the grammar file and converts it into a **parser table**. If you make changes to the grammar file, you must regenerate the **parser table** and recompile Python.

**Note:** The `pgen` application was rewritten in Python 3.8 from C to [pure Python](https://github.com/python/cpython/blob/master/Parser/pgen/pgen.py).

To see `pgen` in action, let’s change part of the Python grammar. Around line 51 you will see the definition of a `pass` statement:

```
pass_stmt: 'pass'
```

Change that line to accept the keyword `'pass'` or `'proceed'` as keywords:

```
pass_stmt: 'pass' | 'proceed'
```

Now you need to rebuild the grammar files. On macOS and Linux, run `make regen-grammar` to run `pgen` over the altered grammar file. For Windows, there is no officially supported way of running `pgen`. However, you can clone [my fork](https://github.com/tonybaloney/cpython/tree/pcbuildregen) and run `build.bat --regen` from within the `PCBuild` directory.

You should see an output similar to this, showing that the new `Include/graminit.h` and `Python/graminit.c` files have been generated:

```
# Regenerate Doc/library/token-list.inc from Grammar/Tokens
# using Tools/scripts/generate_token.py
...
python3 ./Tools/scripts/update_file.py ./Include/graminit.h ./Include/graminit.h.new
python3 ./Tools/scripts/update_file.py ./Python/graminit.c ./Python/graminit.c.new
```

> **Note:** `pgen` works by converting the EBNF statements into a [Non-deterministic Finite Automaton (NFA)](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton), which is then turned into a [Deterministic Finite Automaton (DFA)](https://en.wikipedia.org/wiki/Deterministic_finite_automaton). The DFAs are used by the **parser** as **parsing tables** in a special way that’s unique to CPython. This technique was [formed at Stanford University](http://infolab.stanford.edu/~ullman/dragon/slides1.pdf) and developed in the 1980s, just before the advent of Python.

With the regenerated parser tables, you need to recompile CPython to see the new syntax. Use the same compilation steps you used earlier for your operating system.

If the code compiled successfully, you can execute your new CPython binary and start a REPL.

In the REPL, you can now try defining a function and instead of using the `pass` statement, use the `proceed` keyword alternative that you compiled into the Python grammar:

```
Python 3.8.0b4 (tags/v3.8.0b4:d93605de72, Aug 30 2019, 10:00:03) 
[Clang 10.0.1 (clang-1001.0.46.4)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> def example():
...    proceed
... 
>>> example()
```

Well done! You’ve changed the CPython syntax and compiled your own version of CPython. Ship it!

Next, we’ll explore tokens and their relationship to grammar.

#### Tokens

Alongside the grammar file in the `Grammar` folder is a [`Tokens`](https://github.com/python/cpython/blob/master/Grammar/Tokens) file, which contains each of the unique types found as a **leaf node** in a **parse tree**. We will cover **parser trees** in depth later. Each **token** also has a name and a generated unique ID. The names are used to make it simpler to refer to in the **tokenizer**.

> **Note:** The `Tokens` file is a new feature in Python 3.8.

For example, the left parenthesis is called `LPAR`, and semicolons are called `SEMI`. You’ll see these tokens later in the article:

```
LPAR                    '('
RPAR                    ')'
LSQB                    '['
RSQB                    ']'
COLON                   ':'
COMMA                   ','
SEMI                    ';'
```

As with the `Grammar` file, if you change the `Tokens` file, you need to run `pgen` again.

To see tokens in action, you can use the `tokenize` module in CPython. Create a simple Python script called `test_tokens.py`:

```python
# Hello world!
def my_function():
   proceed
```

Then pass this file through a module built into the standard library called `tokenize`. You will see the list of tokens, by line and character. Use the `-e` flag to output the exact token name:

```shell
$ ./python.exe -m tokenize -e test_tokens.py

0,0-0,0:            ENCODING       'utf-8'        
1,0-1,14:           COMMENT        '# Hello world!'
1,14-1,15:          NL             '\n'           
2,0-2,3:            NAME           'def'          
2,4-2,15:           NAME           'my_function'  
2,15-2,16:          LPAR           '('            
2,16-2,17:          RPAR           ')'            
2,17-2,18:          COLON          ':'            
2,18-2,19:          NEWLINE        '\n'           
3,0-3,3:            INDENT         '   '          
3,3-3,7:            NAME           'proceed'         
3,7-3,8:            NEWLINE        '\n'           
4,0-4,0:            DEDENT         ''             
4,0-4,0:            ENDMARKER      ''              
```

In the output, the first column is the range of the line/column coordinates, the second column is the name of the token, and the final column is the value of the token.

In the output, the `tokenize` module has implied some tokens that were not in the file. The `ENCODING` token for `utf-8`, and a blank line at the end, giving `DEDENT` to close the function declaration and an `ENDMARKER` to end the file.

It is best practice to have a blank line at the end of your Python source files. If you omit it, CPython adds it for you, with a tiny performance penalty.

The `tokenize` module is written in pure Python and is located in [`Lib/tokenize.py`](https://github.com/python/cpython/blob/master/Lib/tokenize.py) within the CPython source code.

> **Important:** There are two tokenizers in the CPython source code: one written in Python, demonstrated here, and another written in C. The tokenizer written in Python is meant as a utility, and the one written in C is used by the Python compiler. They have identical output and behavior. The version written in C is designed for performance and the module in Python is designed for debugging.

> NOTE: The tokenizer written in C is in dir `Parser`.

Now that you have an overview of the Python grammar and the relationship between tokens and statements, there is a way to convert the `pgen` output into an interactive graph.

Here is a screenshot of the Python 3.8a2 grammar:

[![Python 3.8 DFA node graph](https://files.realpython.com/media/Screen_Shot_2019-03-12_at_2.31.16_pm.f36c3e99b8b4.png)](https://files.realpython.com/media/Screen_Shot_2019-03-12_at_2.31.16_pm.f36c3e99b8b4.png)

The Python package used to generate this graph, `instaviz`, will be covered in a later chapter.



### Memory Management in CPython

Throughout this article, you will see references to a [`PyArena`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L128) object. The arena is one of CPython’s memory management structures. The code is within `Python/pyarena.c` and contains a wrapper around C’s memory allocation and deallocation functions.

In a traditionally written C program, the developer *should* allocate memory for data structures before writing into that data. This allocation marks the memory as belonging to the process with the operating system.

Python takes that responsibility away from the programmer and uses two algorithms: [a reference counter and a garbage collector](https://realpython.com/python-memory-management/).

Whenever an interpreter is instantiated, a [`PyArena`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L128) is created and attached one of the fields in the interpreter. During the lifecycle of a CPython interpreter, many arenas could be allocated. They are connected with a linked list. The arena stores a **list** of pointers to Python Objects as a `PyListObject`. Whenever a new Python object is created, a pointer to it is added using [`PyArena_AddPyObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L203). This function call stores a pointer in the arena’s list, `a_objects`.

> Even though Python doesn’t have pointers, there are some [interesting techniques](https://realpython.com/pointers-in-python/) to simulate the behavior of pointers.

The `PyArena` serves a second function, which is to allocate and reference a list of raw memory blocks. For example, a `PyList` would need extra memory if you added thousands of additional values. The `PyList` object’s C code does not allocate memory directly. The object gets raw blocks of memory from the `PyArena` by calling [`PyArena_Malloc()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L180) from the `PyObject` with the required memory size. This task is completed by another abstraction in `Objects/obmalloc.c`. In the object allocation module, memory can be allocated, freed, and reallocated for a Python Object.

A linked list of allocated blocks is stored inside the arena, so that when an interpreter is stopped, all managed memory blocks can be deallocated in one go using [`PyArena_Free()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L157).

Take the `PyListObject` example. If you were to `.append()` an object to the end of a Python list, you don’t need to reallocate the memory used in the existing list beforehand. The `.append()` method calls [`list_resize()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/listobject.c#L36) which handles memory allocation for lists. Each list object keeps a list of the amount of memory allocated. If the item you’re appending will fit inside the existing free memory, it is simply added. If the list needs more memory space, it is expanded. Lists are expanded in length as 0, 4, 8, 16, 25, 35, 46, 58, 72, 88.

[`PyMem_Realloc()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/obmalloc.c#L618) is called to expand the memory allocated in a list. [`PyMem_Realloc()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/obmalloc.c#L618) is an API wrapper for [`pymalloc_realloc()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/obmalloc.c#L1913).

Python also has a special wrapper for the C call `malloc()`, which sets the max size of the memory allocation to help prevent buffer overflow errors (See [`PyMem_RawMalloc()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Modules/overlapped.c#L28)).

In summary:

- Allocation of raw memory blocks is done via `PyMem_RawAlloc()`.
- The pointers to Python objects are stored within the `PyArena`.
- `PyArena` also stores a linked-list of allocated memory blocks.

More information on the API is detailed on the [CPython documentation](https://docs.python.org/3/c-api/memory.html).

#### Reference Counting

To create a variable in Python, you have to assign a value to a *uniquely* named variable:

```
my_variable = 180392
```

Whenever a value is assigned to a variable in Python, the name of the variable is checked within the locals and globals scope to see if it already exists.

Because `my_variable` is not already within the `locals()` or `globals()` dictionary, this new object is created, and the value is assigned as being the numeric constant `180392`.

There is now one reference to `my_variable`, so the reference counter for `my_variable` is incremented by 1.

You will see function calls [`Py_INCREF()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L239) and [`Py_DECREF()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L245) throughout the C source code for CPython. These functions increment and decrement the count of references to that object.

References to an object are decremented when a variable falls outside of the scope in which it was declared. Scope in Python can refer to a function or method, a comprehension, or a lambda function. These are some of the more literal scopes, but there are many other implicit scopes, like passing variables to a function call.

The handling of incrementing and decrementing references based on the language is built into the CPython compiler and the core execution loop, `ceval.c`, which we will cover in detail later in this article.

Whenever `Py_DECREF()` is called, and the counter becomes 0, the [`PyObject_Free()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/obmalloc.c#L707) function is called. For that object [`PyArena_Free()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pyarena.c#L157) is called for all of the memory that was allocated.

#### Garbage Collection

How often does your garbage get collected? Weekly, or fortnightly?

When you’re finished with something, you discard it and throw it in the trash. But that trash won’t get collected straight away. You need to wait for the garbage trucks to come and pick it up.

CPython has the same principle, using a garbage collection algorithm. CPython’s garbage collector is enabled by default, happens in the background and works to deallocate memory that’s been used for objects which are no longer in use.

Because the garbage collection algorithm is a lot more complex than the reference counter, it doesn’t happen all the time, otherwise, it would consume a huge amount of CPU resources. It happens periodically, after a set number of operations.

CPython’s standard library comes with a Python module to interface with the arena and the garbage collector, the `gc` module. Here’s how to use the `gc` module in debug mode:

```
>>> import gc
>>> gc.set_debug(gc.DEBUG_STATS)
```

This will print the statistics whenever the garbage collector is run.

You can get the threshold after which the garbage collector is run by calling `get_threshold()`:

```
>>> gc.get_threshold()
(700, 10, 10)
```

You can also get the current threshold counts:

```
>>> gc.get_count()
(688, 1, 1)
```

Lastly, you can run the collection algorithm manually:

```
>>> gc.collect()
24
```

This will call [`collect()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Modules/gcmodule.c#L987) inside the `Modules/gcmodule.c` file which contains the implementation of the garbage collector algorithm.

### Conclusion

In Part 1, you covered the structure of the source code repository, how to compile from source, and the Python language specification. These core concepts will be critical in Part 2 as you dive deeper into the Python interpreter process.
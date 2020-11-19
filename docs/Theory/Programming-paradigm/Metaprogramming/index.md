# Metaprogramming

在文章Meta中，总结了programming language中的各种meta。

> NOTE: meta是一种抽象



## wikipedia [Metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming) 

**Metaprogramming** is a programming technique in which [computer programs](https://en.wikipedia.org/wiki/Computer_program) have the ability to **treat programs as their data**. It means that a program can be designed to **read**, **generate**, **analyze** or **transform** other programs, and even modify itself while running. [[1\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-1)[[2\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-2) In some cases, this allows programmers to minimize the number of lines of code to express a solution, thus reducing the development time.[[3\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-3)  It also allows programs greater flexibility to efficiently handle new situations without recompilation.

> NOTE: 这一段中描述了metaprogramming的核心思想: "treat programs as their data"; 
>
> 结合使用C++、Python的经验来看，这是一种非常强大的能力，能够带来极大的灵活性，能够大大增加语言的power。

**Metaprogramming** can be used to move computations from [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) to [compile-time](https://en.wikipedia.org/wiki/Compile-time), to generate code using compile time computations, and to enable [self-modifying code](https://en.wikipedia.org/wiki/Self-modifying_code). The language in which the **metaprogram** is written is called the [metalanguage](https://en.wikipedia.org/wiki/Self-modifying_code). The language of the programs that are manipulated is called the [attribute-oriented programming](https://en.wikipedia.org/wiki/Attribute-oriented_programming) language. The ability of a programming language to be its own metalanguage is called [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_science)) or "reflexivity".[[4\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-4) **Reflection** is a valuable language feature to facilitate metaprogramming.

> NOTE: 
>
> 从metaprogramming的角度来看，我们编写的program就包含两类了：
>
> | classification of program |                                  |
> | ------------------------- | -------------------------------- |
> | program                   | 使用**programming language**     |
> | metaprogram               | 使用**metaprogramming language** |
>
> metaprogram是由language的实现来执行，下面是例子:
>
> 1) C++ metaprogram是由compiler来执行的(**static**)
>
> 2) Python metaprogram是由Python interpreter来执行的(**dynamic**)
>
> 第二段中的"**Metaprogramming** can be used to move computations from [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) to [compile-time](https://en.wikipedia.org/wiki/Compile-time)"是典型的compile time function execution，参见`Theory\Compile-time-and-run-time`章节。
>
> 大多数programming language的metalanguage和hosting language是相同的，比如: 
>
> C++和python都支持metaprogramming，它们的programming language和metaprogramming language是相同。具体到C++，它使用的template metaprogramming，所以它的template就属于metaprogram。

### Approaches 

**Metaprogramming** enables developers to write programs and develop code that *falls under*(属于) the [generic programming](https://en.wikipedia.org/wiki/Generic_programming) paradigm. Having the programming language itself as a [first-class data type](https://en.wikipedia.org/wiki/First-class_object) (as in [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)), [Prolog](https://en.wikipedia.org/wiki/Prolog), [SNOBOL](https://en.wikipedia.org/wiki/SNOBOL), or [Rebol](https://en.wikipedia.org/wiki/Rebol)) is also very useful; this is known as *[homoiconicity](https://en.wikipedia.org/wiki/Homoiconicity)*(同质化). **Generic programming** invokes a **metaprogramming** facility within a language by allowing one to write code without the concern of specifying data types since they can be supplied as parameters when used(泛型编程通过允许编写代码而不考虑指定数据类型来调用语言中的元编程工具，因为它们可以在使用时作为参数提供).

> NOTE: 上面这段话描述了metaprogramming 和 generic programming之间的关系。显然metaprogramming是属于generic programming的。

**Metaprogramming** usually works in one of three ways.[[5\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-5)

1. The first approach is to expose the internals of the run-time engine to the programming code through [application programming interfaces](https://en.wikipedia.org/wiki/Application_programming_interface) (APIs) like that for the [.NET](https://en.wikipedia.org/wiki/.NET_Framework) [IL](https://en.wikipedia.org/wiki/Microsoft_Intermediate_Language) emitter(发射器).

2. The second approach is dynamic execution of expressions that contain programming commands, often composed from strings, but can also be from other methods using arguments or context, like Javascript.[[6\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-6) Thus, "programs can write programs." Although both approaches can be used in the same language, most languages tend to lean toward one or the other.

3. The third approach is to step outside the language entirely(完全超越语言). General purpose [program transformation](https://en.wikipedia.org/wiki/Program_transformation) systems such as **compilers**, which accept language descriptions and carry out arbitrary transformations on those languages, are direct implementations of general **metaprogramming**. This allows **metaprogramming** to be applied to virtually any target language without regard to whether that target language has any **metaprogramming** abilities of its own. One can see this at work with [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)) and how it allows to tackle some limitations faced in [C](https://en.wikipedia.org/wiki/C_(programming_language)) by using constructs that were part of the Scheme language itself to extend C.


Not all metaprogramming involves **generative programming**(产生式编程). If programs are modifiable at runtime or if incremental compilation is available (such as in [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language)), [Frink](https://en.wikipedia.org/wiki/Frink_(programming_language)), [Groovy](https://en.wikipedia.org/wiki/Groovy_(programming_language)), [JavaScript](https://en.wikipedia.org/wiki/JavaScript), [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)), [Elixir](https://en.wikipedia.org/wiki/Elixir), [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Perl](https://en.wikipedia.org/wiki/Perl), [PHP](https://en.wikipedia.org/wiki/PHP), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), [REBOL](https://en.wikipedia.org/wiki/REBOL), [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)), [SAS](https://en.wikipedia.org/wiki/SAS_(software)), [Smalltalk](https://en.wikipedia.org/wiki/Smalltalk), and [Tcl](https://en.wikipedia.org/wiki/Tcl)), then techniques can be used to perform metaprogramming without actually generating source code.


### Uses in programming languages 

> NOTE：下面描述了使用metaprogramming的实现technique

#### Macro systems

Main article: [Macro (computer science)](https://en.wikipedia.org/wiki/Macro_(computer_science))

- [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp) and most [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) dialects.

  

#### Macro assemblers

> NOTE: 暂未了解

#### Metaclasses

[Metaclasses](https://en.wikipedia.org/wiki/Metaclass) are provided by the following programming languages:

- [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp)[[10\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-10)
- [Python](https://en.wikipedia.org/wiki/Python_(programming_language))
- [Nil](https://en.wikipedia.org/wiki/Nil_(programming_language))
- [Groovy](https://en.wikipedia.org/wiki/Groovy_(programming_language))
- [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language))
- [Smalltalk](https://en.wikipedia.org/wiki/Smalltalk)

#### Template metaprogramming

Main article: [Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)

- [C "X Macros"](https://en.wikipedia.org/wiki/X_Macro)
- [C++ Templates](https://en.wikipedia.org/wiki/C%2B%2B_Templates)
- [D](https://en.wikipedia.org/wiki/D_(programming_language))
- [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp), [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)) and most Lisp dialects by using the quasiquote ("backquote") operator.[[11\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-11)
- [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language))

> NOTE: C++是使用template metaprogramming的典范，所以将template metaprogramming放到了`C-family-language\C++\Language-reference\Template\Template-metaprogramming.md`。


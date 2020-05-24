# Metaprogramming

在文章Meta中，总结了programming language中的各种meta，其中我们已经总结了：

> meta是一种抽象

## 维基百科[Metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming) 

**Metaprogramming** is a programming technique in which [computer programs](https://en.wikipedia.org/wiki/Computer_program) have the ability to treat programs as their data. It means that a program can be designed to read, generate, analyze or transform other programs, and even modify itself while running. [[1\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-1)[[2\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-2) In some cases, this allows programmers to minimize the number of lines of code to express a solution, thus reducing the development time.[[3\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-3)  It also allows programs greater flexibility to efficiently handle new situations without recompilation.

> NOTE: 上面所描述的是metaprogramming的核心思想。结合我们所使用的具体的programming（c++、python），发现其实它们都提供了对metaprogramming的支持

**Metaprogramming** can be used to move computations from [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) to [compile-time](https://en.wikipedia.org/wiki/Compile-time), to generate code using compile time computations, and to enable [self-modifying code](https://en.wikipedia.org/wiki/Self-modifying_code). The language in which the metaprogram is written is called the [metalanguage](https://en.wikipedia.org/wiki/Self-modifying_code). The language of the programs that are manipulated is called the [attribute-oriented programming](https://en.wikipedia.org/wiki/Attribute-oriented_programming) language. The ability of a programming language to be its own metalanguage is called [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_science)) or "reflexivity".[[4\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-4) **Reflection** is a valuable language feature to facilitate metaprogramming.

> NOTE: 

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
- Scheme [hygienic macros](https://en.wikipedia.org/wiki/Hygienic_macro)
- [MacroML](https://en.wikipedia.org/wiki/MacroML)
- [Template Haskell](https://en.wikipedia.org/wiki/Template_Haskell)
- [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)) macros
- [Clojure](https://en.wikipedia.org/wiki/Clojure_(programming_language)) macros
- [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language))

#### Macro assemblers

The [IBM/360](https://en.wikipedia.org/wiki/IBM/360) and derivatives had powerful [macro assembler](https://en.wikipedia.org/wiki/Macro_assembler) facilities that were often used to generate complete [assembly language](https://en.wikipedia.org/wiki/Assembly_language) programs[*citation needed*] or sections of programs (for different operating systems for instance). Macros provided with [CICS](https://en.wikipedia.org/wiki/CICS) [transaction processing](https://en.wikipedia.org/wiki/Transaction_processing) system had assembler macros that generated COBOL statements as a pre-processing step.

Other assemblers, such as [MASM](https://en.wikipedia.org/wiki/MASM), also support macros.

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

> NOTE: c++是使用template metaprogramming的典范，所以将template metaprogramming放到了`C-family-language\C++\Language-reference\Template\Template-metaprogramming.md`。

#### Staged metaprogramming

- [MetaML](https://en.wikipedia.org/w/index.php?title=MetaML&action=edit&redlink=1)
- [MetaOCaml](https://en.wikipedia.org/wiki/OCaml#MetaOCaml)
- [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)) using the [Lightweight Modular Staging](https://en.wikipedia.org/w/index.php?title=Lightweight_Modular_Staging&action=edit&redlink=1) Framework[[12\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-12)[[13\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-13)
- [Terra](http://terralang.org/)

#### With dependent types

- Usage of [dependent types](https://en.wikipedia.org/wiki/Dependent_type) allows proving that generated code is never invalid.[[14\]](https://en.wikipedia.org/wiki/Metaprogramming#cite_note-14) However, this approach is bleeding-edge and is rarely found outside of research programming languages.
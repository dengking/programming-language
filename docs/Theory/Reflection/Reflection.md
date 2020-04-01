# [Reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming))

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **reflection** is the ability of a [computer program](https://en.wikipedia.org/wiki/Computer_program) to examine, [introspect](https://en.wikipedia.org/wiki/Introspection_(computer_science)), and modify its own structure and behavior at [runtime](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)).[[1\]](https://en.wikipedia.org/wiki/Reflection_(computer_programming)#cite_note-1)

***SUMMARY*** : 关于reflection和[introspect](https://en.wikipedia.org/wiki/Introspection_(computer_science))之间的差异参见[introspect](https://en.wikipedia.org/wiki/Introspection_(computer_science))



## Historical background

The earliest computers were programmed in their native [assembly language](https://en.wikipedia.org/wiki/Assembly_language), which were inherently reflective, as these original architectures could be programmed by defining instructions as data and using [self-modifying code](https://en.wikipedia.org/wiki/Self-modifying_code). As programming moved to compiled higher-level languages such as [Algol](https://en.wikipedia.org/wiki/ALGOL), [Cobol](https://en.wikipedia.org/wiki/Cobol), and [Fortran](https://en.wikipedia.org/wiki/Fortran) (but also [Pascal](https://en.wikipedia.org/wiki/Pascal_(programming_language)) and [C](https://en.wikipedia.org/wiki/C_(programming_language)) and many other languages), this reflective ability largely disappeared until programming languages with **reflection** built into their type systems appeared.[*citation needed*]

[Brian Cantwell Smith](https://en.wikipedia.org/wiki/Brian_Cantwell_Smith)'s 1982 doctoral dissertation[[2\]](https://en.wikipedia.org/wiki/Reflection_(computer_programming)#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Reflection_(computer_programming)#cite_note-3) introduced the notion of computational reflection in procedural [programming languages](https://en.wikipedia.org/wiki/Programming_languages) and the notion of the [meta-circular interpreter](https://en.wikipedia.org/wiki/Meta-circular_interpreter) as a component of [3-Lisp](https://en.wikipedia.org/w/index.php?title=3-Lisp&action=edit&redlink=1).



## Uses

**Reflection** helps programmers make generic software libraries to display data, process different formats of data, perform [serialization](https://en.wikipedia.org/wiki/Serialization) or deserialization of data for communication, or do bundling and unbundling of data for containers or bursts of communication.

***TRANSLATION*** : 反射帮助程序员制作通用软件库以显示数据，处理不同格式的数据，执行数据的序列化或反序列化以进行通信，或者为容器或突发通信进行数据的捆绑和分拆。

Effective use of reflection almost always requires a plan: A design framework, encoding description, object library, a map of a database or entity relations.

Reflection makes a language more suited to network-oriented code. For example, it assists languages such as Java to operate well in networks by enabling libraries for serialization, bundling and varying data formats. Languages without reflection (e.g. [C](https://en.wikipedia.org/wiki/C_(programming_language))) have to use auxiliary compilers, e.g. for [Abstract Syntax Notation](https://en.wikipedia.org/wiki/Abstract_Syntax_Notation), to produce code for serialization and bundling.

Reflection can be used for observing and modifying program execution at runtime. A reflection-oriented program component can monitor the execution of an enclosure of code and can modify itself according to a desired goal related to that enclosure. This is typically accomplished by dynamically assigning program code at runtime.

In object-oriented programming languages such as [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), reflection allows *inspection* of classes, interfaces, fields and methods at runtime without knowing the names of the interfaces, fields, methods at compile time. It also allows *instantiation* of new objects and *invocation* of methods.

Reflection is often used as part of [software testing](https://en.wikipedia.org/wiki/Software_testing), such as for the runtime creation/instantiation of [mock objects](https://en.wikipedia.org/wiki/Mock_object).

Reflection is also a key strategy for [metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming).

In some object-oriented programming languages, such as [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) and [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), reflection can be used to override [member accessibility](https://en.wikipedia.org/wiki/Member_accessibility) rules. For example, reflection makes it possible to change the value of a field marked "private" in a third-party library's class.



## Implementation

A language supporting reflection provides a number of features available at runtime that would otherwise be difficult to accomplish in a lower-level language. Some of these features are the abilities to:

- Discover and modify source-code constructions (such as **code blocks**, [classes](https://en.wikipedia.org/wiki/Class_(computer_science)), **methods**, protocols, etc.) as [first-class objects](https://en.wikipedia.org/wiki/First-class_object) at runtime.

- Convert a [string](https://en.wikipedia.org/wiki/String_(computer_science)) matching the symbolic name of a class or function into a reference to or invocation of that class or function.

- Evaluate a string as if it were a source-code statement at runtime.

  ***SUMMARY*** : 看到此处，想到了python的`eval()`[¶](https://docs.python.org/3/library/functions.html#eval)

- Create a new interpreter for the language's bytecode to give a new meaning or purpose for a programming construct.

These features can be implemented in different ways. In [MOO](https://en.wikipedia.org/wiki/MOO_(programming_language)), reflection forms a natural part of everyday programming idiom. When verbs (methods) are called, various variables such as *verb* (the name of the verb being called) and *this* (the object on which the verb is called) are populated to give the context of the call. Security is typically managed by accessing the caller stack programmatically: Since *callers*() is a list of the methods by which the current verb was eventually called, performing tests on *callers*()[1] (the command invoked by the original user) allows the verb to protect itself against unauthorised use.

Compiled languages rely on their runtime system to provide information about the source code. A compiled [Objective-C](https://en.wikipedia.org/wiki/Objective-C) executable, for example, records the names of all methods in a block of the executable, providing a table to correspond these with the underlying methods (or selectors for these methods) compiled into the program. In a compiled language that supports runtime creation of functions, such as [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp), the runtime environment must include a compiler or an interpreter.

Reflection can be implemented for languages not having built-in reflection facilities by using a [program transformation](https://en.wikipedia.org/wiki/Program_transformation) system to define automated source-code changes.



## Examples

The following code snippets create an [instance](https://en.wikipedia.org/wiki/Instance_(computer_science)) `foo` of [class](https://en.wikipedia.org/wiki/Class_(computer_science)) `Foo` and invoke its [method](https://en.wikipedia.org/wiki/Method_(computer_science)) `PrintHello`. For each [programming language](https://en.wikipedia.org/wiki/Programming_language), normal and reflection-based call sequences are shown.



### Python

The following is an example in [Python](https://en.wikipedia.org/wiki/Python_(programming_language)):

```python
# without reflection
obj = Foo()
obj.hello()

# with reflection
class_name = "Foo"
method = "hello"
obj = globals()[class_name]()
getattr(obj, method)()

# with eval
eval("Foo().hello()")
```
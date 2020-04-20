[TOC]



# [Program analysis](https://en.wikipedia.org/wiki/Program_analysis)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **program analysis**[[1\]](https://en.wikipedia.org/wiki/Program_analysis#cite_note-1) is the process of automatically analyzing the behavior of computer programs regarding a property such as correctness, robustness, safety and liveness. **Program analysis** focuses on two major areas: [program optimization](https://en.wikipedia.org/wiki/Program_optimization) and [program correctness](https://en.wikipedia.org/wiki/Program_correctness). The first focuses on improving the program’s **performance** while reducing the **resource usage** while the latter focuses on ensuring that the program does what it is supposed to do.

Program analysis can be performed without executing the program ([static program analysis](https://en.wikipedia.org/wiki/Static_program_analysis)), during runtime ([dynamic program analysis](https://en.wikipedia.org/wiki/Dynamic_program_analysis)) or in a combination of both.

## Static program analysis



Main article: [static program analysis](https://en.wikipedia.org/wiki/Static_program_analysis)

In the context of program correctness, static analysis can discover vulnerabilities during the development phase of the program[[2\]](https://en.wikipedia.org/wiki/Program_analysis#cite_note-2). These vulnerabilities are easier to correct than the ones found during the testing phase since **static analysis** leads to the root of the vulnerability.

Due to many forms of static analysis being computationally undecidable, the mechanisms for doing it will not always terminate with the right answer – either because they sometimes return a false negative ("no problems found" when the code does in fact have problems) or a false positive, or because they never return the wrong answer but sometimes never terminate. Despite their limitations, the first type of mechanism might reduce the number of vulnerabilities, while the second can sometimes give strong assurance of the lack of a certain class of vulnerabilities.

由于许多形式的静态分析在计算上是不可判定的，所以这样做的机制并不总是以正确的答案终止 - 或者因为它们有时会返回假阴性（“当代码确实存在问题时没有发现问题”）或者误报，或因为他们永远不会回答错误的答案，但有时永远不会终止。尽管存在局限性，但第一种机制可能会减少漏洞的数量，而第二种机制有时可以强有力地保证缺少某类漏洞。

Incorrect optimizations are highly undesirable. So, in the context of program optimization, there are two main strategies to handle computationally undecidable analysis:

不正确的优化是非常不受欢迎的。因此，在程序优化的上下文中，有两种主要策略来处理计算不可判断的分析：

1. An optimizer that is expected to complete in a relatively short amount of time, such as the optimizer in an optimizing compiler, may use a truncated version of an analysis that is guaranteed to complete in a finite amount of time, and guaranteed to only find correct optimizations.

   预期在相对较短的时间内完成的优化器（例如优化编译器中的优化器）可以使用保证在有限时间内完成的分析的截断版本，并保证仅找到正确的优化。

2. A third-party optimization tool may be implemented in such a way as to never produce an incorrect optimization, but also so that it can, in some situations, continue running indefinitely until it finds one (which may never happen). In this case, the developer using the tool would have to stop the tool and avoid running the tool on that piece of code again (or possibly modify the code to avoid tripping up the tool).

   第三方优化工具可以以永不产生不正确的优化的方式实现，但也使得在某些情况下它可以无限期地继续运行直到找到一个（可能永远不会发生）。在这种情况下，使用该工具的开发人员必须停止该工具并避免再次在该段代码上运行该工具（或者可能修改代码以避免使工具失效）。

However, there is also a third strategy that is sometimes applicable for languages that are not completely specified, such as [C](https://en.wikipedia.org/wiki/C_(programming_language)). An optimizing compiler is at liberty to generate code that does anything at runtime – even crashes – if it encounters source code whose semantics are unspecified by the language standard in use.

但是，还有第三种策略有时适用于未完全指定的语言，例如C.优化编译器可以自由生成在运行时执行任何操作的代码 - 甚至崩溃 - 如果它遇到语义为的源代码未指定使用的语言标准。



### Control-flow

Main article: [control-flow analysis](https://en.wikipedia.org/wiki/Control-flow_analysis)

The purpose of control-flow analysis is to obtain information about which functions can be called at various points during the execution of a program. The collected information is represented by a [control flow graph](https://en.wikipedia.org/wiki/Control_flow_graph) (CFG) where the nodes are instructions of the program and the edges represent the flow of control. By identifying code blocks and loops CFG becomes a starting point for compiler made optimizations.

### Data-flow analysis

Main article: [data-flow analysis](https://en.wikipedia.org/wiki/Data-flow_analysis)

Data-flow analysis is a technique designed to gather information about the values at each point of the program and how they change over time. This technique is often used by compilers to optimize the code. One of the most known examples of data-flow analysis is [taint checking](https://en.wikipedia.org/wiki/Taint_checking) which consists of considering all variables which contain user supplied data – which is considered "tainted", i.e. insecure – and preventing those variables from being used until they have been sanitized. This technique is often used to prevent [SQL injection](https://en.wikipedia.org/wiki/SQL_injection) attacks. Taint checking can be done statically or dynamically.



### Abstract interpretation

Main article: [Abstract interpretation](https://en.wikipedia.org/wiki/Abstract_interpretation)

Abstract interpretation allows the extraction of information about a possible execution of a program without actually executing the program. This information can be used by compilers to look for possible optimizations or for certifying a program against certain classes of bugs.



### Type systems

Main article: [type system](https://en.wikipedia.org/wiki/Type_system)

Type systems associate types to programs that fulfill certain requirements. Their purpose is to select a subset of programs of a language that are considered correct according to a property.

- [Type checking](https://en.wikipedia.org/wiki/Type_system#Type_checking) – verify whether the program is accepted by the type system.

Type checking is used in programming to limit how a programming object is used and what can they do. This is done by the compiler or interpreter. Type checking can also help prevent vulnerabilities by ensuring that a signed value isn't attributed to an unsigned variable. Type checking can be done statically (at compile time), dynamically (at runtime) or a combination of both.

Static type information (either [inferred](https://en.wikipedia.org/wiki/Type_inference), or explicitly provided by type annotations in the source code) can also be used to do optimizations, such as replacing [boxed arrays](https://en.wikipedia.org/wiki/Boxed_type) with unboxed arrays.





# [List of performance analysis tools](https://en.wikipedia.org/wiki/List_of_performance_analysis_tools)


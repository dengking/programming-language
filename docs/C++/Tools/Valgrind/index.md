# Valgrind



## wikipedia [Valgrind](https://en.wikipedia.org/wiki/Valgrind)

**Valgrind** ([/ˈvælɡrɪnd/](https://en.wikipedia.org/wiki/Help:IPA/English)) is a [programming tool](https://en.wikipedia.org/wiki/Programming_tool) for [memory debugging](https://en.wikipedia.org/wiki/Memory_debugger), [memory leak](https://en.wikipedia.org/wiki/Memory_leak) detection, and [profiling](https://en.wikipedia.org/wiki/Profiling_(computer_programming)).

> NOTE: 集成了多种功能

Valgrind was originally designed to be a [free](https://en.wikipedia.org/wiki/Free_software) [memory debugging](https://en.wikipedia.org/wiki/Memory_debugging) tool for [Linux](https://en.wikipedia.org/wiki/Linux) on [x86](https://en.wikipedia.org/wiki/X86), but has since evolved to become a generic framework for creating dynamic analysis tools such as checkers and profilers.

> NOTE: 关于上述"generic framework"，需要结合"wikipedia [Valgrind](https://en.wikipedia.org/wiki/Valgrind) # Overview"章节内容和 官网 [Valgrind](https://www.valgrind.org/) 中"Valgrind is an instrumentation framework for building dynamic analysis tools"中的内容来进行理解。

### Overview

Valgrind is in essence a [virtual machine](https://en.wikipedia.org/wiki/Virtual_machine) using [just-in-time](https://en.wikipedia.org/wiki/Just-in-time_compilation) (JIT) compilation techniques, including [dynamic recompilation](https://en.wikipedia.org/wiki/Dynamic_recompilation). Nothing from the original program ever gets run directly on the host [processor](https://en.wikipedia.org/wiki/Central_processing_unit). Instead, Valgrind first translates the program into a temporary, simpler form called **Intermediate Representation (IR)**, which is a processor-neutral, [SSA](https://en.wikipedia.org/wiki/Static_single_assignment_form)-based form. 

After the conversion, a *tool* (see below) is free to do whatever transformations it would like on the IR, before Valgrind translates the IR back into machine code and lets the host processor run it. 

Valgrind recompiles [binary code](https://en.wikipedia.org/wiki/Binary_code) to run on host and target (or simulated) CPUs of the same architecture. 

> NOTE: 各种IR的转换

It also includes a [GDB](https://en.wikipedia.org/wiki/GDB) stub to allow debugging of the target program as it runs in Valgrind, with "monitor commands" that allow querying the Valgrind tool for various information.

A considerable amount of performance is lost in these transformations (and usually, the code the tool inserts); usually, code run with Valgrind and the "none" tool (which does nothing to the IR) runs at 20% to 25% of the speed of the normal program.[[4\]](https://en.wikipedia.org/wiki/Valgrind#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Valgrind#cite_note-5)

### Tools

*See also:* [Memory debugger](https://en.wikipedia.org/wiki/Memory_debugger) *and* [Profiling (computer programming)](https://en.wikipedia.org/wiki/Profiling_(computer_programming))

#### Memcheck



## 官网 [Valgrind](https://www.valgrind.org/) 

Valgrind is an instrumentation framework for building dynamic analysis tools
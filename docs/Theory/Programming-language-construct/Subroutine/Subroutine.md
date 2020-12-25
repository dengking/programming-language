# Subroutine

"subroutine"就是我们平时所说的“函数”，我们对它习以为常，

## wikipedia [Subroutine](https://en.wikipedia.org/wiki/Subroutine)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **subroutine** is a sequence of program instructions that performs a specific task, packaged as a unit. This unit can then be used in programs wherever that particular [task](https://en.wikipedia.org/wiki/Task_(computing)) should be performed.

Subprograms may be defined within programs, or separately in [libraries](https://en.wikipedia.org/wiki/Library_(computer_science)) that can be used by many programs. In different programming languages, a subroutine may be called a **procedure**, a **function**, a **routine**, a [method](https://en.wikipedia.org/wiki/Method_(computing)), or a **subprogram**. The generic term **callable unit** is sometimes used. 

The name *subprogram* suggests a subroutine behaves in much the same way as a computer program that is used as one step in a larger program or another subprogram. A subroutine is often coded so that it can be started several times and from several places during one execution of the program, including from other subroutines, and then branch back (*return*) to the next instruction after the *call*, once the subroutine's task is done. The idea of a subroutine was initially conceived by [John Mauchly](https://en.wikipedia.org/wiki/John_Mauchly) during his work on [ENIAC](https://en.wikipedia.org/wiki/ENIAC), and recorded in a Harvard symposium in January of 1947 entitled 'Preparation of Problems for EDVAC-type Machines'. . [Maurice Wilkes](https://en.wikipedia.org/wiki/Maurice_Wilkes), [David Wheeler](https://en.wikipedia.org/wiki/David_Wheeler_(British_computer_scientist)), and [Stanley Gill](https://en.wikipedia.org/wiki/Stanley_Gill) are generally credited with the formal invention of this concept, which they termed a *closed subroutine*, contrasted with an *open subroutine* or [macro](https://en.wikipedia.org/wiki/Macro_(computer_science)). 

> NOTE： 今天我们都对subprogram习以为常，但是第一个提出的人，却是经过了万千思考，具有开创性地意义。

Subroutines are a powerful [programming](https://en.wikipedia.org/wiki/Programming_language) tool,[[7\]](https://en.wikipedia.org/wiki/Subroutine#cite_note-knuth1-7) and the [syntax](https://en.wikipedia.org/wiki/Syntax_(programming_languages)) of many [programming languages](https://en.wikipedia.org/wiki/Programming_language) includes support for writing and using them. Judicious(明智的) use of subroutines (for example, through the [structured programming](https://en.wikipedia.org/wiki/Structured_programming) approach) will often substantially(大体上) reduce the cost of developing and maintaining a large program, while increasing its quality and reliability.[[8\]](https://en.wikipedia.org/wiki/Subroutine#cite_note-structprog-8) Subroutines, often collected into [libraries](https://en.wikipedia.org/wiki/Library_(computing)), are an important mechanism for sharing and trading software. The discipline of [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) is based on [objects](https://en.wikipedia.org/wiki/Object_(computer_science)) and [methods](https://en.wikipedia.org/wiki/Method_(computer_programming)) (which are subroutines attached to these objects or object [classes](https://en.wikipedia.org/wiki/Class_(computer_programming))).

In the [compiling](https://en.wikipedia.org/wiki/Compiler) method called [threaded code](https://en.wikipedia.org/wiki/Threaded_code), the executable program is basically a sequence of subroutine calls.



### Main concepts

A subroutine may be written so that it expects to obtain one or more data values from the calling program (to replace its [parameters](https://en.wikipedia.org/wiki/Parameter_(computer_science)) or formal parameters). The calling program provides actual values for these parameters, called [arguments](https://en.wikipedia.org/wiki/Argument_(computing)). Different programming languages may use different conventions for passing arguments:

|         Convention         |                         Description                          |                          Common use                          |
| :------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|       Call by value        | Argument is evaluated and copy of value is passed to subroutine； | Default in most Algol-like languages after [Algol 60](https://en.wikipedia.org/wiki/Algol_60), such as Pascal, Delphi, Simula, CPL, PL/M, Modula, Oberon, Ada, and many others. C, `C++`, Java (References to objects and arrays are also passed by value) |
|     Call by reference      |  Reference to argument, typically its **address** is passed  | Selectable in most Algol-like languages after [Algol 60](https://en.wikipedia.org/wiki/Algol_60), such as Algol 68, Pascal, Delphi, Simula, CPL, PL/M, Modula, Oberon, Ada, and many others. `C++`, Fortran, [PL/I](https://en.wikipedia.org/wiki/PL/I) |
|       Call by result       | Parameter value is copied back to argument on return from the subroutine |                      Ada OUT parameters                      |
|    Call by value-result    | Parameter value is copied back on entry to the subroutine and again on return |                            Algol                             |
|        Call by name        | Like a macro – replace the parameters with the unevaluated argument expressions;注意，是**unevaluated** ，这和Call by value截然不同。 | Algol, [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)) |
| Call by **constant** value | Like call by value except that the parameter is treated as a constant |       PL/I NONASSIGNABLE parameters, Ada IN parameters       |

> NOTE: 注意，是否evaluate是一个非常重要的区别；

The subroutine may return a computed value to its caller (its [return value](https://en.wikipedia.org/wiki/Return_value)), or provide various result values or output parameters. Indeed, a common use of subroutines is to implement [mathematical functions](https://en.wikipedia.org/wiki/Function_(mathematics)), in which the purpose of the subroutine is purely to compute one or more results whose values are entirely determined by the arguments passed to the subroutine. (Examples might include computing the [logarithm](https://en.wikipedia.org/wiki/Logarithm) of a number or the [determinant](https://en.wikipedia.org/wiki/Determinant) of a [matrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)).)



A subroutine can be coded so that it may [call itself recursively](https://en.wikipedia.org/wiki/Recursion_(computer_science)), at one or more places, to perform its task. This method allows direct implementation of functions defined by [mathematical induction](https://en.wikipedia.org/wiki/Mathematical_induction) and recursive [divide and conquer algorithms](https://en.wikipedia.org/wiki/Divide_and_conquer_algorithm).

> NOTE: 显然宏替换不存在递归

A subroutine whose purpose is to compute one [boolean-valued function](https://en.wikipedia.org/wiki/Boolean-valued_function) (that is, to answer a yes/no question) is sometimes called a predicate. In [logic programming](https://en.wikipedia.org/wiki/Logic_programming)languages, often[*vague*] all subroutines are called predicates, since they primarily[*vague*] determine success or failure. 





### Language support

[High-level programming languages](https://en.wikipedia.org/wiki/High-level_programming_language) usually include specific constructs to:

- delimit the part of the program (body) that makes up the subroutine
- assign an [identifier](https://en.wikipedia.org/wiki/Identifier) (name) to the subroutine
- specify the names and [data types](https://en.wikipedia.org/wiki/Data_type) of its parameters and return values
- provide a private [naming scope](https://en.wikipedia.org/wiki/Scope_(computer_science)) for its [temporary variables](https://en.wikipedia.org/wiki/Temporary_variable)
- identify variables outside the subroutine that are accessible within it
- call the subroutine
- provide values to its parameters
- the main program contains the **address** of the subprogram
- the sub program contains the address of next instruction of the function call in main program
- specify the return values from within its body
- [return](https://en.wikipedia.org/wiki/Return_statement) to the calling program
- dispose of the values returned by a call
- handle any [exceptional conditions](https://en.wikipedia.org/wiki/Exception_handling) encountered during the call
- package subroutines into a [module](https://en.wikipedia.org/wiki/Modular_programming), [library](https://en.wikipedia.org/wiki/Library_(computing)), [object](https://en.wikipedia.org/wiki/Object_(computer_science)), [class](https://en.wikipedia.org/wiki/Class_(computer_programming)), etc.



In [programming languages](https://en.wikipedia.org/wiki/Programming_language) such as [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), subroutines may also simply be called functions, not to be confused with [mathematical functions](https://en.wikipedia.org/wiki/Mathematical_functions) or [functional programming](https://en.wikipedia.org/wiki/Functional_programming), which are different concepts.

A language's [compiler](https://en.wikipedia.org/wiki/Compiler) will usually translate **procedure calls** and **returns** into machine instructions according to a well-defined [calling convention](https://en.wikipedia.org/wiki/Calling_convention), so that subroutines can be compiled separately from the programs that call them. The instruction sequences corresponding to call and return statements are called the procedure's [prologue and epilogue](https://en.wikipedia.org/wiki/Function_prologue).



###  Advantages

The advantages of breaking a program into subroutines include:

- [Decomposing](https://en.wikipedia.org/wiki/Decomposition_(computer_science)) a complex programming task into simpler steps: this is one of the two main tools of [structured programming](https://en.wikipedia.org/wiki/Structured_programming), along with [data structures](https://en.wikipedia.org/wiki/Data_structure)
- Reducing [duplicate code](https://en.wikipedia.org/wiki/Duplicate_code) within a program
- Enabling [reuse of code](https://en.wikipedia.org/wiki/Code_reuse) across multiple programs
- Dividing a large programming task among various programmers, or various stages of a project
- [Hiding implementation details](https://en.wikipedia.org/wiki/Information_hiding) from users of the subroutine
- Improving readability of code by replacing a block of code with a function call where a [descriptive](https://books.google.co.in/books?id=_i6bDeoCQzsC&pg=PA39&dq=descriptive+function+name) function name serves to describe the block of code. This makes the calling code concise and readable even if the function is not meant to be reused.
- Improving [traceability](https://en.wikipedia.org/wiki/Traceability#Software) (i.e. most languages offer ways to obtain the **call trace** which includes the names of the involved subroutines and perhaps even more information such as file names and line numbers); by not decomposing the code into subroutines, debugging would be severely impaired

### Disadvantages

Invoking a subroutine (versus using in-line code) imposes some [computational overhead](https://en.wikipedia.org/wiki/Computational_overhead) in the call mechanism.

A subroutine typically requires standard [housekeeping](https://en.wikipedia.org/wiki/Housekeeping_(computing)) code – both at entry to, and exit from, the function ([function prologue and epilogue](https://en.wikipedia.org/wiki/Function_prologue) – usually saving [general purpose registers](https://en.wikipedia.org/wiki/General_purpose_register) and return address as a minimum).



### History

### Call stack

Most modern implementations of a **subroutine call** use a [call stack](https://en.wikipedia.org/wiki/Call_stack), a special case of the [stack data structure](https://en.wikipedia.org/wiki/Stack_(data_structure)), to implement subroutine calls and returns. Each procedure call creates a new entry, called a *stack frame*, at the top of the stack; when the procedure returns, its stack frame is deleted from the stack, and its space may be used for other procedure calls. Each **stack frame** contains the *private data* of the corresponding call, which typically includes the procedure's parameters and internal variables, and the return address.

The call sequence can be implemented by a sequence of ordinary instructions (an approach still used in [reduced instruction set computing](https://en.wikipedia.org/wiki/Reduced_instruction_set_computing) (RISC) and [very long instruction word](https://en.wikipedia.org/wiki/Very_long_instruction_word) (VLIW) architectures), but many traditional machines designed since the late 1960s have included special instructions for that purpose.

The **call stack** is usually implemented as a contiguous area of memory. It is an arbitrary design choice whether the bottom of the stack is the lowest or highest address within this area, so that the stack may grow forwards or backwards in memory; however, many architectures chose the latter.[*citation needed*]

> NOTE: 关于call stack的增长方向，可以参考[Virtual address space](https://en.wikipedia.org/wiki/Virtual_address_space)中的视图和[这篇文章](https://cpp.tech-academy.co.uk/memory-layout/) 显然，在process的address space中有专门的stack空间。

Some designs, notably some [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language)) implementations, used two separate stacks, one mainly for control information (like return addresses and loop counters) and the other for data. The former was, or worked like, a call stack and was only indirectly accessible to the programmer through other language constructs while the latter was more directly accessible.

When stack-based procedure calls were first introduced, an important motivation was to **save precious memory**.[*citation needed*] With this scheme, the compiler does not have to reserve separate space in memory for the private data (parameters, return address, and local variables) of each procedure. At any moment, the stack contains only the private data of the calls that are currently *active* (namely, which have been called but haven't returned yet). Because of the ways in which programs were usually assembled from libraries, it was (and still is) not uncommon to find programs that include thousands of subroutines, of which only a handful are active at any given moment.[*citation needed*] For such programs, the call stack mechanism could save significant amounts of memory. Indeed, the call stack mechanism can be viewed as the earliest and simplest method for [automatic memory management](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)).

However, another advantage of the call stack method is that it allows [recursive subroutine calls](https://en.wikipedia.org/wiki/Recursion_(computer_science)), since each nested call to the same procedure gets a separate instance of its private data.



> NOTE: 最后两段话介绍了stack-based procedure call的优势，作用；关于call stack，还是看其专门介绍。



### Local variables, recursion and reentrancy

[stack frames](https://en.wikipedia.org/wiki/Stack_frame)

If a **subprogram** can be executed properly even when another execution of the same subprogram is already in progress, that **subprogram** is said to be *reentrant*. A recursive subprogram must be **reentrant**. Reentrant subprograms are also useful in [multi-threaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) situations, since multiple threads can call the same subprogram without fear of interfering with each other. In the [IBM](https://en.wikipedia.org/wiki/IBM) [CICS](https://en.wikipedia.org/wiki/CICS) [transaction processing system](https://en.wikipedia.org/wiki/Transaction_processing_system), *quasi-reentrant* was a slightly less restrictive, but similar, requirement for application programs that were shared by many threads.

In a [multi-threaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) environment, there is generally more than one stack. An environment that fully supports [coroutines](https://en.wikipedia.org/wiki/Coroutine) or [lazy evaluation](https://en.wikipedia.org/wiki/Lazy_evaluation) may use data structures other than stacks to store their **activation records**.

### Overloading

### Optimization of subroutine calls

There is a significant runtime [overhead](https://en.wikipedia.org/wiki/Computational_overhead) in a **calling a subroutine**, including passing the arguments, branching to the subprogram, and branching back to the caller. The **overhead** often includes saving and restoring certain processor registers, allocating and reclaiming call frame storage, etc.. In some languages, each **subroutine call** also implies automatic testing of the subroutine's return code, or the handling of [exceptions](https://en.wikipedia.org/wiki/Exception_(programming)) that it may raise. In object-oriented languages, a significant source of overhead is the intensively used [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) for method calls.

There are some seemingly obvious optimizations of procedure calls that cannot be applied if the procedures may have **side effects**. For example, in the expression `(f(x)-1)/(f(x)+1)`, the function `f` must be called twice, because the two calls may return different results. Moreover, the value of `x` must be fetched again before the second call, since the first call may have changed it. Determining whether a subprogram may have a side effect is very difficult (indeed, [undecidable](https://en.wikipedia.org/wiki/Undecidable_problem)).[*citation needed*]So, while those optimizations are safe in purely functional programming languages, compilers of typical imperative programming usually have to assume the worst.

#### Inlining

A method used to eliminate this overhead is *inline expansion* or *inlining* of the subprogram's body at each [call site](https://en.wikipedia.org/wiki/Call_site) (versus branching to the subroutine and back). Not only does this avoid the call overhead, but it also allows the [compiler](https://en.wikipedia.org/wiki/Compiler) to [optimize](https://en.wikipedia.org/wiki/Code_optimization) the procedure's *body* more effectively by taking into account the context and arguments at that call. The inserted body can be optimized by the compiler. Inlining however, will usually increase the code size, unless the program contains only one call to the subroutine, or the subroutine body is less code than the call overhead.





## draft

### 20201215

本章总结“subroute”即“函数”相关内容。这部分内容已经转移到了到了工程[programming-language](https://dengking.github.io/programming-language/)。



#### subroutine梳理思路

subroutine是thread的执行单位。

subroutine的执行过程可以以树的形式来进行展示，在tree中进行了总结。

subroutine的时间复杂度、空间复杂度的分析，在算法分析中进行总结。

#### calling convention and call by value、by reference



#### calling convention

每次调用还是都需要new一个栈帧

每次调用函数，还需要保存上一个函数的context



#### 将subroutine和coroutine进行总结
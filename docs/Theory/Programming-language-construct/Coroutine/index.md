# Coroutine

越来越多的language支持Coroutine。

Coroutine是现代programming language实现"cooperative multitasking"的主要方式，支持coroutine的programming language基于了programmer来进行cooperative schedule task的能力，让programmer能够control schedule of task。相比之下，programmer是无法直接控制thread、process的schedule 。

## wikipedia [Coroutine](https://en.wikipedia.org/wiki/Coroutine)

**Coroutines** are [computer program](https://en.wikipedia.org/wiki/Computer_program) components that generalize [subroutines](https://en.wikipedia.org/wiki/Subroutine) for [non-preemptive multitasking](https://en.wikipedia.org/wiki/Non-preemptive_multitasking), by allowing execution to be suspended and resumed. **Coroutines** are well-suited for implementing familiar program components such as [cooperative tasks](https://en.wikipedia.org/wiki/Cooperative_multitasking), [exceptions](https://en.wikipedia.org/wiki/Exception_handling), [event loops](https://en.wikipedia.org/wiki/Event_loop), [iterators](https://en.wikipedia.org/wiki/Iterator), [infinite lists](https://en.wikipedia.org/wiki/Lazy_evaluation) and [pipes](https://en.wikipedia.org/wiki/Pipeline_(software)).

According to [Donald Knuth](https://en.wikipedia.org/wiki/Donald_Knuth), [Melvin Conway](https://en.wikipedia.org/wiki/Melvin_Conway) coined the term *coroutine* in 1958 when he applied it to construction of an [assembly program](https://en.wikipedia.org/wiki/Assembly_language). The first published explanation of the coroutine appeared later, in 1963. 

### Comparison with subroutines

**Subroutines** are special cases of **coroutines**. When subroutines are invoked, execution begins at the start, and once a subroutine exits, it is finished; an instance of a subroutine only returns once, and does not hold state between invocations. By contrast, **coroutines** can exit by **calling** other coroutines, which may later return to the point where they were invoked in the original coroutine; from the coroutine's point of view, it is not **exiting** but calling another coroutine. Thus, a coroutine instance holds state, and varies between invocations; there can be multiple instances of a given **coroutine** at once. The difference between calling another coroutine by means of ["yielding"](https://en.wikipedia.org/wiki/Yield_(multithreading)) to it and simply calling another routine (which then, also, would return to the original point), is that the relationship between two coroutines which yield to each other is not that of caller-callee, but instead symmetric.

> NOTE: 最后一段话非常重要，它的意思是：通过["yielding"](https://en.wikipedia.org/wiki/Yield_(multithreading))来调用另外一个coroutine和简单调用另外一个routine之间的差异是两者之间的关系，通过yield的coroutine之间的关系不是caller-callee，而是对称的；理解这段话的关键是对["yielding"](https://en.wikipedia.org/wiki/Yield_(multithreading)) 理解，yield即让渡，停止当前coroutine并让另外一个coroutine开始执行。

Any subroutine can be translated to a coroutine which does not call *yield*. 

Here is a simple example of how coroutines can be useful. Suppose you have a consumer-producer relationship where one routine creates items and adds them to a queue and another removes items from the queue and uses them. For reasons of efficiency, you want to add and remove several items at once. The code might look like this:

```
var q := new queue

coroutine produce
    loop
        while q is not full
            create some new items
            add the items to q
        yield to consume

coroutine consume
    loop
        while q is not empty
            remove some items from q
            use the items
        yield to produce
```

The queue is then completely filled or emptied before yielding control to the other coroutine using the *yield* command. The further coroutines calls are starting right after the *yield*, in the outer coroutine loop.

Although this example is often used as an introduction to [multithreading](https://en.wikipedia.org/wiki/Thread_(computing)), two threads are not needed for this: the *yield* statement can be implemented by a jump directly from one routine into the other.

### Comparison with threads

**Coroutines** are very similar to [threads](https://en.wikipedia.org/wiki/Thread_(computing)). However, coroutines are [cooperatively](https://en.wikipedia.org/wiki/Cooperative_multitasking) multitasked, whereas threads are typically [preemptively](https://en.wikipedia.org/wiki/Preemptive_multitasking) [multitasked](https://en.wikipedia.org/wiki/Multitasking). This means that coroutines provide [concurrency](https://en.wikipedia.org/wiki/Concurrency_(computer_science)) but not [parallelism](https://en.wikipedia.org/wiki/Parallel_computing). The advantages of coroutines over threads are that they may be used in a [hard-realtime](https://en.wikipedia.org/wiki/Hard_realtime) context ([switching](https://en.wikipedia.org/wiki/Context_switch) between coroutines need not involve any [system calls](https://en.wikipedia.org/wiki/System_calls) or any [blocking](https://en.wikipedia.org/wiki/Blocking_(computing)) calls whatsoever), there is no need for synchronisation primitives such as [mutexes](https://en.wikipedia.org/wiki/Mutex), semaphores, etc. in order to guard [critical sections](https://en.wikipedia.org/wiki/Critical_sections), and there is no need for support from the operating system.

It is possible to implement coroutines using preemptively-scheduled threads, in a way that will be transparent to the calling code, but some of the advantages (particularly the suitability for hard-realtime operation and relative cheapness of switching between them) will be lost.



### Comparison with generators

[Generators](https://en.wikipedia.org/wiki/Generator_(computer_science)), also known as **semicoroutines**,[[5\]](https://en.wikipedia.org/wiki/Coroutine#cite_note-Ralston2000-5) are a subset of coroutines. Specifically, while both can yield multiple times, suspending their execution and allowing re-entry at multiple entry points, they differ in **coroutines**' ability to control where execution continues immediately after they yield, while generators cannot, instead transferring control back to the generator's caller.[[6\]](https://en.wikipedia.org/wiki/Coroutine#cite_note-6) That is, since generators are primarily used to simplify the writing of [iterators](https://en.wikipedia.org/wiki/Iterator), the `yield` statement in a generator does not specify a **coroutine** to jump to, but rather passes a value back to a parent routine.

However, it is still possible to implement coroutines on top of a generator facility, with the aid of a top-level dispatcher routine (a [trampoline](https://en.wikipedia.org/wiki/Trampoline_(computing)), essentially) that passes control explicitly to child generators identified by tokens passed back from the generators:

```
var q := new queue
generator produce
    loop
        while q is not full
            create some new items
            add the items to q
        yield consume
generator consume
    loop
        while q is not empty
            remove some items from q
            use the items
        yield produce
subroutine dispatcher
    var d := new dictionary(generator → iterator)
    d[produce] := start produce
    d[consume] := start consume
    var current := produce
    loop
        current := next d[current]
```

A number of implementations of coroutines for languages with generator support but no native coroutines (e.g. Python[[7\]](https://en.wikipedia.org/wiki/Coroutine#cite_note-MertzIBM-7) before 2.5) use this or a similar model.

### Comparison with mutual recursion

Further information: [Mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion)

Using coroutines for state machines or concurrency is similar to using [mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion) with [tail calls](https://en.wikipedia.org/wiki/Tail_call), as in both cases the control changes to a different one of a set of routines. However, coroutines are more flexible and generally more efficient. Since coroutines yield rather than return, and then resume execution rather than restarting from the beginning, they are able to hold state, both variables (as in a closure) and execution point, and yields are not limited to being in tail position; mutually recursive subroutines must either use shared variables or pass state as parameters. Further, each mutually recursive call of a subroutine requires a new stack frame (unless [tail call elimination](https://en.wikipedia.org/wiki/Tail_call_elimination) is implemented), while passing control between coroutines uses the existing contexts and can be implemented simply by a jump.

### Common uses

Coroutines are useful to implement the following:

- [State machines](https://en.wikipedia.org/wiki/State_machine) within a single subroutine, where the state is determined by the current entry/exit point of the procedure; this can result in more readable code compared to use of [goto](https://en.wikipedia.org/wiki/Goto), and may also be implemented via [mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion) with [tail calls](https://en.wikipedia.org/wiki/Tail_call).
- [Actor model](https://en.wikipedia.org/wiki/Actor_model) of concurrency, for instance in [video games](https://en.wikipedia.org/wiki/Video_game). Each actor has its own procedures (this again logically separates the code), but they voluntarily give up control to central scheduler, which executes them sequentially (this is a form of [cooperative multitasking](https://en.wikipedia.org/wiki/Cooperative_multitasking)).
- [Generators](https://en.wikipedia.org/wiki/Generator_(computer_programming)), and these are useful for [streams](https://en.wikipedia.org/wiki/Stream_(computing)) – particularly input/output – and for generic traversal of data structures.
- [Communicating sequential processes](https://en.wikipedia.org/wiki/Communicating_sequential_processes) where each sub-process is a coroutine. Channel inputs/outputs and blocking operations yield coroutines and a scheduler unblocks them on completion events. Alternatively, each sub-process may be the parent of the one following it in the data pipeline (or preceding it, in which case the pattern can be expressed as nested generators).
- Reverse communication, commonly used in mathematical software, wherein a procedure such as a solver, integral evaluator, ... needs the using process to make a computation, such as evaluating an equation or integrand.



### Implementations

As of 2003, many of the most popular programming languages, including C and its derivatives, do not have direct support for coroutines within the language or their standard libraries. (This is, in large part, due to the limitations of [stack-based](https://en.wikipedia.org/wiki/Call_stack)subroutine implementation.) An exception is the C++ library [Boost.Context](http://www.boost.org/doc/libs/1_55_0/libs/context/doc/html/index.html), part of [boost libraries](http://www.boost.org/), which supports context swapping on ARM, MIPS, PowerPC, SPARC and x86 on POSIX, Mac OS X and Windows. Coroutines can be built upon Boost.Context.

In situations where a coroutine would be the natural implementation of a mechanism, but is not available, the typical response is to use a [closure](https://en.wikipedia.org/wiki/Closure_(computer_science)) – a subroutine with state variables ([static variables](https://en.wikipedia.org/wiki/Static_variable), often boolean flags) to maintain an internal state between calls, and to transfer control to the correct point. Conditionals within the code result in the execution of different code paths on successive calls, based on the values of the state variables. Another typical response is to implement an explicit state machine in the form of a large and complex [switch statement](https://en.wikipedia.org/wiki/Switch_statement) or via a [goto](https://en.wikipedia.org/wiki/Goto) statement, particularly a [computed goto](https://en.wikipedia.org/wiki/Computed_goto). Such implementations are considered difficult to understand and maintain, and a motivation for coroutine support.

[Threads](https://en.wikipedia.org/wiki/Thread_(computing)), and to a lesser extent [fibers](https://en.wikipedia.org/wiki/Fiber_(computer_science)), are an alternative to coroutines in mainstream programming environments today. Threads provide facilities for managing the realtime cooperative interaction of *simultaneously* executing pieces of code. Threads are widely available in environments that support C (and are supported natively in many other modern languages), are familiar to many programmers, and are usually well-implemented, well-documented and well-supported. However, as they solve a large and difficult problem they include many powerful and complex facilities and have a correspondingly difficult learning curve. As such, when a coroutine is all that is needed, using a thread can be overkill.

One important difference between threads and coroutines is that threads are typically preemptively scheduled while coroutines are not. Because threads can be rescheduled at any instant and can execute concurrently, programs using threads must be careful about [locking](https://en.wikipedia.org/wiki/Lock_(computer_science)). In contrast, because coroutines can only be rescheduled at specific points in the program and do not execute concurrently, programs using coroutines can often avoid locking entirely. (This property is also cited as a benefit of [event-driven](https://en.wikipedia.org/wiki/Event-driven_programming) or asynchronous programming.)

Since fibers are cooperatively scheduled, they provide an ideal base for implementing coroutines above.[[20\]](https://en.wikipedia.org/wiki/Coroutine#cite_note-msdn-wrap-20) However, system support for fibers is often lacking compared to that for threads.

## stackoverflow [What is a coroutine?](https://stackoverflow.com/questions/553704/what-is-a-coroutine)
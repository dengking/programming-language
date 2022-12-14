# stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)

C++11 introduced a standardized memory model, but what exactly does that mean? And how is it going to affect C++ programming?

[This article](http://www.theregister.co.uk/2011/06/11/herb_sutter_next_c_plus_plus/page2.html) (by **Gavin Clarke** who quotes **[Herb Sutter](https://en.wikipedia.org/wiki/Herb_Sutter)**) says that,

> The memory model means that C++ code now has a standardized library to call regardless of who made the compiler and on what platform it's running. There's a standard way to control how different threads talk to the processor's memory.
>
> "When you are talking about splitting [code] across different cores that's in the standard, we are talking about the memory model. We are going to optimize it without breaking the following assumptions people are going to make in the code," **Sutter** said.

Well, I can *memorize* this and similar paragraphs available online (as I've had my own memory model since birth :P) and can even post as an answer to questions asked by others, but to be honest, I don't exactly understand this.

C++ programmers used to develop multi-threaded applications even before, so how does it matter if it's POSIX threads, or Windows threads, or C++11 threads? What are the benefits? I want to understand the low-level details.

I also get this feeling that the C++11 memory model is somehow related to C++11 multi-threading support, as I often see these two together. If it is, how exactly? Why should they be related?

As I don't know how the internals of multi-threading work, and what memory model means in general, please help me understand these concepts. :-)



## [A](https://stackoverflow.com/a/6319356)

> NOTE: 
>
> 一、非常好的一篇文章，它解释清楚了如下问题:
>
> 1、为什么引入memory model
>
> 2、C++11 memory model feature
>
> 3、使用一个非常具体的例子，说明清楚了: **sequential consistency**、memory ordering、mutex之间的差异



First, you have to learn to think like a Language Lawyer.

The C++ specification does not make reference to any particular compiler, operating system, or CPU. It makes reference to an *abstract machine* that is a generalization of actual systems. In the Language Lawyer world, the job of the programmer is to write code for the abstract machine; the job of the compiler is to actualize that code on a concrete machine. By coding rigidly to the spec, you can be certain that your code will compile and run without modification on any system with a compliant C++ compiler, whether today or 50 years from now.

> NOTE: 
>
> 一、显然，programming language是"design to an abstraction"；
>
> 关于上面这段话中的思想、相关内容，参见 `Theory\Programming-language\Design-of-programming-language` 章节

The abstract machine in the C++98/C++03 specification is fundamentally single-threaded. So it is not possible to write multi-threaded C++ code that is "fully portable" with respect to the spec. The spec does not even say anything about the *atomicity* of memory loads and stores or the *order* in which loads and stores might happen, never mind things like mutexes.

> NOTE: 
>
> 1、parallel computing
>
> 2、portable
>
> 3、atomic，充分发挥hardware power

Of course, you can write multi-threaded code in practice for particular concrete systems – like pthreads or Windows. But there is no *standard* way to write multi-threaded code for C++98/C++03.

The abstract machine in C++11 is multi-threaded by design. It also has a well-defined *memory model*; that is, it says what the compiler may and may not do when it comes to accessing memory.

> NOTE: 这段话总结了C++11 memory model feature

Consider the following example, where a pair of global variables are accessed concurrently by two threads:

```cpp
           Global
           int x, y;

Thread 1            Thread 2
x = 17;             cout << y << " ";
y = 37;             cout << x << endl;
```

What might Thread 2 output?

Under C++98/C++03, this is not even Undefined Behavior; the question itself is *meaningless* because the standard does not contemplate anything called a "thread".

Under C++11, the result is Undefined Behavior, because loads and stores need not be atomic in general. Which may not seem like much of an improvement... And by itself, it's not.

### Sequential consistency

But with C++11, you can write this:

```cpp
           Global
           atomic<int> x, y;

Thread 1                 Thread 2
x.store(17);             cout << y.load() << " ";
y.store(37);             cout << x.load() << endl;
```

Now things get much more interesting. First of all, the behavior here is *defined*. Thread 2 could now print `0 0` (if it runs before Thread 1), `37 17` (if it runs after Thread 1), or `0 17` (if it runs after Thread 1 assigns to x but before it assigns to y).

What it cannot print is `37 0`, because the default mode for atomic loads/stores in C++11 is to enforce *sequential consistency*. This just means all loads and stores must be "as if" they happened in the order you wrote them within each thread, while operations among threads can be interleaved however the system likes. So the default behavior of atomics provides both *atomicity* and *ordering* for loads and stores.

> NOTE: 
>
> 一、通过上述例子，我们理解了sequential consistency;
>
> "So the default behavior of atomics provides both *atomicity* and *ordering* for loads and stores"，体现了C++ atomic library的强大。

### Relaxed

Now, on a modern CPU, ensuring **sequential consistency** can be expensive. In particular, the compiler is likely to emit full-blown(成熟的) memory barriers between every access here. But if your algorithm can tolerate **out-of-order** loads and stores; i.e., if it requires atomicity but not ordering; i.e., if it can tolerate `37 0` as output from this program, then you can write this:

```cpp
           Global
           atomic<int> x, y;

Thread 1                            Thread 2
x.store(17,memory_order_relaxed);   cout << y.load(memory_order_relaxed) << " ";
y.store(37,memory_order_relaxed);   cout << x.load(memory_order_relaxed) << endl;
```

The more modern the CPU, the more likely this is to be faster than the previous example.

### Acquire-release

Finally, if you just need to keep particular loads and stores in order, you can write:

```cpp
           Global
           atomic<int> x, y;

Thread 1                            Thread 2
x.store(17,memory_order_release);   cout << y.load(memory_order_acquire) << " ";
y.store(37,memory_order_release);   cout << x.load(memory_order_acquire) << endl;
```

This takes us back to the ordered loads and stores – so `37 0` is no longer a possible output – but it does so with minimal overhead. (In this trivial example, the result is the same as full-blown sequential consistency; in a larger program, it would not be.)

Of course, if the only outputs you want to see are `0 0` or `37 17`, you can just wrap a mutex around the original code. But if you have read this far, I bet you already know how that works, and this answer is already longer than I intended :-).

So, bottom line. Mutexes are great, and C++11 standardizes them. But sometimes for performance reasons you want lower-level primitives (e.g., the classic [double-checked locking pattern](http://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-6-double-checked-locking.html)). The new standard provides high-level gadgets(小工具) like mutexes and condition variables, and it also provides low-level gadgets like atomic types and the various flavors of memory barrier. So now you can write sophisticated, high-performance concurrent routines entirely within the language specified by the standard, and you can be certain your code will compile and run unchanged on both today's systems and tomorrow's.

Although to be frank, unless you are an expert and working on some serious low-level code, you should probably stick to mutexes and condition variables. That's what I intend to do.

For more on this stuff, see [this blog post](http://bartoszmilewski.wordpress.com/2008/12/01/c-atomics-and-memory-ordering/).

### 完整测试程序如下

```C++
#include <iostream>
#include <thread>
#include <atomic>
std::atomic<int> x, y;
void t1()
{
	x.store(17, std::memory_order_relaxed);
	y.store(37, std::memory_order_relaxed);
}
void t2()
{
	std::cout << y.load(std::memory_order_relaxed) << " ";
	std::cout << x.load(std::memory_order_relaxed) << std::endl;
}
int main()
{
	std::thread to1(t1);
	std::thread to2(t2);
	to1.join();
	to2.join();
}
// g++ test.cpp --std=c++11 -lpthread


```



### Comments

> NOTE: 评论也非常精彩

#### Out of order execution

@Nemo: Great answer. I've one doubt now: when the output is `37 0` in `Thread 2`, did you not imply that `Thread 1` will execute second statement *before* first statement? If so, would it not produce illogical and undesirable result? I mean, what if the second statement requires the first statement to be executed first to be logically correct? – [Nawaz](https://stackoverflow.com/users/415784/nawaz) [Jun 12 '11 at 14:22](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7392100_6319356) 


@Nawaz: Yes! Memory accesses can get reordered by the compiler or CPU. Think about (e.g.) caches and speculative loads. The order in which system memory gets hit can be nothing like what you coded. The compiler and CPU will ensure such reorderings do not break *single-threaded* code. For multi-threaded code, the "memory model" characterizes the possible re-orderings, and what happens if two threads read/write the same location at the same time, and how you excert control over both. For single-threaded code, the memory model is irrelevant. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 12 '11 at 17:08](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7393384_6319356)



@Nemo Could you please explain the 37, 0 part? Whichever way I look at it, x is always assigned before y. So when y is 37, it means x has already been assigned its value. Because if y is 37 and x is 0 then it means single threaded execution is not sequential, which is just wrong. – [zindarod](https://stackoverflow.com/users/2286337/zindarod) [Apr 8 '17 at 13:01](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment73655600_6319356)

@Zindarod: Both the compiler and CPU are free to reorder both loads and stores when you use `memory_order_relaxed`. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Apr 10 '17 at 3:27](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment73693845_6319356)



@Nemo: I understand that the CPU can generate the "37, 0" problem because of the inconsistency between main memory and cache levels. So the value read or wrote by one thread might be different to the value read / wrote by the other. However I don't see how a compiler might interfere with the order of the operations in each single thread. Afaik it just "translate" your code into machine instructions in the same order you wrote. So the problem seems to be more related to CPUs, memory levels and threads scheduling than compilers in my opinion. – [Bemipefe](https://stackoverflow.com/users/1423806/bemipefe) [Oct 31 '17 at 7:46](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment81006900_6319356)

@Bemipefe: No, the compiler is not obliged to translate your code in the same order you wrote it - it is allowed to re-order operations, provided the overall effect is the same. It might do this, for example, because reordering allows it to produce faster (or smaller) code. – [psmears](https://stackoverflow.com/users/333698/psmears) [Nov 21 '17 at 16:13](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment81789216_6319356)

@Bemipefe "*nisistency between main memory and cache levels*" No, cache is kept consistent at all time. – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Jun 7 '18 at 3:10](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment88472620_6319356)

@psmears "*provided the overall effect is the same*" that statement needs to be qualified. The effect is measurably different! – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Jun 26 '18 at 17:24](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment89087967_6319356)

@curiousguy: Whether the caches are kept consistent is CPU-architecture dependent. – [psmears](https://stackoverflow.com/users/333698/psmears) [Jun 27 '18 at 7:58](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment89105212_6319356)

@curiousguy: I didn't say the effect is the same - perhaps you missed the word "overall"? (i.e. the intermediate states may be different, but the before/after states will be the same) Obviously that's not the full story, but it's hard to fit the entire language definition into the comment box ;-) – [psmears](https://stackoverflow.com/users/333698/psmears) [Jun 27 '18 at 8:00](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment89105322_6319356)

@psmears Which commonly used arch where C, C++ or Java can be reasonably supported doesn't have consistent caches? – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Jun 10 '19 at 5:13](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment99626183_6319356)



#### *sequenced-before* and  inter-thread *happens-before* 

@Nawaz, @Nemo - A minor detail: the new memory model is relevant in single-threaded code insofar( 在这个范围，到这个程度) as it specifies the undefinedness(不确定性) of certain expressions, such as `i = i++`. The old concept of *sequence points* has been discarded; the new standard specifies the same thing using a *sequenced-before* relation which is just a special case of the more general inter-thread *happens-before* concept. – [JohannesD](https://stackoverflow.com/users/279597/johannesd) [Jun 13 '11 at 13:14](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7404214_6319356) 

> NOTE: 这段评论非常好，它提出了一个非常容易忽视的内容；
>
> 另外，上面这段话，让我想到了make it computational: ordering

#### Initialization

Nice answer although note that you would still need to initialize those atomic variables in order to avoid undefined behavior unless somewhere in the new standard integral types are default initialized. Usually only certain compilers in debug builds will set those values to 0 implicitly. – [AJG85](https://stackoverflow.com/users/516725/ajg85) [Jun 13 '11 at 17:35](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7408683_6319356)



@AJG85: Section 3.6.2 of the draft C++0x spec says, "Variables with static storage duration (3.7.1) or thread storage duration (3.7.2) shall be zero-initialized (8.5) before any other initialization takes place." Since x,y are global in this example, they have static storage duration and therefore will zero-initialized, I believe. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 13 '11 at 20:16](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7411265_6319356)

#### Double-checked locking pattern-VS-Meyers singleton

In most cases there is no sence to implement the double-checked locking initialization in C++11 since local static variable initialization is already thread-safe. – [newbie](https://stackoverflow.com/users/2489083/newbie) [Jun 17 '15 at 21:29](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment49844327_6319356)

@newbie: True, C+11 codified common practice and made the "Meyers singleton" thread-safe. I was just providing an example of the sort of thing you can do with the low-level synchronization primitives. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 17 '15 at 22:34](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment49845972_6319356)



#### Sequential consistency

The answer is great, but "37 0" example is simply wrong. Sequential consistency is not enforced on two different atomic variables. This should be corrected before new generation of clueless programmers is repeating mistakes of previous generation – [hamilyon](https://stackoverflow.com/users/3032312/hamilyon) [Jul 6 '20 at 12:53](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment110978175_6319356) 



@hamilyon: You are mistaken. See e.g. [stackoverflow.com/a/14851782](https://stackoverflow.com/a/14851782), or really any tutorial on sequential consistency. By definition, sequential consistency applies to *all* loads and stores, including across multiple memory locations. (It is expensive, not natural, on any modern CPU.) – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jul 6 '20 at 16:04](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment110984513_6319356)

## TODO: [A](https://stackoverflow.com/a/18520606)

I will just give the analogy with which I understand memory consistency models (or memory models, for short). It is inspired by Leslie Lamport's seminal paper ["Time, Clocks, and the Ordering of Events in a Distributed System"](http://dl.acm.org/citation.cfm?id=359545.359563). The analogy is apt(恰当的) and has fundamental significance, but may be overkill(过犹不及，意思是: 太高深了，一般人可能搞不懂) for many people. However, I hope it provides a mental image (a pictorial representation) that facilitates reasoning about memory consistency models.

Let’s view the histories of all memory locations in a **space-time diagram** in which the horizontal(水平的) axis represents the address space (i.e., each memory location is represented by a point on that axis) and the vertical(垂直的) axis represents time (we will see that, in general, there is not a universal notion of time). The history of values held by each memory location is, therefore, represented by a vertical(垂直的) column at that memory address. Each value change is due to one of the threads writing a new value to that location. By a ***memory image***, we will mean the aggregate/combination of values of all memory locations observable ***at a particular time*** by ***a particular thread***.

> NOTE: 这种表示方式是非常好的

Quoting from ["A Primer on Memory Consistency and Cache Coherence"](http://www.morganclaypool.com/doi/abs/10.2200/S00346ED1V01Y201104CAC016)

> The intuitive (and most restrictive) memory model is sequential consistency (SC) in which a multithreaded execution should look like an interleaving of the sequential executions of each constituent thread, as if the threads were time-multiplexed on a single-core processor.

That global memory order can vary from one run of the program to another and may not be known beforehand. The characteristic feature of SC is the set of horizontal(水平) slices(切片) in the address-space-time diagram representing ***planes of simultaneity*** (i.e., memory images). On a given plane, all of its events (or memory values) are simultaneous. There is a notion of *Absolute Time*, in which all threads agree on which memory values are simultaneous. In SC, at every time instant, there is only one memory image shared by all threads. That's, at every instant of time, all processors agree on the memory image (i.e., the aggregate content of memory). Not only does this imply that all threads view the same sequence of values for all memory locations, but also that all processors observe the same *combinations of values* of all variables. This is the same as saying all memory operations (on all memory locations) are observed in the same total order by all threads.

In relaxed memory models, each thread will slice up address-space-time in its own way, the only restriction being that slices of each thread shall not cross each other because all threads must agree on the history of every individual memory location (of course, slices of different threads may, and will, cross each other). There is no universal way to slice it up (no privileged foliation of address-space-time). Slices do not have to be planar (or linear). They can be curved and this is what can make a thread read values written by another thread out of the order they were written in. Histories of different memory locations may slide (or get stretched) arbitrarily relative to each other ***when viewed by any particular thread***. Each thread will have a different sense of which events (or, equivalently, memory values) are simultaneous. The set of events (or memory values) that are simultaneous to one thread are not simultaneous to another. Thus, in a relaxed memory model, all threads still observe the same history (i.e., sequence of values) for each memory location. But they may observe different memory images (i.e., combinations of values of all memory locations). Even if two different memory locations are written by the same thread in sequence, the two newly written values may be observed in different order by other threads.

[Picture from Wikipedia] ![Picture from Wikipedia](https://upload.wikimedia.org/wikipedia/commons/f/f1/Relsim2.GIF)

Readers familiar with Einstein’s **Special Theory of Relativity** will notice what I am alluding to. Translating Minkowski’s words into the memory models realm: address space and time are shadows of address-space-time. In this case, each observer (i.e., thread) will project shadows of events (i.e., memory stores/loads) onto his own world-line (i.e., his time axis) and his own plane of simultaneity (his address-space axis). Threads in the C++11 memory model correspond to ***observers*** that are moving relative to each other in special relativity. Sequential consistency corresponds to the **Galilean space-time** (i.e., all observers agree on one absolute order of events and a global sense of simultaneity).

The resemblance between memory models and special relativity stems from the fact that both define a partially-ordered set of events, often called a causal set. Some events (i.e., memory stores) can affect (but not be affected by) other events. A C++11 thread (or observer in physics) is no more than a chain (i.e., a totally ordered set) of events (e.g., memory loads and stores to possibly different addresses).

In relativity, some order is restored to the seemingly chaotic picture of partially ordered events, since the only temporal ordering that all observers agree on is the ordering among “timelike” events (i.e., those events that are in principle connectible by any particle going slower than the speed of light in a vacuum). Only the timelike related events are invariantly ordered. [Time in Physics, Craig Callender](http://philosophyfaculty.ucsd.edu/faculty/ccallender/index.html).

In C++11 memory model, a similar mechanism (the acquire-release consistency model) is used to establish these ***local causality relations***.

To provide a definition of memory consistency and a motivation for abandoning SC, I will quote from ["A Primer on Memory Consistency and Cache Coherence"](http://www.morganclaypool.com/doi/abs/10.2200/S00346ED1V01Y201104CAC016)

> For a shared memory machine, the memory consistency model defines the architecturally visible behavior of its memory system. The correctness criterion for a single processor core partitions behavior between “*one correct result*” and “*many incorrect alternatives*”. This is because the processor’s architecture mandates that the execution of a thread transforms a given input state into a single well-defined output state, even on an out-of-order core. Shared memory consistency models, however, concern the loads and stores of multiple threads and usually allow *many correct executions* while disallowing many (more) incorrect ones. The possibility of multiple correct executions is due to the ISA allowing multiple threads to execute concurrently, often with many possible legal interleavings of instructions from different threads.
>
> ***Relaxed*** or ***weak*** memory consistency models are motivated by the fact that most memory orderings in strong models are unnecessary. If a thread updates ten data items and then a synchronization flag, programmers usually do not care if the data items are updated in order with respect to each other but only that all data items are updated before the flag is updated (usually implemented using FENCE instructions). Relaxed models seek to capture this increased ordering flexibility and preserve only the orders that programmers “*require*” to get both higher performance and correctness of SC. For example, in certain architectures, FIFO write buffers are used by each core to hold the results of committed (retired) stores before writing the results to the caches. This optimization enhances performance but violates SC. The write buffer hides the latency of servicing a store miss. Because stores are common, being able to avoid stalling on most of them is an important benefit. For a single-core processor, a write buffer can be made architecturally invisible by ensuring that a load to address A returns the value of the most recent store to A even if one or more stores to A are in the write buffer. This is typically done by either bypassing the value of the most recent store to A to the load from A, where “most recent” is determined by program order, or by stalling a load of A if a store to A is in the write buffer. When multiple cores are used, each will have its own bypassing write buffer. Without write buffers, the hardware is SC, but with write buffers, it is not, making write buffers architecturally visible in a multicore processor.
>
> Store-store reordering may happen if a core has a non-FIFO write buffer that lets stores depart in a different order than the order in which they entered. This might occur if the first store misses in the cache while the second hits or if the second store can coalesce with an earlier store (i.e., before the first store). Load-load reordering may also happen on dynamically-scheduled cores that execute instructions out of program order. That can behave the same as reordering stores on another core (Can you come up with an example interleaving between two threads?). Reordering an earlier load with a later store (a load-store reordering) can cause many incorrect behaviors, such as loading a value after releasing the lock that protects it (if the store is the unlock operation). Note that store-load reorderings may also arise due to local bypassing in the commonly implemented FIFO write buffer, even with a core that executes all instructions in program order.

Because cache coherence and memory consistency are sometimes confused, it is instructive to also have this quote:

> Unlike consistency, ***cache coherence*** is neither visible to software nor required. Coherence seeks to make the caches of a shared-memory system as functionally invisible as the caches in a single-core system. Correct coherence ensures that a programmer cannot determine whether and where a system has caches by analyzing the results of loads and stores. This is because correct coherence ensures that the caches never enable new or different ***functional*** behavior (programmers may still be able to infer likely cache structure using ***timing*** information). The main purpose of cache coherence protocols is maintaining the single-writer-multiple-readers (SWMR) invariant for every memory location. An important distinction between coherence and consistency is that coherence is specified on a ***per-memory location basis***, whereas consistency is specified with respect to ***all*** memory locations.

Continuing with our mental picture, the SWMR invariant corresponds to the physical requirement that there be at most one particle located at any one location but there can be an unlimited number of observers of any location.
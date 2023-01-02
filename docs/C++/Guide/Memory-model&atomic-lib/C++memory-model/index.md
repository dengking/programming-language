# C++memory model



## Why C++ need memory model

一、时代背景: parallel computing

对于single thread，其实memory model并没有什么意义；但是对于multi-thread，它有着非常重要的意义，参考:

1、[C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) # [A](https://stackoverflow.com/a/6319356) 的如下comment:

> @Nawaz: Yes! Memory accesses can get reordered by the compiler or CPU. Think about (e.g.) caches and speculative loads. The order in which system memory gets hit can be nothing like what you coded. The compiler and CPU will ensure such reorderings do not break *single-threaded* code. For multi-threaded code, the "memory model" characterizes the possible re-orderings, and what happens if two threads read/write the same location at the same time, and how you excert control over both. **For single-threaded code, the memory model is irrelevant**. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 12 '11 at 17:08](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7393384_6319356)

二、portable

三、充分发挥hardware power

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) 



参考素材:

1、stroustrup C++11FAQ [Memory model](https://www.stroustrup.com/C++11FAQ.html#memory-model) 

2、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) 



### stroustrup C++11FAQ [Memory model](https://www.stroustrup.com/C++11FAQ.html#memory-model) 

A **memory model** is an **agreement** between the **machine architects** and the **compiler writers** to ensure that most programmers do not have to think about the details of **modern computer hardware**. Without a **memory model**, few things related to threading, locking, and lock-free programming would make sense.

> NOTE: 
>
> 一、这是我目前所遇到的最最容易理解的、精简的解答;
>
> C++ memory model是C++ designer对各种modern computer hardware memory的抽象，它由compiler writer来实现dispatch to concrete/implementation，显然它是遵循"Program to an abstraction and polymorphism"；它使programmer能够在不考虑 **modern computer hardware** 细节情况下，编写出正确的program；

The key guarantee is: Two threads of execution can update and access separate memory locations without interfering with each other. But what is a ``memory location?'' A memory location is either an object of **scalar type** or a maximal sequence of adjacent bit-fields all having non-zero width. For example, here **S** has exactly four separate memory locations:

> NOTE: 
>
> 一、上面最后一段话是对memory location的解释。下面的`a`就是 **scalar type** ；

```C++
	struct S {
		char a;			// location #1
		int b:5,		// location #2
		int c:11,
		int :0,			// note: :0 is "special"
		int d:8;		// location #3
		struct {int ee:8;} e;	// location #4
	};
```

Why is this important? Why isn't it obvious? Wasn't this always true? The problem is that when several computations can genuinely(真正的) run in parallel, that is several (apparently) unrelated instructions can execute at the same time, the quirks(差异)of the memory hardware can get exposed. In fact, in the absence of compiler support, issues of instruction and data pipelining and details of cache use *will* be exposed in ways that are completely unmanageable to the applications programmer. This is true even if no two threads have been defined to share data! Consider, two separately compiled ``threads:''

> NOTE: 
>
> 一、上面这段话所总结的是C++ memory model所要解决的问题:
>
> 现代hardware朝着parallel computing的方向发展，而C++ 语言并没有memory model，这就导致了C++ programmer无法对并行执行的C++ program进行控制。

```C++
	// thread 1:
	char c;
	c = 1;
	int x = c;

	// thread 2:
	char b;
	b = 1;
	int y = b;
```

For greater realism, I could have used the separate compilation (within each thread) to ensure that the compiler/optimizer wouldn't be able to eliminate memory accesses and simply ignore **c** and **b** and directly initialize **x** and **y** with 1. What are the possible values of **x** and **y**? According to C++11 the only correct answer is the obvious one: 1 and 1. The reason that's interesting is that if you take a conventional good **pre-concurrency C or C++ compiler**, the possible answers are 0 and 0 (unlikely), 1 and 0, 0 and 1, and 1 and 1. This has been observed in the wild.'How? 

A linker might allocate **c** and **b** right next to each other (in the same word) -- nothing in the C or C++ 1990s standards says otherwise. In that, C++ resembles all languages not designed with real concurrent hardware in mind. However, most modern processors cannot read or write a single character, it must read or write a whole **word**, so the assignment to **c** really is read the word containing **c**, replace the **c** part, and write the word back again.'' Since the assignment to **b** is similar, there are plenty of opportunities for the two threads to clobber(击倒) each other even though the threads do not (according to their source text) share data!

> NOTE: 
>
> 上面所描述的其实是stackoverflow [is assignment operator '=' atomic?](https://stackoverflow.com/questions/8290768/is-assignment-operator-atomic)中列举的一个例子，其中的解释是更加详细的，在工程hardware的CPU-`memory-access\Atomic\Memory-access-unit-and-atomic`章节中，收录了这篇文章；在其中，将这个问题称为: "byte-granular update"。

So, C++11 guarantees that no such problems occur for "**separate memory locations**". More precisely: **A memory location cannot be safely accessed by two threads without some form of locking unless they are both read accesses**. Note that different bitfields within a single word are not separate memory locations, so don't share structs with bitfields among threads without some form of locking. Apart from that caveat, the C++ memory model is simply ""**as everyone would expect**''.

> NOTE: C++11的" **A memory location cannot be safely accessed by two threads without some form of locking unless they are both read accesses**"，显然能够兼容所有的hardware。

> NOTE: "Note that different bitfields within a single word are not separate memory locations, so don't share structs with bitfields among threads without some form of locking."的意思是: 当"bitfields within a single word are not separate memory locations"时，如果它们会被threads同时使用，需要使用lock来保证它们的thread safe

However, it is not always easy to think straight about low-level concurrency issues. Consider:

```C++
	// start with x==0 and y==0

	if (x) y = 1;	// Thread 1 

	if (y) x = 1;	// Thread 2 
```

Is there a problem here? More precisely, is there a data race? (No there isn't).

Fortunately, we have already adapted to modern times and every current C++ compiler (that I know of) gives the one right answer and have done so for years. They do so for most (but unfortunately not yet for all) tricky questions. After all, C++ has been used for serious systems programming of concurrent systems "forever''. The standard should further improve things.

See also

- Standard: 1.7 The C++ memory model [intro.memory]
- Paul E. McKenney, Hans-J. Boehm, and Lawrence Crowl: [C++ Data-Dependency Ordering: Atomics and Memory Model](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2556.html). N2556==08-0066.
- Hans-J. Boehm: [Threads basics](http://www.hpl.hp.com/techreports/2009/HPL-2009-259html.html), HPL technical report 2009-259. ``what every programmer should know about memory model issues.''
- Hans-J. Boehm and Paul McKenney: [A slightly dated FAQ on C++ memory model issues](http://www.hpl.hp.com/personal/Hans_Boehm/c++mm/user-faq.html).



### write-order-to-shared-data-different-in-different-processor-thread

是否会存在不同thread对shared data的修改没有同步过来而导致看到了旧数据？这是一个容易陷入的误区，事实是: 不同thread对shared data的修改，其他的thread是能够看到的，不同thread可能看到的不同的是: memory order的不同，正是由于order的不同，而导致了在lock free情况下会出现问题。因此需要使用memory order进行控制。

> NOTE: 
>
> tag-order of write to shared data may be different among different threads





## C++ memory model feature

总的来说: 它抽象地描述/model了当前主流hardware，能够容纳当今主流的各种CPU memory model(hardware)，容纳它们的feature/差异，让programmer以portable、standard方式，充分发挥hardware power。

> NOTE: 
>
> abstraction and design to an abstraction

下面是一些具体的点: 

1、multi-threaded by design(multicore)

那它如何保证thread-safety呢？后面会进行解答。

2、has a well-defined *memory model*(memory reordering)

3、multiple level，非常的灵活

后面会进行专门的介绍。

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)

2、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)



### How C++memory model ensure thread safety?

在工程 Parallel-computing 的 `Concurrent-computing\Multithread\Thread-safety\What-cause-unsafety` 章节中总结了导致非线程安全的两个原因，那C++memory model 是如何解决这两个问题从而保证thread safety的呢？C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

> 多个thread同时对原子变量进行操作，原子变量能够保证原子性，即没有中间状态，使用原子变量进行lockless programming还需要克服的一个问题是：memory reordering，因此，还需要对memory order进行控制。



## Multiple-level-memory-model

`Multiple-level-memory-model` 对多层次的C++ memory model进行详细的讨论。



## 素材

关于各种memory model的描述；

本章概述C++ memory model，主要参考:

1、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)

3、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model) 



## TODO 

### `memory_ordering` and visibility

stackoverflow [Does seq_cst ordering guarantee immediate visibility?](https://stackoverflow.com/questions/14846494/does-seq-cst-ordering-guarantee-immediate-visibility)

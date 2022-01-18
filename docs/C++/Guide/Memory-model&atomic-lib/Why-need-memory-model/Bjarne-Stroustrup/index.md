# stroustrup C++11FAQ [Memory model](https://www.stroustrup.com/C++11FAQ.html#memory-model)

A **memory model** is an **agreement** between the **machine architects** and the **compiler writers** to ensure that most programmers do not have to think about the details of **modern computer hardware**. Without a **memory model**, few things related to threading, locking, and lock-free programming would make sense.

> NOTE: 这是我目前所遇到的最最容易理解的、精简的解答;
>
> C++ memory model是C++ designer对各种modern computer hardware memory的抽象，它由compiler writer来实现dispatch to concrete/implementation，显然它是遵循"Program to an abstraction and polymorphism"；它使programmer能够在不考虑 **modern computer hardware** 细节情况下，编写出正确的program；

The key guarantee is: Two threads of execution can update and access separate memory locations without interfering with each other. But what is a ``memory location?'' A memory location is either an object of **scalar type** or a maximal sequence of adjacent bit-fields all having non-zero width. For example, here **S** has exactly four separate memory locations:

> NOTE: 上面最后一段话是对memory location的解释。下面的`a`就是 **scalar type** ；

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

> NOTE: 上面这段话所总结的是C++ memory model所要解决的问题:
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

> NOTE: 上面所描述的其实是stackoverflow [is assignment operator '=' atomic?](https://stackoverflow.com/questions/8290768/is-assignment-operator-atomic)中列举的一个例子，其中的解释是更加详细的，在工程hardware的CPU-`memory-access\Atomic\Memory-access-unit-and-atomic`章节中，收录了这篇文章；在其中，将这个问题称为: "byte-granular update"。

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
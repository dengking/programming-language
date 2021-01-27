# aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 

> ∗This is a slightly-modiﬁed version of an article that appeared in Dr. Dobbs Journal in the July (Part I) and August (Part II), 2004, issues.

## 1 Introduction

Google the newsgroups or the web for the names of various design patterns, and you’re sure to ﬁnd that one of the most commonly mentioned is Singleton. Try to put Singleton into practice, however, and you’re all but certain to bump into a signiﬁcant limitation: as traditionally implemented (and as we explain below), Singleton isn’t **thread-safe**. 

Much eﬀort has been put into addressing this shortcoming. One of the most popular approaches is a **design pattern** in its own right, the **Double-Checked Locking Pattern** (DCLP) [13, 14]. DCLP is designed to add eﬃcient threadsafety to initialization of a shared resource (such as a Singleton), but it has a problem: it’s not reliable. Furthermore, there’s virtually no portable way to make it reliable in C++ (or in C) without substantively（本质上） modifying the conventional pattern implementation. To make matters even more interesting, DCLP can fail for diﬀerent reasons on uniprocessor and multiprocessor architectures. 

This article explains why Singleton isn’t thread safe, how DCLP attempts to address that problem, why DCLP may fail on both uni- and multiprocessor architectures, and why you can’t (portably) do anything about it. 

Along the way, it clariﬁes the relationships among **statement ordering** in source code, **sequence points**, compiler and hardware optimizations, and the actual order of **statement execution**. 

Finally, it concludes with some suggestions regarding how to add thread-safety to Singleton (and similar constructs) such that the resulting code is both reliable and eﬃcient.

## 2 The Singleton Pattern and Multithreading

The traditional implementation of the Singleton Pattern [7] is based on making a pointer point to a new object the ﬁrst time the object is requested: 

```c++
// from the header file
class Singleton
{
public:
	static Singleton* instance();
	// ...
private:
	static Singleton *pInstance;
};

// from the implementation file
Singleton *Singleton::pInstance = 0; // Line 11
Singleton* Singleton::instance() // Line 12
{
	if (pInstance == 0) // Line 14
	{
		pInstance = new Singleton; // Line 15
	}
	return pInstance;
}

```

In a single-threaded environment, this generally works ﬁne, though interrupts can be problematic. If you are in `Singleton::instance`, receive an interrupt, and invoke `Singleton::instance` from the handler, you can see how you’d get into trouble. Interrupts aside, however, this implementation works ﬁne in a single-threaded environment. 

> NOTE: 在interrupt handler中使用`Singleton::instance`是非线程安全的，原因在于`new`，关于这个问题，在APUE 10.6 Reentrant Functions中进行了介绍，也可以通过Google “libc malloc  reentrant”，下面是检索到的比较有价值的内容:
>
> 1、stackoverflow [Why are malloc() and printf() said as non-reentrant?](https://stackoverflow.com/questions/3941271/why-are-malloc-and-printf-said-as-non-reentrant)
>
> 2、stackoverflow [Is malloc thread-safe?](https://stackoverflow.com/questions/855763/is-malloc-thread-safe)
>
> 3、gnu libc [24.4.6 Signal Handling and Nonreentrant Functions](https://www.gnu.org/software/libc/manual/html_node/Nonreentrancy.html)
>
> 

Unfortunately, this implementation is not reliable in a multithreaded environment. Suppose that Thread A enters the `instance` function, executes through Line 14 ( `if (pInstance == 0)` ), and is then suspended. At the point where it is suspended, it has just determined that `pInstance` is null, i.e., that no `Singleton` object has yet been created. 

Thread B now enters `instance` and executes Line 14. It sees that `pInstance` is null, so it proceeds to Line 15 and creates a `Singleton` for `pInstance` to point to. It then returns `pInstance` to instance’s caller. 

At some point later, Thread A is allowed to continue running, and the ﬁrst thing it does is move to Line 15, where it conjures(变魔术) up another `Singleton` object and makes `pInstance` point to it. It should be clear that this violates the meaning of a singleton, as there are now two Singleton objects. 

Technically, Line 11 is where `pInstance` is initialized, but for practical purposes, it’s Line 15 that makes it point where we want it to, so for the remainder of this article, we’ll treat Line 15 as the point where `pInstance` is initialized. 

> NOTE: 
>
> 1、上述描述的是典型的race condition: Time-of-check-to-time-of-use(TOCTOU)

Making the classic Singleton implementation thread safe is easy. Just acquire a lock before testing `pInstance`:

```c++
Singleton* Singleton::instance()
{
	Lock lock; // acquire lock (params omitted for simplicity)
	if (pInstance == 0)
	{
		pInstance = new Singleton;
	}
	return pInstance; // release lock (via Lock destructor)
}

```

The downside to this solution is that it may be expensive. Each access to the `Singleton` requires acquisition of a lock, but in reality, we need a lock only when initializing `pInstance`. That should occur only the ﬁrst time instance is called. If instance is called `n` times during the course of a program run, we need the lock only for the ﬁrst call. Why pay forn lock acquisitions when you know that `n−1` of them are unnecessary? DCLP is designed to prevent you from having to.

> NOTE: 上述是典型的read-write-tradeoff，通过这样的tradeoff，来找到优化的思路:
>
> 1、write: "in reality, we need a lock only when initializing `pInstance`. "
>
> 即: 只有在write的时候，才需要lock
>
> 2、只需要write一次，并且write的时机是固定的(即第一次)，后续的所有read都是可以并发的，这不同于使用read-write lock的情况，因为其中需要write多次，且write的时机不固定；
>
> 3、read: "access to the `Singleton` "
>
> 4、上述code是非常直观的写法: 
>
> 如果为null，则说明没有构造，则需要write一次；
>
> 如果非null，则说明已经构造，则可以read；
>
> 5、优化的直观思路是: 只有write的时候，才需要lock，后续的所有的read都是不需要lock的，因此一个非常关键是: 如何有效地判断是否需要write，如果需要write，则lock，然后write；否则不lock，直接read；
>
> 6、下面的"The Double-Checked Locking Pattern"就是运用的这个思路: 
>
> 1st test 就是用于"判断是否需要write"
>
> 

## 3 The Double-Checked Locking Pattern

The crux（关键） of DCLP is the observation that most calls to `instance` will see that `pInstanceis` non-null, hence not even try to initialize it. Therefore, DCLP tests `pInstance` for nullness before trying to acquire a lock. Only if the test succeeds (i.e., if `pInstance` has not yet been initialized) is the lock acquired, and after that the test is performed again to make sure `pInstance` is still null (hence the name **double-checked locking**). The second test is necessary, because, as we just saw, it is possible that another thread happened to initialize `pInstance` between the time `pInstance` was ﬁrst tested and the time the lock was acquired.

> NOTE: 
>
> 1、上面解释了在`lock`后仍然需要check的原因，其实还是race condition: time of check to time of use:
>
> 在  "1st test" 到 `lock`的这段时间内，可能当前的thread被suspend，另外一个thread已经执行了`pInstance = new Singleton;`，因此在获得lock后，仍然需要进行一次check，以判断condition是否仍然处理；

```C++
Singleton* Singleton::instance()
{
	if (pInstance == 0)// 1st test
	{ 
		Lock lock;
		if (pInstance == 0)// 2nd test
		{ 
			pInstance = new Singleton;
		}
	}
	return pInstance;
}

```

## 4 DCLP and Instruction Ordering

Consider again the line that initializes `pInstance`:

```C++
pInstance = new Singleton;
```

This statement causes three things to happen:

Step 1: Allocate memory to hold a `Singleton` object.

Step 2: Construct a `Singleton` object in the allocated memory.

Step 3: Make `pInstance` point to the allocated memory.

Of critical importance is the observation that compilers are not constrained to perform these steps in this order! In particular, compilers are sometimes allowed to swap steps 2 and 3. Why they might want to do that is a question we’ll address in a moment. For now, let’s focus on what happens if they do.

Consider the following code, where we’ve expanded `pInstance`’s initialization line into the three constituent tasks we mentioned above and where we’ve merged steps 1 (memory allocation) and 3 (`pInstance` assignment) into a single statement that precedes step 2 (Singleton construction). The idea is not that a human would write this code. Rather, it’s that a compiler might generate code equivalent to this in response to the conventional **DCLP** source code (shown earlier) that a human would write.

```C++
Singleton* Singleton::instance()
{
	if (pInstance == 0)
	{
		Lock lock;
		if (pInstance == 0)
		{
			pInstance = // Step 3
							operator new(sizeof(Singleton)); // Step 1
			new (pInstance) Singleton; // Step 2
		}
	}
	return pInstance;
}

```

> NOTE: 上述code用到了:
>
> 1、placement new

In general, this is not a valid translation of the original DCLP source code, because the `Singleton` constructor called in step 2 might throw an exception, and if an exception is thrown, it’s important that `pInstance` not yet have been modified. That’s why, in general, compilers cannot move step 3 above step 2.
However, there are conditions under which this transformation is legitimate(合法的). Perhaps the simplest such condition is when a compiler can prove that the `Singleton` constructor cannot throw (e.g., via **post-inlining flow analysis**), but that is not the only condition. Some constructors that throw can also have their
instructions reordered such that this problem arises.

Given the above translation, consider the following sequence of events:

1、Thread A enters instance, performs the first test of `pInstance`, acquires the lock, and executes the statement made up of steps 1 and 3. It is then suspended. At this point `pInstance` is non-null, but no `Singleton` object has yet been constructed in the memory `pInstance` points to.

2、 Thread B enters instance, determines that `pInstance` is non-null, and returns it to instance’s caller. The caller then dereferences the pointer to access the Singleton that, oops, has not yet been constructed.

> NOTE: 典型的Access outside of object lifetime

DCLP will work only if steps 1 and 2 are completed before step 3 is performed, but there is no way to express this constraint in C or C++. That’s the dagger in the heart of DCLP: we need to define a constraint on relative **instruction ordering**, but our languages give us no way to express the constraint.

Yes, the C and C++ standards [16, 15] do define sequence points, which define constraints on the order of evaluation. For example, paragraph 7 of Section 1.9 of the C++ standard encouragingly states:

> At certain specified points in the execution sequence called sequence points, all side effects of previous evaluations shall be complete and no side effects of subsequent evaluations shall have taken place.

Furthermore, both standards state that a **sequence point** occurs at the end of each statement. So it seems that if you’re just careful with how you sequence your statements, everything falls into place. 

> NOTE: sequence pointer让我想起来:
>
> 1、make it computational ordering

Oh, Odysseus, don’t let thyself be lured by sirens’ voices; for much trouble is waiting for thee and thy mates!

> NOTE: 翻译为: 哦，奥德修斯，不要让你自己被塞壬的声音所诱惑;
> 因为有许多麻烦在等着你和你的伙伴们!

Both standards define correct program behavior in terms of the observable behavior of an abstract machine. But not everything about this machine is observable. For example, consider this simple function:

```C++
void Foo()
{
	int x = 0, y = 0; // Statement 1
	x = 5; // Statement 2
	y = 10; // Statement 3
	printf("%d,%d", x, y); // Statement 4
}

```

This function looks silly, but it might plausibly(似乎合理的) be the result of inlining some other functions called by `Foo`.

In both C and C++, the standards guarantee that `Foo` will print "5,10", so we know that that will happen. But that’s about the extent of what we’re guaranteed, hence of what we know.  (后面开始描述我们所不知的)We don’t know whether statements 1-3 will be executed at all, and in fact a good optimizer will get rid of them. If statements 1-3 are executed, we know that statement 1 will precede(领先) statements 2-4 and—assuming that the call to `printf` isn’t inlined and the result further optimized—we know that statement 4 will follow statements 1-3, but we know nothing about the relative ordering of statements 2 and 3. Compilers might choose to execute statement 2 first, statement 3 first, or even to execute them both in parallel, assuming the hardware has some way to do it. Which it might well have. Modern processors have a large word size and several execution units. Two or more arithmetic units are common. (For example, the Pentium 4 has three integer ALUs, PowerPC’s G4e has four, and Itanium has six.) Their machine language allows compilers to generate code that yields parallel execution of two or more instructions in a single clock cycle.

> NOTE: 
>
> 隐藏在"what we know"之下有很多可能的情况是我们所不知的。上面这段就描述了各种我们所不知的；
>
> 上面这段话的字面意思: 但这只是我们所能保证的范围，也就是我们所知道的范围。
>
> 作者所要表达的准确含义是: 

Optimizing compilers carefully analyze and reorder your code so as to execute as many things at once as possible (within the constraints on observable behavior). Discovering and exploiting such parallelism in regular serial code is the single most important reason for rearranging code and introducing **out-of order execution**. But it’s not the only reason. Compilers (and linkers) might also reorder instructions to avoid spilling data from a register, to keep the instruction pipeline full, to perform common subexpression elimination, and to reduce the size of the generated executable [4].

When performing these kinds of optimizations, compilers and linkers for C and C++ are constrained only by the dictates(规定的) of observable behavior on the **abstract machines** defined by the language standards, and—this is the important bit—those **abstract machines** are implicitly single threaded. As languages, neither C nor C++ have threads, so compilers don’t have to worry about breaking threaded programs when they are optimizing. It should therefore not surprise you that they sometimes do.

That being the case, how can one write C and C++ multithreaded programs that actually work? By using system-specific libraries defined for that purpose. Libraries like Posix threads (pthreads) [6] give precise specifications for the execution semantics of various synchronization primitives. These libraries impose restrictions on the code that **library-conformant compilers** are permitted to generate, thus forcing such compilers to emit code that respects the execution ordering constraints on which those libraries depend. That’s why threading packages have parts written in assembler or issue system calls that are themselves written in assembler (or in some unportable language): you have to go outside standard C and C++ to express the ordering constraints that multithreaded programs require. DCLP tries to get by using only language constructs. That’s why DCLP isn’t reliable.

As a rule, programmers don’t like to be pushed around by their compilers. Perhaps you are such a programmer. If so, you may be tempted to try to outsmart your compiler by adjusting your source code so that `pInstance` remains unchanged until after Singleton’s construction is complete. For example, you
might try inserting use of a temporary variable:

```C++
Singleton* Singleton::instance()
{
	if (pInstance == 0)
	{
		Lock lock;
		if (pInstance == 0)
		{
			Singleton *temp = new Singleton; // initialize to temp
			pInstance = temp; // assign temp to pInstance
		}
	}
	return pInstance;
}

```

In essence, you’ve just fired the opening salvo in a war of optimization. Your compiler wants to optimize. You don’t want it to, at least not here. But this is not a battle you want to get into. Your foe is wiley and sophisticated, imbued with strategems dreamed up over decades by people who do nothing but think
about this kind of thing all day long, day after day, year after year. Unless you write optimizing compilers yourself, they are way ahead of you. In this case, for example, it would be a simple matter for the compiler to apply dependence analysis to determine that `temp` is an unnecessary variable, hence to eliminate it,
thus treating your carefully crafted “unoptimizable” code if it had been written in the traditional DCLP manner. Game over. You lose.
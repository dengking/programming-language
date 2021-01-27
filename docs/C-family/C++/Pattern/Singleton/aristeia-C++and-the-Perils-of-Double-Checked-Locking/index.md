# aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 

## 导读

下面这些原因导致了double checked locking的失败:

1、`pInstance`是shared variable，会有multiple thread对它concurrently进行read、write

2、compiler reordering 导致的问题，在"4 DCLP and Instruction Ordering"章节中进行的描述

3、processor reordering 导致的问题，在"4 DCLP and Instruction Ordering"章节中进行的描述

4、***cache coherency*** problem 导致的问题，在"6 DCLP on Multiprocessor Machines"章节中进行的描述 



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

### 理想的Instruction Ordering

Consider again the line that initializes `pInstance`:

```C++
pInstance = new Singleton;
```

This statement causes three things to happen:

Step 1: Allocate memory to hold a `Singleton` object.

Step 2: Construct a `Singleton` object in the allocated memory.

Step 3: Make `pInstance` point to the allocated memory.

### 现实的Instruction ordering

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

### 根本原因

只有当`Singleton` object的initialization完成后，才能够执行step 3，即write to `pInstance` ，给他一个non-null value，如果step 2和step 3对调，即先执行step 3，然后执行step 2，那么可能导致另外一个thread以为`Singleton` object的initialization已经完成了，然后就去access它，就导致access outside of object lifetime，就访问一个未初始化的object，从而导致各种问题；这是典型的Access outside of object lifetime。

从后面的分析可以看出***cache coherency*** problem 会导致相同的问题。

### 导致的问题: Access outside of object lifetime



### 根本解决方法: constraint relative instruction ordering

DCLP will work only if steps 1 and 2 are completed before step 3 is performed, but there is no way to express this constraint in C or C++. That’s the dagger in the heart of DCLP: we need to define a constraint on relative **instruction ordering**, but our languages give us no way to express the constraint.

> NOTE: "define a constraint on relative **instruction ordering**"让我想到了: 
>
> 1、make it computational ordering

### C++03 programming language无法解决这个问题

#### Sequence points

Yes, the C and C++ standards [16, 15] do define sequence points, which define constraints on the order of evaluation. For example, paragraph 7 of Section 1.9 of the C++ standard encouragingly states:

> At certain specified points in the execution sequence called sequence points, all side effects of previous evaluations shall be complete and no side effects of subsequent evaluations shall have taken place.

Furthermore, both standards state that a **sequence point** occurs at the end of each statement. So it seems that if you’re just careful with how you sequence your statements, everything falls into place. 

> NOTE: sequence pointer让我想起来:
>
> 1、make it computational ordering

Oh, Odysseus, don’t let thyself be lured by sirens’ voices; for much trouble is waiting for thee and thy mates!

> NOTE: 翻译为: 哦，奥德修斯，不要让你自己被塞壬的声音所诱惑; 因为有许多麻烦在等着你和你的伙伴们!

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

#### Under the hook: 我们所不知道的

> NOTE: 
>
> 隐藏在"what we know"之下有很多可能的情况是我们所不知的。上面这段就描述了各种我们所不知的；
>
> 上面这段话的字面意思: 但这只是我们所能保证的范围，也就是我们所知道的范围。

In both C and C++, the standards guarantee that `Foo` will print "5,10", so we know that that will happen. But that’s about the extent of what we’re guaranteed, hence of what we know.  (后面开始描述我们所不知的)We don’t know whether statements 1-3 will be executed at all, and in fact a good optimizer will get rid of them. If statements 1-3 are executed, we know that statement 1 will precede(领先) statements 2-4 and—assuming that the call to `printf` isn’t inlined and the result further optimized—we know that statement 4 will follow statements 1-3, but we know nothing about the relative ordering of statements 2 and 3. Compilers might choose to execute statement 2 first, statement 3 first, or even to execute them both in parallel, assuming the hardware has some way to do it. Which it might well have. Modern processors have a large word size and several execution units. Two or more arithmetic units are common. (For example, the Pentium 4 has three integer ALUs, PowerPC’s G4e has four, and Itanium has six.) Their machine language allows compilers to generate code that yields parallel execution of two or more instructions in a single clock cycle.

#### Optimizing compilers: reorder your code 

Optimizing compilers carefully analyze and reorder your code so as to execute as many things at once as possible (within the constraints on observable behavior). Discovering and exploiting such parallelism in regular serial code is the single most important reason for rearranging code and introducing **out-of order execution**. But it’s not the only reason. Compilers (and linkers) might also reorder instructions to avoid spilling data from a register, to keep the instruction pipeline full, to perform common subexpression elimination, and to reduce the size of the generated executable [4].

#### C++03 **abstract machines** are implicitly single threaded

When performing these kinds of optimizations, compilers and linkers for C and C++ are constrained only by the dictates(规定的) of observable behavior on the **abstract machines** defined by the language standards, and—this is the important bit—those **abstract machines** are implicitly single threaded. As languages, neither C nor C++ have threads, so compilers don’t have to worry about breaking threaded programs when they are optimizing. It should therefore not surprise you that they sometimes do.

#### Using system-specific libraries 

That being the case, how can one write C and C++ multithreaded programs that actually work? By using system-specific libraries defined for that purpose. Libraries like Posix threads (pthreads) [6] give precise specifications for the execution semantics of various synchronization primitives. These libraries impose restrictions on the code that **library-conformant compilers** are permitted to generate, thus forcing such compilers to emit code that respects the execution ordering constraints on which those libraries depend. That’s why threading packages have parts written in assembler or issue system calls that are themselves written in assembler (or in some unportable language): you have to go outside standard C and C++ to express the ordering constraints that multithreaded programs require. DCLP tries to get by using only language constructs. That’s why DCLP isn’t reliable.

> NOTE: 下面作者总结了各种programmer可能使用的奇技淫巧来实现组织compiler进行instruction reorder。

As a rule, programmers don’t like to be pushed around by their compilers. Perhaps you are such a programmer. If so, you may be tempted to try to outsmart your compiler by adjusting your source code so that `pInstance` remains unchanged until after Singleton’s construction is complete. For example, you
might try inserting use of a temporary variable:

#### Inserting use of a temporary variable

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

In essence, you’ve just fired the opening salvo(射击) in a war of optimization(本质上，你只是在一场优化战争中打响了第一枪). Your compiler wants to optimize. You don’t want it to, at least not here. But this is not a battle you want to get into. Your foe(敌人) is wiley(狡猾) and sophisticated(老练), imbued with strategems dreamed up over decades by people who do nothing but think about this kind of thing all day long, day after day, year after year(你的敌人诡计多端，老谋深算，满脑子都是那些什么都不做的人几十年来想出的策略，整天、日复一日、年复一年地想着这类事情). Unless you write optimizing compilers yourself, they are way ahead of you. In this case, for example, it would be a simple matter for the compiler to apply dependence analysis to determine that `temp` is an unnecessary variable, hence to eliminate it, thus treating your carefully crafted “unoptimizable” code if it had been written in the traditional DCLP manner. Game over. You lose.

> NOTE: 翻译如下: 
>
> "从本质上讲，您刚刚打响了一场优化之战的序幕。你的编译器想要优化。你不想看到的，至少在这里不是。但这不是你想要卷入的战争。你的敌人诡计多端，老谋深算，满脑子都是那些什么都不做的人几十年来想出的策略，整天、日复一日、年复一年地想着这类事情。除非您自己编写优化编译器，否则它们已经遥遥领先于您了。例如，在这种情况下，编译器可以很简单地应用依赖性分析来确定‘temp’是一个不必要的变量，从而消除它，从而处理你精心制作的“不可优化”的代码，如果它是用传统的DCLP方式编写的。游戏结束。你输了。"



If you reach for bigger ammo and try moving temp to a larger scope (say by making it file static), the compiler can still perform the same analysis and come to the same conclusion. Scope, schmope. Game over. You lose.

#### Declare temp extern 

So you call for backup. You declare `temp` extern and define it in a separate translation unit, thus preventing your compiler from seeing what you are doing. Alas for you, some compilers have the optimizing equivalent of night-vision goggles: they perform interprocedural analysis, discover your ruse with `temp`, and again they optimize it out of existence. Remember, these are optimizing compilers. They’re supposed to track down unnecessary code and eliminate it. Game over. You lose.

> NOTE: 翻译如下: 所以你请求支援。你声明' temp ' extern并在一个单独的翻译单元中定义它，从而防止编译器看到你在做什么。遗憾的是，有些编译器拥有与夜视镜类似的优化功能:它们执行程序间分析，用“temp”发现你的诡计，然后再次优化它。记住，这些都是优化编译器。他们应该追踪并消除不必要的代码。游戏结束。你输了。

#### Defining a helper function in a different file

So you try to disable inlining by defining a helper function in a different file, thus forcing the compiler to assume that the constructor might throw an exception and therefore delay the assignment to `pInstance`. Nice try, but some build environments perform link-time inlining followed by more code optimizations
[5, 11, 4]. GAME OVER. YOU LOSE.

#### 总结

Nothing you do can alter the fundamental problem: you need to be able to specify a constraint on instruction ordering, and your language gives you no way to do it.

> NOTE: C++11提供了控制instruction ordering的控制权

## TODO 5 Almost Famous: The `volatile` Keyword

> NOTE: 这一段并没有阅读完

The desire for specific instruction ordering makes many wonder whether the `volatile` keyword might be of help with multithreading in general and with DCLP in particular. In this section, we restrict our attention to the semantics of `volatile` in C++, and we further restrict our discussion to its impact on DCLP. For a broader discussion of `volatile`, see the accompanying sidebar.

Section 1.9 of the C++ standard [15] includes this information (emphasis ours):

> The observable behavior of the [C++] abstract machine is its sequence of reads and writes to volatile data and calls to library I/O functions. Accessing an object designated by a volatile lvalue, modifying an object, calling a library I/O function, or calling a function that does any of those operations are all side effects, which are changes in the state of the execution environment.

To appreciate how declaring `pInstance` alone volatile is insufficient, consider this:

```C++
class Singleton
{
public:
	static Singleton* instance();
//...
private:
	static Singleton *volatile pInstance; // volatile added
	int x;
	Singleton() :
					x(5)
	{
	}
};
// from the implementation file
Singleton *volatile Singleton::pInstance = 0;
Singleton* Singleton::instance()
{
	if (pInstance == 0)
	{
		Lock lock;
		if (pInstance == 0)
		{
			Singleton *volatile temp = new Singleton; // volatile added
			pInstance = temp;
		}
	}
	return pInstance;
}

```

After inlining the constructor, the code looks like this:

```C++
if (pInstance == 0)
{
	Lock lock;
	if (pInstance == 0)
	{
		Singleton* volatile temp =
		static_cast<Singleton*>(operator new(sizeof(Singleton)));
		temp->x = 5; // inlined Singleton constructor
		pInstance = temp;
	}
}

```

Unfortunately, this all does nothing to address the first problem: C++’s abstract machine is single-threaded, and C++ compilers may choose to generate thread-unsafe code from source like the above, anyway. Otherwise, lost optimization opportunities lead to too big an efficiency hit. After all the discussion, we’re back to square one. But wait, there’s more. More processors.

## 6 DCLP on Multiprocessor Machines

### ***cache coherency*** problem

Suppose you’re on a machine with multiple processors, each of which has its own **memory cache**, but all of which share a common memory space. Such an architecture needs to define exactly how and when writes performed by one processor propagate to the shared memory and thus become visible to other processors. It is easy to imagine situations where one processor has updated the value of a shared variable in its own cache, but the updated value has not yet been flushed to main memory, much less loaded into the other processors’ caches. Such **inter-cache inconsistencies** in the value of a shared variable is known as the ***cache coherency*** problem.

Suppose processor A modifies the memory for shared variable `x` and then later modifies the memory for shared variable `y`. These new values must be flushed to main memory so that other processors will see them. However, it can be more efficient to flush new cache values in **increasing address order**, so if `y`’s address precedes `x`’s, it is possible that `y`’s new value will be written to main memory before `x`’s is. If that happens, other processors may see `y`’s value change before `x`’s.

#### Out order of write->out of order execution

通过上面的描述可知: CPU flush cache to memory的次序可能是"in **increasing address order**": 先flush低地址然后flush高地址；显然CPU这样做的目的是处于performance的考虑；但是，这种order可能和thread实际执行write operation的order不同的，这就是本节标题中"out of order write"的含义；由于最终的执行效果要以commit到memory中的为基准(这个基准是非常重要的，因为shared data是放在memory的、其它core是read memory的)，因此，从整体来看，**看到的执行效果**(这个**看到的执行效果**是以memory为基准来进行度量的)是: thread的执行order是和program中的order不同的，也就是**out of order execution**；显然这种out-of-order，导致了我们精心设计的multithread program的失败: 另外一个thread看到的data与实际的order(程序的order、我们期望的order)不同，从而导致程序的失败；

下面是对上面描述的例子的推演过程:

1、如果`y`的地址precede(小于)`x`，则可能`y`的address被先于`x`被write到main memory中；

2、在thread A中，先改变的是`x`的值，然后改变的是`y`的值，而在thread B中，先看到的是更改后的`y`的值，然后看到的更改后的`x`的值，即**不同的thread看到了相反的变化**；那么如果thread B基于`y`的值来进行调节判断，则程序的行为是不可靠的；

> NOTE: 在下面"Use memory barriers "中，会介绍技术来对3中的问题进行限制

### ***cache coherency*** problem导致DCLP失败

Such a possibility is a serious problem for DCLP. Correct Singleton initialization requires that the `Singleton` be initialized and that `pInstance` be updated to be non-null and that these operations be seen to occur in this order. If a thread on processor A performs step 1 and then step 2, but a thread on processor B sees step 2 as having been performed before step 1, the thread on processor B may again refer to an uninitialized Singleton.

> NOTE: 
>
> 1、不同的thread看到了相反的变化
>
> 2、典型的Access outside of object lifetime，原因和"4 DCLP and Instruction Ordering"中分析的相同

### Use memory barriers 

> NOTE: 
>
> 1、使用memory barrier来控制CPU对shared variable的read、write的reordering

The general solution to **cache coherency problems** is to use memory barriers (i.e., fences): instructions recognized by compilers, linkers, and other optimizing entities that constrain the kinds of reorderings that may be performed on reads and writes of shared memory in multiprocessor systems. In the case of DCLP, we need to use **memory barriers** to ensure that `pInstance` isn’t seen to be non-null until writes to the `Singleton` have been completed. Here’s pseudocode that closely follows an example given in [1]. We show only placeholders for the statements that insert **memory barriers**, because the actual code is platform-specific (typically in assembler).

```C++
Singleton* Singleton::instance()
{
	Singleton *tmp = pInstance;
	//... // insert memory barrier
	if (tmp == 0)
	{
		Lock lock;
		tmp = pInstance;
		if (tmp == 0)
		{
			tmp = new Singleton;
			//... // insert memory barrier
			pInstance = tmp;
		}
	}
	return tmp;
}

```

> NOTE: 
>
> 1、上述第二个`//... // insert memory barrier`保证它的之前的write已经生效了，从而避免了前面的另外一个thread看到的相反的
>
> 2、第一个`//... // insert memory barrier`的目的是什么呢？

Arch Robison (author of [12], but this is from personal communication) points out that this is overkill:

> Technically, you don’t need full bidirectional barriers. The first barrier must prevent downwards migration of `Singleton`’s construction (by another thread); the second barrier must prevent upwards migration of `pInstance`’s initialization. These are called ”acquire” and ”release” operations, and may yield better performance than full barriers on hardware (such as Itainum) that makes the distinction.

> NOTE: 典型的acquire-release 

Still, this is an approach to implementing DCLP that should be reliable, provided you’re running on a machine that supports memory barriers. All machines that can reorder writes to shared memory support memory barriers in one form or another. Interestingly, this same approach works just as well in a uniprocessor setting. This is because memory barriers also act as hard sequence points that prevent the kinds of instruction reorderings that can be so troublesome.

## 7 Conclusions and DCLP Alternatives

There are several lessons to be learned here. First, remember that timeslice-based parallelism(基于时间片的并行) on a uniprocessor is not the same as true parallelism across multiple processors. That’s why a thread-safe solution for a particular compiler on a uniprocessor architecure may not be thread-safe on a multiprocessor architecture, not even if you stick with the same compiler. (This is a general observation. It’s not specific to DCLP.)

Second, though DCLP isn’t intrinsically(本质的) tied to Singleton, use of Singleton tends to lead to a desire to “optimize” thread-safe access via DCLP. You should therefore be sure to avoid implementing Singleton with DCLP. If you (or your clients) are concerned about the cost of locking a synchronization object every time instance is called, you can advise clients to minimize such calls by caching the pointer that instance returns. For example, suggest that instead of writing code like this,

```C++
Singleton::instance()->transmogrify();
Singleton::instance()->metamorphose();
Singleton::instance()->transmute();
```

clients do things this way:

```C++
Singleton* const instance = Singleton::instance(); // cache instance pointer
instance->transmogrify();
instance->metamorphose();
instance->transmute();
```


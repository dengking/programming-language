

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



[A](https://stackoverflow.com/a/6319356)

> NOTE: 非常好的一篇文章，它解释清楚了如下问题:
>
> 1、为什么引入memory model
>
> 2、C++11 memory model feature
>
> 3、使用一个非常具体的例子，说明清楚了: **sequential consistency**、memory ordering、mutex之间的差异



First, you have to learn to think like a Language Lawyer.

The C++ specification does not make reference to any particular compiler, operating system, or CPU. It makes reference to an *abstract machine* that is a generalization of actual systems. In the Language Lawyer world, the job of the programmer is to write code for the abstract machine; the job of the compiler is to actualize that code on a concrete machine. By coding rigidly to the spec, you can be certain that your code will compile and run without modification on any system with a compliant C++ compiler, whether today or 50 years from now.

> NOTE: 显然，programming language是"design to an abstraction"；
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

> NOTE: 通过上述例子，我们理解了sequential consistency;
>
> "So the default behavior of atomics provides both *atomicity* and *ordering* for loads and stores"，体现了C++ atomic library的强大。

Now, on a modern CPU, ensuring **sequential consistency** can be expensive. In particular, the compiler is likely to emit full-blown(成熟的) memory barriers between every access here. But if your algorithm can tolerate **out-of-order** loads and stores; i.e., if it requires atomicity but not ordering; i.e., if it can tolerate `37 0` as output from this program, then you can write this:

```cpp
           Global
           atomic<int> x, y;

Thread 1                            Thread 2
x.store(17,memory_order_relaxed);   cout << y.load(memory_order_relaxed) << " ";
y.store(37,memory_order_relaxed);   cout << x.load(memory_order_relaxed) << endl;
```

The more modern the CPU, the more likely this is to be faster than the previous example.

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
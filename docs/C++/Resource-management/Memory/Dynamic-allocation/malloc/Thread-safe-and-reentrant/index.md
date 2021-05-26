# Thread-safety and reentrance of `malloc`

## 结论

`malloc` 是thread-safe，但是not reentrant:

1、stackoverflow [Is malloc thread-safe?](https://stackoverflow.com/questions/855763/is-malloc-thread-safe) # [A](https://stackoverflow.com/a/856175)

2、stackoverflow [Why are malloc() and printf() said as non-reentrant?](https://stackoverflow.com/questions/3941271/why-are-malloc-and-printf-said-as-non-reentrant) # [A](https://stackoverflow.com/a/3941563)



## creference [malloc](https://en.cppreference.com/w/c/memory/malloc)

`malloc` is thread-safe: it behaves as though only accessing the memory locations visible through its argument, and not any static storage.

> NOTE: 
>
> 对thread safety进行了明确的规定



## stackoverflow [Why are malloc() and printf() said as non-reentrant?](https://stackoverflow.com/questions/3941271/why-are-malloc-and-printf-said-as-non-reentrant)

In UNIX systems we know `malloc()` is a non-reentrant function (system call). Why is that?

Similarly, `printf()` also is said to be non-reentrant; why?

I know the definition of re-entrancy, but I wanted to know why it applies to these functions. What prevents them being guaranteed reentrant?

### [A](https://stackoverflow.com/a/3941563)

> NOTE: 
>
> 这回答是非常好的

`malloc` and `printf` usually use **global structures**, and employ lock-based synchronization internally. That's why they're not reentrant.

The `malloc` function could either be thread-safe or thread-unsafe. Both are not reentrant:

> NOTE: 
>
> C11明确要求`malloc`需要保证thread safe

1、Malloc operates on a **global heap**, and it's possible that two different invocations of `malloc` that happen at the same time, return the same memory block. (The 2nd malloc call should happen before an address of the chunk is fetched, but the chunk is not marked as unavailable). This violates the postcondition of `malloc`, so this implementation would not be re-entrant.

> NOTE: 
>
> 这是典型的race

2、To prevent this effect, a thread-safe implementation of `malloc` would use lock-based synchronization. However, if malloc is called from signal handler, the following situation may happen:

```c
malloc();            //initial call
  lock(memory_lock); //acquire lock inside malloc implementation
signal_handler();    //interrupt and process signal
malloc();            //call malloc() inside signal handler
  lock(memory_lock); //try to acquire lock in malloc implementation
  // DEADLOCK!  We wait for release of memory_lock, but 
  // it won't be released because the original malloc call is interrupted
```

> NOTE: 
>
> 上述例子非常好

This situation won't happen when `malloc` is simply called from different threads. Indeed, the reentrancy concept goes beyond thread-safety and also requires functions to work properly **even if one of its invocation never terminates**. That's basically the reasoning why any function with locks would be not re-entrant.

The `printf` function also operated on global data. Any output stream usually employs a global buffer attached to the resource data are sent to (a buffer for terminal, or for a file). The print process is usually a sequence of copying data to buffer and flushing the buffer afterwards. This buffer should be protected by locks in the same way `malloc` does. Therefore, `printf` is also non-reentrant.

> NOTE: 
>
> 可能也是lock based

### [A](https://stackoverflow.com/a/26873638)

There are at least three concepts here, all of which are conflated in colloquial language, which might be why you were confused.

- *thread-safe*
- *critical section*
- *re-entrant*

#### [*thread-safe*](http://en.wikipedia.org/wiki/Thread_safety)

To take the easiest one first: **Both `malloc` and `printf` are [*thread-safe*](http://en.wikipedia.org/wiki/Thread_safety)**. They have been guaranteed to be thread-safe in Standard C since 2011, in POSIX since 2001, and in practice since long before that. What this means is that the following program is guaranteed not to crash or exhibit bad behavior:

```c
#include <pthread.h>
#include <stdio.h>

void *printme(void *msg) {
  while (1)
    printf("%s\r", (char*)msg);
}

int main() {
  pthread_t thr;
  pthread_create(&thr, NULL, printme, "hello");        
  pthread_create(&thr, NULL, printme, "goodbye");        
  pthread_join(thr, NULL);
}
```

An example of a function which is *not thread-safe* is `strtok`. If you call `strtok` from two different threads simultaneously, the result is undefined behavior — because `strtok` internally uses a static buffer to keep track of its state. glibc adds `strtok_r` to fix this problem, and C11 added the same thing (but optionally and under a different name, because Not Invented Here) as `strtok_s`.

Okay, but doesn't `printf` use global resources to build its output, too? In fact, what would it even *mean* to print to stdout from two threads *simultaneously?* That brings us to the next topic. Obviously **`printf` is going to be a [*critical section*](http://en.wikipedia.org/wiki/Critical_section) in any program that uses it.** Only one thread of execution is allowed to be inside the critical section at once.

At least in POSIX-compliant systems, this is achieved by having `printf` begin with a call to `flockfile(stdout)` and end with a call to `funlockfile(stdout)`, which is basically like taking a global mutex associated with stdout.

However, each distinct `FILE` in the program is allowed to have its own mutex. This means that one thread can call `fprintf(f1,...)` at the same time that a second thread is in the middle of a call to `fprintf(f2,...)`. There's no race condition here. (Whether your libc actually runs those two calls in parallel is a [QoI](https://www.google.com/search?q="quality+of+implementation"++c+programming+language) issue. I don't actually know what glibc does.)

Similarly, `malloc` is unlikely to be a critical section in any modern system, because modern systems are [smart enough to keep one pool of memory for each thread in the system](http://goog-perftools.sourceforge.net/doc/tcmalloc.html), rather than having all N threads fight over a single pool. (The `sbrk` system call will still probably be a critical section, but `malloc` spends very little of its time in `sbrk`. Or `mmap`, or whatever the cool kids are using these days.)

#### [*re-entrancy*](http://en.wikipedia.org/wiki/Reentrancy_(computing)) 

Okay, so **what does [*re-entrancy*](http://en.wikipedia.org/wiki/Reentrancy_(computing)) actually mean?** Basically, it means that the function can safely be called recursively — the current invocation is "put on hold" while a second invocation runs, and then the first invocation is still able to "pick up where it left off." (Technically this *might* not be due to a recursive call: the first invocation might be in Thread A, which gets interrupted in the middle by Thread B, which makes the second invocation. But that scenario is just a special case of *thread-safety*, so we can forget about it in this paragraph.)

Neither `printf` nor `malloc` can possibly *be* called recursively by a single thread, because they are leaf functions (they don't call themselves nor call out to any user-controlled code that could possibly make a recursive call). And, as we saw above, they've been thread-safe against *multi-*threaded re-entrant calls since 2001 (by using locks).

So, whoever told you that `printf` and `malloc` were non-reentrant was wrong; what they meant to say was probably that both of them have the potential to be *critical sections* in your program — bottlenecks where only one thread can get through at a time.

------

Pedantic note: glibc does provide an extension by which `printf` can be made to call arbitrary user code, including re-calling itself. This is perfectly safe in all its permutations — at least as far as thread-safety is concerned. (Obviously it opens the door to absolutely *insane* format-string vulnerabilities.) There are two variants: `register_printf_function` (which is documented and reasonably sane, but officially "deprecated") and `register_printf_specifier` (which is *almost* identical except for one extra undocumented parameter and a [total lack of user-facing documentation](https://www.sourceware.org/glibc/wiki/PrintfHooksDesign)). I wouldn't recommend either of them, and mention them here merely as an interesting aside.

```c
#include <stdio.h>
#include <printf.h>  // glibc extension

int widget(FILE *fp, const struct printf_info *info, const void *const *args) {
  static int count = 5;
  int w = *((const int *) args[0]);
  printf("boo!");  // direct recursive call
  return fprintf(fp, --count ? "<%W>" : "<%d>", w);  // indirect recursive call
}
int widget_arginfo(const struct printf_info *info, size_t n, int *argtypes) {
  argtypes[0] = PA_INT;
  return 1;
}
int main() {
  register_printf_function('W', widget, widget_arginfo);
  printf("|%W|\n", 42);
}
```



## stackoverflow [Is malloc thread-safe?](https://stackoverflow.com/questions/855763/is-malloc-thread-safe)

### [A](https://stackoverflow.com/a/856175)

Question: "is malloc reentrant"?
Answer: no, it is not. Here is one definition of what makes a routine [reentrant](http://en.wikipedia.org/wiki/Reentrant_(subroutine)).

None of the common versions of malloc allow you to re-enter it (e.g. from a signal handler). Note that a reentrant routine may not use locks, and almost all malloc versions in existence do use locks (which makes them thread-safe), or global/static variables (which makes them thread-unsafe *and* non-reentrant).

All the answers so far answer "is malloc thread-safe?", which is an entirely different question. To *that* question the answer is *it depends* on your runtime library, and possibly on the compiler flags you use. On any modern UNIX, you'll get a thread-safe malloc by default. On Windows, use `/MT`, `/MTd`, `/MD` or `/MDd` flags to get thread-safe runtime library.

### [A](https://stackoverflow.com/a/855780)

I read somewhere that if you compile with -pthread, malloc becomes thread safe. I´m pretty sure its implementation dependant though, since malloc is ANSI C and threads are not.

If we are talking gcc:

> Compile and link with -pthread and malloc() will be thread-safe, on x86 and AMD64.

http://groups.google.com/group/comp.lang.c.moderated/browse_thread/thread/2431a99b9bdcef11/ea800579e40f7fa4

Another opinion, more insightful

> {malloc, calloc, realloc, free, posix_memalign} of glibc-2.2+ are thread safe

http://linux.derkeiler.com/Newsgroups/comp.os.linux.development.apps/2005-07/0323.html



## TODO

stackoverflow [malloc() is non-reentrant but thread-safe? [duplicate]](https://stackoverflow.com/questions/8869622/malloc-is-non-reentrant-but-thread-safe)
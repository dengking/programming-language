# 5.3.3 Memory ordering for atomic operations

There are six memory ordering options that can be applied to operations on atomic types: 

1、`memory_order_relaxed`

2、`memory_order_consume`

3、`memory_order_acquire`

4、`memory_order_release`

5、`memory_order_acq_rel`

6、`memory_order_seq_cst`

Unless you specify otherwise for a particular operation, the memory-ordering option for all operations on atomic types is `memory_order_seq_cst`, which is the most stringent(严格的、严厉的) of the available options. 

Although there are six ordering options, they represent three models: 

1、**sequentially consistent ordering** (`memory_order_seq_cst`)

2、**acquire-release ordering** (`memory_order_consume`, `memory_order_acquire`, `memory_order_release`, and `memory_order_acq_rel`), 

3、relaxed ordering (`memory_order_relaxed`).

## Cost of memory model

> NOTE:
>
> 一、
>
> stringent: **sequentially consistent ordering** > **acquire-release ordering** > **relaxed ordering** 
>
> cost: **sequentially consistent ordering** < **acquire-release ordering** < **relaxed ordering** 
>
> 二、在下面章节中也对此进行了讨论:
>
> 1、SEQUENTIALLY CONSISTENT ORDERING
>
> 2、本节末尾

These distinct memory-ordering models can have varying costs on different CPU architectures. For example, on systems based on architectures with fine control over the visibility of operations by processors other than the one that made the change, additional **synchronization instructions** can be required for **sequentially consistent ordering** over **acquire-release ordering** or **relaxed ordering** and for **acquire-release ordering** over **relaxed ordering**. If these systems have many processors, these additional synchronization instructions may take a significant amount of time, thus reducing the overall performance of the system. On the other hand, CPUs that use the x86 or x86-64 architectures (such as the Intel and AMD processors common in desktop PCs) don’t require any additional instructions for **acquire-release ordering** beyond those necessary for ensuring atomicity, and even **sequentially-consistent ordering** doesn’t require any special treatment for load operations, although there’s a small additional cost on stores.

The availability of the distinct memory-ordering models allows experts to take advantage of the increased performance of the more fine-grained ordering relationships where they’re advantageous while(同时) allowing the use of the default **sequentially consistent ordering** (which is considerably easier to reason about than the others) for those cases that are less critical.



## SEQUENTIALLY CONSISTENT ORDERING

The default ordering is named **sequentially consistent** because it implies that the behavior of the program is consistent with a simple sequential view of the world. If all operations on instances of atomic types are **sequentially consistent**, the behavior of a multithreaded program is as if all these operations were performed in some particular sequence by **a single thread**. This is by far the easiest **memory ordering** to understand, which is why it’s the default: ***all threads must see the same order of operations***. This makes it easy to reason about the behavior of code written with **atomic variables**. You can write down all the possible sequences of operations by different threads, eliminate those that are **inconsistent**, and verify that your code behaves as expected in the others. It also means that operations can’t be reordered; if your code has one operation before another in one thread, that ordering must be seen by all other threads.

> NOTE:
>
> 一、上面这段话的内容需要和 "NON-SEQUENTIALLY CONSISTENT MEMORY ORDERINGS"  章节的内容对比着来进行阅读
>
> 二、"it implies that the behavior of the program is consistent with a simple sequential view of the world"
>
> 读完这段话我的第一想法是  **sequentially consistent** 是最最符合我们的直觉: "a simple sequential view of the world"
>
> 三、"If all operations on instances of atomic types are **sequentially consistent**, the behavior of a multithreaded program is as if all these operations were performed in some particular sequence by a single thread."
>
> 这个观点是站在整体的角度来进行分析，理解本节给出的例子也需要这个这个思考角度。
>
> 四、"It also means that operations can’t be reordered; if your code has one operation before another in one thread, that ordering must be seen by all other threads."
>
> 由于要求" ***all threads must see the same order of operations*** "，因此一旦发生了reorder，那么就可能导致违背这个前提条件
>
> 上述对reorder的解释是非常容易理解的。



From the point of view of **synchronization**, a sequentially consistent store **synchronizes-with** a sequentially consistent load of the same variable that reads the value stored. This provides one ordering constraint on the operation of two (or more) threads, but **sequential consistency** is more powerful than that. Any sequentially consistent atomic operations done after that load must also appear after the store to other threads in the system using **sequentially consistent** atomic operations. The example in listing 5.4 demonstrates this ordering constraint in action. This constraint doesn’t carry forward to threads that use atomic operations with **relaxed memory orderings**; they can still see the operations in a different order, so you must use **sequentially consistent** operations on all your threads in order to get the benefit.

> NOTE:
>
> 一、"From the point of view of **synchronization**, a sequentially consistent store **synchronizes-with** a sequentially consistent load of the same variable that reads the value stored."
>
> 这段话的前提是: 先store然后load，中文翻译如下:
>
> "从synchronization的观点来看，a sequentially consistent store 与一个读取它的值的 sequentially consistent load 之间存在 **synchronizes-with** 关系"
>
> 二、"Any sequentially consistent atomic operations done after that load must also appear after the store to other threads in the system using **sequentially consistent** atomic operations. "
>
> 这段话的意思其实就是 "***all threads must see the same order of operations***"

This ease of understanding can come at a price, though. On a **weakly ordered machine** with many processors, it can impose a noticeable performance penalty, because the overall sequence of operations must be kept consistent between the processors, possibly requiring extensive (and expensive!) synchronization operations between the processors. That said, some processor architectures (such as the common x86 and x86-64 architectures) offer **sequential consistency** relatively cheaply, so if you’re concerned about the performance implications of using **sequentially consistent ordering**, check the documentation for your target processor architectures.	

> NOTE:
>
> 一、"the overall sequence of operations must be kept consistent between the processors, possibly requiring extensive (and expensive!) synchronization operations between the processors."
>
> 在multiprocessor中，为了维持 **sequentially consistent ordering** 是需要付出代价的，在本节的最后也对这个topic进行了讨论。

The following listing shows **sequential consistency** in action. The loads and stores to `x` and `y` are explicitly tagged with `memory_order_seq_cst`, although this tag could be omitted in this case because it’s the default.

### Listing 5.4 Sequential consistency implies a total ordering

```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst)) // x 为 true，退出循环
        ;
    if (y.load(std::memory_order_seq_cst)) 
        ++z; // y为true则执行
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst)) // y 为 true，退出循环
        ;
    if (x.load(std::memory_order_seq_cst))
        ++z; // x为true则执行
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0);
}

```

The `assert` can never fire, because either the store to `x` or the store to `y` must happen first, even though it’s not specified which. If the load of `y` in `read_x_then_y` returns `false`, the store to `x` must occur before the store to `y`, in which case the load of `x` in `read_y_then_x` must return `true`, because the while loop ensures that the `y` is `true` at this point. Because the semantics of `memory_order_seq_cst` require a **single total ordering** over all operations tagged `memory_order_seq_cst`, there’s an implied ordering relationship between a load of `y` that returns `false` and the store to `y` . For there to be a **single total order**, if one thread sees `x==true` and then subsequently sees `y==false`, this implies that the store to `x` occurs before the store to `y` in this **total order**.

Of course, because everything is symmetrical, it could also happen the other way around, with the load of `x` returning false, forcing the load of `y` to return true. In both cases, `z` is equal to 1. Both loads can return true, leading to `z` being 2, but under no circumstances can `z` be zero.

> NOTE:
>
> 一、可以肯定: 上述四个thread执行顺序是未指定的
>
> 二、"The `assert` can never fire, because either the store to `x` or the store to `y` must happen first, even though it’s not specified which" 的理解如下:
>
> 因为在 `read_x_then_y()`、`read_y_then_x()` 都会首先执行一个while loop，显然如果要退出while loop就需要 "either the store to `x` or the store to `y` must happen first"，这样它们的值才能够为true。需要注意的是:  无法确定 `write_x()`、`write_y()` 执行的先后顺序是没有指定的。
>
> 
>
> `read_x_then_y()` 和 `read_y_then_x()` 是相反的，其实这就是第二段中的"symmetrical"的含义。



The operations and **happens-before** relationships for the case that `read_x_then_y` sees `x` as true and `y` as `false` are shown in figure 5.3. The dashed line from the load of `y` in `read_x_then_y` to the store to `y` in `write_y` shows the implied ordering relationship required in order to maintain **sequential consistency**: the load must occur before the store in the global order of `memory_order_seq_cst` operations in order to achieve the outcomes given here.



![](./Figure-5.3-Sequential-consistency-and-happens-before.jpg)

### 承上启下

**Sequential consistency** is the most straightforward and intuitive ordering, but it’s also the most expensive memory ordering because it requires **global synchronization** between all threads. On a multiprocessor system this may require quite extensive and time-consuming communication between processors.

In order to avoid this synchronization cost, you need to step outside the world of **sequential consistency** and consider using other memory orderings.



## NON-SEQUENTIALLY CONSISTENT MEMORY ORDERINGS

Once you step outside the nice **sequentially consistent** world, things start to get complicated. Probably the single biggest issue to come to grips with is the fact that ***there’s no longer a single global order of events***. This means that different threads can see different views of the same operations, and any mental model you have of operations from different threads neatly(仅仅) interleaved one after the other must be thrown away. Not only do you have to account for(考虑) things happening truly concurrently, but threads don’t have to agree on the order of events. In order to write (or even just to understand) any code that uses a **memory ordering** other than the default `memory_order_seq_cst`, it’s absolutely vital to get your head around this. It’s not just that the compiler can reorder the instructions. Even if the threads are running the same bit of code, they can disagree on the order of events because of operations in other threads in the absence of **explicit ordering constraints**, because the different **CPU caches** and **internal buffers** can hold different values for the same memory. It’s so important I’ll say it again: ***threads don’t have to agree on the order of events***.

Not only do you have to throw out mental models based on interleaving operations, you also have to throw out mental models based on the idea of the compiler or processor reordering the instructions. ***In the absence of other ordering constraints, the only requirement is that all threads agree on the modification order of each individual variable***. Operations on distinct variables can appear in different orders on different threads, provided the values seen are consistent with any additional ordering constraints imposed.

> NOTE:
>
> 一、上面总结的非常好

This is best demonstrated by stepping completely outside the **sequentially consistent** world and using `memory_order_relaxed` for all operations. Once you’ve come to grips with that, you can move back to **acquire-release ordering**, which allows you to selectively introduce ordering relationships between operations and claw back some of your sanity.

> NOTE:
>
> 一、" Once you’ve come to grips with that, you can move back to **acquire-release ordering**, which allows you to selectively introduce ordering relationships between operations and claw back some of your sanity."
>
> 上面这段话的翻译如下:
>
> "一旦你掌握了，就可以回过头来看 **acquire-release ordering**，它让你选择性地在operations之间引入ordering relationship 并且 夺回一些理性"

## RELAXED ORDERING

Operations on atomic types performed with **relaxed ordering** don’t participate in **synchronizes-with** relationships. Operations on the same variable within a single thread still obey **happens-before** relationships, but there’s almost no requirement on ordering relative to other threads. 

The only requirement is that accesses to a single atomic variable from the same thread can’t be reordered; once a given thread has seen a particular value of an atomic variable, a subsequent read by that thread can’t retrieve an earlier value of the variable. 

> NOTE:
>
> 一、上面总结了一个requirement

Without any additional synchronization, the **modification order** of each variable is the only thing shared between threads that are using `memory_order_relaxed`.

To demonstrate just how relaxed your relaxed operations can be, you need only two threads, as shown in the following listing.

### Listing 5.5 Relaxed operations have very few ordering requirements

```c++
#include <atomic>
#include <thread>
#include <assert.h>
std::atomic<bool> x, y;
std::atomic<int> z;
void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}
void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed))
        ;
    if (x.load(std::memory_order_relaxed))
        ++z;
}
int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);
}
```

This time the `assert` can fire, because the load of `x` can read `false`, even though the load of `y` reads `true` and the store of `x` **happens-before** the store of `y`. `x` and `y` are different variables, so there are no ordering guarantees relating to the visibility of values arising from operations on each.

Relaxed operations on different variables can be freely reordered provided they obey any **happens-before** relationships they’re bound by (for example, within the same thread). They don’t introduce **synchronizes-with** relationships. The **happens-before** relationships from listing 5.5 are shown in figure 5.4, along with a possible outcome. Even though there’s a **happens-before** relationship between the stores and between the loads, there isn’t one between either store and either load, and so the loads can see the stores out of order.

![](./Figure-5.4-Relaxed-atomics-and-happens-before.jpg)



### Listing 5.6 Relaxed operations on multiple threads

Let’s look at the slightly more complex example with three variables and five threads in the next listing.



```c++
#include <thread>
#include <atomic>
#include <iostream>

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> go(false);
unsigned const loop_count = 10;

struct read_values
{
    int x, y, z;
};

read_values values1[loop_count];
read_values values2[loop_count];
read_values values3[loop_count];
read_values values4[loop_count];
read_values values5[loop_count];

void increment(std::atomic<int> *var_to_inc, read_values *values)
{
    while (!go)
        std::this_thread::yield();
    for (unsigned i = 0; i < loop_count; ++i)
    {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        var_to_inc->store(i + 1, std::memory_order_relaxed);
        std::this_thread::yield();
    }
}
void read_vals(read_values *values)
{
    while (!go)
        std::this_thread::yield();
    for (unsigned i = 0; i < loop_count; ++i)
    {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        std::this_thread::yield();
    }
}
void print(read_values *v)
{
    for (unsigned i = 0; i < loop_count; ++i)
    {
        if (i)
            std::cout << ",";
        std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << ")";
    }
    std::cout << std::endl;
}
int main()
{
    std::thread t1(increment, &x, values1);
    std::thread t2(increment, &y, values2);
    std::thread t3(increment, &z, values3);
    std::thread t4(read_vals, values4);
    std::thread t5(read_vals, values5);
    go = true;
    t5.join();
    t4.join();
    t3.join();
    t2.join();
    t1.join();
    print(values1);
    print(values2);
    print(values3);
    print(values4);
    print(values5);
}
```

This is a really simple program in essence. You have three **shared global atomic variables** and five threads. Each thread loops 10 times, reading the values of the three atomic variables using `memory_order_relaxed` and storing them in an array. Three of the threads each update one of the **atomic variables** each time through the loop, while the other two threads just read. Once all the threads have been joined, you print the values from the arrays stored by each thread.

The atomic variable `go` is used to ensure that the threads all start the loop as near to the same time as possible. Launching a thread is an expensive operation, and without the explicit delay, the first thread may be finished before the last one has started. Each thread waits for `go` to become true before entering the main loop, and `go` is set to true only once all the threads have started.

One possible output from this program is as follows:

```c++
(0,0,0),(1,0,0),(2,0,0),(3,0,0),(4,0,0),(5,7,0),(6,7,8),(7,9,8),(8,9,8),(9,9,10)
(0,0,0),(0,1,0),(0,2,0),(1,3,5),(8,4,5),(8,5,5),(8,6,6),(8,7,9),(10,8,9),(10,9,10)
(0,0,0),(0,0,1),(0,0,2),(0,0,3),(0,0,4),(0,0,5),(0,0,6),(0,0,7),(0,0,8),(0,0,9)
(1,3,0),(2,3,0),(2,4,1),(3,6,4),(3,9,5),(5,10,6),(5,10,8),(5,10,10),(9,10,10),(10,10,10)
(0,0,0),(0,0,0),(0,0,0),(6,3,7),(6,5,7),(7,7,7),(7,8,7),(8,8,7),(8,8,9),(8,8,9)
```

The first three lines are the threads doing the updating, and the last two are the threads doing just reading. Each triplet is a set of the variables `x`, `y` and `z` in that order from one pass through the loop. There are a few things to notice from this output:

1、The first set of values shows `x` increasing by one with each triplet, the second set has `y` increasing by one, and the third has `z` increasing by one.

2、The `x` elements of each triplet only increase within a given set, as do the `y` and `z` elements, but the increments are uneven, and the relative orderings vary between all threads.

3、Thread 3 doesn’t see any of the updates to `x` or `y`; it sees only the updates it makes to `z`. This doesn’t stop the other threads from seeing the updates to `z` mixed in with the updates to `x` and `y` though.

This is a valid outcome for relaxed operations, but it’s not the only valid outcome. Any set of values that’s consistent with the three variables each holding the values 0 to 10 in turn and that has the thread incrementing a given variable printing the values 0 to 9 for that variable is valid.

### UNDERSTANDING RELAXED ORDERING

> NOTE:
>
> 一、将thread load、store atomic variable比作两个人之间通话

To understand how this works, imagine that each variable is a man in a cubicle(小卧室) with a notepad(笔记本). On his notepad is a list of values. You can phone him and ask him to give you a value, or you can tell him to write down a new value. If you tell him to write down a new value, he writes it at the bottom of the list. If you ask him for a value, he reads you a number from the list.

The first time you talk to this man, if you ask him for a value, he may give you any value from the list he has on his pad at the time. If you then ask him for another value, he may give you the same one again or a value from farther down the list. He’ll never give you a value from farther up the list. If you tell him to write down a number and then subsequently ask him for a value, he’ll give you either the number you told him to write down or a number below that on the list.

> NOTE:
>
> 一、list of value记录了atomic variable的变化过程，其实就是前面说的modification order，新值会append to tail of the list，因此，head是最旧的，tail是最新的。
>
> 二、在 `5, 10, 23, 3, 1, 2` 中，5是head，2是tail。
>
> 三、在进行load的时候，不会load跟旧的值，因此"If you tell him to write down 42, he’ll add it to the end of the list. If you ask him for a number again, he’ll keep telling you “42” until he has another number on his list and he feels like telling it to you"。这就是"It’s like he keeps track of which number he told to whom with a little movable sticky note for each person, like in figure 5.5"

Imagine for a moment that his list starts with the values 5, 10, 23, 3, 1, 2. If you ask for a value, you could get any of those. If he gives you 10, then the next time you ask he could give you 10 again, or any of the later ones, but not 5. If you call him five times, he could say “10, 10, 1, 2, 2,” for example. If you tell him to write down 42, he’ll add it to the end of the list. If you ask him for a number again, he’ll keep telling you “42” until he has another number on his list and he feels like telling it to you.

Now, imagine your friend Carl also has this man’s number. Carl can also phone him and either ask him to write down a number or ask for one, and he applies the same rules to Carl as he does to you. He has only one phone, so he can only deal with one of you at a time, so the list on his pad is a nice straightforward list. However, just because you got him to write down a new number doesn’t mean he has to tell it to Carl, and vice versa. If Carl asked him for a number and was told “23,” then just because you asked the man to write down 42 doesn’t mean he’ll tell that to Carl next time. He may tell Carl any of the numbers 23, 3, 1, 2, 42, or even the 67 that Fred told him to write down after you called. He could very well tell Carl “23, 3, 3, 1, 67” without
being inconsistent with what he told you. It’s like he keeps track of which number he told to whom with a little movable sticky note for each person, like in figure 5.5.

> NOTE:
>
> 一、"movable sticky note"的意思是"移动便签"

![](./Figure-5.5-The-notebook-for-the-man-in-the-cubicle.jpg)

Now imagine that there’s not just one man in a cubicle but a whole cubicle farm, with loads of men with phones and notepads. These are all our **atomic variables**. Each variable has its own **modification order** (the list of values on the pad), but there’s no relationship between them at all. If each caller (you, Carl, Anne, Dave, and Fred) is a thread, then this is what you get when every operation uses `memory_order_relaxed`. There are a few additional things you can tell the man in the cubicle, such as:

1、 “write down this number, and tell me what was at the bottom of the list” (`exchange`) 

2、“write down this number if the number on the bottom of the list is that; otherwise tell me what I should have guessed” (`compare_exchange_strong`), but that doesn’t affect the general principle.

If you think about the program logic from listing 5.5, then `write_x_then_y` is like some guy calling up the man in cubicle `x` and telling him to write `true` and then calling up the man in cubicle `y` and telling him to write true. The thread running `read_y_then_x` repeatedly calls up the man in cubicle `y` asking for a value until he says `true` and then calls the man in cubicle `x` to ask for a value. The man in cubicle `x` is under no obligation to tell you any specific value off his list and is quite within his rights to say `false`.

> NOTE:
>
> 一、" The man in cubicle `x` is under no obligation to tell you any specific value off his list and is quite within his rights to say `false`." 的意思是:
>
> 在这个小隔间x中的人没有义务告诉你他列表上的任何一个具体的值，并且还有权利说`false`。

This makes relaxed atomic operations difficult to deal with. They must be used in combination with atomic operations that feature stronger ordering semantics in order to be useful for **inter-thread synchronization**. I strongly recommend avoiding relaxed atomic operations unless they’re absolutely necessary and even then using them only with extreme caution. Given the unintuitive results that can be achieved with just two threads and two variables in listing 5.5, it’s not hard to imagine the possible complexity when more threads and more variables are involved. 

> NOTE:
>
> 一、"Given the unintuitive results that can be achieved with just two threads and two variables in listing 5.5, it’s not hard to imagine the possible complexity when more threads and more variables are involved. " 的意思是:
>
> 在 listing 5.5 中，仅仅是两个线程和两个变量就能够让所得到的结果这样不直观，鉴于此，不难想象涉及到更多thread和更多variable的时候会变得多么复杂。

### 承上启下

One way to achieve additional synchronization without the overhead of full-blown sequential consistency is to use acquire-release ordering.

## ACQUIRE-RELEASE ORDERING

**Acquire-release ordering** is a step up from **relaxed ordering**; there’s still no **total order** of operations, but it does introduce some **synchronization**. Under this ordering model, **atomic loads** are **acquire operations** (`memory_order_acquire`), **atomic stores** are **release operations** (`memory_order_release`), and atomic read-modify-write operations (such as `fetch_add()` or `exchange()`) are either `acquire`, `release`, or both (`memory_order_acq_rel`). Synchronization is **pairwise**, between the thread that does the **release** and the thread that does the **acquire**. ***A release operation synchronizes-with an acquire operation that reads the value written***. This means that different threads can still see different orderings, but these orderings are restricted. The following listing is a rework of listing 5.4 using **acquire-release semantics** rather than **sequentially consistent** ones.

> NOTE:
>
> 一、"**Acquire-release ordering** is a step up from **relaxed ordering**;"的意思是:
>
> **Acquire-release ordering** 是 **relaxed ordering** 的进步。

### Listing 5.7 Acquire-release doesn’t imply a total ordering

```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
    x.store(true, std::memory_order_release);
}

void write_y()
{
    y.store(true, std::memory_order_release);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire))
        ;
    if (y.load(std::memory_order_acquire))
        ++z;
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire))
        ;
    if (x.load(std::memory_order_acquire))
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0);
}
```

In this case the `assert` can fire (just like in the **relaxed-ordering** case), because it’s possible for both the load of `x` and the load of `y` to read `false`. `x` and `y` are written by different threads, so the ordering from the **release** to the **acquire** in each case has no effect on the operations in the other threads.

Figure 5.6 shows the **happens-before** relationships from listing 5.7, along with a possible outcome where the two reading threads each have a different view of the world. This is possible because there’s no **happens-before** relationship to force an ordering, as described previously.

![](./Figure-5.6-Acquire-release-and-happens-before.jpg)

> NOTE:
>
> 一、需要注意: 由于在 `read_x_then_y` 中 `y.load` 没有使用while loop（`x.load` 使用了while loop），因此它和`write_y()` 其实没有"synchronizes-with"关系，即使两者使用了acquire-release 
>
> 二、stackoverflow [Acquire/release semantics with 4 threads](https://stackoverflow.com/questions/48383867/acquire-release-semantics-with-4-threads)
>
> Shouldn't the store to `y` also sync with the load in `read_x_then_y`, and the store to `x` sync with the load in `read_y_then_x`? I'm very confused.
>
> EDIT:
>
> Thank you for your responses, I understand how atomics work and how to use Acquire/Release. I just don't get this specific example. I was trying to figure out IF the assertion fires, then what did each thread do? And why does the assertion never fire if we use sequential consistency.
>
> 三、stackoverflow [Will two atomic writes to different locations in different threads always be seen in the same order by other threads?](https://stackoverflow.com/questions/27807118/will-two-atomic-writes-to-different-locations-in-different-threads-always-be-see)
>
> ```C++
> -- Initially --
> std::atomic<int> x{0};
> std::atomic<int> y{0};
> 
> -- Thread 1 --
> x.store(1, std::memory_order_release);
> 
> -- Thread 2 --
> y.store(2, std::memory_order_release);
> 
> -- Thread 3 --
> int r1 = x.load(std::memory_order_acquire);   // x first
> int r2 = y.load(std::memory_order_acquire);
> 
> -- Thread 4 --
> int r3 = y.load(std::memory_order_acquire);   // y first
> int r4 = x.load(std::memory_order_acquire);
> ```
>
> 
>
> [A](https://stackoverflow.com/a/50679223/10173843)
>
> This kind of reordering test is called **IRIW (Independent Readers, Independent Writers)**, where we're checking if two readers can see the same pair of stores appear in different orders. Related, maybe a duplicate: [Acquire/release semantics with 4 threads](https://stackoverflow.com/questions/48383867/acquire-release-semantics-with-4-threads)

### Listing 5.8 Acquire-release operations can impose ordering on relaxed operations

In order to see the benefit of acquire-release ordering, you need to consider two stores from the same thread, like in listing 5.5. If you change the store to `y` to use `memory_order_release` and the load from `y` to use `memory_order_acquire` like in the following listing, then you actually impose an ordering on the operations on `x`.



```c++
#include <atomic>
#include <thread>
#include <assert.h>
std::atomic<bool> x, y;
std::atomic<int> z;
void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_release);
}
void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire))
        ;
    if (x.load(std::memory_order_relaxed))
        ++z;
}
int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);
}
```



Eventually, the load from `y` 3 will see `true` as written by the store 2. Because the store uses `memory_order_release` and the load uses `memory_order_acquire`, the store **synchronizes-with** the load. The store to `x` 1 **happens-before** the store to `y` 2, because they’re in the same thread. Because the store to `y` **synchronizes-with** the load from `y`, the store to `x` also **happens-before** the load from `y` and by extension **happens-before**
the load from `x` 5. Thus the load from `x` must read true, and the `assert` 6 can’t fire. If the load from `y` wasn’t in a while loop, this wouldn’t necessarily be the case; the load from `y` might read `false`, in which case there’d be no requirement on the value read from `x`. In order to provide any synchronization, acquire and release operations must be paired up. The value stored by a release operation must be seen by an acquire operation for either to have any effect. If either the store at 2 or the load at 3 was a relaxed operation, there’d be no ordering on the accesses to `x`, so there’d be no guarantee that the load at 4 would read `true`, and the `assert` could fire.

> NOTE:
>
> 一、"The value stored by a release operation must be seen by an acquire operation for either to have any effect. "
>
> 由release operation 存储的值必须被acquire operation 看到，以便两者中的任意一个生效。

### 模拟

You can still think about **acquire-release ordering** in terms of our men with notepads in their cubicles, but you have to add more to the model. First, imagine that every store that’s done is part of some **batch** of updates, so when you call a man to tell him to write down a number, you also tell him which **batch** this update is part of: “Please write down 99, as part of batch 423.” For the **last store** in a batch, you tell this to the man too: “Please write down 147, which is the **last store** in batch 423.” The man in the cubicle will then duly(及时的) write down this information, along with who gave him the value. This models a **store-release operation**. The next time you tell someone to write down a value, you increase the batch number: “Please write down 41, as part of batch 424.”

> NOTE:
>
> 一、batch
>
> 1、batch number是单调递增的
>
> 2、对于last store，需要特别地说明，这样下次的时候，会重新新建一个batch      
>
> 3、放到同一个batch的目的是: 维护happens-before关系                                                                                                                                                                                                                                                                                                              

When you ask for a value, you now have a choice: you can either just ask for a value (which is a **relaxed load**), in which case the man just gives you the number, or you can ask for a value and information about whether it’s the last in a **batch** (which models a **load-acquire**). If you ask for the **batch information**, and the value wasn’t the **last** in a batch, the man will tell you something like, “The number is 987, which is just a ‘normal’ value,” whereas if it was the **last** in a batch, he’ll tell you something like “The number is 987, which is the **last** number in batch 956 from Anne.” Now, here’s where the **acquire-release** semantics kick in(开始生效): if you tell the man all the batches you know about when you ask for a value, he’ll look down his list for the last value from any of the batches you know about and either give you that number or one further down the list.

How does this model **acquire-release semantics**? Let’s look at our example and see. First off, thread `a` is running `write_x_then_y` and says to the man in cubicle `x`, “Please write `true` as part of batch 1 from thread `a`,” which he duly(及时的) writes down. Thread `a` then says to the man in cubicle y, “Please write true as the last write of batch 1 from thread `a`,” which he duly(及时的) writes down. In the meantime, thread `b` is running `read_y_then_x`. Thread `b` keeps asking the man in box `y` for a value with batch information until he says “true.” He may have to ask many times, but eventually the man will say “true.” The man in box `y` doesn’t just say “true” though; he also says, “This is the last write in batch 1 from thread a.”

> NOTE:
>
> 一、thread a 对 `x`、`y` 的store都放在同一batch中

Now, thread b goes on to ask the man in box x for a value, but this time he says, “Please can I have a value, and by the way I know about batch 1 from thread a.” So now, the man from cubicle x has to look down his list for the last mention of batch 1 from thread a. The only mention he has is the value true, which is also the last value on his list, so he must read out that value; otherwise, he’s breaking the rules of the game.

### 承上启下

If you look at the definition of **inter-thread happens-before** back in section 5.3.2, one of the important properties is that it’s transitive: if A inter-thread happens-before B and B inter-thread happens-before C, then A inter-thread happens-before C. This means that **acquire release ordering** can be used to synchronize data across several threads, even when the “intermediate” threads haven’t actually touched the data.

## TRANSITIVE SYNCHRONIZATION WITH ACQUIRE-RELEASE ORDERING

In order to think about transitive ordering, you need at least three threads. The first thread modifies some shared variables and does a store-release to one of them. A second thread then reads the variable subject to the store-release with a load-acquire and performs a store-release on a second shared variable. Finally, a third thread does a load-acquire on that second shared variable. Provided that the load-acquire operations see the values written by the store-release operations to ensure the synchronizes-with relationships, this third thread can read the values of the other variables stored by the first thread, even if the intermediate thread didn’t touch any of them. This scenario is shown in the next listing.

### Listing 5.9 Transitive synchronization using acquire and release ordering



```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);

void thread_1()
{
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141, std::memory_order_relaxed);
    data[4].store(2003, std::memory_order_relaxed);
    sync1.store(true, std::memory_order_release);
}

void thread_2()
{
    while (!sync1.load(std::memory_order_acquire))
        ;
    sync2.store(true, std::memory_order_release);
}

void thread_3()
{
    while (!sync2.load(std::memory_order_acquire))
        ;
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == -141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
}

```

Even though `thread_2` only touches the variables `sync1` c and `sync2` d, this is enough for synchronization between `thread_1` and `thread_3` to ensure that the asserts don’t fire. First off, the stores to data from `thread_1` **happens-before** the store to `sync1` B, because they’re **sequenced-before** it in the same thread. Because the load from `sync1` B is in a while loop, it will eventually see the value stored from `thread_1` and thus form the second half of the **release-acquire** pair. Therefore, the store to `sync1` **happens-before** the final load from `sync1` in the while loop. This load is **sequenced-before** (and thus **happens-before**) the store to `sync2` d, which forms a **release-acquire** pair with the final load from the while loop in `thread_3` e. The store to `sync2` d thus **happens-before** the load e, which **happens-before** the loads from data. Because of the **transitive** nature of **happens-before**, you can chain it all together: the stores to data happen-before the store to sync1 B, which happens-before the load from sync1 c, which happens-before the store to sync2 d, which happens-before the load from sync2 e, which **happens-before** the loads from data. Thus the stores to data in `thread_1` happen-before the loads from data in `thread_3`, and the asserts can’t fire.

In this case, you could combine `sync1` and `sync2` into a single variable by using a read-modify-write operation with `memory_order_acq_rel` in `thread_2`. One option would be to use `compare_exchange_strong()` to ensure that the value is updated only once the store from `thread_1` has been seen:

```c++
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<int> sync(0);
void thread_1()
{
    // ...
    sync.store(1, std::memory_order_release);
}
void thread_2()
{
    int expected = 1;
    while (!sync.compare_exchange_strong(expected, 2,
                                         std::memory_order_acq_rel))
        expected = 1;
}
void thread_3()
{
    while (sync.load(std::memory_order_acquire) < 2)
        ;
    // ...
}
int main()
{
}

```



## DATA DEPENDENCY WITH ACQUIRE-RELEASE ORDERING AND `MEMORY_ORDER_CONSUME`


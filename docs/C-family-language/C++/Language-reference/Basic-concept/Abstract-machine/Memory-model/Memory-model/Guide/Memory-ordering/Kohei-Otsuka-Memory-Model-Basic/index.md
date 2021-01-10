# medium [Memory Model Basic](https://medium.com/@koheiotsuka701/memory-model-basic-d8b5f8fddd5f)

*Memory orderings for multi thread programming in C++*

## Introduction

As many know, since C++11, `std::atomic<T>`was introduced as a part of the standard library. Probably the most obvious part of the functionality is that each instantiated type of`std::atomic<T>` can be atomically operated on from different threads without causing any data race. But also, there is another aspect of`std::atomic<T>`that is important to know in order to avoid having tricky bugs, or to improve performance of your programs. The aspect is related to memory model¹ ,especially memory ordering².

There are six memory orderings that are specified in the C++ standard: `memory_order_relaxed`, `memory_order_consume`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel` and `memory_order_seq_cst`³. You can specify these memory orderings with atomic operation like below.

```
example) x.store(true, std::memory_order_relaxed);
```

The six models can be largely categorized into three ordering categories as below.

- Relaxed ordering (`memory_order_relaxed`).
- Acquire-release ordering (`memory_order_consume`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`)
- Sequentially consistent ordering (`memory_order_seq_cst`)

In this post, I will explain about the three categories by using simple diagrams and concrete examples and hopefully it will give you enough understanding to go look into the details of each memory_order options.

## Relaxed ordering

First, I will explain the relaxed ordering (`std::memory_order_relaxed`). Let’s see the example program below. As you can see, there are two threads (`thread_1` and `thread_2`).

In `thread_1`, it is storing `true` to atomic object `x`, then storing `true` value to atomic object `y`.

In `thread_2`, it is checking the value of `y` in a `while` loop and repeat until it reads `true`. After it goes out of the loop, if the value of `x`is `true` then it prints `"y == true also x == true”`.

```
#include <atomic>
#include <thread>
#include <iostream>std::atomic<bool> x,y;void func_1() {
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}void func_2() {
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed)) {
    std::cout << "y == true also x == true \n";  // This might not be executed.
  }
}int main() {
 x = false;
 y = false;
 std::thread thread_1(func_1);
 std::thread thread_2(func_2);
 thread_1.join();
 thread_2.join();
}
```

So what’s the result of the program execution? Since `x` is set to `true` before `y` is set to `true` in `thread_1` (`func_1`) , you would expect that the program always prints `"y == true also x == true"`. However, the truth is that `"y == true also x == true”`**might not be printed (It may be printed but not guaranteed by the standard).**

Why is that? Of course, modern compilers, CPUs can reorder the memory access operations (like `x.store, x.load`) for optimizations.

But, this surprise does not happen in single-threaded program. In single-threaded program, the compilers and CPUs are only allowed to do the re-orderings if they do not break the final result (called as-if rule⁴). The as-if rule keeps things intuitive to programmer because the resulting observable effect looks as if the written program was executed as it is (without any re-orderings).

On the contrary, in multi-threaded program, specified memory-orderings decide the possible re-orderings by compiler and CPU.

`memory_order_relaxed`which was specified in the example above is the least constrained ordering. It gives almost no synchronization on the order of memory access operations between different threads. One exception is that it does give that guarantee that the order of modifications to the same object has to be same among different threads.

So what does it mean? Let me try to explain it using some conceptual diagrams.

![Image for post](https://miro.medium.com/max/48/1*1jS-JieV6xnNrJC-R13Cwg.png?q=20)

![Image for post]()

The horizontal axis represents memory location. You can see object `x` and `y` from the example program are located in the two distinct memory locations and it has two states (`True`, `False`) that changes over time.

The green lines represent “memory snapshots” from a particular thread’s point of view at a particular time. This “point of view” is the key part. They are the “observed” memory snapshots from a particular thread at certain times. So, to a thread, the values on the memory at a certain time looks like as if you sliced along the green line. Conceptually, there are infinite snapshots along the direction of the green arrow.

The snapshot lines will not cross each other because of the above mentioned synchronization requirement “the order of modifications to the same object has to be same among different threads”. So all threads still observe the same order of events on each memory location.

Here’s the interesting part. As you can see, `thread_1`(top) and `thread_2` (bottom) have their own snapshot lines that slice up the memory location space in their own ways (different angles).

To explain the surprising outcome of the example program, we need to see the transition of lines A to B.

From `thread_1`‘s point of view, it looks like (See the value of `x`, `y` at the points where the green lines cross)

1. First, `x` is set to `true`.
2. Then, `y` is set to `true`.

From `thread_2`‘s point of view, it looks like

1. First, `y` is set to `true`.
2. Them, `x` is set to `true`.

Hence, from `thread_2`‘s point of view, there could be a case when y is `true` but x is `false`. That’s why the example program may not print the expected result.

In general, relaxed ordering has the least synchronization overhead compared to other options. However, as you saw, it is not intuitive and difficult to reason about the behavior in practice.

## Acquire-Release ordering

Next, let’s see acquire-release ordering (`memory_order_acquire`, `memory_order_release`). Acquire-release ordering add more synchronization between threads compared to relaxed ordering. It ensures synchronization between threads that are storing and loading the same atomic object.

Let’s see the example below, `thread_1` is storing, `thread_2` is loading `y` with `memory_order_release`,`memory_order_acquire` options respectively. In order for acquire-release ordering to work, the `memory_order_release`,`memory_order_acquire`options always have to be used as pairs on the same atomic object.

Acquire-release ordering guarantees that all memory operations which happen before the storing operation (in this case,`y.store(true, std::memory_order_release)`) in one thread will be visible to the other thread that is doing the corresponding loading operation (likewise, `y.load(std::memory_order_acquire)`).

```
#include <atomic>
#include <thread>
#include <iostream>std::atomic<bool> x,y;void func_1() {
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_release);
}void func_2() {
  while(!y.load(std::memory_order_acquire));
  if(x.load(std::memory_order_relaxed)) {
    std::cout << "y == true also x == true \n";  // This is guaranteed to be executed.
  }
}int main() {
 x = false;
 y = false;
 std::thread thread_1(func_1);
 std::thread thread_2(func_2);
 thread_1.join();
 thread_2.join();
}
```

So, this time, the memory snapshot diagram looks like below. Basically, the additional release-acquire synchronization ensures that the memory snapshot lines for `thread_1` and `thread_2` look similar (it is drawn as same for in the diagram below for simplicity). Why? Because when `thread_2` load `y`, all memory operations done before `thread_1` stored `y` should be visible to `thread_2`. In order to ensure that, the memory snapshot lines of `thread_2` (how it slices up memory space along with time) during the transition from A to B should look like the ones from `thread_1`. If you take the order of `x`, `y` updates as example, during the transition from A to B, from both threads, it looks like

1. First, `x` is set to `true`.
2. Then, `y` is set to `true`

So, with release-acquire ordering, it is guaranteed that the example program prints `"y == true also x == true”`.

![Image for post](https://miro.medium.com/max/48/1*MM0_z62RuI2yNZsWqqxakw.png?q=20)

![Image for post]()

So, as you saw, release-acquire ordering model looks more intuitive than relaxed ordering model. However, There are cases it could produce surprising result. Let’s see the next example.

```
#include <atomic>
#include <thread>
#include <assert.h>
#include <iostream>
std::atomic<bool> x,y;void func_1() {
  x.store(true, std::memory_order_release);
}void func_2() {
  y.store(true, std::memory_order_release);
}void func_3() {
  while(!x.load(std::memory_order_acquire));
  if(y.load(std::memory_order_acquire)) {
    std::cout << "x == true then also y == true \n"; 
  }
}void func_4() {
  while(!y.load(std::memory_order_acquire));
  if(x.load(std::memory_order_acquire)) {
    std::cout << "y == true then also x == true \n";
  }
}// It is possible that neither func_3 or func_4 executes print out.int main() {
 x = false;
 y = false;
 std::thread thread_1(func_1);
 std::thread thread_2(func_2);
 std::thread thread_3(func_3);
 std::thread thread_4(func_4);
 thread_1.join();
 thread_2.join();
 thread_3.join();
 thread_4.join();
}
```

At this time, there are four threads. Two separate threads(`thread_1`, `thread_2`) are storing `x`, `y` separately. `thread_3` and `thread_4` are loading `x`, `y` in the different orders, then prints out message if both `x`, `y` are `true`.

The store/load operations in each thread are properly paired with corresponding store/load operation with `std::memory_order_release` and `std::memory_order_acquire` for `x`, `y`. All look good so far.

If we guess the possible execution results naively, It would be that the program prints either,

1. `x== true then also y == true`or,
2. `y == true then also x == true`or,
3. `x== true then also y == true`and `y == true then also x == true`

However, again surprisingly **the program may** **NOT prints anything.**

Let’s go back to the memory snapshot diagrams. As you can see, `thread_3` and `thread_4`have different memory snapshot lines. And using the similar explanations from before, focusing on the transition from A to B on the both threads,

From `thread_3`, it looks like,

1. First, `x` is set to `true`.
2. Then, `y` is set to `true`.

From `thread_4`, it looks like,

1. First, `y` is set to `true`.
2. Then, `x` is set to `true`.

Hence, the example program may not print out the expected result. So, what’s my point? Basically, release-acquire ordering ensures the threads with corresponding store-load pairs are synchronized, however it does not force threads to agree with the ordering if they are not paired with store-load operations(so, `thread_3` and `thread_4`).

![Image for post](https://miro.medium.com/max/48/1*k4zd8R3kL1gwr9vMKkxtBA.png?q=20)

![Image for post]()

![Image for post](https://miro.medium.com/max/48/1*lNd_FTKbZBjxZNbJLZoyrg.png?q=20)

![Image for post]()

So, release-acquire ordering has more guarantee on the synchronization between threads (with more overhead) compared to relaxed ordering. However, it still has non-intuitive result since still different threads can have different memory snapshot lines as seen in above example.

C++ standard has more strict memory ordering that can solve the problem(with more overhead). It is called sequential consistent ordering and that is what we take a look next.

## Sequential Consistent ordering

The last memory ordering is sequential consistent ordering model (`memory_order_seq_cst`). It is the most strict ordering and it guarantees the most intuitive result for programmer. Since it is the only ordering model that can be reasoned easily, if you don’t specify ordering model on atomic operations, this is set as default option.

Sequential consistency ordering not only guarantees the same synchronization among threads as release-acquire ordering , but also guarantees that all threads must see the same order of memory operations. In other words, it guarantees that there is only one common order of memory operations history among different threads.

Let’s look at the example program below,

```
#include <atomic>
#include <thread>
#include <assert.h>
#include <iostream>
std::atomic<bool> x,y;void func_1() {
  x.store(true, std::memory_order_seq_cst);
}void func_2() {
  y.store(true, std::memory_order_seq_cst);
}void func_3() {
  while(!x.load(std::memory_order_seq_cst));
  if(y.load(std::memory_order_seq_cst)) {
    std::cout << "x == true then also y == true \n";
  }
}void func_4() {
  while(!y.load(std::memory_order_seq_cst));
  if(x.load(std::memory_order_seq_cst)) {
    std::cout << "y == true then also x == true \n";
  }
}int main() {
 x = false;
 y = false;
 std::thread thread_1(func_1);
 std::thread thread_2(func_2);
 std::thread thread_3(func_3);
 std::thread thread_4(func_4);
 thread_1.join();
 thread_2.join();
 thread_3.join();
 thread_4.join();
}
```

At this time, all atomic operations are tagged with `memory_order_seq_cst`.

This ensures that there is only one common order of memory operations history among `thread_1`, `thread_2`, `thread_3` and `thread_4`. This has the same meaning as all of the threads having the same snapshot lines as below.

![Image for post](https://miro.medium.com/max/48/1*cgtPGbXmD3axzWRK1bLvBw.png?q=20)

![Image for post]()

![Image for post](https://miro.medium.com/max/48/1*WCT2ozPwXHg3lEDs09P_KQ.png?q=20)

![Image for post]()

In this particular case, it happens to be that the common agreed snapshot lines say

1. First, `x` is set to `true`.
2. Then, `y` is set to `true`.

As a result, at least the print from `thread_4` (`func_4`) executes `y == true then also x == true`.

## Conclusion

Since C++11, memory ordering model for multi-threaded program was introduced as part of the language standard together with standard libraries for multi threads synchronization such as `std::atomic`that run on top of the memory models. This makes it easier to write portable programs since before C++11, the memory ordering models were defined only by each specific architecture.

The language offers multiple memory ordering options to use with `std::atomic`operations.

`memory_order_seq_cst`gives the most intuitive behavior which is easy to reason about and it is usually the one you need in most cases. As a result,`memory_order_seq_cst`is the default option for all atomic operations in the standard library.

Still, it is helpful to know about other options if you need to understand or debug behavior of programs written by someone else that use memory orderings other than `memory_order_seq_cst `Or if you need to optimize programs and you cannot tolerate the overhead incurred from `memory_order_seq_cst`.

Using the memory snapshot image, hopefully you could grasp the basic idea of possible memory orderings and prepared to check them in further detail.

If you want to know more about the topics, there are great [videos](https://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-1-of-2) from Herb Sutter, I recommend to watch them.

[1]: https://en.wikipedia.org/wiki/Memory_model_(programming)
[2]: https://en.wikipedia.org/wiki/Memory_ordering
[3]: https://en.cppreference.com/w/cpp/atomic/memory_order
[4]: https://en.wikipedia.org/wiki/As-if_rule
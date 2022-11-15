# 7.3 Guidelines for writing lock-free data structures



## 7.3.1 Guideline: use `std::memory_order_seq_cst` for prototyping

`std::memory_order_seq_cst` is much easier to reason about than any other memory ordering because all such operations form a total order. In all the examples in this chapter, you’ve started with `std::memory_order_seq_cst` and only relaxed the memory-ordering constraints once the basic operations were working. In this sense, using other memory orderings is an **optimization**, and as such you need to avoid doing it prematurely(过早的). In general, you can only determine which operations can be relaxed when you can see the full set of code that can operate on the guts(内脏) of the data structure. Attempting to do otherwise just makes your life harder. This is complicated by the fact that the code may work when tested but isn’t guaranteed. Unless you have an algorithm checker that can systematically test all possible combinations of thread visibilities that are consistent with the specified ordering guarantees (and such things do exist), just running the code isn’t enough.



## 7.3.2 Guideline: use a lock-free memory reclamation scheme

One of the biggest difficulties with lock-free code is managing memory. It’s essential to avoid deleting objects when other threads might still have references to them, but you still want to delete the object as soon as possible in order to avoid excessive memory consumption. In this chapter you’ve seen three techniques for ensuring that memory can safely be reclaimed:

1、Waiting until no threads are accessing the data structure and deleting all objects that are pending deletion

2、Using **hazard pointers** to identify that a thread is accessing a particular object

3、Reference counting the objects so that they aren’t deleted until there are no outstanding references

In all cases the key idea is to use some method to keep track of how many threads are accessing a particular object and only delete each object when it’s no longer referenced from anywhere.

There are many other ways of reclaiming memory in lock-free data structures. For example, this is the ideal scenario for using a **garbage collector**. It’s much easier to write the algorithms if you know that the **garbage collector** will free the nodes when they’re no longer used, but not before.

Another alternative is to recycle nodes and only free them completely when the data structure is destroyed. Because the nodes are reused, the memory never becomes invalid, so some of the difficulties in avoiding undefined behavior go away. The downside here is that another problem becomes more prevalent. This is the so-called **ABA problem**.

## 7.3.3 Guideline: watch out for the ABA problem

The ABA problem is something to be wary(小心翼翼的) of in any compare/exchange–based algorithm. It goes like this:

1、Thread 1 reads an atomic variable `x` and finds it has value `A`.


2、Thread 1 performs some operation based on this value, such as dereferencing it (if it’s a pointer) or doing a lookup or something.

3、Thread 1 is stalled(搁置) by the operating system.

4、Another thread performs some operations on `x` that changes its value to `B`.

5、A thread then changes the data associated with the value `A` such that the value held by thread 1 is no longer valid. This may be as drastic(极端的) as freeing the pointed-to memory or just changing an associated value.

6、A thread then changes `x` back to `A` based on this new data. If this is a pointer, it may be a new object that just happens to share the same address as the old one.

7、Thread 1 resumes and performs a compare/exchange on `x`, comparing against A. The compare/exchange succeeds (because the value is indeed A), but this is the wrong A value. The data originally read at step 2 is no longer valid, but thread 1 has no way of telling and will thus corrupt the data structure.


None of the algorithms presented here suffer from this problem, but it’s easy to write lock-free algorithms that do. The most common way to avoid the problem is to include an **ABA counter** alongside the variable `x`. The compare/exchange operation is then done on the combined structure of `x` plus the counter as a single unit. Every time the value is replaced, the counter is incremented, so even if `x` has the same value, the compare/exchange will fail if another thread has modified `x`.

The **ABA problem** is particularly prevalent in algorithms that use free lists or otherwise recycle nodes rather than returning them to the allocator.

## 7.3.4 Guideline: identify busy-wait loops and help the other thread

In the final queue example you saw how a thread performing a push operation had to wait for another thread also performing a push to complete its operation before it could proceed.
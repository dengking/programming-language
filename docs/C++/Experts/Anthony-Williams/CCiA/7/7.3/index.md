# 7.3 Guidelines for writing lock-free data structures



## 7.3.1 Guideline: use `std::memory_order_seq_cst` for prototyping

`std::memory_order_seq_cst` is much easier to reason about than any other memory ordering because all such operations form a total order.



## 7.3.2 Guideline: use a lock-free memory reclamation scheme

One of the biggest difficulties with lock-free code is managing memory. It’s essential to avoid deleting objects when other threads might still have references to them, but you still want to delete the object as soon as possible in order to avoid excessive memory consumption. In this chapter you’ve seen three techniques for ensuring that memory can safely be reclaimed:

1、Waiting until no threads are accessing the data structure and deleting all objects that are pending deletion

2、Using **hazard pointers** to identify that a thread is accessing a particular object

3、Reference counting the objects so that they aren’t deleted until there are no outstanding references

In all cases the key idea is to use some method to keep track of how many threads are accessing a particular object and only delete each object when it’s no longer referenced from anywhere.

There are many other ways of reclaiming memory in lock-free data structures. For example, this is the ideal scenario for using a garbage collector. It’s much easier to write the algorithms if you know that the garbage collector will free the nodes when they’re no longer used, but not before.

Another alternative is to recycle nodes and only free them completely when the data structure is destroyed. Because the nodes are reused, the memory never becomes invalid, so some of the difficulties in avoiding undefined behavior go away. The downside here is that another problem becomes more prevalent. This is the so-called ABA problem.

## 7.3.3 Guideline: watch out for the ABA problem

The ABA problem is something to be wary of in any compare/exchange–based algorithm.

## 7.3.4 Guideline: identify busy-wait loops and help the other thread

In the final queue example you saw how a thread performing a push operation had to wait for another thread also performing a push to complete its operation before it could proceed.
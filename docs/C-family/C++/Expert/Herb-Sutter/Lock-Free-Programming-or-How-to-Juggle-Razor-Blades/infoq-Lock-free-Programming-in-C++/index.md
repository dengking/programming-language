# infoq [Lock-free Programming in C++ with Herb Sutter](https://www.infoq.com/news/2014/10/cpp-lock-free-programming/)

> NOTE: 
>
> 1、未读完

At CppCon 2014, Herb Sutter gave a talk about lock-free programming in C++ ([part 1](https://www.youtube.com/watch?v=c1gO9aB9nbs), [part 2](http://www.youtube.com/watch?v=CmxkPChOcvw)) where he provided the fundamental concepts of lock-free programming, and presented three algorithms to show lock-free techniques. Here is a summary of the most relevant points in the talk.

## What benefits lock-free code can bring

The first point that Herb addresses is what benefits lock-free code can bring:

1、Improving concurrency and scalability by reducing blocking/waiting.

2、Getting rid of potential issues such as race conditions, deadlocks, scarce composability.

Lock-free programming is no panacea(灵丹妙药), though, since a lock-free algorithm is usually more complex to design, and it has other potential issues, such as contention, which can badly affect performance. From this, it ensues Herb's first strong suggestion:

1、Only apply lock-free technique once you have measured your program and found it has some performance or scalability problem.

2、After implementing the lock-free algorithm, measure again to check that things improved effectively.

## **Fundamentals**

There a few basic principles when thinking about lock-free algorithms:

1、**Consistency**: any significant piece of code should be though of as a transaction bringing the system from one consistent state to another.

2、**Isolation**: two transactions never operate on the same data.

3、**Durability**: a committed transaction is never overwritten by another before the latter has "seen" the results of the former.

> NOTE: 
>
> 1、非常类似于DB transaction的ACID

The fundamental technique to achieve that is:

- computing all changes in some private area;
- using one single atomic write through a special compare/exchange function to make them public.

In C++11, this translates into using `atomic<T>`, which gives two big advantages:

- You can think of each read or write operation as atomic, with no lock required.
- Furthermore, reads and writes are guaranteed not to be reordered.

A few important points about `atomic` are the following:

1、For small types, atomic is usually implemented through platform-specific operations.

2、`atomic` objects must always be initialized (otherwise they will contain garbage).

3、Two atomic operations are guaranteed to be individually atomic, but the status of the object can change in between.

> NOTE:
>
> 1、ABA problem
>
> 2、time of check to time of use

## Example: Double-checked Locking

The first example Herb presents is ensuring a global object is constructed only once.

The key idea is: protecting the atomic write operation through a lock, while letting the atomic read happen without locking. Blocking can thus only happens among those threads that compete to initialize the singleton. The reason behind the algorithm name is that the instance pointer is checked twice, before and after getting the lock on it:

```c++
atomic<Widget*> Widget::pInstance{ nullptr };
Widget* Widget::Instance() {
   if (pInstance == nullptr) {
      lock_guard<mutex> lock { mutW };
      if (pInstance == nullptr) {
         pInstance = new Widget();
      }
   }
}
```

## Example: Producer-consumers

The second example described by Herb is a classical producer-consumers algorithm. He begins by describing a traditional solution using locks where:

1、The producer gets a lock on the shared queue and push a few objects on to it; for each objects, a condition variable is notified, so consumers know.

2、On the other hand, consumers just try to get a lock on the queue and when one gets it, it checks if there is an object to consume; if there is, then it is popped and processed after releasing the mutex.

> NOTE: 
>
> 1、上述是典型的lock-based的做法

A first variant of this algorithm is possible using a lock-free technique, whereby the slist is accessed through an `atomic` variable. What this allow is for the producer to create all of its items at once, then publish them to the consumer by atomically setting the queue's head. Consumers remain the same.

> NOTE: 
>
> 1、"publish them to the consumer by atomically setting the queue's head"要如何理解？
>
> 2、结合上下文来看，上述`slist`指的是slot list

Next, a full lock-free implementation is considered. In this case, the algorithm is based on the idea that the producer has to fill a certain number of slots. When it has got a new task, it checks if there is an empty slot, and it store the task there. In the following code, slot is an atomic variable:

```c++
curr = 0;
// Phase 1: build and distribute tasks
while (ThereAreMoreTasks()) {
   task = AllocateAndBuildNewTask();
   while (slot[curr] != null)
      curr = (curr+1)%K;
   slot[curr] = task;
   sem[curr].signal();
}
// Phase 2: Stuff the mailbox with done
numNotified = 0;
while (numNotified < K) {
   while (slot[curr] != null)
      curr = (curr+1)%K;
   slot[curr] = done;
   sem[curr].signal();
   ++numNotified;
}
```

As to consumers, the code is simpler:

```c++
myTask = null;
while (myTask != done) {
   while (myTask = slot[mySlot]) == null)
      sem[mySlot].wait();
   if (myTask != done) {
      slot[mySlot] = null;
      DoWork(myTask);
   }
}
```

A consumer waits on a semaphore until some task is in slot. When a task comes in, the consumer gets it and empties the slot. After doing that, the consumer starts processing its data. This responds to the same idea as before of doing work outside of the **critical section**. But if the consumer is slower than the producer, then it could make sense to do the work *before* releasing the lock, so that the producer would not fill that same slot again while the consumer is busy, but would preferably fill another empty slot. This shows how you can make different decisions subtly affecting throughput/scalability vs. load balancing.

## Example: Singly-linked list

A singly-linked list is one of the simplest possible data structures, supporting in this case just four operations: `construct, destroy, find, push_front`.

The lock-free implementation proposed by Herb uses an `atomic<Node*> head{ nullptr };` to access the slist head. The only method actually presenting concurrency issues is `push_front`, which in a single-threaded version could look like this:

```
template
void slist<T>::push_front(T t) {
   auto p = new Node;
   p->t = t;
   p->next = head;
   head = p;
}
```

This code has problems since it opens up the possibility of races when setting the new `head` value. We can fix this problem by using `compare_exchange` when writing to `head`, as shown below:

```
template
void slist<T>::push_front(T t) {
   auto p = new Node;
   p->t = t;
   p->next = head;

   while (!head.compare_exchange_weak(p->next, p))
      {}
}
```

Here we try to swap `head` and `p` until we succeed. This use of `compare*exchange*weak` is typical in lock-free code. It is used mostly with loops, while outside of loops `compare*exchange*strong` is used.

More issues arise when trying to implement a pop operation, which will erase the first element from the list. In this case, one major cause of complexity is the possibility of returning a pointer to an object that could be deleted by another thread shortly thereafter. This problem is well-known in literature as the [ABA problem](http://en.wikipedia.org/wiki/ABA_problem), and Herb goes into detail describing how it can arise in the given scenario.

C++11 allows an elegant solution to this problem, by not using raw pointers and replacing them with a `shared_ptr`. In pseudo-code, the implementation becomes:

```
template
struct Node { T t; shared_ptr<Node> next; };
atomic<shared_ptr<Node>> head;

public:
   slist() =default;
   ~slist() =default;
   
   class reference { 
      shared_ptr<Node> p;
   public:
      reference(shared_ptr<Node> p_) : p{_p} {}
      T& operator*() { return p->t; }
      T* operator->() { return &p->t; }
   };
   auto find(T t) const {
      auto p = head.load();
      while (p && p->t != t)
         p = p->next;
      return reference{move(p)};
      
   void push_front(T t) {
      auto p = make_shared<Node>();
      p->t = t;
      p->next = head;
      while (head.compare_exchange_weak(p->next, p))
         {}
   }
   
   void pop_front() {
      auto p = head.load();
      while (p && !head.compare_exchange_weak(p, p->next))
         {}
   }
};
```

Here the trick is that the pointer is returned wrapped into a `shared_ptr`, so we are safe as to deletion concerns.

This implementation shows a nice property known as [linearizability](http://en.wikipedia.org/wiki/Linearizability), which makes that a series of overlapping operations, can be though of as if they were executed in sequence.

The final part of the talk is dedicated to discussing an example of measuring a program to find out how it behaves and what benefits it can get from going lock-free.
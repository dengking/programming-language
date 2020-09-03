# aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 

> ∗This is a slightly-modiﬁed version of an article that appeared in Dr. Dobbs Journal in the July (Part I) and August (Part II), 2004, issues.

## 1 Introduction

Google the newsgroups or the web for the names of various design patterns, and you’re sure to ﬁnd that one of the most commonly mentioned is Singleton. Try to put Singleton into practice, however, and you’re all but certain to bump into a signiﬁcant limitation: as traditionally implemented (and as we explain below), Singleton isn’t **thread-safe**. 

Much eﬀort has been put into addressing this shortcoming. One of the most popular approaches is a **design pattern** in its own right, the **Double-Checked Locking Pattern** (DCLP) [13, 14]. DCLP is designed to add eﬃcient threadsafety to initialization of a shared resource (such as a Singleton), but it has a problem: it’s not reliable. Furthermore, there’s virtually no portable way to make it reliable in C++ (or in C) without substantively（本质上） modifying the conventional pattern implementation. To make matters even more interesting, DCLP can fail for diﬀerent reasons on uniprocessor and multiprocessor architectures. This article explains why Singleton isn’t thread safe, how DCLP attempts to address that problem, why DCLP may fail on both uni- and multiprocessor architectures, and why you can’t (portably) do anything about it. Along the way, it clariﬁes the relationships among **statement ordering** in source code, **sequence points**, compiler and hardware optimizations, and the actual order of **statement execution**. Finally, it concludes with some suggestions regarding how to add thread-safety to Singleton (and similar constructs) such that the resulting code is both reliable and eﬃcient.

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

> NOTE: 关于上面描述的场景，在APUE 10.6 Reentrant Functions中进行了介绍。

Unfortunately, this implementation is not reliable in a multithreaded environment. Suppose that Thread A enters the `instance` function, executes through Line 14 ( `if (pInstance == 0)` ), and is then suspended. At the point where it is suspended, it has just determined that pInstance is null, i.e., that no `Singleton` object has yet been created. Thread B now enters `instance` and executes Line14. It sees that `pInstance` is null, so it proceeds to Line 15 and creates a `Singleton` for `pInstance` to point to. It then returns `pInstance` to instance’s caller. At some point later, Thread A is allowed to continue running, and the ﬁrst thing it does is move to Line 15, where it conjures up another Singleton object and makes `pInstance` point to it. It should be clear that this violates the meaning of a singleton, as there are now two Singleton objects. Technically, Line 11 is where `pInstance` is initialized, but for practical purposes, it’s Line 15 that makes it point where we want it to, so for the remainder of this article, we’ll treat Line 15 as the point where `pInstance` is initialized. Making the classic Singleton implementation thread safe is easy. Just acquire a lock before testing `pInstance`:

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

## 3 The Double-Checked Locking Pattern

The crux（关键） of DCLP is the observation that most calls to `instance` will see that `pInstanceis` non-null, hence not even try to initialize it. Therefore, DCLP tests `pInstance` for nullness before trying to acquire a lock. Only if the test succeeds (i.e., if `pInstance` has not yet been initialized) is the lock acquired, and after that the test is performed again to make sure pInstance is still null (hence the name double-checked locking). The second test is necessary, because, as we just saw, it is possible that another thread happened to initialize `pInstance` between the time `pInstance` was ﬁrst tested and the time the lock was acquired.
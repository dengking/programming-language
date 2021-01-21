# The Rule of Zero



## feabhas [The Rule of Zero](https://blog.feabhas.com/2015/01/the-rule-of-zero/)

In a previous article – ”[The Rule of the Big Four (and a half)](https://blog.feabhas.com/2015/01/the-rule-of-the-big-four-and-a-half-move-semantics-and-resource-management/)” we looked at resource management policies in C++.

> NOTE: a half所指的是`swap`

Resource management is the general term for using the mechanisms in C++ to ensure that resources – files, dynamic memory, sockets, mutexes, etc – have their lifetimes automatically controlled so as to prevent resource leaks, deadlocks, etc. C++ refers to these mechanisms as `RAII/RDID` ( “Resource Acquisition Is Initialisation / Resource Destruction is Deletion”)

In this article we’ll have a look at a complementary guideline to help simplify your application code, without risking resource management issues – *The Rule of Zero*.

The term The Rule of Zero was coined by R. Martinho Fernandes in his 2012 paper (http://flamingdangerzone.com/cxx11/2012/08/15/rule-of-zero.html). This article merely reflects Martinho Fernandes’ work and I highly recommend reading the original paper to get the full details of the concepts.

If you’re not already familiar with the concepts of resource management I’d suggest having a look at the previous articles – [The Rule of Three (and a half)](https://blog.feabhas.com/2014/12/the-rule-of-the-big-three-and-a-half-resource-management-in-c/) and [The Rule of Four (and a half)](https://blog.feabhas.com/2015/01/the-rule-of-the-big-four-and-a-half-move-semantics-and-resource-management/) before reading on.

### The four categories of resource manager

From a resource management perspective we can categorise types in four ways:

- Objects that can be both moved and copied
- Objects for which it makes sense to copy but not move
- Objects for which it makes sense to move but not copy
- Objects which should neither be moved not copied.

The Rule of The Big four (and a half) is a guideline for implementing the **copy/move** policies for the types in your system. Essentially, it states:

- If you have written a (non-default) destructor for a class you should implement the copy constructor and assignment operator for the class; or mark them as deleted.
- Similarly, if you have written either a (non-default) copy constructor or assignment operator for the class you must write a destructor.
- If your class can be moved you must implement both the **move constructor** and **move assignment** operator; or mark them as deleted.

With copy policy it is often performance issues – memory, speed, efficiency – that determine whether it is ‘sensible’ to copy a class. For example, it is possibly unwise to copy a 1MByte data file being owned as a resource!

In other cases the decision may be made based on whether replicating the owned resource may have detrimental（有害的） consequences – for example, replicating an OS mutex could cause potential difficult-to-identify race conditions in code.

Moving a resource is commonly considered an efficiency optimization for copying. However, the moved-from object must be left in an ‘empty’ state. What ‘defines’ empty (obviously) varies from object to object but a good rule-of-thumb is: If the class does not have a **default constructor** then it probably shouldn’t support **move semantics**.

```

```

### The Rule of Zero

An alternative to “The Rule of The Big Four (and a half)” has appeared in the form of “*The Rule of Zero”*. “The Rule of Zero” basically states:

> *You should NEVER implement a destructor, copy constructor, move constructor or assignment operators in your code.*

With the (very important) corollary（推论） to this:

> *You should NEVER use a raw pointer to manage a resource.*

The aim of The Rule of Zero is to simplify your application code by deferring all resource management to Standard Library constructs, and letting them do all the hard work for you.
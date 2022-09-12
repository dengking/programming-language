# 5 The C++ memory model and operations on atomic types

> NOTE:
>
> 一、relation and ordering
>
> 包括两个维度:
>
> 1、对同一object的modification order
>
> 这在 5.1.3 Modification orders 节进行主要描述
>
> 2、对不同object的modification之间的相对order
>
> relation: happens-before、synchronizes-with
>
> 这在 "5.3 Synchronizing operations and enforcing ordering" 节进行主要描述

## 5.1 Memory model basics

There are two aspects to the memory model: the basic **structural aspects**, which relate to how things are laid out in memory, and then the **concurrency aspects**. The **structural aspects** are important for concurrency, especially when you’re looking at low level atomic operations, so I’ll start with those. In C++, it’s all about objects and memory locations.

> NOTE:
>
> 一、
>
> "5.1.1 Objects and memory locations"章节描述: "the basic **structural** aspects"。
>
> "5.1.2 Objects, memory locations, and concurrency"章节描述: "the **concurrency** aspects"

### 5.1.1 Objects and memory locations

> NOTE:
>
> 一、这一点所描述的其实就是C++的object 

All data in a C++ program is made up of **objects**. It’s just a statement about the building blocks of data in C++. The C++ Standard defines an object as “a region of storage,” although it goes on to assign properties to these objects, such as their **type** and **lifetime**.



### 5.1.2 Objects, memory locations, and concurrency

Now, here’s the part that’s crucial for multithreaded applications in C++: ***everything hinges on those memory locations***. 

> NOTE:
>
> 一、"***everything hinges on those memory locations***" 的翻译如下:
>
> "一切都取决于这些内存位置"

If two threads access separate memory locations, there’s no problem: everything works fine. On the other hand, if two threads access the same memory location, then you have to be careful. If neither thread is updating the memory location, you’re fine; read-only data doesn’t need protection or synchronization. If either thread is modifying the data, there’s a potential for a **race condition**, as described in chapter 3.

#### Enforce ordering / synchronization

> NOTE:
>
> 一、所谓"enforce ordering"其实就是"synchronization"

In order to avoid the **race condition**, there has to be an **enforced ordering** between the accesses in the two threads. 

One way to ensure there’s a defined ordering is to use mutexes as described in chapter 3; if the same mutex is locked prior to both accesses, only one thread can access the memory location at a time, so one must **happen before** the other. 

> NOTE:
>
> 一、上面这段话使用ordering的角度来解释mutex

The other way is to use the synchronization properties of atomic operations (see section 5.2 for the definition of atomic operations) either on the same
or other memory locations to enforce an ordering between the accesses in the two threads. The use of atomic operations to enforce an ordering is described in section 5.3. If more than two threads access the same memory location, each pair of accesses must have a defined ordering.

### 承上启下

Before we look at atomic operations, there’s one more concept that’s important to understand about objects and memory locations: modification orders.

### 5.1.3 Modification orders

> NOTE:
>
> 一、modification order是从shared data的角度来建立的

Every object in a C++ program has a defined **modification order** composed of all the writes to that object from all threads in the program, starting with the object’s **initialization**. In most cases this order will vary between runs, but in any given execution of the program all threads in the system must agree on the order. If the object in question isn’t one of the atomic types described in section 5.2, you’re responsible for making certain that there’s sufficient synchronization to ensure that threads agree on the **modification order** of each variable. If different threads see distinct sequences of values for a single variable, you have a **data race** and **undefined behavior** (see section 5.1.2). If you do use atomic operations, the compiler is responsible for ensuring that the necessary synchronization is in place.


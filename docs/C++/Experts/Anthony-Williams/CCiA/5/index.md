# 5 The C++ memory model and operations on atomic types

## 5.1 Memory model basics

There are two aspects to the memory model: the basic **structural** aspects, which relate to how things are laid out in memory, and then the **concurrency** aspects.

> NOTE:
>
> 一、
>
> "5.1.1 Objects and memory locations"章节描述: "the basic **structural** aspects"。
>
> "5.1.2 Objects, memory locations, and concurrency"章节描述: "the **concurrency** aspects"



### 5.1.2 Objects, memory locations, and concurrency

Before we look at atomic operations, there’s one more concept that’s important to understand about objects and memory locations: modification orders.

### 5.1.3 Modification orders

> NOTE:
>
> 一、modification order是从shared data的角度来建立的

Every object in a C++ program has a defined **modification order** composed of all the writes to that object from all threads in the program, starting with the object’s initialization. In most cases this order will vary between runs, but in any given execution of the program all threads in the system must agree on the order. If the object in question isn’t one of the atomic types described in section 5.2, you’re responsible for making certain that there’s sufficient synchronization to ensure that threads agree on the **modification order** of each variable. If different threads see distinct sequences of values for a single variable, you have a data race and undefined behavior (see section 5.1.2). If you do use atomic operations, the compiler is responsible for ensuring that the necessary synchronization is in place.


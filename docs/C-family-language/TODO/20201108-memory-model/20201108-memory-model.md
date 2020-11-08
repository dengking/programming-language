# C++ memory model

## 1 sciencedirect sequential-consistency

https://www.sciencedirect.com/topics/computer-science/sequential-consistency



### 8.4.1 Memory Models and Memory Ordering Options

**Three** memory models proposed by C++11, which, together with the memory ordering options that can be used in each one of them, are described next. Memory order options are symbolic constants passed as second argument to the atomic class member functions. As stated before, the same memory models are implemented in TBB and Windows, with minor implementation differences.

#### 1 Sequential consistency

This memory model imposes the strongest memory order constraints. In fact, there is more to **sequential consistency** than the **happens before relation** exhibited in Listing 8.2. Broader global synchronizations are implemented, forcing all threads to see all the sequential consistent synchronizations in the program happening in the same order. There is, in a given program, a unique global order of sequentially consistent atomic operations, seen by all the threads in the process.

##### `memory_order_seq_cst`

This ordering enforces sequential consistency, preventing preceding (or succeeding) memory operations to be reordered beyond (or before) this point. But there are also subsidiary global synchronizations as discussed above. An example will be given next. *This is the default value for the second argument in member functions*.

#### 2 Acquire-release

In this model, stores and loads still synchronize as described above, but there is no global order. Different threads may see different synchronizations happening in different orders. Memory order options are:

##### `memory_order_release` 

Prevents preceding memory operations from being reordered past this point.

##### `memory_order_acquire` 

Prevents succeeding memory operations from being reordered before this point.

##### `memory_order_consume` 

A weaker form of the acquire option: memory order constraints only apply to succeeding operations that are computationally dependent on the value retrieved (loaded) by the atomic variable.

##### `memory_order_acq_rel`

Combines both release and acquire memory order constraints: a full memory fence is in operation.

#### 3 Relaxed

In this model, the basic store-load synchronization described above does not occur, and no “happens before” relations across threads are established.

##### `memory_order_relaxed`

There is no ordering constraint. Following operations may be reordered before, and preceding operations may be reordered after the atomic operation.



Finally, it is important to observe that not all memory ordering options are adapted to all member functions. Atomic operations are of three kinds: *load, store*—the load() and store() member functions—and *read_modify_write*—all the other member functions.Table 8.2 lists the memory ordering options available to each type of atomic operation.

Table 8.2. Memory Order Options for Atomic Operations

|                   | Options for Atomic Operations |
| :---------------- | :---------------------------- |
| **Operation**     | **Memory Options**            |
| Store             | `memory_order_seq_cst`        |
|                   | `memory_order_release`        |
|                   | `memory_order_relaxed`        |
| Load              | `memory_order_seq_cst`        |
|                   | `memory_order_acquire`        |
|                   | `memory_order_consume`        |
|                   | `memory_order_relaxed`        |
| Read-modify-write | all available options         |

The rest of this section provides a very qualitative discussion of the differences between the different memory ordering options, so that readers can feel more or less at ease when dealing with references on this subject. A couple of examples will also be examined. There are, in this subject, a substantial number of subtleties that are beyond the scope of this book. Readers willing to go deeper into this subject should consult the exhaustive discussion presented in [14].



## modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

Since C++11, C++ has a memory model. It is the foundation for multithreading. Without it, multithreading is not well defined. 

 

The C++ memory model consists of two aspects. On one hand, there is the enormous complexity of the memory model, which often contradicts our intuition. On the other hand, the memory model helps a lot to get a deeper insight into the multithreading challenges.

### The contract

In the first approach, the C++ memory model defines a contract. This contract is established between the programmer and the system. The system consists of the compiler, which compiles the program into assembler instructions, the processor, which performs the assembler instructions and the different caches, which stores the state of the program. The contract requires from the programmer to obey certain rules and gives the system the full power to optimise the program as far as no rules are broken. The result is - in the good case - a well-defined program, that is maximal optimised. Precisely spoken, there is not only a single contract, but a fine-grained set of contracts. Or to say it differently. The weaker the rules are the programmer has to follow, the more potential is there for the system to generate a highly optimised executable.

The rule of thumb is quite easy. The stronger the contract, the fewer liberties for the system to generate an optimised executable. Sadly, the other way around will not work. In case the programmer uses an extremely weak contract or memory model, there are a lot of optimisation choices. But the program is only manageable by a few worldwide known experts. 

There are three levels of the contract in C++11.

![Overview](https://www.modernescpp.com/images/blog/Speichermodell/Speichermodell/Overview.png)

## cppreference [Memory model](https://en.cppreference.com/w/cpp/language/memory_model)





## utexas [Memory Consistency Models: A Tutorial](https://www.cs.utexas.edu/~bornholt/post/memory-models.html)





## cpluspluspedia [C++11 Memory Model](https://cpluspluspedia.com/en/tutorial/7975/cplusplus11-memory-model)



## stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)
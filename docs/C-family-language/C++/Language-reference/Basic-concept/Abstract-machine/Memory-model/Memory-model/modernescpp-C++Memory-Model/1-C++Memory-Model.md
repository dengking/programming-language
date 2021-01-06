# modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

Since C++11, C++ has a memory model. It is the foundation for multithreading. Without it, multithreading is not well defined. 

 

The C++ memory model consists of two aspects. On one hand, there is the enormous complexity of the memory model, which often contradicts(违反) our intuition(直觉). On the other hand, the memory model helps a lot to get a deeper insight into the multithreading challenges.

## The contract

> NOTE: 契约

In the first approach, the C++ memory model defines a contract. This contract is established between the programmer and the **system**. The system consists of the **compiler**, which compiles the program into assembler instructions, the **processor**, which performs the assembler instructions and the different **caches**, which stores the state of the program. 

> NOTE: 上述组成部分的分析是非常好的:
>
> 1、compiler
>
> 2、processor
>
> 3、cache

The contract requires from the programmer to obey certain rules and gives the system the full power to optimise the program as far as no rules are broken. The result is - in the good case - a well-defined program, that is maximal optimised. Precisely spoken, there is not only a single contract, but **a fine-grained set of contracts**. Or to say it differently. The weaker the rules are the programmer has to follow, the more potential is there for the system to generate a highly optimised executable.

> NOTE: 提供了"**a fine-grained set of contracts**"，能够保证灵活性；
>
> programmer需要tradeoff

The rule of thumb is quite easy. The stronger the contract, the fewer liberties for the system to generate an optimised executable. Sadly, the other way around will not work. In case the programmer uses an extremely weak contract or memory model, there are a lot of optimisation choices. But the program is only manageable by a few worldwide known experts. 

> NOTE: 使用weak memory model的难度是较大的，需要有expert级别的知识

There are three levels of the contract in C++11.

![Overview](https://www.modernescpp.com/images/blog/Speichermodell/Speichermodell/Overview.png)

Before C++11 there was only one contract. C++ was not aware of the existence of multithreading or atomics. The system only knows about one control flow and therefore there were only restricted opportunities to optimise the executable. The key point of the system was it, to keep the illusion for the programmer, that the observed behaviour of the program corresponds to the sequence of the instructions in the source code. Of course, there was no memory model. Instead of that, there was the concept of a sequence point. Sequence points are points in the program, at which the effects of all instructions before must be observable. The start or the end of the execution of a function are sequence points. But in case you invoke a function with two arguments, the C++ standard makes no guarantee, which arguments will be evaluated at first. So the behaviour is unspecified. The reason is straightforward. The comma operator is no sequence point. That will not change in C++11.

But with C++ all will change. C++11 is the first time aware of multiple threads. The reason for the well-defined behaviour of threads is the C++ memory model. The C++ memory model is inspired by the Java memory model, but the C++ one goes - as ever - a few steps further. But that will be a topic of the next posts. So the programmer has to obey to a few rules in dealing with shared variables to get a well-defined program. The program is undefined if there exists at least one data race. As I already mentioned, you have to be aware of data races, if your threads share mutable data. So tasks are a lot easier to use than threads or condition variables.

With atomics, we enter the domain of the experts. This will become more evident, the further we weaken the C++ memory model. Often, we speak about lock-free programming, when we use atomics. I spoke in the posts about the weak and strong rules. Indeed, the sequential consistency is called strong memory model, the relaxed semantic weak memory model.
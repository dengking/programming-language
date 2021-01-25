# modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

Since C++11, C++ has a memory model. It is the foundation for multithreading. Without it, multithreading is not well defined. 

 

The C++ memory model consists of two aspects. On one hand, there is the enormous complexity of the memory model, which often contradicts(违反) our intuition(直觉). On the other hand, the memory model helps a lot to get a deeper insight into the multithreading challenges.

## The contract

> NOTE: 契约，其实就是 design to an abstraction

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



With **atomics**, we enter the domain of the experts. This will become more evident, the further we weaken the C++ **memory model**. Often, we speak about lock-free programming, when we use atomics. I spoke in the posts about the **weak** and **strong** rules. **Indeed, the sequential consistency is called strong memory model, the relaxed semantic weak memory model.**

> NOTE: multiple level memory model
>
> strong->weak

## The meat of the contract

> NOTE: contract的核心内容

The contract between the programmer and the system consists of three parts:

1、**Atomic operations**: Operations, which will be executed without interruption.

2、**The partial order of operations**: Sequence of operations, which can not be changed.

> NOTE: 其实就是memory ordering

3、**Visible effects of operations**: Guarantees, when an operation on shared variables will be visible in another thread.

> NOTE: sequenced-before等

The foundation of the contract are operations on atomics. These operations have two characteristics. They are atomic and they create synchronisation and order constraints on the program execution. These synchronisations and order constraints will often also hold for not atomic operations. At one hand an atomic operation is always atomic, but on the other hand, you can tailor the synchronisations and order constraints to your needs.

> NOTE: 上面这段话说的比较绕

## Back to the big picture

The more we weaken the **memory model**, the more our focus will change.

1、More optimisation potential for the system

2、The number of control flows of the program increases exponential

3、Domain for the experts

4、Break of the intuition

5、Area for micro optimisation

To make multithreading, we should be an expert. In case we want to deal with atomics (sequential consistency), we should open the door to the next expertise level. And you know, what will happen when we talk about the acquire-release or relaxed semantic? We'll go each time one step higher to the next expertise level.

![ExpertLevels](https://www.modernescpp.com/images/blog/Speichermodell/Speichermodell/ExpertLevels.png)
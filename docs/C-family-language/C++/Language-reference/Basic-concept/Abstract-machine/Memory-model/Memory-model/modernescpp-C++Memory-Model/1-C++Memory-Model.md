# modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

Since C++11, C++ has a memory model. It is the foundation for multithreading. Without it, multithreading is not well defined. 

 

The C++ memory model consists of two aspects. On one hand, there is the enormous complexity of the memory model, which often contradicts(违反) our intuition(直觉). On the other hand, the memory model helps a lot to get a deeper insight into the multithreading challenges.

## The contract

> NOTE: 契约

In the first approach, the C++ memory model defines a contract. This contract is established between the programmer and the **system**. The system consists of the **compiler**, which compiles the program into assembler instructions, the **processor**, which performs the assembler instructions and the different **caches**, which stores the state of the program. 

The contract requires from the programmer to obey certain rules and gives the system the full power to optimise the program as far as no rules are broken. The result is - in the good case - a well-defined program, that is maximal optimised. 

Precisely spoken, there is not only a single contract, but a fine-grained set of contracts. Or to say it differently. The weaker the rules are the programmer has to follow, the more potential is there for the system to generate a highly optimised executable.

The rule of thumb is quite easy. The stronger the contract, the fewer liberties for the system to generate an optimised executable. Sadly, the other way around will not work. In case the programmer uses an extremely weak contract or memory model, there are a lot of optimisation choices. But the program is only manageable by a few worldwide known experts. 

There are three levels of the contract in C++11.
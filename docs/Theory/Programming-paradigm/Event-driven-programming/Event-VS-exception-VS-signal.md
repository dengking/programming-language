在APUE的chapter 10 signals中有这样的描述：

> Signals are software interrupts. 

> Signals are classic examples of **asynchronous events**. They occur at what appear to be random times to the process. The process can’t simply test a variable (such as `errno`) to see whether a signal has occurred; instead, the process has to tell the kernel ‘‘if and when this signal occurs, do the following.’’





**异步**倾向于是当，某个事情发生，时我应该做什么。所以异步往往就需要注册一个回调函数，就像APUE的第十章的第一节中所介绍的那样。Unix中的异步IO都是基于信号的。

在有道云笔记中我总结event driving programming 范式，在这些中，event往往是系统内部由程序而产生的，是程序员故意而为之产生的用于进行通信的方式，event的接受者在没有信号的情况下往往会阻塞自己。

而当提及信号时他和事件的语义就有所不同了，信号往往是由操作系统而产生的，而不是由应用程序来。所以我们应用程序是信号的接受者，信号的产生者是操作系统。这是信号和事件之间的差异之一；可以说第一种差异决定了在对信号和消息进行react的时候采取的是完全不同的方式：由于信号是由**OS**所产生，所以对signal的所有的操作都是按照OS的接口来进行，并且信号的产生是random的：Signals are classic examples of **asynchronous events**，并且process的 [flow of execution](https://en.wikipedia.org/wiki/Control_flow)  是由OS来进行控制的，所以OS有能力来决定当收到信号的时候，interrupts the target process' normal [flow of execution](https://en.wikipedia.org/wiki/Control_flow) to deliver the signal（这个观点摘取自[Signal (IPC)](https://en.wikipedia.org/wiki/Signal_(IPC)) ），即OS有能力在收到signal的时候，中断process当前的执行而转去执行signal handler；而message等往往都是由application产生的，application无法向OS那样来interrupt  process' normal [flow of execution](https://en.wikipedia.org/wiki/Control_flow) ，而只能够在没有message的时候，就阻塞自己，在收到message的时候，转去执行相应的函数；



需要注意的是信号是异步事件，但是异步事件不一定是信号。

## Event VS exception

event-driven programming，它是对event-driven model的实现。

exception可以可看做是一种event，即它是由external environment所产生的，现代programming language对它进行了抽象。

signal也可以看做是一种event。




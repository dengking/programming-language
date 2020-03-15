# Event-driven model

在文章[Abstraction and model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model)中，我们提出了[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)，它是一个非常强大的模型，能够描述非常非常广泛的问题。在本文，我们将说明什么是event-driven model、什么是event、如何实现event-driven model。

## Event-driven model

Event-driven model需要持续不断地进行运转以监控事件源、收集event，一旦收集到了event，就进行dispatch，即调用这个event对应的event handler。其实，我们可以将它看做是一个abstract machine。

理解event-driven model的最最简单的例子就是linux OS kernel和hardware的交互，在工程[Linux-OS](https://dengking.github.io/Linux-OS)的文章[Linux-OS-kernel-is-event-driven](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-interaction-with-the-hardware/Linux-OS-kernel-is-event-driven/)中我们已经总结了可以使用[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)来描述linux OS kernel和hardware的交互。在hardware层，一旦通电，则hardware就持续不断地运转起来了，一旦触发了interrupt，则它的interrupt handler就会被执行。







## What is event?

Event是一个非常概括、宽泛的概念，在Event-driven model中，非常多的行为都可以看做是它的event，比如：

- IO（其实IO就包括了非常多的内容，用户操作、网络通信等都可以看做是IO）

- hardware interrupt

event可能源自于system 之外，也可能源自于system之内。下面补充了维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))来进行详细说明。

### 维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))









## Event-driven programming

event-driven programming告诉我们如何实现event-driven model。



Source of events

handler of event

channel



provider channel consumer

event provider

consumer需要侦听，以等待event的到来，然后调用event handler。



event，event可能有多种，需要描述清楚每种事件对应的event handler。

IO multiplexing 



event-driven model实现中需要考虑的一些问题：

- 如何声明event和event handler之间的关系，显然event driven model需要记录下event和event handler之间的映射关系？

- 如何进行持续监控？

### 如何进行持续监控

event loop



在软件层，我们需要显式地使用一个main loop来指示整个软件系统不断地运转下去。对于OS kernel，它的main loop是hardware实现的，只要通电后，整个OS就运转起来

肯定有一个main loop，负责收集event，然后进行dispatch，即调用该event对应的handler。







### 关联event和event handler

table

####  Interrupt Descriptor Table

参见：

工程[Linux-OS](https://dengking.github.io/Linux-OS)的[4.2-Interrupts-and-Exceptions](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/)的[4.2.3. Interrupt Descriptor Table](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/#423-interrupt-descriptor-table)

维基百科[Interrupt descriptor table](https://en.wikipedia.org/wiki/Interrupt_descriptor_table)



### 维基百科[Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **event-driven programming** is a [programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm) in which the [flow of the program](https://en.wikipedia.org/wiki/Control_flow) is determined by [events](https://en.wikipedia.org/wiki/Event_(computing)) such as user actions ([mouse](https://en.wikipedia.org/wiki/Computer_mouse) clicks, key presses), [sensor](https://en.wikipedia.org/wiki/Sensor) outputs, or [messages](https://en.wikipedia.org/wiki/Message_passing) from other programs or [threads](https://en.wikipedia.org/wiki/Thread_(computer_science)). Event-driven programming is the dominant paradigm used in [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interface) and other applications (e.g., JavaScript [web applications](https://en.wikipedia.org/wiki/Web_application)) that are centered on performing certain actions in response to [user input](https://en.wikipedia.org/wiki/Input/output). This is also true of programming for [device drivers](https://en.wikipedia.org/wiki/Device_driver) (e.g., [P](https://en.wikipedia.org/wiki/P_(programming_language)) in USB device driver stacks).

In an event-driven application, there is generally a [main loop](https://en.wikipedia.org/wiki/Event_loop) that listens for events, and then triggers a [callback function](https://en.wikipedia.org/wiki/Callback_(computer_programming)) when one of those events is detected. In [embedded systems](https://en.wikipedia.org/wiki/Embedded_system), the same may be achieved using [hardware interrupts](https://en.wikipedia.org/wiki/Hardware_interrupt) instead of a constantly running main loop. Event-driven programs can be written in any [programming language](https://en.wikipedia.org/wiki/Programming_language), although the task is easier in languages that provide [high-level abstractions](https://en.wikipedia.org/wiki/Abstraction_(computer_science)), such as [await](https://en.wikipedia.org/wiki/Async/await) and [closures](https://en.wikipedia.org/wiki/Closure_(computer_programming)).

> NOTE: 维基百科的这篇文章对event-driven programming总结地非常好。

#### Event handlers

Main article: [Event handler](https://en.wikipedia.org/wiki/Event_handler)

##### A trivial event handler

Because the code for checking for events and the [main loop](https://en.wikipedia.org/wiki/Main_loop) are common amongst applications, many programming frameworks take care of their implementation and expect the user to provide only the code for the event handlers. 

> NOTE: programming framework always do things common

In this simple example there may be a call to an event handler called `OnKeyEnter()` that includes an argument with a string of characters, corresponding to what the user typed before hitting the ENTER key. To add two numbers, storage outside the event handler must be used. The implementation might look like below.

#### Common uses

In addition, systems such as Node.js are also event-driven.
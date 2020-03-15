# Event-driven model

在文章[Abstraction and model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model)中，我们提出了[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)，它是一个非常强大的模型，能够描述非常非常广泛的问题。在本文，我们将说明什么是event-driven model、什么是event、如何实现event-driven model。

## Event-driven model

Event-driven model需要持续不断地进行运转以监控事件源、收集event，一旦收集到了event，就进行dispatch，即根据event和event handler之间的映射关系，调用这个event对应的event handler。其实，我们可以将它看做是一个**abstract machine**。

### on

可以使用Event-driven model来进行描述的问题的一个典型的模式就是：当某个事件发生的时候，就执行某个函数。我觉得这种模式，使用JavaScript来进行描述是最最便利的。

https://www.w3schools.com/js/js_events.asp

https://storm.cis.fordham.edu/~mesterharm/2350/event.html

### Example

理解event-driven model的最最简单的例子就是linux OS kernel和hardware的交互，在工程[Linux-OS](https://dengking.github.io/Linux-OS)的文章[Linux-OS-kernel-is-event-driven](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-interaction-with-the-hardware/Linux-OS-kernel-is-event-driven/)中我们已经总结了可以使用[Event-driven model](https://dengking.github.io/Post/Abstraction/Abstraction-and-model/#event-driven-model)来描述linux OS kernel和hardware的交互。在hardware层，一旦通电，则hardware就持续不断地运转起来了，一旦触发了interrupt，则它的interrupt handler就会被执行。

### Event-driven model的组成

一个event-driven model的组成成分：

- monitor/listener，监控event
- event和event handler之间的映射关系表
- dispatcher，派发event，即按照event和event handler之间的映射关系，通知executor执行event handler
- executor，执行event handler

## What is event?

Event是一个非常概括、宽泛的概念，在Event-driven model中，非常多的行为都会产生event，我们把它称为source of event，我们可以将source of event划分为两类：

### external environment

我们以自底向上的思路来分析源自external environment的event，一个[computing system](https://dengking.github.io/Linux-OS/Architecture/Architecture-of-computing-system/)的最底层是hardware，hardware产生的[interrupt](https://en.wikipedia.org/wiki/Interrupt)，然后由OS kernel将这些interrupt“转换”为signal（现代programming language会使用exception来抽象signal）、IO（因为IO的实现是依赖于interrupt的，IO包括了非常多的内容，用户操作、网络通信等都可以看做是IO，events can represent availability of new data for reading a file or network stream.）event等，并通知到application process。

关于这一点，参见维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))的[Event handler](https://en.wikipedia.org/wiki/Event_(computing)#Event_handler)段。

### 程序内部

Event可能源自于external environment，也可能源自于程序之内，即程序内部将一些条件等看做event，比如condition variable。

TODO: 需要补充一些具体例子。

下面补充了维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))来进行详细说明。

### 维基百科[Event (computing)](https://en.wikipedia.org/wiki/Event_(computing))

#### [Delegate event model](https://en.wikipedia.org/wiki/Event_(computing)#Delegate_event_model)

> NOTE: 原文的这一段没有读懂。



## Event-driven programming

Event-driven programming告诉我们如何实现event-driven model。

### Event-driven programming中需要考虑的一些问题

Event-driven model实现中需要考虑的一些问题：

- 如何声明event和event handler之间的关系，显然event driven model需要记录下event和event handler之间的映射关系？

- 如何进行持续监控，即如何实现monitor？

#### Monitor：如何进行持续监控

在硬件层，只要通电后，则hardware就持续不断地运转起来了，一旦触发了interrupt，则它的interrupt handler就会被执行。在软件层，我们需要显式地使用一个[main loop](./Event-loop/Event-loop.md)来指示整个model需要不断地运转下去。

关于这一点，参见维基百科[Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)的第二段。

#### 关联event和event handler

Event-driven model肯定需要记录下event和event handler之间的映射关系，实现这种映射关系的方式是非常多的，可以显示地使用诸如map的数据结构。

下面罗列了一些实现案例：

##### Interrupt Descriptor Table

参见：工程[Linux-OS](https://dengking.github.io/Linux-OS)的[4.2-Interrupts-and-Exceptions](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/)的[4.2.3. Interrupt Descriptor Table](https://dengking.github.io/Linux-OS/Kernel/Book-Understanding-the-Linux-Kernel/Chapter-4-Interrupts-and-Exceptions/4.2-Interrupts-and-Exceptions/#423-interrupt-descriptor-table)

维基百科[Interrupt descriptor table](https://en.wikipedia.org/wiki/Interrupt_descriptor_table)



##### Dispatcher

所谓的event dispatcher是指当event发生时，event-driven model将event传递到executor，通知executor执行对应的handler。Dispatcher的实现方式是有executor决定的， 在下一节对此进行具体情况具体说明。



#### Executor：execution of event handler

如何来执行event handler？在决定如何来执行event handler的时候，开发者需要考虑如下问题：

- event handler执行的成本，此处的成本可以有多种解释，比如，它可以表示event handler执行的时长、可以表示event handler执行的资源耗费

- 并发性，同时发生的事件发生可能多，如何快速地处理这些事件呢？显然这就涉及了concurrency的问题，即并发地执行handler（event and concurrency）

所以开发者需要根据需求选择合适的实现方式。下面罗列一些执行方式：

##### Single process

###### single thread

monitor和executor位于同一个线程，这种比较适合event handler的执行成本比较小的情况。

###### multi thread

monitor和executor分别处于两个不同的线程，这种情况dispatcher的实现显然涉及到inter-thread communication。

##### Multiple process

monitor和executor分别处于两个不同的进程，这种情况dispatcher的实现显然涉及inter-process communication。

对于这种情况，可以将整体看做是一个event-driven system，也可以看做是多个event-driven system进行pipeline。



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



### Pattern

实现event-driven model的一些pattern。

#### [Message queue](https://en.wikipedia.org/wiki/Message_queue)

#### [Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern)

#### [Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern)

一个典型的例子就是redis的pub/sub

在这种情况下，pub需要注册回调函数，用于指定当收到信息时，需要执行的动作。这非常类似于signal handler。

#### Actor model

参见[Actor model](https://en.wikipedia.org/wiki/Actor_model)



#### [Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern) 

#### [Proactor pattern](https://en.wikipedia.org/wiki/Proactor_pattern)

#### [Messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern)



### Framework/library

参见[Event-library.md](./Library/Event-library.md)。
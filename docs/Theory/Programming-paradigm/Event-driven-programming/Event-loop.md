# [Event loop](https://en.wikipedia.org/wiki/Event_loop)



In [computer science](https://en.wikipedia.org/wiki/Computer_science), the **event loop**, **message dispatcher**, **message loop**, **message pump**, or **run loop** is a programming construct that **waits for** and **dispatches** [events](https://en.wikipedia.org/wiki/Event-driven_programming) or [messages](https://en.wikipedia.org/wiki/Message_Passing_Interface) in a [program](https://en.wikipedia.org/wiki/Computer_program). It works by making a request to some internal or external "event provider" (that generally [blocks](https://en.wikipedia.org/wiki/Blocking_(computing)) the request until an event has arrived), and then it calls the relevant [event handler](https://en.wikipedia.org/wiki/Event_handler) ("dispatches the event"). The event-loop may be used in conjunction with a [reactor](https://en.wikipedia.org/wiki/Reactor_pattern), if the **event provider** follows the [file interface](https://en.wikipedia.org/wiki/Event_loop#File_interface), which can be selected or 'polled' (the Unix system call, not actual [polling](https://en.wikipedia.org/wiki/Polling_(computer_science))). The **event loop** almost always operates asynchronously with the message originator.

***SUMMARY***:关于上面这段话中"that generally [blocks](https://en.wikipedia.org/wiki/Blocking_(computing)) the request until an event has arrived"的解释：它是指通常情况下，为了效率起见，event provider在没有event的时候会block 掉event loop查询event的请求，redis的[BRPOP](https://redis.io/commands/brpop)和[BLPOP](https://redis.io/commands/blpop) 就是按照这种思路来实现的，在[这篇文章](https://redis.io/topics/data-types-intro#blocking-operations-on-lists)中对这些进行了详细介绍。

其实，从这里也可以看出block和asynchronous之间的差异所在。

When the event loop forms the central [control flow](https://en.wikipedia.org/wiki/Control_flow) construct of a program, as it often does, it may be termed(称之为) the **main loop** or **main event loop**. This title is appropriate, because such an **event loop** is at the highest level of control within the program.



## Message passing

Message pumps are said to 'pump' messages from the program's [message queue](https://en.wikipedia.org/wiki/Message_queuing_service) (assigned and usually owned by the underlying operating system) into the program for processing. In the strictest sense, an event loop is one of the methods for implementing [inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication). In fact, message processing exists in many systems, including a [kernel-level](https://en.wikipedia.org/wiki/Kernel_(computer_science)) component of the [Mach operating system](https://en.wikipedia.org/wiki/Mach_kernel). The event loop is a specific implementation technique of systems that use [message passing](https://en.wikipedia.org/wiki/Message_passing).



## Alternative designs

This approach is in contrast to a number of other alternatives:

- Traditionally, a program simply ran once, then terminated. This type of program was very common in the early days of computing, and lacked any form of user interactivity. This is still used frequently, particularly in the form of [command-line-driven](https://en.wikipedia.org/wiki/Command_line) programs. Any [parameters](https://en.wikipedia.org/wiki/Parameters) are set up in advance and passed in one go when the program starts.
- Menu-driven designs. These still may feature a main loop, but are not usually thought of as [event driven](https://en.wikipedia.org/wiki/Event-driven_programming) in the usual sense[*citation needed*]. Instead, the user is presented with an ever-narrowing set of options until the task they wish to carry out is the only option available. Limited interactivity through the menus is available.



## Usage

Due to the predominance(优势) of [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interface), most modern applications feature a **main loop**. The `get_next_message()` routine is typically provided by the **operating system**, and [blocks](https://en.wikipedia.org/wiki/Blocking_(computing)) until a message is available. Thus, the loop is only entered when there is something to process.

```pseudocode
function main
    initialize()
    while message != quit
        message := get_next_message()
        process_message(message)
    end while
end function
```

## File interface

Under [Unix](https://en.wikipedia.org/wiki/Unix), the "[everything is a file](https://en.wikipedia.org/wiki/Everything_is_a_file)" paradigm naturally leads to a **file-based event loop**. Reading from and writing to files, inter-process communication, network communication, and device control are all achieved using **file I/O**, with the target identified by a [file descriptor](https://en.wikipedia.org/wiki/File_descriptor). The [select](https://en.wikipedia.org/wiki/Select_(Unix)) and [poll](https://en.wikipedia.org/wiki/Poll_(Unix)) system calls allow a set of file descriptors to be monitored for a change of state, e.g. when data becomes available to be read.

For example, consider a program that reads from a continuously updated file and displays its contents in the [X Window System](https://en.wikipedia.org/wiki/X_Window_System), which communicates with clients over a **socket** (either [Unix domain](https://en.wikipedia.org/wiki/Unix_domain_socket) or [Berkeley](https://en.wikipedia.org/wiki/Berkeley_sockets)):

```pseudocode
main():
    file_fd = open ("logfile")
    x_fd = open_display ()
    construct_interface ()
    while changed_fds = select ({file_fd, x_fd}):
        if file_fd in changed_fds:
            data = read_from (file_fd)
            append_to_display (data)
            send_repaint_message ()
        if x_fd in changed_fds:
            process_x_messages ()
```



### Handling signals

One of the few things in Unix that does not conform to the **file interface** are **asynchronous events** ([signals](https://en.wikipedia.org/wiki/Signal_(computing))). Signals are received in [signal handlers](https://en.wikipedia.org/wiki/Signal_handler), small, limited pieces of code that run while the rest of the task is suspended; if a signal is received and handled while the task is blocking in `select()`, select will return early with [EINTR](https://en.wikipedia.org/wiki/Error_codes_in_Linux); if a signal is received while the task is [CPU bound](https://en.wikipedia.org/wiki/CPU_bound), the task will be suspended between instructions until the **signal handler** returns.

Thus an obvious way to handle signals is for **signal handlers** to set a **global flag** and have the **event loop** check for **the flag** immediately before and after the `select()` call; if it is set, handle the **signal** in the same manner as with **events on file descriptors**. Unfortunately, this gives rise to a [race condition](https://en.wikipedia.org/wiki/Race_condition): if a signal arrives immediately between checking the flag and calling `select()`, it will not be handled until `select()` returns for some other reason (for example, being interrupted by a frustrated user).

***SUMMARY*** : 上述这种思路可以参见：[Interrupting blocked read](https://stackoverflow.com/a/6249629)中给出的demo；

***SUMMARY*** : 不幸的是，这会引起竞争条件：如果信号在检查标志和调用select（）之间立即到达，则直到select（）因某些其他原因（例如被沮丧的用户中断）返回时才会处理它；这段话中所描述的race condition并没有搞清楚；

The solution arrived at by [POSIX](https://en.wikipedia.org/wiki/POSIX) is the `pselect()` call, which is similar to `select()` but takes an additional `sigmask` parameter, which describes a *signal mask*. This allows an application to mask signals in the **main task**, then remove the mask for the duration of the `select()` call such that **signal handlers** are only called while the application is [I/O bound](https://en.wikipedia.org/wiki/I/O_bound). However, implementations of `pselect()` have only recently[*when?*] become reliable; versions of Linux prior to 2.6.16 do not have a `pselect()`system call, forcing [glibc](https://en.wikipedia.org/wiki/Glibc) to emulate it via a method prone to the very same race condition `pselect()` is intended to avoid.

An alternative, more portable solution, is to convert **asynchronous events** to **file-based events** using the *self-pipe trick*,[[1\]](https://en.wikipedia.org/wiki/Event_loop#cite_note-1) where "a signal handler writes a byte to a pipe whose other end is monitored by `select()` in the main program".[[2\]](https://en.wikipedia.org/wiki/Event_loop#cite_note-2) In [Linux kernel](https://en.wikipedia.org/wiki/Linux_kernel) version 2.6.22, a new system call `signalfd()` was added, which allows receiving signals via a **special file descriptor**.



***SUMMARY*** :参见《the linux program interface 》的 63.5.2 The Self-Pipe Trick

# [Is an event loop just a for/while loop with optimized polling?](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling)

***SUMMARY***:看了这么多，发现提问者的最终意图是想要知道event loop是如何实现的，提问者认为event loop的实现方式是类似polling的；根据下面的回答，显然答案是No。答案[A4](#A4)是非常直接的给出了答案；它引用了Wikipedia的上的[Polling](https://en.wikipedia.org/wiki/Polling_(computer_science))文章，在这篇文章中，有下面的一段话：

> Although not as wasteful of [CPU](https://en.wikipedia.org/wiki/CPU) cycles as busy waiting, this is generally not as efficient as the alternative to polling, [interrupt](https://en.wikipedia.org/wiki/Interrupt)-driven I/O.

这段话中的 [interrupt](https://en.wikipedia.org/wiki/Interrupt)-driven I/O正是event loop所采用的。

I'm trying to understand what an **event loop** is. Often the explanation is that in an event loop, you do something until you're **notified** that an event has occurred. You then handle the event and continue doing what you were doing before.

***keyword***：notify

To map the above definition with an example. I have a server which 'listens' in a event loop, and when a socket connection is detected, the data from it gets read and displayed, after which the server resumes/starts listening as it did before.

------

However, this event happening and us getting notified 'just like that' are to much for me to handle. You can say: "It's not 'just like that' you have to register an event listener". But what's an **event listener** but a function which for some reason isn't returning. Is it in it's own loop, waiting to be notified when an event happens? Should the **event listener** also register an event listener? Where does it end?

------

Events are a nice abstraction to work with, however just an abstraction. I believe that in the end, **polling is unavoidable**. Perhaps we are not doing it in our code, but the lower levels (the programming language implementation or the OS) are doing it for us.

***SUMMARY***:从上面这段话可以看出，提问者的最终意图是想要知道event loop到底是如何实现的。

It basically comes down to the following pseudo code which is running somewhere **low** enough so it doesn't result in busy waiting:

```pseudocode
while(True):
    do stuff
    check if event has happened (poll)
    do other stuff
```

------

This is my understanding of the whole idea, and I would like to hear if this is correct. I'm open in accepting that the whole idea is fundamentally wrong, in which case I would like the correct explanation.

***COMMENTS*** 

- Event systems are implementations of the [Observer pattern](http://en.wikipedia.org/wiki/Observer_pattern). Understanding the pattern should cement your understanding of events. No polling is required. – [Steven Evers](https://softwareengineering.stackexchange.com/users/1554/steven-evers) [Oct 18 '13 at 20:21](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling#comment425067_214889)

- 

  Ultimately yes, even if language constructs abstract it away. – [GrandmasterB](https://softwareengineering.stackexchange.com/users/897/grandmasterb) [Oct 18 '13 at 20:30](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling#comment425072_214889)

- 

  @SteveEvers In your wiki link. What is the `EventSource` doing if not polling the keyboard input? – [TheMeaningfulEngineer](https://softwareengineering.stackexchange.com/users/102293/themeaningfulengineer) [Oct 18 '13 at 20:36](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling#comment425077_214889)

- 

  @Alan: It could be doing anything, but for keyboard input specifically there do exist APIs to register your application as listening to keyboard events, which you can then use as your eventsource with no polling involved. Of course, if you go down far enough, USB is *always* polling, but let's assume we're using a PS/2 keyboard, which is interrupt driven, and then you have a keyboard input stack based on events with zero polling.– [Phoshi](https://softwareengineering.stackexchange.com/users/10502/phoshi) [Oct 25 '13 at 8:25](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling#comment426616_214889)

- 

  I've had this questions since years, but I can't tell why I never bothered to ask it. Thanks, I am now satisfied with the understanding @Karl Bielefeldt has enlightened me with. – [0xc0de](https://softwareengineering.stackexchange.com/users/47219/0xc0de) [Dec 2 '18 at 5:03](https://softwareengineering.stackexchange.com/questions/214889/is-an-event-loop-just-a-for-while-loop-with-optimized-polling#comment842138_214889)

## [A1](https://softwareengineering.stackexchange.com/a/214895)

Most **event loops** will suspend if there are no events ready(没有event就绪，则event loop就会被挂起), which means the operating system will not give the task any execution time until an event happens.

***SUMMARY***:这段话就直接点明了event loop和polling之间的差异所在。

Say the event is a key being pressed. You might ask if there's a loop somewhere in the operating system checking for keypresses. The answer is no. Keys being pressed generate an [interrupt](http://en.wikipedia.org/wiki/Interrupt), which is handled asynchronously by the hardware. Likewise for timers, mouse movements, a packet arriving, etc.

***SUMMARY***:interrupt 是绕开loop的一种非常好的方式，这就是实现event loop的一种非常好的方式。

In fact, for most operating systems, polling for events is the abstraction（也就是说实现可能并非如此）. The hardware and OS handle events asynchronously and put them in a queue that can be polled by applications. You only really see true polling at the hardware level in embedded systems, and even there not always.



**讨论：**

Isn't an interrupt a change of voltage on a wire? Can that trigger an event by itself or must we poll the pin for the voltage value? – [TheMeaningfulEngineer](https://softwareengineering.stackexchange.com/users/102293/themeaningfulengineer) 

That can trigger an event by itself. The processor is designed that way. In fact a processor can be woken up from sleep by an interrupt. – [Karl Bielefeldt](https://softwareengineering.stackexchange.com/users/3965/karl-bielefeldt) 

I am confused with the statement `Most event loops will block`. How does this fit into the "the event loop paradigm, opposed to using threads, uses nonblocking asynchronous calls"? – [TheMeaningfulEngineer](https://softwareengineering.stackexchange.com/users/102293/themeaningfulengineer) 

If you look at event loops for a library like GTK+, they check for new events then call event handlers in a loop, but if there aren't any events, they block on a semaphore or a timer or something. Individual developers make their own **event loops** that don't block on an empty event queue, but the widely-used libraries all block. Event loops are too inefficient otherwise. – [Karl Bielefeldt](https://softwareengineering.stackexchange.com/users/3965/karl-bielefeldt) 

***SUMMARY***:上面这段话指出在某些情况下，如empty queue，event loop将被block，显然这就解释了`Most event loops will block`；至于 How does this fit into the "the event loop paradigm, opposed to using threads, uses nonblocking asynchronous calls"，我想这个问题中所描述的是指，当event loop被通知某个event的时候，它执行event handler是采用的nonblocking asynchronous calls。



## A2

Event systems are implementations of the [Observer pattern](http://en.wikipedia.org/wiki/Observer_pattern). Understanding the pattern should cement your understanding of events. No polling is required. – [Steven Evers](https://softwareengineering.stackexchange.com/users/1554/steven-evers)

## [A3](https://softwareengineering.stackexchange.com/a/214897)

I think of an **event listener** not as a function running its own loop, but as a relay race（接力比赛） with the first runner waiting for the starting gun. A significant reason for using **events** instead of **polling** is that they are more efficient with CPU cycles. Why? Look at it from the hardware up (rather than the source code down).

Consider a Web server. When your server calls `listen()` and blocks, your code is taking its place as a relay runner. When the first packet of a new connection arrives, the network card **starts** the race by **interrupting**（中断） the operating system. The OS runs an **interrupt service routine** (ISR) that grabs the packet. The ISR passes the baton（接力棒） to a higher-level routine that establishes the connection. Once the connection is alive, that routine passes the baton to `listen()`, which passes the baton up to your code. At that point, you can do what you want with the connection. For all we know, between races each relay runner could be going to the pub. A strength of the event abstraction is that your code doesn't have to know or care.

***SUMMARY***:上面对触发式的比喻比较形象；感觉这非常像经典的Unix网络编程模型。

Some operating systems include event-handling code that runs its portion of the race, hands off the baton, then loops back to its starting point to wait for the next race to start. In that sense, event handling is optimized polling in lots of concurrent loops. However, there is always an outside **trigger** that kicks off the process. The event listener is not a function that isn't returning, but a function that is waiting for that external trigger before it runs. Rather than:

```p
while(True):
    do stuff
    check if event has happened (poll)
    do other stuff
```

I think of this as:

```pseudocode
on(some event):    //I got the baton
     do stuff
     signal the next level up    //Pass the baton
```

and between the `signal` and the next time the handler runs, there is conceptually no code running or looping.

***SUMMARY***:对比上面两段伪代码，on可以翻译为“当”，这是非常契合interrupt的；

## [A4](https://softwareengineering.stackexchange.com/a/215529)

No. It is not "optimized polling." [An event-loop uses interrupt-driven I/O instead of polling.](http://en.wikipedia.org/wiki/Polling_%28computer_science%29)

While, Until, For, etc. loops are polling loops.

"Polling" is the process of repeatedly checking something. Since the loop code executes continuously, and *because* it is a small, "tight" loop, there is little time for the processor to switch tasks and do anything else. Almost all "hangs," "freezes," "lockups" or whatever you want to call it when the computer becomes unresponsive, are the manifestation（展示） of code being stuck in an unintended polling loop. Instrumentation will show 100% CPU usage.

**Interrupt-driven event loops are far more efficient than polling loops. Polling is an extremely wasteful use of CPU cycles so every effort is made to eliminate or minimize it.**



However, to optimize code quality, most languages try to use the **polling loop paradigm** as closely as possible for event handing commands since they serve functionally similar purposes within a program（因为它们在程序中提供功能上类似的用途）. Thus, with polling being the more familiar way to wait for a keypress or something, it is easy for the inexperienced（不熟练的） to use it and wind up with a program that may run fine by itself（最终使用可能自行运行的程序）, but nothing else works while it's running. It has "taken over" the machine.

As explained in other answers, in **interrupt-driven event handing**, essentially a "flag" is set within the CPU and the process is "suspended" (not allowed to run) until that flag is changed by some other process (such as the keyboard driver changing it when the user has pressed a key). If the flag is an actual **hardware condition** such as a line being "pulled high," it's called an "interrupt" or "hardware interrupt." Most however, are implemented as just a memory address on the CPU or in main memory (RAM) and are called "semaphores."

**Semaphores** can be changed under software control and so can provide a very fast, simple signalling mechanism between software processes.

Interrupts, however, can only be changed by hardware. The most ubiquitous（普遍存在的） use of interrupts is the one triggered at regular intervals by the internal clock chip. One of the countless kinds of software actions activated by clock interrupts, is the changing of semaphores.

I've left out a lot but had to stop somewhere. Please ask if you need more details.


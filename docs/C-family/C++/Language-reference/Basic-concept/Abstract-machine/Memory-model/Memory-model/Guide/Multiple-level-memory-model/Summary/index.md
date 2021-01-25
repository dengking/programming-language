# 关于本章

本章概述C++ memory model，主要参考:

1、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)

3、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

## 三种还是四种memory model?

可以看到:

一、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

有如下三种memory model:

1 Sequential consistency

2 Acquire-release

3 Relaxed

> NOTE: 由强到弱

二、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)  # [A](https://www.zhihu.com/question/24301047/answer/85844428) 

有如下四种memory model:

\1. Relaxed ordering

\2. Release -- acquire

\3. Release -- consume

\4. **Sequential consistency**

> NOTE: 有弱到强

三、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

有如下三种:

1 Sequential consistency

2 Acquire-release

3 Relaxed

> NOTE: 由强到弱

四、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)

有四种:

1、[Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 

2、[Release-Consume ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Consume_ordering) 

3、[Release-Acquire ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering) 

4、[Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Sequentially-consistent_ordering) 

## Memory model的本质

abstraction，design to an abstraction



### C++ memory ordering

C++ memory ordering**所限制的一个线程内的指令的执行顺序**。

参见:

1、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492)

这篇文章对C++ memory ordering的分析是非常深刻的。

2、stackoverflow [What do each memory_order mean?](https://stackoverflow.com/questions/12346487/what-do-each-memory-order-mean)



## TODO `memory_ordering` and visibility

stackoverflow [Does seq_cst ordering guarantee immediate visibility?](https://stackoverflow.com/questions/14846494/does-seq-cst-ordering-guarantee-immediate-visibility)
# 综述C++ multiple level memory model

本节标题的含义是: 多层次的memory model，它能够帮助我们建立对C++ memory model高屋建瓴的认识。

关于各种memory model的描述；

本章概述C++ memory model，主要参考:

1、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)

3、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

## 三种还是四种还是六种?

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

在 `Explanation` 段中给出了四种:

1、[Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 

2、[Release-Consume ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Consume_ordering) 

3、[Release-Acquire ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering) 

4、[Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Sequentially-consistent_ordering) 

在 `Constants`中给出了六种:

1、`memory_order_relaxed`

2、`memory_order_consume`

3、`memory_order_acquire`

4、`memory_order_release`

5、`memory_order_acq_rel`

6、`memory_order_seq_cst`




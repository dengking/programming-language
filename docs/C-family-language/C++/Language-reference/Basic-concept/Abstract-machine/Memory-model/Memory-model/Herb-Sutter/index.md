# Herb Sutter on C++ Memory Model



## herbsutter [atomic Weapons: The C++ Memory Model and Modern Hardware](https://herbsutter.com/2013/02/11/atomic-weapons-the-c-memory-model-and-modern-hardware/)

Most of the talks I gave at [*C++ and Beyond 2012*](http://cppandbeyond.com/) last summer are already online at Channel 9. Here are two more.

This is a two-part talk that covers the C++ memory model, how locks and atomics and fences interact and map to hardware, and more. 

> NOTE: 这其实触及到了implementation

Even though we’re talking about C++, much of this is also applicable to Java and .NET which have similar memory models, but not all the features of C++ (such as relaxed atomics).

> NOTE: 这三门语言使用了类似的memory model

Note: This is about the basic structure and tools, not how to write lock-free algorithms using atomics. That next-level topic may be on deck for this year’s [*C++ and Beyond* in December](http://cppandbeyond.com/2013/02/11/cb-2013-dates-finalized-december-9-12-2013/), we’ll see…

> NOTE: 理解memory model是学习lock free programming的前提

1、[**Part 1:** Optimizations, races, and the memory model; acquire and release ordering; mutexes vs. atomics vs. fences](http://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-1-of-2)



2、[**Part 2:** Restrictions on compilers and hardware (incl. common bugs); code generation and performance on x86/x64, IA64, POWER, ARM, and more; relaxed atomics; volatile](http://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-2-of-2)

> NOTE: 上诉两个都是链接到的channel9

This session in one word: ***Deep.***

It’s a session that includes topics I’ve publicly said for years is Stuff You Shouldn’t Need To Know and I Just Won’t Teach, but it’s becoming achingly(极其) clear that people do need to know about it. Achingly, heartbreakingly clear, because some hardware incents(刺激) you to pull out the big guns to achieve top performance, and C++ programmers just are so addicted to full performance that they’ll reach for the big red levers with the flashing warning lights. Since we can’t keep people from pulling the big red levers, we’d better document the A to Z of what the levers actually do, so that people don’t [*SCRAM*](http://en.wikipedia.org/wiki/Scram) unless they really, really, really meant to.

> NOTE: 翻译: 这堂课包括了我多年来一直在公开场合说的一些话题，这些话题你们不需要知道，我也不会教，但人们确实需要知道，这一点越来越清楚，令人心痛。
>
> 上面这段话的大致意思是: C++ programmer为了充分发挥hardware的power，这就促使作者写作本文，帮助programmer使用C++来充分发挥hardware的power。

### **Topics Covered:**

1、**The facts:** The **C++11 memory model** and what it requires you to do to make sure your code is correct and stays correct. We’ll include clear answers to several FAQs: “**how do the compiler and hardware cooperate to remember how to respect these rules?**”, “what is a race condition?”, and the ageless(永恒的) one-hand-clapping question “how is a race condition like a debugger?”

> NOTE: 上面这段话中的"**how do the compiler and hardware cooperate to remember how to respect these rules?**"，其实触及了implementation；
>
> 最后一句话: "how is a race condition like a debugger?"的含义是什么？

2、**The tools:** The deep interrelationships and fundamental tradeoffs among mutexes, atomics, and fences/barriers. I’ll try to convince you why standalone memory barriers are bad, and why barriers should always be associated with a specific load or store.

> NOTE: 告诉我们如何使用mutexes, atomics, and fences/barriers

3、**The unspeakables:** I’ll grudgingly(不情愿地) and reluctantly(嫌恶地) talk about the Thing I Said I’d Never Teach That Programmers Should Never Need To Now: relaxed atomics. Don’t use them! If you can avoid it. But here’s what you need to know, even though it would be nice if you didn’t need to know it.

3、**The rapidly-changing hardware reality:** How locks and atomics map to hardware instructions on ARM and x86/x64, and throw in POWER and Itanium for good measure – and I’ll cover how and why the answers are actually different last year and this year, and how they will likely be different again a few years from now. We’ll cover how the latest CPU and GPU hardware memory models are rapidly evolving, and how this directly affects C++ programmers.

## TODO

1、herbsutter [Reader Q&A: Acquire/release and sequential consistency](https://herbsutter.com/2013/10/28/reader-qa-acquirerelease-and-sequential-consistency/)



2、channel9 [C++ and Beyond 2012: Herb Sutter - atomic<> Weapons, 1 of 2](https://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-1-of-2)

https://www.bilibili.com/video/BV17t4y117nS/?spm_id_from=333.788.videocard.0



3、channel9 [C++ and Beyond 2012: Herb Sutter - atomic<> Weapons, 2 of 2](https://channel9.msdn.com/Shows/Going+Deep/Cpp-and-Beyond-2012-Herb-Sutter-atomic-Weapons-2-of-2)

https://www.bilibili.com/video/av54796461?from=search




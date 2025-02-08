# [Martin Thompson](https://github.com/mjpt777?tab=repositories) 

[oreilly Martin Thompson](https://www.oreilly.com/people/martin-thompson/)

Martin is a Java Champion with over 2 decades of experience building complex and high-performance computing systems. He is most recently known for his work on Aeron and SBE. Previously at LMAX he was the co-founder and CTO when he created the Disruptor. Prior to LMAX Martin worked for Betfair, three different content companies wrestling with the world largest product catalogues, and was a lead on some of the most significant `C++` and Java systems of the 1990s in the automotive and finance domains. He blogs at mechanical-sympathy.blogspot.com, and can be found giving training courses on performance and concurrency when he is not cutting code to make systems better. 



## What is Mechanical Sympathy

### 百度AI [Mechanical Sympathy](https://www.baidu.com/s?word=Mechanical Sympathy&rsv_dl=DQA_PC_COPY) 

**[Mechanical Sympathy](https://www.baidu.com/s?rsv_dl=re_dqa_generate&sa=re_dqa_generate&wd=Mechanical Sympathy&rsv_pq=a5baa2d102531933&oq=Mechanical Sympathy&rsv_t=765as1Pd6+qivYfTxDgbKl1aMKTFpHDReQgDL2X2v8MuBBpEA25XwaD2UueF7++si3HA&tn=baiduhome_pg&ie=utf-8)**‌是一个描述**底层硬件**如何运作以及如何与**软件**协同工作的概念。这个短语直译为“机械顺从性”，但在这里，“sympathy”并不是指同情，而是指共鸣、一致的意思。‌**Martin Thompson**‌是这一概念的主要倡导者，他在多个场合提到这一概念，强调软件开发应与硬件的运作方式相协调，以提高性能和效率‌12。

#### 历史背景和应用场景

Martin Thompson在2020年QCon技术大会上首次公开谈论了‌**[LMAX](https://www.baidu.com/s?rsv_dl=re_dqa_generate&sa=re_dqa_generate&wd=LMAX&rsv_pq=a5baa2d102531933&oq=Mechanical Sympathy&rsv_t=765as1Pd6+qivYfTxDgbKl1aMKTFpHDReQgDL2X2v8MuBBpEA25XwaD2UueF7++si3HA&tn=baiduhome_pg&ie=utf-8)**‌和‌**[Disruptor](https://www.baidu.com/s?rsv_dl=re_dqa_generate&sa=re_dqa_generate&wd=Disruptor&rsv_pq=a5baa2d102531933&oq=Mechanical Sympathy&rsv_t=765as1Pd6+qivYfTxDgbKl1aMKTFpHDReQgDL2X2v8MuBBpEA25XwaD2UueF7++si3HA&tn=baiduhome_pg&ie=utf-8)**‌，这两个项目都是由他主导的。LMAX能够在1毫秒内完成10万次交易，而现代的交易所系统已经能够将吞吐量提升至每秒100万次交易，同时延迟降低到几十微秒。这种性能的提升很大程度上得益于对硬件的优化和利用，包括CPU缓存、内存访问等‌2。

#### 具体应用实例

在LMAX的高并发系统中，‌**Business Logic Processor**‌（业务逻辑处理器）完全在内存中计算，没有使用任何持久化介质。这种设计极大地简化了并发编程的复杂性，并通过单线程处理提高了系统的稳定性和性能‌3。此外，Martin Thompson还提倡使用复制状态机、分布式日志等技术来提高交易系统的可靠性和弹性‌2。



### [AWS Well-Architected Framework](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/index.en.html) [Concepts](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.concepts.wa-concepts.en.html) # [Mechanical Sympathy](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.concept.mechanical-sympathy.en.html)

Mechanical sympathy is when you use a tool or system with an understanding of how it operates best.

*You don't have to be an engineer to be be a racing driver, but you do have to have Mechanical Sympathy.* Jackie Stewart, racing driver

When you understand how a system is designed to be used, you can align with the design to gain optimal [performance](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.pillar.performance.en.html). For example, if you know that a certain type of [memory](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.concept.memory.en.html) is more efficient when addresses are multiples of a factor, you can optimize your [performance](https://wa.aws.amazon.com/wellarchitected/2020-07-02T19-33-23/wat.pillar.performance.en.html) by using data structure alignment.

Resources: [The LMAX Architecture and Mechanical Sympathy](https://martinfowler.com/articles/lmax.html?ref=wellarchitected#QueuesAndTheirLackOfMechanicalSympathy)

## Posts

[infoq Evolution of Financial Exchange Architectures](https://www.infoq.com/presentations/financial-exchange-architecture/)

> 翻译: [zhihu 交易系统架构演进](https://zhuanlan.zhihu.com/p/706028592)

[martinfowler The LMAX Architecture](https://martinfowler.com/articles/lmax.html?ref=wellarchitected#QueuesAndTheirLackOfMechanicalSympathy)

[个人网站 Mechanical Sympathy](https://mechanical-sympathy.blogspot.com/)
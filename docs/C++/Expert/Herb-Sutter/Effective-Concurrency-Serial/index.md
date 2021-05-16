# Effective Concurrency Serial

http://www.gotw.ca/publications/

## 文章

> NOTE: 
>
> 1、下面这些内容是从[Effective Concurrency: Prefer Using Active Objects Instead of Naked Threads](https://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)中摘抄过来的
>
> 2、可以看到，Herb Sutter的Effective Concurrency Serial覆盖了concurrency programming的方方面面
>
> 3、总得来说，有如下原则:
> a、使用good abstraction

I hope you enjoy it. Finally, here are links to previous Effective Concurrency columns: 

1 [The Pillars of Concurrency](http://www.ddj.com/hpc-high-performance-computing/200001985) (Aug 2007)

给出了framework、model，非常好的guide。

2 [How Much Scalability Do You Have or Need?](http://www.ddj.com/hpc-high-performance-computing/201202924) (Sep 2007)

如何设计thread model以充分发挥scalability，这是对 "Pillar 2: Throughput and Scalability Via Concurrent Collections"的展开。



3 [Use Critical Sections (Preferably Locks) to Eliminate Races](http://ddj.com/cpp/201804238) (Oct 2007)

4 [Apply Critical Sections Consistently](http://www.ddj.com/hpc-high-performance-computing/202401098) (Nov 2007)

5 [Avoid Calling Unknown Code While Inside a Critical Section](http://ddj.com/architect/202802983) (Dec 2007)

6 [Use Lock Hierarchies to Avoid Deadlock](http://www.ddj.com/hpc-high-performance-computing/204801163) (Jan 2008)

前面都是描述如何lock的。

---



7 [Break Amdahl’s Law!](http://www.ddj.com/cpp/205900309) (Feb 2008)

8 [Going Superlinear](http://www.ddj.com/hpc-high-performance-computing/206100542) (Mar 2008)

9 [Super Linearity and the Bigger Machine](http://www.ddj.com/hpc-high-performance-computing/206903306) (Apr 2008)

10 [Interrupt Politely](http://ddj.com/architect/207100682) (May 2008)



11 [Maximize Locality, Minimize Contention](http://ddj.com/architect/208200273) (Jun 2008)



12 [Choose Concurrency-Friendly Data Structures](http://www.ddj.com/hpc-high-performance-computing/208801371) (Jul 2008)



13 [The Many Faces of Deadlock](http://www.ddj.com/hpc-high-performance-computing/209900973) (Aug 2008)



14 [Lock-Free Code: A False Sense of Security](http://www.ddj.com/cpp/210600279) (Sep 2008)



15 [Writing Lock-Free Code: A Corrected Queue](http://www.ddj.com/hpc-high-performance-computing/210604448) (Oct 2008)



16 [Writing a Generalized Concurrent Queue](http://www.ddj.com/cpp/211601363) (Nov 2008)



17 [Understanding Parallel Performance](http://www.ddj.com/cpp/211800538) (Dec 2008)



18 [Measuring Parallel Performance: Optimizing a Concurrent Queue](http://www.ddj.com/hpc-high-performance-computing/212201163) (Jan 2009)



19 [volatile vs. volatile](http://www.ddj.com/hpc-high-performance-computing/212701484) (Feb 2009)



20 [Sharing Is the Root of All Contention](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=214100002) (Mar 2009)

shared data

21 [Use Threads Correctly = Isolation + Asynchronous Messages](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=215900465) (Apr 2009)

22 [Use Thread Pools Correctly: Keep Tasks Short and Nonblocking](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=216500409) (Apr 2009)

23 [Eliminate False Sharing](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=217500206) (May 2009)

24 [Break Up and Interleave Work to Keep Threads Responsive](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=217801299) (Jun 2009)

25 [The Power of “In Progress”](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=218401447) (Jul 2009)

26 [Design for Manycore Systems](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=219200099) (Aug 2009)

27 [Avoid Exposing Concurrency – Hide It Inside Synchronous Methods](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=220600388) (Oct 2009)

28 [Prefer structured lifetimes – local, nested, bounded, deterministic](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=221601309) (Nov 2009)

29 [Prefer Futures to Baked-In “Async APIs”](http://www.ddj.com/go-parallel/article/showArticle.jhtml?articleID=222301165) (Jan 2010)

30 [Associate Mutexes with Data to Prevent Races](http://www.drdobbs.com/go-parallel/article/showArticle.jhtml?articleID=224701827) (May 2010)

31 [Prefer Using Active Objects Instead of Naked Threads](http://www.drdobbs.com/go-parallel/article/showArticle.jhtml;jsessionid=JM3XD1KM22SCRQE1GHPSKH4ATMY32JVN?articleID=225700095) (June 2010)
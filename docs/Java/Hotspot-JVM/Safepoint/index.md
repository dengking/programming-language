# Safepoint

[cnblogs JVM系列之:再谈java中的safepoint](https://www.cnblogs.com/flydean/p/13450929.html) 



## [stackoverflow Java GC safepoint](https://stackoverflow.com/questions/19201332/java-gc-safepoint) 

[A](https://stackoverflow.com/a/19201753/23877800) 

The exact definition and implementation of a `safepoint` changes from one VM implementation to another, but considering **Hotspot VM**, you can find a nice definition in: [Safepoints in HotSpot JVM](http://blog.ragozin.info/2012/10/safepoints-in-hotspot-jvm.html).

[HotSpot glossary](http://openjdk.java.net/groups/hotspot/docs/HotSpotGlossary.html) says:

> A point during program execution at which all GC roots are known and all heap object contents are consistent. From a global point of view, all threads must block at a **safepoint** before the GC can run.

Typically, the **safepoint** is implemented by JVM injecting(注入) a **safepoint check**(检查) into a method, most call sites qualify as safepoints - when reaching the **safepoint check**(检查), the thread will check if the **safepoint** is required (e.g. FullGC is scheduled), if yes, then the thread blocks(阻塞). When all the threads in the VM block(阻塞), you have reached the **safepoint** where all the the objects in the VM are perfectly reachable. Then, the VM operation that requested the safepoint is performed (e.g. a FullGC), after that the threads are resumed.

> NOTE: HotSpot VM的 **safepoint**，即它是一个全局的safepoint

Check for the list of VM operations requiring a safepoint : [Safety First: Safepoints](http://jpbempel.blogspot.com/2013/03/safety-first-safepoints.html).

You can study safepoint behavior in Hotspot by using `-XX:+PrintSafepointStatistics –XX:PrintSafepointStatisticsCount=1`.



## [OpenJDK # HotSpot Glossary of Terms # safepoint](https://openjdk.org/groups/hotspot/docs/HotSpotGlossary.html)

A point during program execution at which all GC roots are known and all heap object contents are consistent. 

From a **global** point of view, all threads must block at a safepoint before the GC can run. (As a special case, threads running JNI code can continue to run, because they use only handles. During a safepoint they must block instead of loading the contents of the handle.) 

From a **local** point of view, a safepoint is a distinguished point in a block of code where the executing thread may block for the GC. Most call sites qualify as safepoints. 

There are strong invariants which hold true at every safepoint, which may be disregarded at non-safepoints. Both compiled Java code and `C/C++` code be optimized between safepoints, but less so across safepoints. The JIT compiler emits a GC map at each safepoint. `C/C++` code in the VM uses stylized macro-based conventions (e.g., TRAPS) to mark potential safepoints.



## gpt-4o-mini jvm SafePoint

In the context of the Java Virtual Machine (JVM), a **SafePoint** is a specific point during the execution of a Java program where the **JVM** can safely perform certain operations, such as **garbage collection (GC)** or **thread stack walking**. SafePoints are crucial for the efficient management of memory and resources in a multi-threaded environment.

### Key Concepts of SafePoints in JVM

1. **Definition**: A **SafePoint** is a location in the program's execution where all threads can be paused safely. This allows the JVM to perform operations that require a consistent view of the program's state, such as garbage collection.

2. **Garbage Collection**: During garbage collection, the JVM needs to ensure that all threads are stopped at a **SafePoint** so that it can accurately determine which objects are still in use and which can be collected. If threads were allowed to continue executing during garbage collection, the JVM might not have an accurate view of the heap, leading to potential memory leaks or crashes.

3. **Thread Management**: SafePoints are also used for thread management tasks, such as **stack walking**, which involves inspecting the call stack of threads. This is important for debugging, profiling, and monitoring applications.

4. **Implementation**: The JVM uses various mechanisms to implement SafePoints. For example, it may insert specific bytecode instructions at certain locations in the compiled code to mark **SafePoints**. When the JVM needs to perform a GC or other operations, it can signal all threads to reach the **nearest SafePoint**.

5. **Performance Considerations**: The presence of SafePoints can impact the performance of Java applications. If SafePoints are too frequent or if threads take too long to reach them, it can lead to increased pause times during garbage collection. JVM implementations strive to balance the frequency of SafePoints with the overall performance of the application.

6. **Types of SafePoints**: There are generally two types of SafePoints:
   - **Voluntary(自愿、自主) SafePoints**: These are points where the application code can be paused voluntarily, such as at the end of a method or loop.
   - **Involuntary SafePoints**: These are points where the JVM forces a pause, typically during garbage collection or other critical operations.

### Conclusion

SafePoints are an essential part of the JVM's architecture, enabling safe and efficient memory management in a multi-threaded environment. Understanding how SafePoints work can help developers optimize their Java applications and troubleshoot performance issues related to garbage collection and thread management.



## [ragozin Safepoints in HotSpot JVM](https://blog.ragozin.info/2012/10/safepoints-in-hotspot-jvm.html) 

#### When safepoints are used?

Below are few reasons for HotSpot JVM to initiate a safepoint:

- Garbage collection pauses
- Code deoptimization
- Flushing code cache
- Class redefinition (e.g. hot swap or instrumentation)
- **Biased lock** revocation(解除偏置)
- Various debug operation (e.g. **deadlock check** or **stacktrace dump**)



## zhihu [JVM相关 - SafePoint 与 Stop The World 全解](https://zhuanlan.zhihu.com/p/161710652)

1.GC 本身没有花多长时间，但是 JVM 暂停了很久，例如下面：

![img](https://pic4.zhimg.com/v2-25b57f4def447c93f38b407aaca5f961_1440w.jpg)

> NOTE: 上图中第一个框是JVM暂停时间、第二个框是GC时间

2.JVM 没有 GC，但是程序暂停了很久，而且这种情况时不时就出现。

这些问题一般和 **SafePoint** 还有 **Stop the World** 有关

### 什么是 SafePoint？什么是 Stop the world？他们之间有何关系？

我们先来设想下如下场景：

1. 当需要 GC 时，需要知道哪些对象还被使用，或者已经不被使用可以回收了，这样就需要每个线程的对象使用情况。
2. 对于偏向锁（Biased Lock），在[高并发](https://zhida.zhihu.com/search?content_id=124069063&content_type=Article&match_order=1&q=高并发&zhida_source=entity)时想要解除偏置，需要线程状态还有获取锁的线程的精确信息(Biased lock revocation)。
3. 对方法进行[即时编译](https://zhida.zhihu.com/search?content_id=124069063&content_type=Article&match_order=1&q=即时编译&zhida_source=entity)优化（OSR栈上替换），或者反优化（bailout栈上反优化），这需要线程究竟运行到方法的哪里的信息。

对于这些操作，都需要线程的各种信息，例如[寄存器](https://zhida.zhihu.com/search?content_id=124069063&content_type=Article&match_order=1&q=寄存器&zhida_source=entity)中到底有啥，**堆使用信息**以及**栈方法代码信息**等等等等，并且做这些操作的时候，线程需要暂停，等到这些操作完成，否则会有并发问题。这就需要 SafePoint。

**Safepoint 可以理解成是在代码执行过程中的一些特殊位置**，当线程执行到这些位置的时候，**线程可以暂停**。在 SafePoint 保存了其他位置没有的**一些当前线程的运行信息，供其他线程读取**。这些信息包括：线程上下文的任何信息，例如对象或者非对象的内部指针等等。我们一般这么理解 SafePoint，就是线程只有运行到了 SafePoint 的位置，他的**一切状态信息，才是确定的**，也只有这个时候，才知道这个线程用了哪些内存，没有用哪些；并且，只有线程处于 SafePoint 位置，这时候对 JVM 的堆栈信息进行修改，例如回收某一部分不用的内存，线程才会感知到，之后继续运行，每个线程都有一份自己的内存使用快照，这时候其他线程对于内存使用的修改，线程就不知道了，只有再**进行到 SafePoint 的时候，才会感知**。

所以，GC 一定需要所有线程同时进入 SafePoint，并停留在那里，等待 GC 处理完内存，再让所有线程继续执。像这种**所有线程进入 SafePoint **等待的情况，就是 Stop the world（此时，突然想起[承太郎](https://zhida.zhihu.com/search?content_id=124069063&content_type=Article&match_order=1&q=承太郎&zhida_source=entity)的：食堂泼辣酱，the world！！！）。



### 为什么需要 SafePoint 以及 Stop The World？

在 SafePoint 位置**保存了线程上下文中的任何东西**，包括对象，指向对象或非对象的内部指针，在线程处于 SafePoint 的时候，**对这些信息进行修改，线程才能感知到**。所以，只有线程处于 SafePoint 的时候，才能针对线程使用的内存进行 GC，以及改变正在执行的代码，例如 OSR （On Stack Replacement，栈上替换现有代码为JIT优化过的代码）或者 Bailout（栈上替换JIT过优化代码为去优化的代码）。并且，还有一个重要的 Java 线程特性也是基于 SafePoint 实现的，那就是 `Thread.interrupt()`，**线程只有运行到 SafePoint 才知道是否 interrupted**。

为啥需要 Stop The World，有时候我们需要全局所有线程进入 SafePoint 这样才能统计出那些内存还可以回收用于 GC，，以及回收不再使用的代码清理 CodeCache，以及执行某些 Java instrument 命令或者 JDK 工具，例如 jstack 打印堆栈就需要 Stop the world 获取当前所有线程快照。

### SafePoint 如何实现的？


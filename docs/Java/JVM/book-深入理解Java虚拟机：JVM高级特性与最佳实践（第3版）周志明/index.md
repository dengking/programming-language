## 术语

字节码解释器: bytecode interpreter

https://en.wikipedia.org/wiki/Bytecode



## 1.4 Java虚拟机家族



### 1.4.1 虚拟机始祖：Sun Classic/Exact VM

即时编译器

> NOTE:
>
> 一、[Just-in-time compilation](https://en.wikipedia.org/wiki/Just-in-time_compilation) 
>
> > This may consist of [source code](https://en.wikipedia.org/wiki/Source_code) translation but is more commonly [bytecode](https://en.wikipedia.org/wiki/Bytecode) translation to [machine code](https://en.wikipedia.org/wiki/Machine_code), which is then executed directly
>
> 在Java世界往往指的是后者。



Exact VM因它使用准确式内存管理（Exact Memory Management，也可以叫Non-Conservative/Accurate Memory Management）而得名。**准确式内存管理**是指虚拟机可以知道内存中某个位 置的数据具体是什么类型。譬如内存中有一个32bit的整数123456，虚拟机将有能力分辨出它到底是一 个指向了123456的内存地址的**引用类型**还是一个数值为123456的整数，准确分辨出哪些内存是**引用类型**，这也是在**垃圾收集时**准确判断堆(heap)上的数据是否还可能被使用的前提。由于使用了**准确式内存管理**，Exact VM可以抛弃掉以前Classic VM基于句柄（Handle）的对象查找方式（原因是垃圾收集后对象将可能会被移动位置，如果地址为123456的对象移动到654321，在没有明确信息表明内存中哪些数据是引用类型的前提下，那虚拟机肯定是不敢把内存中所有为123456的值改成654321的，所以要使用句柄来保持引用值的稳定），这样每次定位对象都少了一次间接查找的开销，显著提升执行性能。

> NOTE:
>
> 一、关于 Exact VM，参见:
>
> https://blog.csdn.net/qq_41813208/article/details/108570189
>
> 二、Exact Memory Management
>
> 从上述内容可知，Exact Memory Management的特性能够为GC的实现带来便利

### 1.4.2 武林盟主：HotSpot VM

HotSpot既继承了Sun之前两款商用虚拟机的优点（如前面提到的准确式内存管理），也有许多自己新的技术优势，如它名称中的HotSpot指的就是它的**热点代码探测**技术（这里的描写带有“历史由胜利者书写”的味道，其实HotSpot与Exact虚拟机基本上是同时期的独立产品，HotSpot出现得还稍早一 些，一开始HotSpot就是基于**准确式内存管理的**，而Exact VM之中也有与HotSpot几乎一样的热点探测 技术，为了Exact VM和HotSpot VM哪个该成为Sun主要支持的虚拟机，在Sun公司内部还争吵过一场， HotSpot击败Exact并不能算技术上的胜利），HotSpot虚拟机的热点代码探测能力可以通过执行计数器 找出最具有编译价值的代码，然后通知即时编译器以方法为单位进行编译。如果一个方法被频繁调 用，或方法中有效循环次数很多，将会分别触发标准即时编译和栈上替换编译（On-Stack Replacement，OSR）行为[1]。通过编译器与解释器恰当地协同工作，可以在最优化的程序响应时间与 最佳执行性能中取得平衡，而且无须等待本地代码输出才能执行程序，即时编译的时间压力也相对减 小，这样有助于引入更复杂的代码优化技术，输出质量更高的本地代码。

> NOTE:
>
> 一、[HotSpot (virtual machine)](https://en.wikipedia.org/wiki/HotSpot_(virtual_machine)) 
>
> > **HotSpot**, released as **Java HotSpot Performance Engine**,[[1\]](https://en.wikipedia.org/wiki/HotSpot_(virtual_machine)#cite_note-HotSpot1.0PR-1) is a [Java virtual machine](https://en.wikipedia.org/wiki/Java_virtual_machine) for [desktop](https://en.wikipedia.org/wiki/Desktop_computer) and [server](https://en.wikipedia.org/wiki/Server_(computing)) computers, developed by [Sun Microsystems](https://en.wikipedia.org/wiki/Sun_Microsystems) and now maintained and distributed by [Oracle Corporation](https://en.wikipedia.org/wiki/Oracle_Corporation). Its features improved performance via methods such as [just-in-time compilation](https://en.wikipedia.org/wiki/Just-in-time_compilation) and [adaptive optimization](https://en.wikipedia.org/wiki/Adaptive_optimization). It is the de facto Java Virtual Machine, serving as the reference implementation of the [Java programming language](https://en.wikipedia.org/wiki/Java_programming_language).
>
> 二、栈上替换编译（On-Stack Replacement，OSR）
>
> [zhihu-OSR（On-Stack Replacement）是怎样的机制？](https://www.zhihu.com/question/45910849) 
>
> [graalvm-On-Stack Replacement (OSR)](https://www.graalvm.org/latest/graalvm-as-a-platform/language-implementation-framework/OnStackReplacement/) 



Oracle收购Sun以后，建立了HotRockit项目来把原来BEA JRockit中的优秀特性融合到 HotSpot之中。到了2014年的JDK 8时期，里面的HotSpot就已是两者融合的结果，HotSpot在这个过程里移除掉**永久代**，吸收了JRockit的Java Mission Control监控工具等功能。

> NOTE:
>
> 一、永久代 PermGen
>
> [baidu-永久代](https://answer.baidu.com/answer/land?params=gwPpKGAoO%2BSrdFrj5kZgbe%2Flky3%2FJSJPfo3s5NwyrVmDPIqo7v8chS%2BaT2T%2FeN3BY6wtxjOr9QNyZ%2ByB21Y7XRhQvJmZZUgJQG%2F6x1QJFthtpnTOc2d05gQYaTa6VEL%2F8p15kz8HX2iHjBZgTVE13PF8h04R9LbXJ2XOatSr9CjM46TI5bLKKpVnYJjHFqhh%2Fg3Zl7oxeiQVKqeDuwV5pw%3D%3D&from=dqa&lid=8f4f8f7b00003856&word=%E6%B0%B8%E4%B9%85%E4%BB%A3) 
>
> [stackoverflow-What does PermGen actually stand for?](https://stackoverflow.com/questions/318942/what-does-permgen-actually-stand-for) 

### 1.5.1 无语言倾向(Graal VM)

> NOTE:
>
> 一、
>
> https://www.graalvm.org/
>
> https://en.wikipedia.org/wiki/GraalVM

### 1.5.2 新一代即时编译器

对需要长时间运行的应用来说，由于经过充分预热，热点代码会被HotSpot的探测机制准确定位捕获，并将其编译为物理硬件可直接执行的机器码，在这类应用中Java的运行效率很大程度上取决于即 时编译器所输出的代码质量。

HotSpot虚拟机中含有两个即时编译器，分别是**编译耗时短但输出代码优化程度较低**的**客户端编译器**（简称为C1）以及**编译耗时长但输出代码优化质量也更高**的**服务端编译器**（简称为C2），通常它们会在分层编译机制下与解释器互相配合来共同构成HotSpot虚拟机的执行子系统（这部分具体内容将在 本书第11章展开讲解）。

自JDK 10起，HotSpot中又加入了一个全新的即时编译器：Graal编译器，看名字就可以联想到它 是来自于前一节提到的Graal VM。Graal编译器是以C2编译器替代者的身份登场的。C2的历史已经非 常长了，可以追溯到Cliff Click大神读博士期间的作品，这个由C++写成的编译器尽管目前依然效果拔群，但已经复杂到连Cliff Click本人都不愿意继续维护的程度。而Graal编译器本身就是由Java语言写成，实现时又刻意与C2采用了同一种名为“Sea-of-Nodes”的高级中间表示（High IR）形式，使其能够 更容易借鉴C2的优点。Graal编译器比C2编译器晚了足足二十年面世，有着极其充沛的后发优势，在保 持输出相近质量的编译代码的同时，开发效率和扩展性上都要显著优于C2编译器，这决定了C2编译器 中优秀的代码优化技术可以轻易地移植到Graal编译器上，但是反过来Graal编译器中行之有效的优化在 C2编译器里实现起来则异常艰难。这种情况下，Graal的编译效果短短几年间迅速追平了C2，甚至某些 测试项中开始逐渐反超C2编译器。Graal能够做比C2更加复杂的优化，如**“部分逃逸分析”（Partial Escape Analysis）**，也拥有比C2更容易使用**激进预测性优化（Aggressive Speculative Optimization）**的 策略，支持自定义的预测性假设等。

> NOTE:
>
> 一、[github-Cliff Click](https://github.com/cliffclick) 
>
> 二、
>
> [wikipedia-Sea of nodes](https://en.wikipedia.org/wiki/Sea_of_nodes)  
>
> [Sea of Nodes](https://darksi.de/d.sea-of-nodes/) 
>
> [cjovi-Sea-of-nodes 论文阅读笔记](https://www.cjovi.icu/compilers/1654.html) 
>
> 三、关于"逃逸分析"，参见: 
>
> wikipedia [Escape analysis](https://en.wikipedia.org/wiki/Escape_analysis) 
>
> 它是一种pointer analysis，它可以实现:
>
> > *Converting [heap allocations](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) to [stack allocations](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)*.
> >
> > *Breaking up objects* or *scalar replacement*.
>
> [stack allocations](https://en.wikipedia.org/wiki/Stack-based_memory_allocation) 即"栈上分配"，参见: wikipedia [Stack-based memory allocation](https://en.wikipedia.org/wiki/Stack-based_memory_allocation) 
>
> scalar replacement 即"标量替换"
>
> 上述内容在"2.2.4 Java堆（Java Heap）"章节中也有描述。

### 1.5.3 向Native迈进

而酝酿中的一 个更彻底的解决方案，是逐步开始对提前编译（Ahead of Time Compilation，AOT）提供支持。

提前编译是相对于即时编译的概念，提前编译能带来的最大好处是Java虚拟机加载这些已经预编 译成二进制库之后就能够直接调用，而无须再等待即时编译器在运行时将其编译成二进制机器码。理 论上，提前编译可以减少即时编译带来的预热时间，减少Java应用长期给人带来的“第一次运行慢”的 不良体验，可以放心地进行很多全程序的分析行为，可以使用时间压力更大的优化措施[1]。

> NOTE:
>
> 一、[wikipedia-Ahead-of-time compilation](https://en.wikipedia.org/wiki/Ahead-of-time_compilation) 

但是提前编译的坏处也很明显，它破坏了Java“一次编写，到处运行”的承诺，必须为每个不同的 硬件、操作系统去编译对应的发行包；也显著降低了Java链接过程的动态性，必须要求加载的代码在 编译期就是全部已知的，而不能在运行期才确定，否则就只能舍弃掉已经提前编译好的版本，退回到 原来的即时编译执行状态。



### 1.5.4 灵活的胖子



## 2.2 运行时数据区域

> NOTE:
>
> 一、对Java运行时数据区域的划分是依据这些内存区域是否是线程共享来进行划分的

### 2.2.1 程序计数器（Program Counter Register）

此内存区域是唯 一一个在《Java虚拟机规范》中没有规定任何OutOfMemoryError情况的区域。

> NOTE:
>
> 一、这段话如何理解？



### 2.2.2 Java虚拟机栈（Java Virtual Machine Stack）

与程序计数器一样，Java虚拟机栈（Java Virtual Machine Stack）也是线程私有的，它的生命周期 与线程相同。

> NOTE:
>
> 一、
>
> [csdn-18.JVM变量槽Slot](https://blog.csdn.net/u011069294/article/details/107099147)



### 2.2.3 本地方法栈（Native Method Stacks）

本地方法栈（Native Method Stacks）与虚拟机栈所发挥的作用是非常相似的，其区别只是虚拟机 栈为虚拟机执行Java方法（也就是字节码）服务，而本地方法栈则是为虚拟机使用到的本地（Native） 方法服务。



### 2.2.4 Java堆（Java Heap）

对于Java应用程序来说，Java堆（Java Heap）是虚拟机所管理的内存中最大的一块。Java堆是被所有线程共享的一块内存区域，在虚拟机启动时创建。此内存区域的唯一目的就是存放对象实例，Java 世界里“几乎”所有的对象实例都在这里分配内存。在《Java虚拟机规范》中对Java堆的描述是：“所有的对象实例以及数组都应当在堆上分配[1]”，而这里笔者写的“几乎”是指从实现角度来看，随着Java语言的发展，现在已经能看到些许迹象表明日后可能出现**值类型**的支持，即使只考虑现在，由于即时编译技术的进步，尤其是逃逸分析技术的日渐强大，栈上分配、标量替换[2]优化手段已经导致一些微妙 的变化悄然发生，所以说Java对象实例都分配在堆上也渐渐变得不是那么绝对了。

> NOTE:
>
> 一、"而这里笔者写的“几乎”是指从实现角度来看，随着Java语言的发展，现在已经能看到些许迹象表明日后可能出现**值类型**的支持，即使只考虑现在，由于即时编译技术的进步，尤其是逃逸分析技术的日渐强大，栈上分配、标量替换[2]优化手段已经导致一些微妙 的变化悄然发生，所以说Java对象实例都分配在堆上也渐渐变得不是那么绝对了"
>
> 1、"逃逸分析技术"，参见前面
>
> 2、"值类型"，它属于value semantic

Java堆是**垃圾收集器**管理的内存区域，因此一些资料中它也被称作“GC堆”（**Garbage Collected Heap**，幸好国内没翻译成“垃圾堆”）。从回收内存的角度看，由于现代**垃圾收集器**大部分都是基于**分代收集理论**设计的，所以Java堆中经常会出现:

“新生代”“老年代”“永久代”“Eden空间”“From Survivor空 间”“To Survivor空间”等名词，

这些概念在本书后续章节中还会反复登场亮相，在这里笔者想先说明的 是这些区域划分仅仅是一部分垃圾收集器的共同特性或者说设计风格而已，而非某个Java虚拟机具体实现的固有内存布局，更不是《Java虚拟机规范》里对Java堆的进一步细致划分。不少资料上经常写着 类似于“Java虚拟机的堆内存分为新生代、老年代、永久代、Eden、Survivor……”这样的内容。在十年 之前（以G1收集器的出现为分界），作为业界绝对主流的HotSpot虚拟机，它内部的垃圾收集器全部 都基于“经典分代” [3]来设计，需要新生代、老年代收集器搭配才能工作，在这种背景下，上述说法还 算是不会产生太大歧义。但是到了今天，垃圾收集器技术与十年前已不可同日而语，HotSpot里面也出 现了不采用分代设计的新垃圾收集器，再按照上面的提法就有很多需要商榷的地方了。

> NOTE:
>
> 一、关于 "**分代收集理论**"，参见:
>
> wikipedia [Tracing garbage collection # Generational GC (ephemeral GC)](https://en.wikipedia.org/wiki/Tracing_garbage_collection#Generational_GC_(ephemeral_GC)) 



### 2.2.5 方法区（Method Area）

方法区（Method Area）与Java堆一样，是各个线程共享的内存区域，它用于存储已被虚拟机(VM)加载的类型信息、常量、静态变量、即时编译器(JIT)编译后的代码缓存等数据。虽然《Java虚拟机规范》中把方法区描述为堆的一个逻辑部分，但是它却有一个别名叫作“非堆”（Non-Heap），目的是与Java堆区分开来。

“永久代”（Permanent Generation）。

> NOTE:
>
> 一、Method Area 不等于 Permanent Generation

## 3.1 概述

为什么我们还要去了解垃圾收集和内存分配？答案很简单：当需要排查各种内存溢出、内存泄漏问题时，当垃圾收集(GC)成为系统达到更高并发量的瓶颈时，我们就必须对这些“自动 化”的技术实施必要的监控和调节。

而Java堆和方法区这两个区域则有着很显著的不确定性：一个接口的多个实现类需要的内存可能 会不一样，一个方法所执行的不同条件分支所需要的内存也可能不一样，只有处于运行期间，我们才能知道程序究竟会创建哪些对象，创建多少个对象，这部分内存的分配和回收是动态的。垃圾收集器所关注的正是这部分内存该如何管理，本文后续讨论中的“内存”分配与回收也仅仅特指这一部分内 存。

> NOTE:
>
> 一、heap、method area都是线程共享的

## 3.2 对象已死？

在堆里面存放着Java世界中几乎所有的对象实例，垃圾收集器在对堆进行回收前，第一件事情就 是要确定这些对象之中哪些还“存活”着，哪些已经“死去”（“死去”即不可能再被任何途径使用的对 象）了。



## 3.2.2 可达性分析算法

当前主流的商用程序语言（Java、C#，上溯至前面提到的古老的Lisp）的内存管理子系统，都是通过**可达性分析（Reachability Analysis）算法**来判定对象是否存活的。这个算法的基本思路就是通过 一系列称为“GC Roots”的根对象作为起始节点集，从这些节点开始，根据引用关系向下搜索，搜索过 程所走过的路径称为“引用链”（Reference Chain），如果某个对象到GC Roots间没有任何引用链相连， 或者用图论的话来说就是从GC Roots到这个对象不可达时，则证明此对象是不可能再被使用的。



在Java技术体系里面，固定可作为GC Roots的对象包括以下几种：

- 在虚拟机栈（栈帧中的本地变量表([local variables](https://en.wikipedia.org/wiki/Java_virtual_machine))）中引用的对象，譬如各个线程被调用的方法堆栈中使用到的参数、局部变量、临时变量等。
- 在方法区(method area)中类静态属性引用的对象，譬如Java类的引用类型静态变量。

除了这些固定的**GC Roots集合**以外，根据用户所选用的**垃圾收集器**以及当前回收的内存区域不同，还可以有其他对象“临时性”地加入，共同构成完整**GC Roots集合**。譬如后文将会提到的**分代收集**和**局部回收（Partial GC）**，如果只针对Java堆中某一块区域发起垃圾收集时（如最典型的只针对**新生代的垃圾收集**），必须考虑到内存区域是虚拟机自己的实现细节（在用户视角里任何内存区域都是不可见的），更不是孤立封闭的，所以某个区域里的对象完全有可能被位于堆中其他区域的对象所引用，这时候就需要将这些关联区域的对象也一并加入GC Roots集合中去，才能保证**可达性分析**的正确性。



## 3.2.3 再谈引用

> NOTE:
>
> 原文这一段讲得一般

在JDK 1.2版之前，Java里面的引用是很传统的定义： 如果reference类型的数据中存储的数值代表的是另外一块内存的起始地址，就称该reference数据是代表某块内存、某个对象的引用。这种定义并没有什么不对，只是现在看来有些过于狭隘了，一个对象在这种定义下只有“被引用”或者“未被引用”两种状态，对于描述一些“食之无味，弃之可惜”的对象就显得无能为力。譬如我们希望能描述一类对象：当内存空间还足够时，能保留在内存之中，如果内存空间在进行垃圾收集后仍然非常紧张，那就可以抛弃这些对象——很多系统的缓存功能都符合这样的应用场景。 在JDK1.2版之后，Java对引用的概念进行了扩充，将引用分为强引用（Strongly Reference）、软引用（Soft Reference）、弱引用（Weak Reference）和虚引用（Phantom Reference）4种，这4种引用强度依次逐渐减弱。



## 3.3 垃圾收集算法

从如何判定对象消亡的角度出发，垃圾收集算法可以划分为“引用计数式垃圾收集”（Reference Counting GC）和“追踪式垃圾收集”（Tracing GC）两大类，这两类也常被称作“直接垃圾收集”和“间接垃圾收集”。

## 3.3.1 分代收集理论

当前商业虚拟机的垃圾收集器，大多数都遵循了“分代收集”（Generational Collection）[1]的理论进 行设计，分代收集名为理论，实质是一套符合大多数程序运行实际情况的经验法则，它建立在两个分 代假说之上： 

1）弱分代假说（Weak Generational Hypothesis）：绝大多数对象都是朝生夕灭的。 

2）强分代假说（Strong Generational Hypothesis）：熬过越多次垃圾收集过程的对象就越难以消亡。

这两个**分代假说**共同奠定了多款常用的垃圾收集器的一致的设计原则：收集器应该将Java堆划分出不同的区域，然后将回收对象依据其年龄（年龄即对象熬过垃圾收集过程的次数）分配到不同的区域之中存储。显而易见，如果一个区域中大多数对象都是朝生夕灭，难以熬过垃圾收集过程的话，那么把它们集中放在一起，每次回收时只关注如何保留少量存活而不是去标记那些大量将要被回收的对象，就能以较低代价回收到大量的空间；如果剩下的都是难以消亡的对象，那把它们集中放在一块， 虚拟机便可以使用较低的频率来回收这个区域，这就同时兼顾了垃圾收集的时间开销和内存的空间有效利用。

在Java堆划分出不同的区域之后，**垃圾收集器**才可以每次只回收其中某一个或者某些部分的区域 ——因而才有了“Minor GC”“Major GC”“Full GC”这样的回收类型的划分；也才能够针对不同的区域安排与里面存储对象存亡特征相匹配的垃圾收集算法——因而发展出了“标记-复制算法”、“标记-清除算法”、“标记-整理算法”等针对性的垃圾收集算法。这里笔者提前提及了一些新的名词，它们都是本章的重要角色，稍后都会逐一登场，现在读者只需要知道，这一切的出现都始于分代收集理论。

把**分代收集理论**具体放到现在的商用Java虚拟机里，设计者一般至少会把Java堆划分为**新生代 （Young Generation）**和**老年代（Old Generation）**两个区域[2]。顾名思义，在新生代中，每次垃圾收集时都发现有大批对象死去，而每次回收后存活的少量对象，将会逐步晋升到老年代中存放。如果读者有兴趣阅读HotSpot虚拟机源码的话，会发现里面存在着一些名为“*Generation”的实现， 如“DefNewGeneration”和“ParNewGeneration”等，这些就是HotSpot的“分代式垃圾收集器框架”。原本HotSpot鼓励开发者尽量在这个框架内开发新的垃圾收集器，但除了最早期的两组四款收集器之外，后来的开发者并没有继续遵循。导致此事的原因有很多，最根本的是分代收集理论仍在不断发展之中， 如何实现也有许多细节可以改进，被既定的代码框架约束反而不便。其实我们只要仔细思考一下，也很容易发现分代收集并非只是简单划分一下内存区域那么容易，它至少存在一个明显的困难：对象不是孤立的，对象之间会存在跨代引用。

3）跨代引用假说（Intergenerational Reference Hypothesis）：跨代引用相对于同代引用来说仅占极少数。

这其实是可根据前两条假说逻辑推理得出的隐含推论：存在互相引用关系的两个对象，是应该倾 向于同时生存或者同时消亡的。举个例子，如果某个新生代对象存在跨代引用，由于老年代对象难以 消亡，该引用会使得新生代对象在收集时同样得以存活，进而在年龄增长之后晋升到老年代中，这时 跨代引用也随即被消除了。


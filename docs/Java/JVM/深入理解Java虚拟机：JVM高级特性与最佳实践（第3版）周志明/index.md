

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



Oracle收购Sun以后，建立了HotRockit项目来把原来BEA JRockit中的优秀特性融合到 HotSpot之中。到了2014年的JDK 8时期，里面的HotSpot就已是两者融合的结果，HotSpot在这个过程里移除掉永久代，吸收了JRockit的Java Mission Control监控工具等功能。

> NOTE:
>
> 一、永久代 PermGen
>
> [baidu-永久代](https://answer.baidu.com/answer/land?params=gwPpKGAoO%2BSrdFrj5kZgbe%2Flky3%2FJSJPfo3s5NwyrVmDPIqo7v8chS%2BaT2T%2FeN3BY6wtxjOr9QNyZ%2ByB21Y7XRhQvJmZZUgJQG%2F6x1QJFthtpnTOc2d05gQYaTa6VEL%2F8p15kz8HX2iHjBZgTVE13PF8h04R9LbXJ2XOatSr9CjM46TI5bLKKpVnYJjHFqhh%2Fg3Zl7oxeiQVKqeDuwV5pw%3D%3D&from=dqa&lid=8f4f8f7b00003856&word=%E6%B0%B8%E4%B9%85%E4%BB%A3) 
>
> [stackoverflow-What does PermGen actually stand for?](https://stackoverflow.com/questions/318942/what-does-permgen-actually-stand-for) 



### 1.5.2 新一代即时编译器

对需要长时间运行的应用来说，由于经过充分预热，热点代码会被HotSpot的探测机制准确定位捕 获，并将其编译为物理硬件可直接执行的机器码，在这类应用中Java的运行效率很大程度上取决于即 时编译器所输出的代码质量。

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

### 1.5.3 向Native迈进

而酝酿中的一 个更彻底的解决方案，是逐步开始对提前编译（Ahead of Time Compilation，AOT）提供支持。

提前编译是相对于即时编译的概念，提前编译能带来的最大好处是Java虚拟机加载这些已经预编 译成二进制库之后就能够直接调用，而无须再等待即时编译器在运行时将其编译成二进制机器码。理 论上，提前编译可以减少即时编译带来的预热时间，减少Java应用长期给人带来的“第一次运行慢”的 不良体验，可以放心地进行很多全程序的分析行为，可以使用时间压力更大的优化措施[1]。

> NOTE:
>
> 一、[wikipedia-Ahead-of-time compilation](https://en.wikipedia.org/wiki/Ahead-of-time_compilation) 

但是提前编译的坏处也很明显，它破坏了Java“一次编写，到处运行”的承诺，必须为每个不同的 硬件、操作系统去编译对应的发行包；也显著降低了Java链接过程的动态性，必须要求加载的代码在 编译期就是全部已知的，而不能在运行期才确定，否则就只能舍弃掉已经提前编译好的版本，退回到 原来的即时编译执行状态。



### 1.5.4 灵活的胖子


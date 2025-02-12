# Java agent

[wikipedia Application performance management](https://en.wikipedia.org/wiki/Application_performance_management)

[juejin【JVM】Java agent超详细知识梳理](https://juejin.cn/post/7157684112122183693)

[oracle-The JVM Tool Interface (JVM TI): How VM Agents Work](https://www.oracle.com/technical-resources/articles/javase/jvm-tool-interface.html) 

[medium Java Agent - A powerful tool you might have missed](https://sathiyakugan.medium.com/java-agent-a-powerful-tool-you-might-have-missed-fe6a85884481) 

[jolokia JVM Agent](https://jolokia.org/reference/html/manual/agents/jvm.html) 

Java Agent探针技术介绍

## 实现方式

### Agent library based on [JVMTI](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html)

TODO:

agent library的说法是源自 [oracle-The JVM Tool Interface (JVM TI): How VM Agents Work](https://www.oracle.com/technical-resources/articles/javase/jvm-tool-interface.html) 

### [Instrument API](https://docs.oracle.com/en/java/javase/23/docs/api/java.instrument/module-summary.html)

 

## 加载agent时机/方式

参考自:

- [meituan Java 动态调试技术原理及实践](https://tech.meituan.com/2019/11/07/java-dynamic-debugging-technology.html)

- [jolokia JVM Agent](https://jolokia.org/reference/html/manual/agents/jvm.html)

- ......

### 静态加载/JVM启动时

### 动态加载/JVM运行时

dynamic attach

[csdn Java Attach机制实现原理](https://blog.csdn.net/qq_40378034/article/details/116092658)  

> 通过attach机制，可以直接attach到目标JVM进程，然后进行一些操作，比如获取内存dump、线程dump、类信息统计（比如已加载的类以及实例个数等）、**动态加载agent**、获取系统属性等

[oracle javase 9 jdk.attach](https://docs.oracle.com/javase/9/docs/api/jdk.attach-summary.html)

> Provides the API to attach to a **Java™ virtual machine**.
> 
> A tool, written in the Java Language, uses this API to attach to a target **virtual machine (VM)** and load its **tool agent** into the **target VM**. For example, a management console might have a **management agent** which it uses to obtain management information from **instrumented objects** in a **Java virtual machine**.

### [meituan Java 动态调试技术原理及实践](https://tech.meituan.com/2019/11/07/java-dynamic-debugging-technology.html)

#### What is agent?

Agent是一个运行在目标JVM的特定程序，它的职责是负责从目标JVM中获取数据，然后将数据传递给外部进程。加载Agent的时机可以是目标JVM启动之时，也可以是在目标JVM运行时进行加载

#### Agent的实现模式?

[JVMTI](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html) 是一套Native接口

在Java SE 5之前，要实现一个Agent只能通过编写Native代码来实现。

从Java SE 5开始，可以使用Java的Instrumentation接口（[java.lang.instrument](https://docs.oracle.com/en/java/javase/23/docs/api/java.instrument/module-summary.html)）来编写Agent。

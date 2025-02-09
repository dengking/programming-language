# [Java](https://docs.oracle.com/en/java/index.html) / [Java Components](https://docs.oracle.com/en/java/java-components/index.html) / [JDK Mission Control (JMC)](https://docs.oracle.com/en/java/java-components/jdk-mission-control/) 



> NOTE: 
>
> 在 [JVM相关 - SafePoint 与 Stop The World 全解](https://zhuanlan.zhihu.com/p/161710652) 中使用了JMC
>
> 它是一种可视化工具
>
> [async-profiler](https://github.com/async-profiler/async-profiler/tree/master)/[docs](https://github.com/async-profiler/async-profiler/tree/master/docs)/[JfrVisualization](https://github.com/async-profiler/async-profiler/blob/master/docs/JfrVisualization.md)
>
> > [JDK Mission Control](https://www.oracle.com/java/technologies/jdk-mission-control.html) (JMC) is a popular GUI tool to analyze JFR recordings. It has been originally developed to work in conjunction with the JDK Flight Recorder, however, async-profiler recordings are also fully compatible with JMC.



JDK Mission Control (JMC) is an advanced set of tools for managing, monitoring, profiling, and troubleshooting Java applications. JMC enables efficient and detailed data analysis for areas such as **code performance**, **memory**, and **latency** without introducing the **performance overhead** normally associated with profiling and monitoring tools.



## [Java](https://www.oracle.com/java/) / [Technologies](https://www.oracle.com/java/technologies/) / [JDK Mission Control](https://www.oracle.com/java/technologies/jdk-mission-control.html) 

**Java Flight Recorder** and **JDK Mission Control** together create a complete tool chain to continuously collect low level and detailed runtime information enabling after-the-fact(事后) incident analysis(事件分析). **Java Flight Recorder** is a profiling and event collection framework built into the Oracle JDK. It allows Java administrators and developers to gather detailed low level information about how the **Java Virtual Machine (JVM)** and the Java application are behaving. **JDK Mission Control** is an advanced set of tools that enables efficient and detailed analysis of the extensive of data collected by **Java Flight Recorder**. The tool chain enables developers and administrators to collect and analyze data from Java applications running locally or deployed in production environments.

> NOTE: 这段话详细论述了JFR和JMC之间的关系



## wikipedia [JDK Mission Control](https://en.wikipedia.org/wiki/JDK_Mission_Control) 

JDK Mission Control primarily consists of the following tools:

- A JFR ([JDK Flight Recorder](https://en.wikipedia.org/wiki/JDK_Flight_Recorder)) analyzer and visualizer
- A [JMX](https://en.wikipedia.org/wiki/Java_Management_Extensions) Console

There are also various plug-ins available, such as:

- A [heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) dump (hprof format) analyzer (JOverflow)



## [JDK Mission Control Usage Demonstration](https://www.oracle.com/java/technologies/jdk-mission-control.html?ytid=qytuEgVmhsI)



## [JDK Mission Control User Guide](https://docs.oracle.com/en/java/java-components/jdk-mission-control/9/user-guide/) 



## [Oracle JDK Mission Control and Java Flight Recorder: Advanced Java Diagnostics and Monitoring without Performance Overhead](https://www.oracle.com/docs/tech/java/java-mission-control-technical-brief.pdf) (White Paper)





## Example

[baeldung Monitoring Java Applications with Flight Recorder](https://www.baeldung.com/java-flight-recorder-monitoring)

[juejin Java性能优化（二）——JFR（Java Flight Recorder）使用教程](https://juejin.cn/post/6959405798556434440)

# JVM attach

[csdn Java Attach机制实现原理](https://blog.csdn.net/qq_40378034/article/details/116092658) 

[cnblogs JVM Attach实现原理剖析](https://www.cnblogs.com/yungyu16/p/13166189.html) 

[m0d9 从冰蝎antiAgent看Java Attach机制](https://m0d9.me/2021/06/21/JVM-Attach/) 

## JVM Attach API

[csdn 字节码进阶之JVM Attach API详解](https://blog.csdn.net/wangshuai6707/article/details/133848692) 

### [oracle javase 9 jdk.attach](https://docs.oracle.com/javase/9/docs/api/jdk.attach-summary.html)

Provides the API to attach to a Java™ virtual machine.

A tool, written in the Java Language, uses this API to attach to a target **virtual machine (VM)** and load its **tool agent** into the **target VM**. For example, a **management console** might have a **management agent** which it uses to obtain management information from **instrumented objects** in a Java virtual machine. If the **management console** is required to manage an application that is running in a virtual machine that does not include the **management agent**, then this API can be used to attach to the target VM and load the agent.



# [JVM Tool Interface](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html)

- inspect

- control

JVM. 

JVM TI is a two-way interface. A client of JVM TI, hereafter called an *agent*, can be notified of interesting occurrences through [events](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html#EventSection). JVM TI can query and control the application through many [functions](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html#FunctionSection), either in response to events or independent of them.

## See also

### [oracle-The JVM Tool Interface (JVM TI): How VM Agents Work](https://www.oracle.com/technical-resources/articles/javase/jvm-tool-interface.html)

The **JVM tool interface (JVM TI)** is a standard native API that allows for **native libraries** to capture events and control a **Java Virtual Machine (JVM)** for the Java platform. [*](https://www.oracle.com/technical-resources/articles/javase/jvm-tool-interface.html#footnote) 

These native libraries are sometimes called **agent libraries** and are often used as a basis for the **Java technology-level tool APIs**, such as the **Java Debugger Interface (JDI)** that comes with the **Java Development Kit (JDK)**. Profiler tool vendors will often need to create an agent library that uses JVM TI. This article explores some basics of writing a JVM TI agent library by walking through the `heapTracker` demo agent available in the JDK downloads.
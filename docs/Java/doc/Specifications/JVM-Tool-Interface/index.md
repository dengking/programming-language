# [JVM Tool Interface](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html)

- inspect

- control

JVM. 

JVM TI is a two-way interface. A client of JVM TI, hereafter called an *agent*, can be notified of interesting occurrences through [events](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html#EventSection). JVM TI can query and control the application through many [functions](https://docs.oracle.com/en/java/javase/23/docs/specs/jvmti.html#FunctionSection), either in response to events or independent of them.
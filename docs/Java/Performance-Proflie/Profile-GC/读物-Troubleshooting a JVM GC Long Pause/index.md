# [dev Troubleshooting a JVM GC Long Pause](https://dev.to/ppsrap/troubleshooting-a-jvm-gc-long-pause-10c8)

> NOTE: [详细解析一次JVM GC长暂停的排查过程](https://blog.csdn.net/wdj_yyds/article/details/130367267?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7EPaidSort-1-130367267-blog-132728779.235%5Ev43%5Epc_blog_bottom_relevance_base6&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7EPaidSort-1-130367267-blog-132728779.235%5Ev43%5Epc_blog_bottom_relevance_base6&utm_relevant_index=1)  

## 摘要

文章详述了一次由于**JVM Full GC**导致的长时间暂停问题的排查过程，问题根源在于JVM使用了swap空间，通过设置`vm.swappiness`为0避免了swap的使用，从而解决了问题。此外，讨论了GC与swap的关系，以及不同场景下JVM内存需求的考虑。

## 背景

在高并发下，Java程序的GC问题属于很典型的一类问题，带来的影响往往会被进一步放大。不管是「**GC频率过快**」还是「**GC耗时太长**」，由于GC期间都存在Stop The World问题，因此很容易导致服务超时，引发性能问题。



1. Analyze GC logs. The GC log records the execution time and result of each GC. By analyzing the GC logs, you can optimize heap and GC settings, or improve the **object allocation pattern** of the application.



In this case, the reason for Full GC is `Ergonomics`, because `UseAdaptiveSizePolicy` is enabled, and the JVM is adapting and adjusting itself, causing Full GC.



This log mainly reflects the changes before and after **GC**, but it is currently not clear what is causing the issue.

To enable GC logs, the following JVM startup parameters need to be added:

```shell
-XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:/export/log/risk_pillar/gc.log
```

The meanings of common Young GC and Full GC logs are as follows:

[![Image description](https://media2.dev.to/dynamic/image/width=800%2Cheight=%2Cfit=scale-down%2Cgravity=auto%2Cformat=auto/https%3A%2F%2Fdev-to-uploads.s3.amazonaws.com%2Fuploads%2Farticles%2Fveof98dioez15ijcoior.png)](https://media2.dev.to/dynamic/image/width=800%2Cheight=%2Cfit=scale-down%2Cgravity=auto%2Cformat=auto/https%3A%2F%2Fdev-to-uploads.s3.amazonaws.com%2Fuploads%2Farticles%2Fveof98dioez15ijcoior.png)


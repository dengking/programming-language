# Automatic memory management

一、“automatic memory management”即“自动内存管理”

二、它的典型代表就是garbage collection

三、从control theory的角度来说，Automatic memory management属于implicit、automatic control

四、Automatic memory management是非常重要的技术，在多个领域中，都有着非常重要的运用:

1、lock-less data structure中，memory reclamation，可以看做是一种Automatic memory management。

2、可以使用Automatic memory management来解决multi-thread中，经常出现的dangling pointer问题，参见 `Access-outside-of-object-lifetime\Multi-thread-and-observer-pattern` 章节



深入理解Java虚拟机：JVM高级特性与最佳实践（第3版）周志明

> 从如何判定对象消亡的角度出发，垃圾收集算法可以划分为“引用计数式垃圾收集”（Reference Counting GC）和“追踪式垃圾收集”（Tracing GC）两大类，这两类也常被称作“直接垃圾收集”和“间接 垃圾收集”。

# 总结



## Multi level: 从"控制范围"来进行对比

简单概括来说:

1、sequential consistent ordering: 所有的thread看到的modification order是一致的

2、使用release-acquire、release-acquire实现的happens-before而构建起的modification order，仅限于参与的thread看到的modification order是一致的

下面总结对memory order的控制范围

| Ordering                | 范围                                                     |
| ----------------------- | -------------------------------------------------------- |
| Relaxed                 | 不控制，因此范围为0                                      |
| Release-consume         | 对存在data dependency的shared data的memory order进行控制 |
| Release-acquire         | 不考虑data dependency关系，直接画两条线，不允许越界      |
| Sequentially consistent | 全局                                                     |



## Implementation

都会涉及strong、weak ordered memory CPU。





## Acquire release memory order VS sequential consistency



### 基于producer-consumer来进行分析

multiple-producer-multiple-consumer需要sequential consistency

acquire release 能否实现multiple-producer-multiple-consumer？



## stackoverflow [What's are practical example where acquire release memory order differs from sequential consistency?](https://stackoverflow.com/questions/41858540/whats-are-practical-example-where-acquire-release-memory-order-differs-from-seq)



## herbsutter [Reader Q&A: Acquire/release and sequential consistency](https://herbsutter.com/2013/10/28/reader-qa-acquirerelease-and-sequential-consistency/)



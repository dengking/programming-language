# Acquire release memory order VS sequential consistency

## 基于modification order来进行对比

"cppreference或者说C++ language design中，对memory ordering的描述是基于modification order的"，在 `Release-Acquire-ordering`、`Sequentially-consistent-ordering` 章节中，都基于了**modification order** 来对它们进行了说明，参考其中的内容可以对比出它们之间的差异。

简单概括来说:

1、sequential consistent ordering: 所有的thread看到的modification order是一致的

2、使用acquire-release实现的happens-before而构建起的modification order，仅限于参与的thread看到的modification order是一致的

## 基于producer-consumer来进行分析

multiple-producer-multiple-consumer需要sequential consistency

acquire release 能否实现multiple-producer-multiple-consumer？

## stackoverflow [What's are practical example where acquire release memory order differs from sequential consistency?](https://stackoverflow.com/questions/41858540/whats-are-practical-example-where-acquire-release-memory-order-differs-from-seq)



## herbsutter [Reader Q&A: Acquire/release and sequential consistency](https://herbsutter.com/2013/10/28/reader-qa-acquirerelease-and-sequential-consistency/)



## modernescpp [Ongoing Optimization: Acquire-Release Semantic with CppMem](https://www.modernescpp.com/index.php/ongoing-optimization-acquire-release-semantic-with-cppmem)




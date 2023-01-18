# MLIR



## zhihu [MLIR介绍（一）概览](https://zhuanlan.zhihu.com/p/465464378)

### 3. 与LLVM区别

个人认为MLIR更适合和LLVM做比较，而不是TVM等dl compiler。LLVM和MLIR的很多概念都比较像，了解LLVM的话MLIR会比较容易上手。

LLVM IR由于当时的历史局限性，类型只设计了标量和定长vector，有个给LLVM加matrix类型的提案目前看来也没有进展。而MLIR自带tensor类型，对深度学习领域更友好。

MLIR有Operation和Dialect的概念，Dialect，Operation，Attribute，Type等都可以通过td文件比较方便地定义出来。而LLVM定义新的intrinsic比较麻烦，定义新的IR就更麻烦了。LLVM IR主要表示硬件指令操作，而MLIR能表示更多东西，比如表示神经网络的图结构。因为有Dialect，MLIR是组件化，去中心的，不像LLVM的ir是一种大而全的。

MLIR执行过程和LLVM一样，IR会过由Pass组成的Pipeline，不断地变换生成最终的IR。不同的是MLIR的IR可以是不同dialect的，构成了Multi-Level的效果。



## zhihu [MLIR中国社区](https://www.zhihu.com/people/mlir-70)



## zhihu [如何评价MLIR项目中Linalg Dialect的设计思想？](https://www.zhihu.com/question/442964082/answer/1718438248)

> NOTE:
>
> 一、这个套路有一些比较好的内容



## zhihu [mackler](https://www.zhihu.com/people/mackler)



zhihu [Chris Lattner在ASPLOS上力推的CIRCT项目能否真的解决芯片设计的挑战？ - mackler的回答 - 知乎](https://www.zhihu.com/question/461169562/answer/2610097528) 

zhihu [如何评价MLIR项目中Linalg Dialect的设计思想？ - mackler的回答 - 知乎]( https://www.zhihu.com/question/442964082/answer/1718438248)



## TVM VS MLIR

zhihu [tvm or mlir ？](https://zhuanlan.zhihu.com/p/388452164)

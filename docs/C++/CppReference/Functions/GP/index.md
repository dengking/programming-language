# 关于本章

本章讨论generic programming中的function相关的内容，

## 大的思路

对于function generic programming，涉及的内容非常多，因此我们需要建立起一个完整的、清晰的思路，下面是我们的梳理: 

维度一: input、output

对于一个function，我们需要关注的是:

input: 入参

output: 出参、返回值

维度二: type、value category(涉及perfect forwarding、pass-by、return-by)

在generic programming中，type、value category是和template parameter息息相关的重要的两个topic。

将上述两个维度进行组合就构成了function generic programming的大部分内容。



## Function template

这部分内容放到了 `Function-template` 章节。

## First class function

"first class function"是一个非常重要的内容，这在 `First-class-function` 章节进行了讨论。"first class function"对于将function用于generic programming非常重要。

## 以consistent、uniform、generic的方式来操作function

在将function作为template parameter后("first class function")，就需要提供consistent、uniform API来操作function，对于将function用于generic programming是非常重要的，关于"consistent、uniform "，参见 `Uniform-consistent-interface-API-syntax` 章节。

目前包含如下内容: 

1、以uniform、generic来调用function，这在 `Callable` 章节中进行了讨论

2、以uniform、generic来获得、查询 `Callable` 的 return type


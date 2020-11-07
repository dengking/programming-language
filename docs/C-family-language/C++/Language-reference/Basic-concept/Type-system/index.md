# 关于本章

在`Theory\Type-system\index.md`中提出了“Type determines everything”，在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`中对其进行了例证。

本章描述C++的type system。

## Operation on type

对于type，是可以执行很多操作的，我们以对data的operation来进行分析:

1) 增: 显然type是不支持"增"操作的

2) 删: 可以删除type，这就是type erasure

3) 改: 可以修改type的，这就是type conversion

4) 查: 可以查询type的，比如`decltype`、`typeid`


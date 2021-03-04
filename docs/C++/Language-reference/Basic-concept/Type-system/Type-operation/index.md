# Operation on type

对于type，是可以执行很多操作的，我们以对data的operation来进行分析:

1) 增: 显然type是不支持"增"操作的

2) 删: 可以删除type，这就是type erasure

3) 改: 可以修改type的，这就是type conversion

4) 查: 可以查询type的，比如`decltype`、`typeid`

5) ...

C++提供了丰富的type operation，除了本章介绍的内容，在下面章节中也涉及了type operation:

1) `C++\Library\Standard-library\Utility-library\Language-support\Type-support` (标准库的支持)

2) `C-and-C++\Interpretation-model`
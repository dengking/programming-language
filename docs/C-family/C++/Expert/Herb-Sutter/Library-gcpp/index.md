# [hsutter](https://github.com/hsutter)/**[gcpp](https://github.com/hsutter/gcpp)**

## zhihu [如何评价 Herb Sutter 的 gcpp ？](https://www.zhihu.com/question/50934400/answer/123399603)

简单来说创建了一个局部的堆/池，跟踪对象并一次性回收或手动collect。最大的好处是整个过程可控，可以类似unique_ptr那样明确地推断销毁发生时机。

整套技术不复杂，很优雅，2016年才出现多少有点不可思议。
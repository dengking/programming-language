# 关于本章

本章讨论compiler的optimization。

## Compiler optimization principle

Compiler是遵循 "optimization principle" 的，即它总是尽可能地去optimize生成的object file，它optimization的方向有:

1 将operation转移到compile time完成，从而去除runtime overhead，比如 `typeid`

compiler会尽可能地运用所有的已知信息(最多的是type info)来进行optimization。

2 去除冗余操作，从而降低runtime overhead，比如 Copy elision

作为programmer，我们谨记 compiler 的 optimization principle 是能够帮助我们理解很多 C++ implementation相关的设计的。下面是一些例子:

### `typeid`

参见 `C++\Language-reference\Basic-concept\Type-system\Type-operation\Query-type\typeid`。

 

### Copy elision

参见 `./Copy-elision` 章节。



### RVO


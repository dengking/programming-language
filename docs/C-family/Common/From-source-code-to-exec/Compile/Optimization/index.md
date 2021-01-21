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

参见 `C++\Language-reference\Initialization\Copy-elision` 章节。



## Give hint to compiler

本节标题的含义是: 给编译器提示；虽然当今compiler比较强大，但是毕竟它不是万能的，作为programmer，我们可以给予它一些hint，让能够满足我们的目的: 

1) 让compiler能够更好地优化code

关于此的例子有:

`inline`

`likely` 和 `unlikely`

2) 让compiler实现static polymorphism

含义是: 让compiler选择(statically)正确的实现

例子:

1 multiple dispatch，参见

- `Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Behavioral-pattern\Visitor-pattern`
- `C++\Pattern\Visitor-pattern`

2 primary template 和 specialization

当两者处于不同header file的时候，在使用的时候，需要将它们都包含到同一个translation unit中才能够生效。

3 CRTR都是give hint to compiler

它都是generic programming中的技巧，参见 `Generic-programming\Curiously-recurring-template-pattern` 章节。

4 tag dispatch


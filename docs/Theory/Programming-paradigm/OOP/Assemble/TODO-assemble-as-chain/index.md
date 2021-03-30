# Assemble as chain

## 为什么要assemble as chain？

1、非常地灵活、优雅。

2、符合结构的linear特性（linear structure）

## Application

1、对于流程型application

2、stream

## Dynamic chain and static chain

1、decorator pattern就是典型的dynamic chain

2、C++中，有很多static chain的例子

## 例子

### AOP

1、aspect chain

### Function composition、Pipeline

1、将上一个的输出作为下一个的输入，这是非常典型的。

2、这是在stream programming model中，使用地比较多的



### Decorator pattern

参见相关章节。

### Chain of responsibility

参见相关章节。

### Linked list model

1、需要注意的是，本节标题中，使用了"linked list"，其实它并不一定对应的是linked list，而是类似linked list的模式。

2、linked list的模式是: 通过next pointer将多个node串联起来，形成一个list、chain

3、我们将next pointer推广: 如何可以充当类似linked list中next pointer功能的，可以将两个node(此处的node应该是抽象的node)，都能够形成逻辑上的linked list

#### Stream append model

Stream append model就是属于Linked list model。

典型的例子包括:

使用一个operator来将多个node连接起来，比如C++ `<<` operator。

#### 补充

参见 C++ `Assemble-as-chain` 章节，其中有很多内容。
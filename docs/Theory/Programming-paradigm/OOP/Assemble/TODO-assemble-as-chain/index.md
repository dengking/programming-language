# Assemble as chain

## Application

1、对于流程型application

2、对于stream

## Dynamic chain and static chain

decorator pattern就是典型的dynamic chain

## 例子

### AOP

1、aspect chain

2、它非常类似于function composition

### Decorator pattern



### Chain of responsibility



### Linked list model

1、需要注意的是，本节标题中，使用了"linked list"，其实它并不一定对应的是linked list，而是类似linked list的模式。

2、linked list的模式是: 通过next pointer将多个node串联起来，形成一个list、chain

3、我们将next pointer推广: 如何可以充当类似linked list中next pointer功能的，可以将两个node(此处的node应该是抽象的node)，都能够形成逻辑上的linked list

4、典型的例子包括:

使用一个operator来将多个node连接起来，比如C++ `<<` operator。


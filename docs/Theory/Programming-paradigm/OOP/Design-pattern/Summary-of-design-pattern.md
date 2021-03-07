# Summary of design pattern



## Assemble design pattern

每种design pattern都提出了一种核心思想/抽象思路，现实问题往往非常复杂，仅仅依靠一种design pattern是难以描述的，因此就需要assemble多种 design pattern来描述复杂的现实问题。

### Node is like event/message

在visitor pattern中，需要traverse node in  the structure，对于不同type的node，需要执行不同的algorithm，其实这非常类似于与event/message driven model的，即对于不同type的event/message，执行不同的callback；

### Example

1、`Active-object+observer+visitor`

参见工程`parallel-computing`的`Concurrent-computing\TODO-Concurrent-server\Design-pattern\Active-object+observer+visitor`章节



## Abstraction

本节从Abstraction的角度来分析各种design pattern: 

1、command是对各种operation的更加abstract的描述；

2、iterator是对各种container的更加abstract的描述；

## Structure

本节从structure的角度来分析各种design pattern;

1、composit pattern: tree

2、chain of responsibility pattern: chain

3、decorator pattern: chain

## Command pattern VS  visitor pattern

command 和 visitor都对应的是operation/algorithm。

关于此，在 refactoring.guru [Visitor](https://refactoring.guru/design-patterns/visitor) 中有这样的描述: 

> You can treat [Visitor](https://refactoring.guru/design-patterns/visitor) as a powerful version of the [Command](https://refactoring.guru/design-patterns/command) pattern. Its objects can execute operations over various objects of different classes.

decorator pattern VS 

### TODO

stackoverflow [visitor vs servant vs command patterns](https://stackoverflow.com/questions/31986332/visitor-vs-servant-vs-command-patterns)



## Chain of responsibility VS decorator pattern

这两种pattern是非常值得对比的，在下面的文章中对它们进行了对比:

a、wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern)

b、refactoring.guru [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility)

### Dynamic pipeline flow of handling request

两种pattern都能够Pipeline flow of handling request。

## Message/event/request processing pattern/flow of request

pipeline flow of request

### refactoring.guru [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility)、

[Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility), [Command](https://refactoring.guru/design-patterns/command), [Mediator](https://refactoring.guru/design-patterns/mediator) and [Observer](https://refactoring.guru/design-patterns/observer) address various ways of connecting senders and receivers of requests:

1、*Chain of Responsibility* passes a request sequentially along a dynamic chain of potential receivers until one of them handles it.

2、*Command* establishes unidirectional connections between senders and receivers.

3、*Mediator* eliminates(消除) direct connections between senders and receivers, forcing them to communicate indirectly via a mediator object.

4、*Observer* lets receivers dynamically subscribe to and unsubscribe from receiving requests.



### refactoring.guru [Command](https://refactoring.guru/design-patterns/command) 

### AOP

对于 "flow of handling request" 即流程相关的，也非常适合用AOP来进行抽象。



## Interface: [Adapter](https://en.wikipedia.org/wiki/Adapter_pattern)、[Decorator](https://en.wikipedia.org/wiki/Decorator_pattern)、[Facade](https://en.wikipedia.org/wiki/Facade_pattern)

这三种idiom都是围着interface转，在各自的Wikipedia page中，都对它们进行了对比，总结得不错:

|                           Pattern                            |                            Intent                            |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|   [Adapter](https://en.wikipedia.org/wiki/Adapter_pattern)   | Converts one interface to another so that it matches what the client is expecting |
| [Decorator](https://en.wikipedia.org/wiki/Decorator_pattern) | Dynamically adds responsibility to the interface by wrapping the original code |
|    [Facade](https://en.wikipedia.org/wiki/Facade_pattern)    |               Provides a simplified interface                |
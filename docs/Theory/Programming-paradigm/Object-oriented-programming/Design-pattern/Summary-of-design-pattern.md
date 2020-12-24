# Summary of design pattern



## Assemble design pattern

每种design pattern都提出了一种核心思想/抽象思路，现实问题往往非常复杂，仅仅依靠一种design pattern是难以描述的，因此就需要assemble多种 design pattern来描述复杂的现实问题。

### Node is like event/message

在visitor pattern中，需要traverse node in  the structure，对于不同type的node，需要执行不同的algorithm，其实这非常类似于与event/message driven model的，即对于不同type的event/message，执行不同的callback；

### Observer pattern and visitor pattern

event source就是是一个abstract structure，event source会产生各种event，对于每种event，不同的listener需要执行不同的algorithm，这个过程其实非常类似于visitor:

1) 将event source看做是由这些event组成的abstract structure，则整个过程就是对这个abstract structure的visit；

2) call back

#### Observer pattern + Visitor pattern for message system

https://stackoverflow.com/questions/32079697/observer-pattern-visitor-pattern-for-message-system



## Command and iterator 

command是对各种operation的更加abstract的描述；

iterator是对各种container的更加abstract的描述；

## Command pattern VS  visitor pattern

command 和 visitor都对应的是operation/algorithm。

关于此，在 refactoring.guru [Visitor](https://refactoring.guru/design-patterns/visitor) 中有这样的描述: 

> You can treat [Visitor](https://refactoring.guru/design-patterns/visitor) as a powerful version of the [Command](https://refactoring.guru/design-patterns/command) pattern. Its objects can execute operations over various objects of different classes.



### TODO

stackoverflow [visitor vs servant vs command patterns](https://stackoverflow.com/questions/31986332/visitor-vs-servant-vs-command-patterns)

## Message/event/request processing pattern

这些pattern往往都和AOP有关。

在refactoring.guru [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility)、refactoring.guru [Command](https://refactoring.guru/design-patterns/command) 中，都有如下总结: 

[Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility), [Command](https://refactoring.guru/design-patterns/command), [Mediator](https://refactoring.guru/design-patterns/mediator) and [Observer](https://refactoring.guru/design-patterns/observer) address various ways of connecting senders and receivers of requests:

- *Chain of Responsibility* passes a request sequentially along a dynamic chain of potential receivers until one of them handles it.
- *Command* establishes unidirectional connections between senders and receivers.
- *Mediator* eliminates direct connections between senders and receivers, forcing them to communicate indirectly via a mediator object.
- *Observer* lets receivers dynamically subscribe to and unsubscribe from receiving requests.


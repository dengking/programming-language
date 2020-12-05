# Command pattern

是在阅读 refactoring.guru [Visitor](https://refactoring.guru/design-patterns/visitor) 时，其中有这样的描述:

> You can treat [Visitor](https://refactoring.guru/design-patterns/visitor) as a powerful version of the [Command](https://refactoring.guru/design-patterns/command) pattern. Its objects can execute operations over various objects of different classes.

上面这段话中的"its"所指的是visitor。

command是横切关注点，command pattern其实体现了 AOP。

## refactoring.guru [Command](https://refactoring.guru/design-patterns/command)



### Intent

**Command** is a **behavioral design pattern** that turns a request into a stand-alone object that contains all information about the request. This transformation lets you parameterize methods with different requests, delay or queue a request’s execution, and support undoable(可撤销的) operations.
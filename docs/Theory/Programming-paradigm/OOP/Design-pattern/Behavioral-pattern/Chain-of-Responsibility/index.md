# Chain of Responsibility

1、这个pattern是非常适合于"pipeline flow of request"的，因此在实现上是需要形成chain structure

2、这个pattern相对容易理解



## refactoring.guru [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility)

**Chain of Responsibility** is a behavioral design pattern that lets you pass requests along a chain of handlers. Upon receiving a request, each handler decides either to process the request or to pass it to the next handler in the chain.

> NOTE: 
>
> 1、每个handler有两个option:
>
> a、handle
>
> b、pass to next handler

![Chain of Responsibility design&nbsp;pattern](https://refactoring.guru/images/patterns/content/chain-of-responsibility/chain-of-responsibility.png)

### Problem

### Solution

![Handlers are lined-up one by one, forming a chain](https://refactoring.guru/images/patterns/diagrams/chain-of-responsibility/solution1-en.png)

Handlers are lined up one by one, forming a chain.

> NOTE: 
>
> 1、上图是非常形象的

### Real-World Analogy

![Talking with tech support can be hard](https://refactoring.guru/images/patterns/content/chain-of-responsibility/chain-of-responsibility-comic-1-en.png)

A call to tech support can go through multiple operators.

### Structure



### Applicability



### Relations with Other Patterns



## wikipedia [Chain of responsibility pattern](https://en.wikipedia.org/wiki/Chain-of-responsibility_pattern)
# Dependency injection

第一次接触Dependency injection，是在学习spring的时候，正如[Spring is flexible](https://spring.io/why-spring)中所说

> Spring’s flexible and comprehensive set of extensions and third-party libraries let developers build almost any application imaginable. At its core, Spring Framework’s [Inversion of Control (IoC)](https://en.wikipedia.org/wiki/Inversion_of_control) and [Dependency Injection (DI)](https://en.wikipedia.org/wiki/Dependency_injection) features provide the foundation for a wide-ranging set of features and functionality. Whether you’re building secure, reactive, cloud-based microservices for the web, or complex streaming data flows for the enterprise, Spring has the tools to help.



[SOLID](https://en.wikipedia.org/wiki/SOLID)中的D所指为[Dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)，本文将要进行描述的是[Dependency injection](https://en.wikipedia.org/wiki/Dependency_injection)。



## 维基百科[Dependency injection](https://en.wikipedia.org/wiki/Dependency_injection)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), **dependency injection** is a technique in which an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) receives other objects that it depends on. These other objects are called **dependencies**. In the typical "using" relationship[[1\]](https://en.wikipedia.org/wiki/Dependency_injection#cite_note-1) the receiving object is called a [client](https://en.wikipedia.org/wiki/Client_(computing)) and the passed (that is, "injected") object is called a [service](https://en.wikipedia.org/wiki/Service_(systems_architecture)). The code that passes the **service** to the **client** can be many kinds of things and is called the **injector**. Instead of the client specifying which service it will use, the **injector** tells the **client** what service to use. The "injection" refers to the passing of a **dependency** (a service) into the **object** (a client) that would use it.

> NOTE: 上面这段话已经描述清楚了dependency injection的含义了。
# Interface



## wikipedia [Interface (computing)](https://en.wikipedia.org/wiki/Interface_(computing))

In computing, an **interface** is a shared boundary across which two or more separate components of a [computer system](https://en.wikipedia.org/wiki/Computer_system) exchange information. The exchange can be between [software](https://en.wikipedia.org/wiki/Software), [computer hardware](https://en.wikipedia.org/wiki/Computer_hardware), [peripheral](https://en.wikipedia.org/wiki/Peripheral) devices, [humans](https://en.wikipedia.org/wiki/User_interface), and combinations of these.[[1\]](https://en.wikipedia.org/wiki/Interface_(computing)#cite_note-HookwayInterface14-1)



### Hardware interfaces

*Main article:* [Hardware interface](https://en.wikipedia.org/wiki/Hardware_interface)

### Software interfaces

*See also:* [Application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface) *and* [Application programming interface](https://en.wikipedia.org/wiki/Application_programming_interface)

#### In object-oriented languages

*Main articles:* [Protocol (object-oriented programming)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)) *and* [Concept (generic programming)](https://en.wikipedia.org/wiki/Concept_(generic_programming))



#### Programming to the interface

The use of interfaces allows for a programming style called *programming to the interface*. The idea behind this approach is to base programming logic on the interfaces of the objects used, rather than on internal implementation details. Programming to the interface reduces dependency on implementation specifics and makes code more reusable.[[12\]](https://en.wikipedia.org/wiki/Interface_(computing)#cite_note-12)

Pushing this idea to the extreme, [inversion of control](https://en.wikipedia.org/wiki/Inversion_of_control) leaves the *context* to inject the code with the specific implementations of the interface that will be used to perform the work.





## Draft

### 文章the interface principle

阅读完这篇文章，我的一个感受是: 一致性接口的价值。



#### C++ trait is an interface

trait 是一种interface/abstraction，关于此参见: `C++\Idiom\Templates-and-generic-programming\SFINAE-trait-enable-if\Trait`





### Interface is a kind of abstraction

关于interface is a kind of abstraction的一些素材:

wikipedia [Kernel_(operating_system)](https://en.wikipedia.org/wiki/Kernel_(operating_system)) 中有这样的描述:

> The kernel's [interface](https://en.wikipedia.org/wiki/Application_programming_interface) is a [low-level](https://en.wikipedia.org/wiki/High-_and_low-level) [abstraction layer](https://en.wikipedia.org/wiki/Abstraction_layer). When a [process](https://en.wikipedia.org/wiki/Process_(computing)) requests a service to the kernel, it must invoke a [system call](https://en.wikipedia.org/wiki/System_call)

### Uniform

统一的描述方式，其实和一致性接口类似，最最典型的例子就是: unified initialization





### 20201230

#### OOP

program to an abstraction，其实本质上是program to an interface，因为interface是 对abstraction的描述；

subtyping是interface inheritance，显然它是program to an interface；

在wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)中有这样的描述: 

> Composition and interfaces
>
> The C++ examples in this section demonstrate the principle of using composition and interfaces to achieve code reuse and polymorphism.

 

显然，无论是subtyping 还是 composition，都是program to an abstraction，所以 最终都是program to an interface。
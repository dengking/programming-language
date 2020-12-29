# Policy-based design

在阅读[boost Generic Programming Techniques#Policy Classes](https://www.boost.org/community/generic_programming.html#policy)时，发现的这个policy-based design。



## wikipedia [*Modern C++ Design*#Policy-based design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#Policy-based_design)

**Policy-based design**, also known as **policy-based class design** or **policy-based programming**, is the term used in *Modern C++ Design* for a design approach based on an [idiom](https://en.wikipedia.org/wiki/Idiom#Computer_science) for C++ known as **policies**. It has been described as a [compile-time](https://en.wikipedia.org/wiki/Compile-time) variant of the [strategy pattern](https://en.wikipedia.org/wiki/Strategy_pattern), and has connections with C++ [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming). It was first popularized in C++ by Andrei Alexandrescu with *Modern C++ Design* and with his column *`Generic<Programming>`* in the *[C/C++ Users Journal](https://en.wikipedia.org/wiki/C/C%2B%2B_Users_Journal)*, and it is currently closely associated with C++ and [D](https://en.wikipedia.org/wiki/D_(programming_language)) as it requires a [compiler](https://en.wikipedia.org/wiki/Compiler) with highly [robust](https://en.wikipedia.org/wiki/Robustness_(computer_science)) support for [templates](https://en.wikipedia.org/wiki/Template_(programming)), which was not common before about 2003.

> NOTE: static polymorphism

The central idiom in policy-based design is a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) template (called the *host* class), taking several [type](https://en.wikipedia.org/wiki/Data_type) [parameters](https://en.wikipedia.org/wiki/Parameter_(computer_science)) as input, which are [instantiated](https://en.wikipedia.org/wiki/Instantiation_(computer_science)) with types selected by the user (called *policy classes*), each [implementing](https://en.wikipedia.org/wiki/Implementation) a particular implicit [interface](https://en.wikipedia.org/wiki/Interface_(computer_science)) (called a *policy*), and [encapsulating](https://en.wikipedia.org/wiki/Separation_of_concerns) some [orthogonal](https://en.wikipedia.org/wiki/Orthogonality#Computer_science) (or mostly orthogonal) aspect of the behavior of the instantiated host class. By supplying a host class combined with a set of different, canned(表明意思是: 听装的，引申意思:  封装的) implementations for each policy, a [library](https://en.wikipedia.org/wiki/Library_(computing)) or [module](https://en.wikipedia.org/wiki/Module_(programming)) can support an [exponential](https://en.wikipedia.org/wiki/Exponential_growth) number of different behavior combinations, resolved at compile time, and selected by mixing and matching the different supplied policy classes in the instantiation of the host class template. Additionally, by writing a custom implementation of a given policy, a **policy-based library** can be used in situations requiring behaviors unforeseen by the library implementor. Even in cases where no more than one implementation of each policy will ever be used, **decomposing** a class into policies can aid the design process, by increasing **modularity** and highlighting exactly where orthogonal design decisions have been made.

> NOTE: 上述"combinations"其实对应的就是N * M code reuse。

While assembling software components out of interchangeable modules is a far from new concept, **policy-based design** represents an innovation(创新) in the way it applies that concept at the (relatively low) level of defining the behavior of an individual class. 

> NOTE: 上面这段话的表明意思是: 虽然从可互换模块中组装软件组件远非一个新概念，但基于策略的设计代表了一种创新，即在定义单个类行为的(相对较低的)级别上应用这个概念。
>
> policy-based design也是基于"decompose and assemble"，它的创新点在于"policy class"，policy class是at the (relatively low) level的。
>
> decompose and assemble，它们是相反的过程。
>
> behavior and interface: 使用interface来描述behavior

### Policy classes  VS callback

Policy classes have some similarity to [callbacks](https://en.wikipedia.org/wiki/Callback_(computer_science)), but differ in that, rather than consisting of a single [function](https://en.wikipedia.org/wiki/Subroutine), a policy class will typically contain several related functions ([methods](https://en.wikipedia.org/wiki/Method_(computer_science))), often combined with [state](https://en.wikipedia.org/wiki/State_(computer_science)) [variables](https://en.wikipedia.org/wiki/Variable_(programming)) or other facilities such as nested types. A policy-based host class can be thought of as a type of [metafunction](https://en.wikipedia.org/wiki/Template_metaprogramming), taking a set of behaviors represented by types as input, and returning as output a type representing the result of combining those behaviors into a functioning whole. (Unlike [MPL](https://en.wikipedia.org/wiki/Boost_C%2B%2B_Libraries) metafunctions, however, the output is usually represented by the instantiated host class itself, rather than a nested output type.)

---

### Inheritance and composition

A key feature of the *policy* idiom is that, usually (though it is not strictly necessary), the host class will [derive](https://en.wikipedia.org/wiki/Inheritance_(computer_science)) from (make itself a [child class](https://en.wikipedia.org/wiki/Subclass_(computer_science)) of) each of its **policy classes** using (public) [multiple inheritance](https://en.wikipedia.org/wiki/Multiple_inheritance). (Alternatives are for the host class to merely contain a member variable of each policy class type, or else to inherit the policy classes privately; however inheriting the policy classes publicly has the major advantage that a policy class can add new methods, inherited by the instantiated host class and accessible to its users, which the host class itself need not even know about.) A notable feature of this aspect of the policy idiom is that, relative to [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), policies invert(颠倒) the relationship between [base class](https://en.wikipedia.org/wiki/Superclass_(computer_science)) and derived class - whereas in OOP interfaces are traditionally represented by ([abstract](https://en.wikipedia.org/wiki/Virtual_function#Abstract_classes_and_pure_virtual_functions)) base classes and implementations of interfaces by derived classes, in policy-based design the derived (host) class represents the interfaces and the base (policy) classes implement them. In the case of policies, the public inheritance does not represent an **is-a** relationship between the host and the policy classes. While this would traditionally be considered evidence of a design defect(缺点) in OOP contexts, this doesn't apply in the context of the **policy idiom**.

> NOTE: 之前在实现AMUST API的时候，使用的是multiple inheritance。
>
> 上面这段话中涉及了composition和inheritance，它让我想到了"composition over inheritance"。
>
> policy-based design中，inheritance的目的是: code reuse

A disadvantage of policies in their current incarnation is that the policy interface doesn't have a direct, explicit representation in [code](https://en.wikipedia.org/wiki/Source_code), but rather is defined implicitly, via [duck typing](https://en.wikipedia.org/wiki/Duck_typing), and must be documented separately and manually, in [comments](https://en.wikipedia.org/wiki/Comment_(computer_programming)). The main idea is to use commonality-variability analysis to divide the type into the fixed implementation and interface, the policy-based class, and the different policies. The trick is to know what goes into the main class, and what policies should one create. The article mentioned above gives the following answer: wherever we would need to make a possible limiting design decision, we should postpone(推迟) that decision, we should delegate it to an appropriately named policy.

> NOTE: template is behavior-based

Policy classes can contain implementation, type definitions and so forth. Basically, the designer of the main template class will define what the policy classes should provide, what customization points they need to implement.

It may be a delicate task to create a good set of policies, just the right number (e.g., the minimum necessary). The different customization points, which belong together, should go into one policy argument, such as **storage policy**, **validation policy** and so forth. Graphic designers are able to give a name to their policies, which represent concepts, and not those which represent operations or minor implementation details.

Policy-based design may incorporate other useful techniques. For example, the [template method pattern](https://en.wikipedia.org/wiki/Template_method_pattern) can be reinterpreted for compile time, so that a main class has a [skeleton algorithm](https://en.wikipedia.org/wiki/Skeleton_algorithm), which – at customization points – calls the appropriate functions of some of the policies.

This will be achieved dynamically by [concepts](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B))[[3\]](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#cite_note-3) in future versions of C++.



## drdobbs [Policy-Based Design in the Real World](https://www.drdobbs.com/policy-based-design-in-the-real-world/184401861)



## Trait and policy

stackoverflow [What is the difference between a trait and a policy?](https://stackoverflow.com/questions/14718055/what-is-the-difference-between-a-trait-and-a-policy)

stackoverflow [What is the point of STL Character Traits?](https://stackoverflow.com/questions/5319770/what-is-the-point-of-stl-character-traits)

etutorials [8.4 Traits and Policies](http://etutorials.org/Programming/Programming+Cpp/Chapter+8.+Standard+Library/8.4+Traits+and+Policies/)
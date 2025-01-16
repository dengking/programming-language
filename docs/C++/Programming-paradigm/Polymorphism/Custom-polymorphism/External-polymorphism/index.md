# External Polymorphism



1、在阅读 stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 时，它的comment中，有这样的描述:

> Side note: the pattern used i.a. by `Boost.Any` is called the external polymorphism pattern – [wmamrak](https://stackoverflow.com/users/469659/wmamrak) [Dec 4](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment18828402_5450159)

其中提及了"external polymorphism"，于是我Google了一下，检索结果中包含 wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism) ，这篇文章总结地不错，通过其中的内容，我发现我之前很多的做法其实都可以归为 external polymorphism，显然我们使用 external polymorphism 的目的是为了实现 custom polymorphism，即自定义polymorphism。我又联想到了之前总结的custom virtual table，显然它也是为了实现 custom polymorphism；我又联想到了之前阅读的visitor pattern中实现multiple dispatch，其中是明确反对custom polymorphism、external polymorphism的，因此这就是导致了我编写本文。



## wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism)

Any mechanism for implementing **polymorphism** that is external to the definition of the object displaying polymorphic behavior. Switch statements based on a type code are probably the simplest of such mechanisms.

> NOTE: 
>
> 1、关于 "Switch statements based on a type code are probably the simplest of such mechanisms" 的最最典型的案例就是在 gieseanw [Stop reimplementing the virtual table and start using double dispatch](https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/) 中给出的如下示例:
>
> ![img](https://gieseanw.files.wordpress.com/2018/12/doubledispatchheader.png?w=660)

There are other, more ingenious mechanisms, available as well:

1、The paper "External Polymorphism -- An object structural pattern for transparently extending C++ concrete data types" by Cleeland, Schmidt, and Harrison cleverly combines dynamic binding and the template mechanism. http://www.cs.wustl.edu/~schmidt/PDF/External-Polymorphism.pdf

> NOTE: 这篇paper收录在了`C++\Pattern\Adapter-pattern\External-polymorphism-pattern\TODO-paper-vanderbilt.edu-External-Polymorphism`章节

2、The [GenericFunction](https://wiki.c2.com/?GenericFunction) (which is used by the [TranslatorPattern](https://wiki.c2.com/?TranslatorPattern)) uses a hash table with class names as keys and corresponding functions (see [FunctionObjectPattern](https://wiki.c2.com/?FunctionObjectPattern)) as values.

> NOTE: 
>
> 1、这种方式是最常使用的一种方式，我们将它称为generic function pattern
>
> 2、上述 [TranslatorPattern](https://wiki.c2.com/?TranslatorPattern) ，我看了一下，和interpreter pattern是类似的，因此，将它归入到了interpreter pattern章节中

3、[MultiMethods](https://wiki.c2.com/?MultiMethods) -- methods that are polymorphic on all of their arguments rather than one. Implemented in languages such as [CommonLispObjectSystem](https://wiki.c2.com/?CommonLispObjectSystem), [DylanLanguage](https://wiki.c2.com/?DylanLanguage), [CecilLanguage](https://wiki.c2.com/?CecilLanguage), and [NiceLanguage](https://wiki.c2.com/?NiceLanguage). These are useful for [ExternalPolymorphism](https://wiki.c2.com/?ExternalPolymorphism) even if having a single (polymorphic) argument, as they can be implemented without changing the underyling class.

> NOTE: 
>
> 1、很多programming language是不支持multi method的，C++也是如此



## Generic Function Pattern

在 阅读 wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism) 时，其中提及了Generic Function Pattern。

### wiki.c2 [Generic Function](https://proxy.c2.com/cgi/fullSearch?search=GenericFunction)

> NOTE: 其实我们经常使用， 但是没有对它进行命名

A [GenericFunction](https://wiki.c2.com/?GenericFunction) implements [ExternalPolymorphism](https://wiki.c2.com/?ExternalPolymorphism) and [MultipleDispatch](https://wiki.c2.com/?MultipleDispatch) by maintaining a dispatch table that associates parameter types with [FunctorObject](https://wiki.c2.com/?FunctorObject)s. When a [GenericFunction](https://wiki.c2.com/?GenericFunction) is called it uses the dynamic types of its parameters as keys into the dispatch table in order to retrieve a specialized [FunctorObject](https://wiki.c2.com/?FunctorObject). The call is then delegated to the [FunctorObject](https://wiki.c2.com/?FunctorObject).


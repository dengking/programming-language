# Generic Function Pattern

在 阅读 wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism) 时，其中提及了Generic Function Pattern。

## wiki.c2 [Generic Function](https://proxy.c2.com/cgi/fullSearch?search=GenericFunction)

> NOTE: 其实我们经常使用， 但是没有对它进行命名

A [GenericFunction](https://wiki.c2.com/?GenericFunction) implements [ExternalPolymorphism](https://wiki.c2.com/?ExternalPolymorphism) and [MultipleDispatch](https://wiki.c2.com/?MultipleDispatch) by maintaining a dispatch table that associates parameter types with [FunctorObject](https://wiki.c2.com/?FunctorObject)s. When a [GenericFunction](https://wiki.c2.com/?GenericFunction) is called it uses the dynamic types of its parameters as keys into the dispatch table in order to retrieve a specialized [FunctorObject](https://wiki.c2.com/?FunctorObject). The call is then delegated to the [FunctorObject](https://wiki.c2.com/?FunctorObject).


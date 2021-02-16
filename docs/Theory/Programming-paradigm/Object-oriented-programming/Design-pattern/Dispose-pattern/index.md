# Dispose pattern



## wikipedia [Dispose pattern](https://en.wikipedia.org/wiki/Dispose_pattern)

The dispose pattern is primarily used in languages whose [runtime environment](https://en.wikipedia.org/wiki/Runtime_environment) have [automatic garbage collection](https://en.wikipedia.org/wiki/Automatic_garbage_collection) (see motivation below), and thus may be styled as *manual* resource management in languages with [*automatic* memory management](https://en.wikipedia.org/wiki/Automatic_memory_management).

> NOTE: python、java等都采用的是这种pattern，一般采用这种pattern的programming language的[exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax)中都需要使用`finally-clause`。
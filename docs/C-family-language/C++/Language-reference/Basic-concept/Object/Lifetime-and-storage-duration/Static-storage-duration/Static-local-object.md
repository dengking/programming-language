# Static local variables

> NOTE: 对于static local variable，

Variables declared at **block scope** with the specifier `static` or `thread_local` (since C++11) have static or thread (since C++11) storage duration but are initialized the first time control passes through their declaration (unless their initialization is [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization), which can be performed before the block is first entered). On all further calls, the declaration is skipped.

> NOTE: 上面这段话关于initialization的描述是不易理解的？它的意思是：对于static local variable，它的initialization的发生时间如下：
>
> - [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) can be performed before the block is first entered
> - others are initialized the first time control passes through their declaration



#### Initialization of static local variable concurrently (since C++11)

> NOTE: 这种情况的典型就是：线程执行函数中声明了一个static local variable。

If multiple threads attempt to initialize the same **static local variable** concurrently, the **initialization** occurs exactly once (similar behavior can be obtained for arbitrary functions with [std::call_once](../thread/call_once.html)).

Note: usual implementations of this feature use variants of the double-checked locking pattern, which reduces runtime overhead for already-initialized local statics to a single non-atomic boolean comparison.

> NOTE: double-checked locking pattern在工程parallel-computing的`Synchronization\Lock`章节描述。
# Static local variables

对于static local variable，cppreference是在[Storage class specifiers#Static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)章节进行描述的。

## cppreference [Storage class specifiers#Static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)

### Dnitialization

Variables declared at **block scope** with the specifier `static` or `thread_local` (since C++11) have static or thread (since C++11) storage duration but are initialized the first time control passes through their declaration (unless their initialization is [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization), which can be performed before the block is first entered). On all further calls, the declaration is skipped.

> NOTE: 上面这段话关于initialization的描述是不易理解的？它的意思是：对于static local variable，它的initialization的发生时间如下：
>
> - [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) can be performed before the block is first entered
> - others are initialized the first time control passes through their declaration



#### Initialization of static local variable concurrently (since C++11)

> NOTE: 因为C++11支持multiple thread，所以标准需要对此进行特别的规定。
>
> 这种情况的典型就是：线程执行函数中声明了一个static local variable。

If multiple threads attempt to initialize the same **static local variable** concurrently, the **initialization** occurs exactly once (similar behavior can be obtained for arbitrary functions with [std::call_once](../thread/call_once.html)).

Note: usual implementations of this feature use variants of the double-checked locking pattern, which reduces runtime overhead for already-initialized local statics to a single non-atomic boolean comparison.

> NOTE: double-checked locking pattern在工程parallel-computing的`Synchronization\Lock`章节描述。
>
> 在`C++\Pattern\Singleton\Cpp-and-the-Perils-of-Double-Checked-Locking`中解释了为何double-checked locking pattern无法实现singleton。

### Deinitialization

The destructor for a block-scope static variable [is called at program exit](https://en.cppreference.com/w/cpp/utility/program/exit), but only if the initialization took place successfully.

### Inline

Function-local static objects in all definitions of the same [inline function](https://en.cppreference.com/w/cpp/language/inline) (which may be implicitly inline) all refer to the same object defined in one translation unit.

> NOTE: 能够保证唯一性



## Application

### Singleton

参见`C++\Pattern\Singleton`。
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

If multiple threads attempt to initialize the same **static local variable** concurrently, the **initialization** occurs exactly once (similar behavior can be obtained for arbitrary functions with `std::call_once`).

> NOTE:
>
> static local object，具备如下特性:
>
> 1、construct on first use、lazy initialization
>
> 2、call once 
>
> 3、guaranteed-destruction(后"End"章节进行了介绍)
>
> Meyers singleton就是利用了static local object的上述两个特性来实现的

Note: usual implementations of this feature use **variants of the double-checked locking pattern**, which reduces runtime overhead for already-initialized local statics to a single non-atomic boolean comparison.

> NOTE: 
>
> 1、double-checked locking pattern在工程parallel-computing的`Synchronization\Lock`章节描述。
>
> 2、在`C++\Pattern\Singleton\Cpp-and-the-Perils-of-Double-Checked-Locking`中解释了为何double-checked locking pattern无法实现singleton。

### End

The destructor for a block-scope static variable [is called at program exit](https://en.cppreference.com/w/cpp/utility/program/exit), but only if the initialization took place successfully.

> NOTE: 
>
> 1、利用这个特性可以实现exit guard，关于此参见:
>
> a、stackexchange [Modern C++ Singleton Template](https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template) ，其中的`static MemGuard g; // clean up on program end`正是这个用法

### Inline

Function-local static objects in all definitions of the same [inline function](https://en.cppreference.com/w/cpp/language/inline) (which may be implicitly inline) all refer to the same object defined in one translation unit.

> NOTE: 能够保证唯一性



## cppreference [Initialization#Static local variables](https://en.cppreference.com/w/cpp/language/initialization#Static_local_variables)

For initialization of locals (that is, block scope) static and thread-local variables, see [static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables).

Initializer is not allowed in a block-scope declaration of a variable with [external or internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage). Such a declaration must appear with extern and cannot be a definition.

> NOTE: 
>
> 对于static local variable with external linkage，如果带上initializer，则它就是definition了，显然这是违反了ODR的。
>
> 一般，我们使用的static local variable是no linkage的。

## Application

### Singleton

参见`C++\Pattern\Singleton`。
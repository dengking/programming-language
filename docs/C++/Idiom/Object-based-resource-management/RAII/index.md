# RAII

RAII是C++的一个重要feature。

## cppreference [RAII](https://en.cppreference.com/w/cpp/language/raii)

*Resource Acquisition Is Initialization* or RAII, is a C++ programming technique which binds the life cycle of a **resource** that must be acquired before use (allocated heap memory, thread of execution, open socket, open file, locked mutex, disk space, database connection—anything that exists in limited supply) to the [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) of an object.

RAII guarantees that the resource is available to any function that may access the object (resource availability is a [class invariant](https://en.wikipedia.org/wiki/Class_invariant), eliminating redundant runtime tests). It also guarantees that all resources are released when the lifetime of their **controlling object** ends, in reverse order of acquisition. Likewise, if **resource acquisition** fails (the constructor exits with an exception), all resources acquired by every **fully-constructed member** and **base subobject** are released in reverse order of initialization. This leverages the core language features ([object lifetime](https://en.cppreference.com/w/cpp/language/lifetime), [scope exit](https://en.cppreference.com/w/cpp/language/statements), [order of initialization](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order) and [stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding)) to eliminate resource leaks and guarantee **exception safety**. Another name for this technique is *Scope-Bound Resource Management* (SBRM), after the basic use case where the lifetime of an RAII object ends due to scope exit.

RAII can be summarized as follows:

1、encapsulate each resource into a class, where

- the constructor acquires the resource and establishes all class invariants or throws an exception if that cannot be done,

- the destructor releases the resource and never throws exceptions;

2、always use the resource via an instance of a RAII-class that either

- has **[automatic storage duration](https://en.cppreference.com/w/cpp/language/storage_duration)** or **temporary lifetime** itself, or
- has lifetime that is bounded by the lifetime of an automatic or temporary object

**Move semantics** make it possible to safely transfer resource ownership between objects, across scopes, and in and out of threads, while maintaining resource safety.

Classes with `open()`/`close()`, `lock()`/`unlock()`, or `init()`/`copyFrom()`/`destroy()` member functions are typical examples of non-RAII classes:

```c++
std::mutex m;
 
void bad() 
{
    m.lock();                    // acquire the mutex
    f();                         // if f() throws an exception, the mutex is never released
    if(!everything_ok()) return; // early return, the mutex is never released
    m.unlock();                  // if bad() reaches this statement, the mutex is released
}
 
void good()
{
    std::lock_guard<std::mutex> lk(m); // RAII class: mutex acquisition is initialization
    f();                               // if f() throws an exception, the mutex is released
    if(!everything_ok()) return;       // early return, the mutex is released
}                                      // if good() returns normally, the mutex is released
```

### The standard library

The C++ library classes that manage their own resources follow RAII: [std::string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::thread](https://en.cppreference.com/w/cpp/thread/thread), and many others acquire their resources in constructors (which throw exceptions on errors), release them in their destructors (which never throw), and don't require explicit cleanup.

In addition, the standard library offers several RAII wrappers to manage user-provided resources:

1、[std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) and [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) to manage dynamically-allocated memory or, with a user-provided deleter, any resource represented by a plain pointer;

2、[std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard), [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock), [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock) to manage mutexes.

### Notes

RAII does not apply to the management of the resources that are not acquired before use: CPU time, cores, and cache capacity, entropy pool capacity, network bandwidth, electric power consumption, stack memory.





## wikipedia [Resource acquisition is initialization](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) 

> NOTE: 本文主要讨论了RAII idiom的写法以及这样写的好处。

**Resource acquisition is initialization** (**RAII**)  is a [programming idiom](https://en.wikipedia.org/wiki/Programming_idiom) used in several [object-oriented languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language) to describe a particular language behavior. In **RAII**, holding a resource is a [class invariant](https://en.wikipedia.org/wiki/Class_invariant)(), and is tied to [object lifetime](https://en.wikipedia.org/wiki/Object_lifetime): [resource allocation](https://en.wikipedia.org/wiki/Resource_allocation_(computer)) (or acquisition) is done during object creation (specifically initialization), by the [constructor](https://en.wikipedia.org/wiki/Constructor_(object-oriented_programming)), while resource deallocation (release) is done during object destruction (specifically finalization), by the [destructor](https://en.wikipedia.org/wiki/Destructor_(computer_programming)). Thus the resource is guaranteed to be held between when **initialization** finishes and **finalization** starts (holding the resources is a class invariant), and to be held only when the object is alive. Thus if there are no **object leaks**, there are no [resource leaks](https://en.wikipedia.org/wiki/Resource_leak).

> NOTE : object-oriented programming告诉我们使用class来描述，抽象事物，object就是一个class的instance；RAII使用一个object来管理resource，这是否也符合object-oriented programming呢？我感觉这是符合object-oriented programming的。

**RAII** is associated most prominently with [C++](https://en.wikipedia.org/wiki/C%2B%2B) where it originated, but also [D](https://en.wikipedia.org/wiki/D_(programming_language)), [Ada](https://en.wikipedia.org/wiki/Ada_(programming_language)), [Vala](https://en.wikipedia.org/wiki/Vala_(programming_language)), and [Rust](https://en.wikipedia.org/wiki/Rust_(programming_language)). The technique was developed for [exception-safe](https://en.wikipedia.org/wiki/Exception_safety) [resource management](https://en.wikipedia.org/wiki/Resource_management_(computing)) in C++ during 1984–89, primarily by [Bjarne Stroustrup](https://en.wikipedia.org/wiki/Bjarne_Stroustrup) and [Andrew Koenig](https://en.wikipedia.org/wiki/Andrew_Koenig_(programmer)), and the term itself was coined by Stroustrup. RAII is generally pronounced as an [initialism](https://en.wikipedia.org/wiki/Initialism), sometimes pronounced as "R, A, double I". 

Other names for this idiom include *Constructor Acquires, Destructor Releases* (CADRe)  and one particular style of use is called *Scope-based Resource Management* (SBRM). This latter term is for the special case of [automatic variables](https://en.wikipedia.org/wiki/Automatic_variable). **RAII** ties resources to object *lifetime,* which may not coincide(重叠) with entry and exit of a scope. (Notably variables allocated on the **free store**(heap) have lifetimes unrelated to any given scope.) However, using **RAII** for automatic variables (SBRM) is the most common use case.

> NOTE: *Constructor Acquires, Destructor Releases*这种描述比起RAII更加容易理解。其实对RAII的好处的理解也就是作者提出RAII的意图的理解还涉及到c++中何时调用析构函数。结合后面的内容，可以肯定的是在一个scope结束的时候，就会调用destructor。并且，在阅读了后面的内容后，可以肯定的是RAII的好处主要体现在SBRM，它能够保证程序的安全性。

C++11 example H3

The following [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) example demonstrates usage of **RAII** for file access and mutex locking:

```c++
#include <mutex>
#include <iostream>
#include <string> 
#include <fstream>
#include <stdexcept>

void write_to_file (const std::string & message) {
    // mutex to protect file access (shared across threads)
    static std::mutex mutex;

    // lock mutex before accessing file
    std::lock_guard<std::mutex> lock(mutex);

    // try to open file
    std::ofstream file("example.txt");
    if (!file.is_open())
        throw std::runtime_error("unable to open file");
    
    // write message to file
    file << message << std::endl;
    
    // file will be closed 1st when leaving scope (regardless of exception)
    // mutex will be unlocked 2nd (from lock destructor) when leaving
    // scope (regardless of exception)
}
```

This code is **exception-safe** because `C++` guarantees that all **stack objects** are destroyed at the end of the **enclosing scope**, known as [stack unwinding](https://en.wikipedia.org/wiki/Stack_unwinding). The destructors of both the *lock* and *file* objects are therefore guaranteed to be called when returning from the function, whether an **exception** has been thrown or not.[[9\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-dtors-shouldnt-throw-9)

> NOTE: 
>
> 总结：最后一句话是非常重要的：无论是否抛出异常，当从该函数返回的时候，*lock*和*file*的析构函数都能够保证被调用。那这就引起了我对c++11的异常管理的好奇了，c++11的异常管理机制是怎样的呢？

**Local variables** allow easy management of multiple resources within a single function: they are destroyed in the reverse order of their construction, and an object is destroyed only if fully constructed—that is, if no exception propagates from its constructor.[[10\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-C++17_International_Standard_(%C2%A76.6.4)-10)

Using **RAII** greatly simplifies resource management, reduces overall code size and helps ensure program correctness. RAII is therefore highly recommended in C++, and most of the C++ standard library follows the idiom.[[11\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-too-many-trycatch-blocks-11)

### Benefits 

The advantages of **RAII** as a resource management technique are that it provides 

- encapsulation
- [exception safety](https://en.wikipedia.org/wiki/Exception_safety) (for stack resources), 
- locality (it allows **acquisition** and **release** logic to be written next to each other).

**Encapsulation** is provided because resource management logic is defined once in the class, not at each call site. 

**Exception safety** is provided for stack resources (resources that are released in the same scope as they are acquired) by tying(绑定) the resource to the lifetime of a **stack variable** (a **local variable** declared in a given scope): if an [exception](https://en.wikipedia.org/wiki/Exception_handling) is thrown, and proper exception handling is in place, the only code that will be executed when exiting the current [scope](https://en.wikipedia.org/wiki/Scope_(computer_science)) are the **destructors** of objects declared in that scope.

> NOTE: 
>
> 总结：其实从这段话中也是可以推测出c++的异常处理机制的：在一个函数执行的过程中如果出现了异常，则会退出当前scope的执行，但是在退出当前scope之前是会调用所有声明在当前域中的object的destructor的。

Finally, **locality of definition** is provided by writing the constructor and destructor definitions next to each other in the class definition.

Resource management therefore needs to be tied to the lifespan of suitable objects in order to gain automatic **allocation** and **reclamation**. Resources are acquired during initialization, when there is no chance of them being used before they are available, and released with the destruction of the same objects, which is guaranteed to take place even in case of errors.

Comparing **RAII** with the `finally` construct used in Java, Stroustrup wrote that “In realistic systems, there are far more resource acquisitions than kinds of resources, so the "resource acquisition is initialization" technique leads to less code than use of a "finally" construct.”[[1\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-faq-1)

### Typical uses 

The **RAII** design is often used for controlling **[mutex](https://en.wikipedia.org/wiki/Mutex) locks** in [multi-threaded](https://en.wikipedia.org/wiki/Thread_(computing)#Multithreading) applications. In that use, the object releases the lock when destroyed(对象在被销毁的时候释放锁). Without **RAII** in this scenario the potential for [deadlock](https://en.wikipedia.org/wiki/Deadlock) would be high and **the logic to lock the mutex** would be far from **the logic to unlock it**. With **RAII**, the code that locks the mutex essentially includes the logic that the lock will be released when execution leaves the scope of the RAII object(使用RAII，锁定互斥锁的代码基本上包括当执行离开RAII对象的范围时将释放锁的逻辑).

Another typical example is interacting with files: We could have an object that represents a file that is open for writing, wherein the file is opened in the constructor and closed when execution leaves the object's scope. In both cases, RAII ensures only that the resource in question is released appropriately; care must still be taken to maintain exception safety. If the code modifying the data structure or file is not exception-safe, the mutex could be unlocked or the file closed with the data structure or file corrupted.

Ownership of dynamically allocated objects (memory allocated with `new` in C++) can also be controlled with **RAII**, such that the object is released when the **RAII** (stack-based) object is destroyed. For this purpose, the [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) standard library defines the [smart pointer](https://en.wikipedia.org/wiki/Smart_pointer) classes `std::unique_ptr` for **single-owned objects** and `std::shared_ptr` for objects with **shared ownership**. Similar classes are also available through `std::auto_ptr` in C++98, and `boost::shared_ptr` in the [Boost libraries](https://en.wikipedia.org/wiki/Boost_(C%2B%2B_libraries)).

### Clang and GCC "cleanup" extension for C 

Both [Clang](https://en.wikipedia.org/wiki/Clang) and [GNU Compiler Collection](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) implement a non-standard extension to the [C](https://en.wikipedia.org/wiki/C_(programming_language)) language to support RAII: the "cleanup" variable attribute.[[12\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-12)The following [macro](https://en.wikipedia.org/wiki/Macro_(computer_science)) annotates a variable with a given **destructor function** that it will call when the variable goes out of scope:

```c++
static inline void fclosep(FILE **fp) { if (*fp) fclose(*fp); }
#define _cleanup_fclose_ __attribute__((cleanup(fclosep)))
```

This macro can then be used as follows:

```c++
void example_usage() {
  _cleanup_fclose_ FILE *logfile = fopen("logfile.txt", "w+");
  fputs("hello logfile!", logfile);
}
```

In this example, the compiler arranges for the *fclosep* function to be called on *logfile* before *example_usage* returns.



### Limitations 

**RAII** only works for resources acquired and released (directly or indirectly) by **stack-allocated objects**, where there *is* a well-defined static object lifetime. Heap-allocated objects which themselves acquire and release resources are common in many languages, including `C++`. RAII depends on heap-based objects to be implicitly or explicitly deleted along all possible execution paths, in order to trigger its resource-releasing destructor (or equivalent).[[13\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-toplas2008-13):8:27 This can be achieved by using [smart pointers](https://en.wikipedia.org/wiki/Smart_pointer) to manage all heap objects, with weak-pointers for cyclically referenced objects.

In `C++`, **stack unwinding** is only guaranteed to occur if the exception is caught somewhere. This is because "If no matching handler is found in a program, the function `terminate()` is called; whether or not the stack is unwound before this call to `terminate()` is implementation-defined (15.5.1)." (C++03 standard, §15.3/9).[[14\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-stackoverflow11-14) This behavior is usually acceptable, since the operating system releases remaining resources like memory, files, sockets, etc. at program termination.



### Reference counting 

[Perl](https://en.wikipedia.org/wiki/Perl), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) (in the [CPython](https://en.wikipedia.org/wiki/CPython) implementation),[[15\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-15) and [PHP](https://en.wikipedia.org/wiki/PHP)[[16\]](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization#cite_note-16) manage object lifetime by [reference counting](https://en.wikipedia.org/wiki/Reference_counting), which makes it possible to use RAII. Objects that are no longer referenced are immediately destroyed or finalized and released, so a [destructor](https://en.wikipedia.org/wiki/Destructor_(computer_programming)) or [finalizer](https://en.wikipedia.org/wiki/Finalizer) can release the resource at that time. However, it is not always idiomatic in such languages, and is specifically discouraged in Python (in favor of [context managers](https://en.wikipedia.org/w/index.php?title=Context_manager&action=edit&redlink=1) and *finalizers* from the *weakref* package).

However, **object lifetimes** are not necessarily bound to any scope, and objects may be destroyed non-deterministically or not at all. This makes it possible to accidentally leak resources that should have been released at the end of some scope. Objects stored in a static variable (notably a global variable) may not be finalized when the program terminates, so their resources are not released; CPython makes no guarantee of finalizing such objects, for instance. Further, objects with circular references will not be collected by a simple reference counter, and will live indeterminately long; even if collected (by more sophisticated garbage collection), destruction time and destruction order will be non-deterministic. In CPython there is a cycle detector which detects cycles and finalizes the objects in the cycle, though prior to CPython 3.4, cycles are not collected if any object in the cycle has a finalizer.[[17\]]





## 一些思考

### RAII and GC

`C++`的RAII让我想到了python中的`with`、 [context managers](https://en.wikipedia.org/w/index.php?title=Context_manager&action=edit&redlink=1) 。上面这篇文章中的Reference counting 章节就谈到了这一点。

### zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 

这是阅读这篇文章的感想：RAII虽然简单，但是能够处理非常多的问题：将一个resource的lifetime与一个object的lifetime bind到一起。c++又允许programmer来控制object的lifetime，同时C++有支持value and  reference semantic， 因此对于resource的处理将变得非常灵活且安全。GC的language无法实现RAII，因为它们无法控制object的lifetime。



## See also

1、florianwolters [The Rule of Zero](http://blog.florianwolters.de/educational/2015/01/31/The_Rule_of_Zero/)

在其中也对RAII进行了描述。

2、tomdalling [Resource Acquisition is Initialisation (RAII) Explained](https://www.tomdalling.com/blog/software-design/resource-acquisition-is-initialisation-raii-explained/)




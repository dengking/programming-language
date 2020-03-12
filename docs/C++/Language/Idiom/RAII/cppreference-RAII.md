
# [RAII](https://en.cppreference.com/w/cpp/language/raii)

*Resource Acquisition Is Initialization* or RAII, is a C++ programming technique[[1\]](https://en.cppreference.com/w/cpp/language/raii#cite_note-1)[[2\]](https://en.cppreference.com/w/cpp/language/raii#cite_note-2) which binds the life cycle of a **resource** that must be acquired before use (allocated heap memory, thread of execution, open socket, open file, locked mutex, disk space, database connection—anything that exists in limited supply) to the [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) of an object.

***SUMMARY*** : 上面这段话的句子主干是: bind the life cycle of a resource to the lifetime of an object.

RAII guarantees that the resource is available to any function that may access the object (resource availability is a [class invariant](https://en.wikipedia.org/wiki/Class_invariant), eliminating redundant runtime tests). It also guarantees that all resources are released when the lifetime of their controlling object ends, in reverse order of acquisition. Likewise, if **resource acquisition** fails (the constructor exits with an exception), all resources acquired by every **fully-constructed member** and base subobject are released in reverse order of initialization. This leverages the core language features ([object lifetime](https://en.cppreference.com/w/cpp/language/lifetime), [scope exit](https://en.cppreference.com/w/cpp/language/statements), [order of initialization](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order) and [stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding)) to eliminate resource leaks and guarantee **exception safety**. Another name for this technique is *Scope-Bound Resource Management* (SBRM), after the basic use case where the lifetime of an RAII object ends due to scope exit.

RAII can be summarized as follows:

- encapsulate each resource into a class, where

  - the constructor acquires the resource and establishes all class invariants or throws an exception if that cannot be done,

  - the destructor releases the resource and never throws exceptions;

- always use the resource via an instance of a RAII-class that either

  - has **[automatic storage duration](https://en.cppreference.com/w/cpp/language/storage_duration)** or **temporary lifetime** itself, or
  - has lifetime that is bounded by the lifetime of an automatic or temporary object

**Move semantics** make it possible to safely transfer resource ownership between objects, across scopes, and in and out of threads, while maintaining resource safety.

Classes with open()/close(), lock()/unlock(), or init()/copyFrom()/destroy() member functions are typical examples of non-RAII classes:

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

- [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) and [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) to manage dynamically-allocated memory or, with a user-provided deleter, any resource represented by a plain pointer;
- [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard), [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock), [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock) to manage mutexes.

### Notes

RAII does not apply to the management of the resources that are not acquired before use: CPU time, cores, and cache capacity, entropy pool capacity, network bandwidth, electric power consumption, stack memory.



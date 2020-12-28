# Placement new

在学习perfect forwarding的时候，阅读了thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/) ，其中以 `emplace_back` 为例来进行说明进行说明，所以我又查阅了 cppreference [`std::vector<T,Allocator>::emplace_back`](https://en.cppreference.com/w/cpp/container/vector/emplace_back)，其中有这样的说明:

> Appends a new element to the end of the container. The element is constructed through [std::allocator_traits::construct](https://en.cppreference.com/w/cpp/memory/allocator_traits/construct), which typically uses placement-new to construct the element in-place at the location provided by the container.

其中的 "placement-new" 让我发现了 "placement-new" 用法。



## stackoverflow [What uses are there for “placement new”?](https://stackoverflow.com/questions/222557/what-uses-are-there-for-placement-new)

**comments**

- It is used in [the C++11 Wikipedia article](https://en.wikipedia.org/wiki/C%2B%2B11#Unrestricted_unions) in the constructor of a union.



### [A](https://stackoverflow.com/a/222578)

**Placement new allows you to construct an object in memory that's already allocated.**

You may want to do this for optimization when you need to construct multiple instances of an object, and it is faster not to re-allocate memory each time you need a new instance. Instead, it might be more efficient to perform a single allocation for a chunk of memory that can hold multiple objects, even though you don't want to use all of it at once.

DevX gives a [good example](http://www.devx.com/tips/Tip/12582):

> Standard C++ also supports placement new operator, which constructs an object on a pre-allocated **buffer**. This is useful when building a **memory pool**, a **garbage collector** or simply when **performance** and **exception safety** are paramount(至高无上的) (there's no danger of allocation failure since the memory has already been allocated, and constructing an object on a pre-allocated buffer takes less time):

```cpp
char *buf  = new char[sizeof(string)]; // pre-allocated buffer
string *p = new (buf) string("hi");    // placement new
string *q = new string("hi");          // ordinary heap allocation
```

You may also want to be sure there can be no allocation failure at a certain part of critical code (for instance, in code executed by a pacemaker(心脏起搏器)). In that case you would want to allocate memory earlier, then use placement new within the critical section.

**Deallocation in placement new**

You should not deallocate every object that is using the **memory buffer**. Instead you should `delete[]` only the original buffer. You would have to then call the destructors of your classes manually. For a good suggestion on this, please see Stroustrup's FAQ on: [Is there a "placement delete"](http://www.stroustrup.com/bs_faq2.html#placement-delete)?

### [A](https://stackoverflow.com/a/222615)



We use it with custom **memory pools**. Just a sketch:

```cpp
class Pool {
public:
    Pool() { /* implementation details irrelevant */ };
    virtual ~Pool() { /* ditto */ };

    virtual void *allocate(size_t);
    virtual void deallocate(void *);

    static Pool::misc_pool() { return misc_pool_p; /* global MiscPool for general use */ }
};

class ClusterPool : public Pool { /* ... */ };
class FastPool : public Pool { /* ... */ };
class MapPool : public Pool { /* ... */ };
class MiscPool : public Pool { /* ... */ };

// elsewhere...

void *pnew_new(size_t size)
{
   return Pool::misc_pool()->allocate(size);
}

void *pnew_new(size_t size, Pool *pool_p)
{
   if (!pool_p) {
      return Pool::misc_pool()->allocate(size);
   }
   else {
      return pool_p->allocate(size);
   }
}

void pnew_delete(void *p)
{
   Pool *hp = Pool::find_pool(p);
   // note: if p == 0, then Pool::find_pool(p) will return 0.
   if (hp) {
      hp->deallocate(p);
   }
}

// elsewhere...

class Obj {
public:
   // misc ctors, dtors, etc.

   // just a sampling of new/del operators
   void *operator new(size_t s)             { return pnew_new(s); }
   void *operator new(size_t s, Pool *hp)   { return pnew_new(s, hp); }
   void operator delete(void *dp)           { pnew_delete(dp); }
   void operator delete(void *dp, Pool*)    { pnew_delete(dp); }

   void *operator new[](size_t s)           { return pnew_new(s); }
   void *operator new[](size_t s, Pool* hp) { return pnew_new(s, hp); }
   void operator delete[](void *dp)         { pnew_delete(dp); }
   void operator delete[](void *dp, Pool*)  { pnew_delete(dp); }
};

// elsewhere...

ClusterPool *cp = new ClusterPool(arg1, arg2, ...);

Obj *new_obj = new (cp) Obj(arg_a, arg_b, ...);
```

Now you can cluster objects together in a single memory arena, select an allocator which is very fast but does no deallocation, use memory mapping, and any other semantic you wish to impose by choosing the pool and passing it as an argument to an object's placement new operator.

### [A](https://stackoverflow.com/a/222573)

It's useful if you want to separate allocation from initialization. STL uses placement new to create container elements.

### [A](https://stackoverflow.com/a/222817)

I've used it in **real-time** programming. We typically *don't* want to perform any dynamic allocation (or deallocation) after the system starts up, because there's no guarantee how long that is going to take.

What I can do is preallocate a large chunk of memory (large enough to hold any amount of whatever that the class may require). Then, once I figure out at runtime how to construct the things, placement new can be used to construct objects right where I want them. One situation I know I used it in was to help create a heterogeneous [circular buffer](http://en.wikipedia.org/wiki/Circular_buffer).

It's certainly not for the faint of heart, but that's why they make the syntax for it kinda gnarly.

## geeksforgeeks [Placement new operator in C++](https://www.geeksforgeeks.org/placement-new-operator-cpp/)



## cppreference [new expression # Allocation # Placement new](https://en.cppreference.com/w/cpp/language/new#Allocation)

If `placement_params` are provided, they are passed to the allocation function as additional arguments. Such allocation functions are known as "placement new", after the standard allocation function `void*` [operator new](http://en.cppreference.com/w/cpp/memory/new/operator_new)([std::size_t](http://en.cppreference.com/w/cpp/types/size_t)`, void*)`, which simply returns its second argument unchanged. This is used to construct objects in allocated storage:

```C++
// within any block scope...
{
    alignas(T) unsigned char buf[sizeof(T)];
    // Statically allocate the storage with automatic storage duration
    // which is large enough for any object of type `T`.
    T* tptr = new(buf) T; // Construct a `T` object, placing it directly into your 
                          // pre-allocated storage at memory address `buf`.
    tptr->~T();           // You must **manually** call the object's destructor
                          // if its side effects is depended by the program.
}                         // Leaving this block scope automatically deallocates `buf`.
```

Note: this functionality is encapsulated by the member functions of the [*Allocator*](https://en.cppreference.com/w/cpp/named_req/Allocator) classes.

---

### since C++17

When allocating an object whose alignment requirement exceeds `__STDCPP_DEFAULT_NEW_ALIGNMENT__` or an array of such objects, the new-expression passes the alignment requirement (wrapped in [std::align_val_t](https://en.cppreference.com/w/cpp/memory/new/align_val_t)) as the second argument for the allocation function (for placement forms, `placement_params` appear after the alignment, as the third, fourth, etc arguments). If overload resolution fails (which happens when a class-specific allocation function is defined with a different signature, since it hides the globals), overload resolution is attempted a second time, without alignment in the argument list. This allows alignment-unaware class-specific allocation functions to take precedence over the global alignment-aware allocation functions.

```C++
new T;      // calls operator new(sizeof(T))
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T))))
new T[5];   // calls operator new[](sizeof(T)*5 + overhead)
            // (C++17) or operator new(sizeof(T)*5+overhead, std::align_val_t(alignof(T))))
new(2,f) T; // calls operator new(sizeof(T), 2, f)
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)
```

---

If a non-throwing allocation function (e.g. the one selected by new([std::nothrow](http://en.cppreference.com/w/cpp/memory/new/nothrow)) T) returns a null pointer because of an allocation failure, then the new-expression returns immediately, it does not attempt to initialize an object or to call a deallocation function. If a null pointer is passed as the argument to a non-allocating placement new-expression, which makes the selected standard non-allocating placement allocation function return a null pointer, the behavior is undefined.
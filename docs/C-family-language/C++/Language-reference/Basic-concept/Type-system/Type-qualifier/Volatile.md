# volatile 

## volatile in c++

[Why does volatile exist?](https://stackoverflow.com/questions/72552/why-does-volatile-exist)

[C++ and the Perils of Double-Checked Locking ∗](http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf)

[volatile: The Multithreaded Programmer's Best Friend](http://www.drdobbs.com/cpp/volatile-the-multithreaded-programmers-b/184403766)

[Const and volatile](http://publications.gbdirect.co.uk/c_book/chapter8/const_and_volatile.html)

[Why do we use volatile keyword in C++?](https://stackoverflow.com/questions/4437527/why-do-we-use-volatile-keyword-in-c/4437555#4437555)

这个解释是非常好的。



## volatile: The Multithreaded Programmer's Best Friend

The `volatile` keyword was devised(设计) to prevent **compiler optimizations** that might render code incorrect in the presence of certain asynchronous events.

总结：合理地利用compiler optimization是非常重要的。

I don't want to spoil your mood, but this column addresses the dreaded topic of multithreaded programming(我不想破坏你的心情，但本专栏解决了多线程编程的可怕主题). If — as the previous installment of Generic<Programming> says — exception-safe programming is hard, it's child's play compared to multithreaded programming( 异常安全编程很难，那么与多线程编程相比，它就是孩子的游戏).



Programs using multiple threads are notoriously hard to write, prove correct, debug, maintain, and tame in general. Incorrect multithreaded programs might run for years without a glitch, only to unexpectedly run amok because some critical timing condition has been met.

Needless to say(毋庸置疑), a programmer writing multithreaded code needs all the help she can get. This column focuses on **race conditions** — a common source of trouble in multithreaded programs — and provides you with **insights** and **tools** on how to avoid them and, amazingly enough, have the compiler work hard at helping you with that(令人惊讶的是，让编译器努力帮助您解决这个问题).

### Just a Little Keyword

Although both `C` and `C++` Standards are conspicuously silent when it comes to threads, they do make a little concession(让步) to multithreading, in the form of the `volatile` keyword.

Just like its better-known counterpart(对应) `const`, `volatile` is a **type modifier**. It's intended to be used in conjunction with variables that are accessed and modified in different threads. Basically, without `volatile`, either writing multithreaded programs becomes impossible, or the compiler wastes vast optimization opportunities. An explanation is in order.

Consider the following code:

```cpp
class Gadget
{
public:
    void Wait()
    {
        while (!flag_)
        {
            Sleep(1000); // sleeps for 1000 milliseconds
        }
    }
    void Wakeup()
    {
        flag_ = true;
    }
    ...
private:
    bool flag_;
};
```

The purpose of `Gadget::Wait` above is to check the `flag_` member variable every second and return when that variable has been set to `true` by another thread. At least that's what its programmer intended, but, alas, `Wait` is incorrect.

Suppose the compiler figures out that `Sleep(1000)` is a call into an **external library** that cannot possibly modify the member variable `flag_`. Then the compiler concludes that it can cache `flag_`in a **register** and use that register instead of accessing the slower on-board **memory**. This is an excellent optimization for single-threaded code, but in this case, it harms correctness: after you call `Wait` for some `Gadget` object, although another thread calls `Wakeup`, `Wait` will loop forever. This is because the change of `flag_` will not be reflected in the **register** that caches `flag_`. The optimization is too ... optimistic.

**Caching variables in registers** is a very valuable optimization that applies most of the time, so it would be a pity to waste it. C and C++ give you the chance to explicitly disable such caching. If you use the `volatile` modifier on a variable, the compiler won't cache that variable in registers — each access will **hit the actual memory location** of that variable. So all you have to do to make `Gadget`'s `Wait`/`Wakeup` combo work is to qualify(修饰，限定) `flag_` appropriately:

```cpp
class Gadget
{
public:
    ... as above ...
private:
    volatile bool flag_;
};
```

Most explanations of the rationale(基本原理) and usage of `volatile` stop here and advise you to `volatile`-qualify the **primitive types** that you use in multiple threads. However, there is much more you can do with `volatile`, because it is part of C++'s wonderful type system.

### Using volatile with User-Defined Types

You can `volatile`-qualify not only **primitive types**, but also **user-defined types**. In that case, `volatile` modifies the type in a way similar to `const`. (You can also apply `const` and `volatile` to the same type simultaneously.)

Unlike `const`, `volatile` discriminates(区分) between **primitive types** and **user-defined types**. Namely, unlike classes, **primitive types** still support all of their operations (addition, multiplication, assignment, etc.) when `volatile`-qualified. For example, you can assign a non-`volatile` `int` to a `volatile` `int`, but you cannot assign a non-`volatile` object to a `volatile` object.

总结：上面这段话的意思是，对于**primitive types** 和**user-defined types**，对**primitive types** 而言，`volatile`并不改变其operations，但是对于 **user-defined types**而言，`volatile`则会改变其operations。

Let's illustrate how `volatile` works on user-defined types on an example.

```cpp
class Gadget
{
public:
    void Foo() volatile;
    void Bar();
    ...
private:
    String name_;
    int state_;
};
...
Gadget regularGadget;
volatile Gadget volatileGadget;
```

If you think `volatile` is not that useful with objects, prepare for some surprise.

```cpp
volatileGadget.Foo(); // ok, volatile fun called for
                      // volatile object
 
 
regularGadget.Foo();  // ok, volatile fun called for
                      // non-volatile object
volatileGadget.Bar(); // error! Non-volatile function called for
                      // volatile object!
```

The conversion from a **non-qualified type** to its `volatile` counterpart is trivial. However, just as with `const`, you cannot make the trip back from `volatile` to **non-qualified**. You must use a cast:

```cpp
Gadget& ref = const_cast<Gadget&>(volatileGadget);
ref.Bar(); // ok
```

A `volatile`-qualified class gives access only to a **subset** of its interface, a subset that is under the control of the class implementer. Users can gain full access to that type's interface only by using a `const_cast`. In addition, just like `const`ness, `volatile`ness propagates from the **class** to its **members** (for example, `volatileGadget.name_` and `volatileGadget.state_` are `volatile` variables)但是并不会propagate to member method.

总结：这段描述了给object添加`volatile`所带来的效果。

### volatile, Critical Sections, and Race Conditions

The simplest and the most often-used synchronization device in multithreaded programs is the `mutex`. A `mutex` exposes the `Acquire` and `Release` primitives. Once you call `Acquire` in some thread, any other thread calling `Acquire` will block. Later, when that thread calls `Release`, precisely one thread blocked in an `Acquire` call will be released. In other words, for a given `mutex`, only one thread can get processor time in between a call to `Acquire` and a call to `Release`. The executing code between a call to `Acquire` and a call to `Release` is called a *critical section*. (Windows terminology is a bit confusing because it calls the mutex itself a critical section, while "mutex" is actually an inter-process mutex. It would have been nice if they were called thread mutex and process mutex.)

Mutexes are used to protect data against **race conditions**. By definition, a **race condition** occurs when the effect of more threads on data depends on how threads are scheduled. **Race conditions** appear when two or more threads compete for using the same data. Because threads can interrupt(打断) each other at arbitrary moments in time, data can be corrupted(损坏) or misinterpreted. Consequently, changes and sometimes accesses to data must be carefully protected with critical sections. In object-oriented programming, this usually means that you store a `mutex` in a class as a member variable and use it whenever you access that class' state.

Experienced multithreaded programmers might have yawned(打哈欠) reading the two paragraphs above, but their purpose is to provide an intellectual workout, because now we will link with the `volatile` connection. We do this by drawing a parallel between the C++ types' world and the threading semantics world.

- `Outside` a **critical section**, any thread might interrupt any other at any time; there is no control, so consequently variables accessible from multiple threads are `volatile`. This is in keeping with the original intent of `volatile` — that of preventing the compiler from unwittingly caching values used by multiple threads at once.
- `Inside` a **critical section** defined by a `mutex`, only *one* thread has access. Consequently, *inside a critical section, the executing code has single-threaded semantics*. The controlled variable is not `volatile` anymore — you can remove the `volatile` qualifier.

In short, data shared between threads is conceptually `volatile` outside a critical section, and non-`volatile` inside a critical section.

思考：对于data shared between threads而言，inside a critical section，它们能否是`volatile`的？

You enter a critical section by locking a `mutex`. You remove the `volatile` qualifier from a type by applying a `const_cast`. If we manage to put these two operations together, *we create a connection between C++'s type system and an application's threading semantics*. We can make the compiler check **race conditions** for us.

### LockingPtr

We need a tool that collects a `mutex` acquisition and a `const_cast`. Let's develop a `LockingPtr`class template that you initialize with a `volatile` object `obj` and a mutex `mtx`. During its lifetime, a `LockingPtr` keeps `mtx` acquired. Also, `LockingPtr` offers access to the `volatile`-stripped `obj`(通过const_cast将`volatile`去除). The access is offered in a smart pointer fashion, through `operator->` and `operator*`. The `const_cast` is performed inside `LockingPtr`. The cast is semantically valid because `LockingPtr` keeps the `mutex `acquired for its lifetime.

First, let's define the skeleton of a class `Mutex` with which `LockingPtr` will work:

```cpp
class Mutex
{
public:
    void Acquire();
    void Release();
    ...    
};
```

To use `LockingPtr`, you implement `Mutex` using your operating system's native data structures and primitive functions.

`LockingPtr` is templated with the type of the controlled variable. For example, if you want to control a `Widget`, you use a `LockingPtr` `<Widget>` that you initialize with a variable of type `volatile Widget`.

`LockingPtr`'s definition is very simple. `LockingPtr` implements an unsophisticated smart pointer. It focuses solely on collecting a `const_cast` and a critical section.

```cpp
template <typename T>
class LockingPtr {
public:
   // Constructors/destructors
   LockingPtr(volatile T& obj, Mutex& mtx)
       : pObj_(const_cast<T*>(&obj)),
        pMtx_(&mtx)
   {    mtx.Lock();    }
   ~LockingPtr()
   {    pMtx_->Unlock();    }
   // Pointer behavior
   T& operator*()
   {    return *pObj_;    }
   T* operator->()
   {   return pObj_;   }
private:
   T* pObj_;
   Mutex* pMtx_;
   LockingPtr(const LockingPtr&);
   LockingPtr& operator=(const LockingPtr&);
};
```

In spite of its simplicity, `LockingPtr` is a very useful aid in writing correct multithreaded code. You should define objects that are shared between threads as `volatile` and never use `const_cast` with them — always use `LockingPtr` automatic objects. Let's illustrate this with an example.

Say you have two threads that share a `vector<char>` object:

```cpp
class SyncBuf {
public:
    void Thread1();
    void Thread2();
private:
    typedef vector<char> BufT;
    volatile BufT buffer_;
    Mutex mtx_; // controls access to buffer_
};
```

Inside a thread function, you simply use a `LockingPtr<BufT>` to get controlled access to the `buffer_` member variable:

```cpp
void SyncBuf::Thread1() {
    LockingPtr<BufT> lpBuf(buffer_, mtx_);
    BufT::iterator i = lpBuf->begin();
    for (; i != lpBuf->end(); ++i) {
        ... use *i ...
    }
}
```

The code is very easy to write and understand — whenever you need to use `buffer_`, you must create a `LockingPtr<BufT>` pointing to it. Once you do that, you have access to `vector`'s entire interface.

总结：从上面的代码可以看出，一个`volatile` object是无法访问其`non-volatile`方法的。

The nice part is that if you make a mistake, the compiler will point it out:

```c++
void SyncBuf::Thread2() {
    // Error! Cannot access 'begin' for a volatile object
    BufT::iterator i = buffer_.begin();
    // Error! Cannot access 'end' for a volatile object
    for (; i != lpBuf->end(); ++i) {
        ... use *i ...
    }
}
```

You cannot access any function of `buffer_` until you either apply a `const_cast` or use `LockingPtr`. The difference is that `LockingPtr` offers an *ordered* way of applying `const_cast` to volatile variables.

总结：因为在`LockingPtr`函数的构造函数中，会进行`Lock`，所以如果加锁失败，则会一直被block。

`LockingPtr` is remarkably expressive. If you only need to call one function, you can create an unnamed temporary `LockingPtr` object and use it directly:

```cpp
unsigned int SyncBuf::Size() {
    return LockingPtr<BufT>(buffer_, mtx_)->size();
}
```

### Back to Primitive Types

We saw how nicely `volatile` protects objects against uncontrolled access and how `LockingPtr`provides a simple and effective way of writing thread-safe code. Let's now return to **primitive types**, which are treated differently by `volatile`.

Let's consider an example where multiple threads share a variable of type `int`.

```cpp
class Counter
{
public:
    ...
    void Increment() { ++ctr_; }
    void Decrement() { —ctr_; }
private:
    int ctr_;
};
```

If `Increment` and `Decrement` are to be called from different threads, the fragment above is buggy. First, `ctr_` must be `volatile`. Second, even a seemingly atomic operation such as `++ctr_` is actually a **three-stage operation**(并非原子操作). Memory itself has no arithmetic capabilities. When incrementing a variable, the processor:

- Reads that variable in a register
- Increments the value in the register
- Writes the result back to memory

This **three-step operation** is called RMW (Read-Modify-Write). During the Modify part of an RMW operation, most processors free the memory bus in order to give other processors access to the memory.

If at that time another processor performs a RMW operation on the same variable, we have a **race condition**: the second write overwrites the effect of the first.

To avoid that, you can rely, again, on `LockingPtr`:

```cpp
class Counter
{
public:
    ...
    void Increment() { ++*LockingPtr<int>(ctr_, mtx_); }
    void Decrement() { —*LockingPtr<int>(ctr_, mtx_); }
private:
    volatile int ctr_;
    Mutex mtx_;
};
```

Now the code is correct, but its quality is inferior when compared to `SyncBuf`'s code. Why? Because with `Counter`, the compiler will *not* warn you if you mistakenly access `ctr_` directly (without locking it). The compiler compiles `++ctr_` if `ctr_` is `volatile`, although the generated code is simply incorrect. The compiler is not your ally(盟友) anymore, and only your attention can help you avoid race conditions.

总结：这段话中的意思其实在前面的[Using volatile with User-Defined Types](#Using volatile with User-Defined Types)章节中已经描述了，不过这里结合了race condition一起来说的。

What should you do then? Simply encapsulate the primitive data that you use in higher-level structures and use `volatile` with those structures. Paradoxically(矛盾的是), it's worse to use `volatile`directly with built-ins, in spite of the fact that initially this was the usage intent of `volatile`!

### volatile Member Functions

So far, we've had classes that aggregate(聚合) `volatile` data members; now let's think of designing classes that in turn will be part of larger objects and shared between threads. Here is where `volatile` member functions can be of great help.

When designing your class, you `volatile`-qualify only those member functions that are thread safe. You must assume that code from the **outside** will call the `volatile` functions from any code at any time( 您必须假设来自外部的代码将随时从任何代码调用volatile函数). Don't forget: `volatile` equals free multithreaded code and no **critical section**; non-`volatile` equals single-threaded scenario or inside a critical section(不要忘记：volatile等于免费的多线程代码并且没有**critical section**;  non-`volatile` 等于单线程场景或处于**critical section**。).

For example, you define a class `Widget` that implements an operation in two variants — a thread-safe one and a fast, unprotected one.

```cpp
class Widget
{
public:
    void Operation() volatile;
    void Operation();
    ...
private:
    Mutex mtx_;
};
```

Notice the use of overloading. Now `Widget`'s user can invoke `Operation` using a uniform syntax either for `volatile` objects and get thread safety, or for regular objects and get speed. The user must be careful about defining the shared `Widget` objects as `volatile`.

When implementing a `volatile` member function, the first operation is usually to lock `this` with a `LockingPtr`. Then the work is done by using the non-` volatile` sibling:

```cpp
void Widget::Operation() volatile
{
    LockingPtr<Widget> lpThis(*this, mtx_);
    lpThis->Operation(); // invokes the non-volatile function
}
```

### Summary

When writing multithreaded programs, you can use `volatile` to your advantage. You must stick to the following rules:

- Define all shared objects as `volatile`.
- Don't use `volatile` directly with primitive types.
- When defining shared classes, use `volatile` member functions to express thread safety.

If you do this, and if you use the simple generic component `LockingPtr`, you can write thread-safe code and worry much less about race conditions, because the compiler will worry for you and will diligently point out the spots where you are wrong.

A couple of projects I've been involved with use `volatile` and `LockingPtr` to great effect. The code is clean and understandable. I recall a couple of deadlocks, but I prefer deadlocks to race conditions because they are so much easier to debug. There were virtually no problems related to race conditions. But then you never know.

### Acknowledgements

Many thanks to James Kanze and Sorin Jianu who helped with insightful ideas.
# drdobbs [Generic: Change the Way You Write Exception-Safe Code — Forever](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758)

> NOTE: 标题的意思是: "更改你书写exception-safe code的方式，永远"。

**Let's face it: Writing exception-safe code is hard. But it just got a lot easier with this amazing template.**

## Part 1

Let's face it: Writing exception-safe code is hard. But it just got a lot easier with this amazing template.

Call it overselling(过分吹嘘), but we'll tell you up front: we have killer material for this article. This is only because I convinced my good friend Petru Marginean to be my coauthor. Petru has developed a library facility that is helpful with exceptions. Together, we streamlined(使合理化) the implementation until we obtained a lean(精益的), mean library that can make writing **exception-safe code** much easier.

Let's face it, writing correct code in the presence of exceptions is a not an easy task. Exceptions establish a separate control flow that has little to do with the main control flow of the application. Figuring out the exception flow requires a different way of thinking, as well as new tools.

> NOTE: 大多数programmer面临的挑战

### Writing Exception-Safe Code Is Hard: An Example

Let's say you are developing one of those trendy(流行的) instant messaging servers. Users can log on and off the system and can send messages to each other. You hold a server-side database of users, plus in-memory information for users who are logged on. Each user can have friends. The list of friends is also kept both in the database and in memory.

When a user adds or removes a friend, you need to do two things: update the database and update the in-memory cache that you keep for that user. It's that simple.

Assuming that you model per-user information with a class called `User` and the user database with a `UserDatabase` class, the code for adding a friend might look like this:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
class User
{
    ...
    string GetName();
    void AddFriend(User& newFriend);
private:
    typedef vector<User*> UserCont;
    UserCont friends_;
    UserDatabase* pDB_;
};
void User::AddFriend(User& newFriend)
{
    // Add the new friend to the database
    pDB_->AddFriend(GetName(), newFriend.GetName());
    // Add the new friend to the vector of friends
    friends_.push_back(&newFriend);
}
```

Surprisingly, the two-liner `User::AddFriend` hides a pernicious(险恶的) bug. In an **out-of-memory condition**, `vector::push_back` can fail by throwing an exception. In that case, you will end up having the friend added to the database, but not to the in-memory information.

Now we've got a problem, haven't we? In any circumstance, this **inconsistent** information is dangerous. It is likely that many parts of your application are based on the assumption that the database is **in sync with** the in-memory information.

> NOTE: "in sync with"是"consistent"的同义词

A simple approach to the problem is to switch the two lines of code:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
void User::AddFriend(User& newFriend)
{
    // Add the new friend to the vector of friends
    // If this throws, the friend is not added to
    //     the vector, nor the database
    friends_.push_back(&newFriend);
    // Add the new friend to the database
    pDB_->AddFriend(GetName(), newFriend.GetName());
}
```

This definitely causes **consistency** in the case of `vector::push_back` failing. Unfortunately, as you consult `UserDatabase::AddFriend`'s documentation, you discover with annoyance that *it* can throw an exception, too! Now you might end up with the friend in the vector, but not in the database!

> NOTE: 关于consistency，参见 工程parallel-computing的`CAP\Consistency`章节

It's time to interrogate(质问) the database folks: "Why don't you guys return an error code instead of throwing an exception?" "Well," they say, "we're using a highly reliable cluster of XYZ database servers on a TZN network, so failure is extremely rare. Being this rare, we thought it's best to model failure with an exception, because exceptions appear only in exceptional conditions, right?"

It makes sense, but you still need to address failure. You don't want a database failure to drag the whole system towards chaos(混乱). This way you can fix the database without having to shut down the whole server.

In essence, you must do two operations, either of which can fail. If either fails, you must undo the whole thing. Let's see how this can be done.

### Solution 1: Brute Force

A simple solution is to throw in (sic!) a `try`-`catch` block:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
void User::AddFriend(User& newFriend)
{
    friends_.push_back(&newFriend);
    try
    {
        pDB_->AddFriend(GetName(), newFriend.GetName());
    }
    catch (...)
    {
        friends_.pop_back();
        throw;
    }
}
```

If `vector::push_back` fails, that's okay because `UserDatabase::AddFriend` is never reached. If `UserDatabase::AddFriend` fails, you catch the exception (no matter what it is), you undo the `push_back` operation with a call to `vector::pop_back`, and you nicely re-throw the exact same exception.

The code works, but at the cost of increased size and clumsiness. The two-liner just became a ten-liner. This technique isn't appealing; imagine littering all of your code with such try-catch statements.

Moreover, this technique doesn't scale well. Imagine you have a *third* operation to do. In that case, things suddenly become much clumsier. You can choose between equally awkward solutions: nested `try` statements or a more complicated control flow featuring additional flags. These solutions raise code bloating issues, efficiency issues, and, most important, severe understandability and maintenance issues.

### Solution 2: The Politically(政治上) Correct Approach

Show the above to any C++ expert, and you're likely to hear: "Nah, that's no good. You must use the *initialization is resource acquisition* idiom [[1\]](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#1) and leverage destructors for automatic resource deallocation in case of failure."

> NOTE: RAII

Okay, let's go down that path. For each operation that you must undo, there's a corresponding class. The constructor of that class "does" the operation, and the destructor rolls that operation back. Unless you call a "commit" function, in which case the destructor does nothing.

Some code will make all this crystal clear. For the `push_back` operation, let's put together a `VectorInserter` class like so:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
class VectorInserter
{
public:
    VectorInserter(std::vector<User*>& v, User& u)
    : container_(v), commit_(false)
    {
        container_.push_back(&u);
    }
    void Commit() throw()
    {
        commit_ = true;
    }
    ~VectorInserter()
    {
        if (!commit_) container_.pop_back();
    }
private:
    std::vector<User*>& container_;
    bool commit_;
};
```

Maybe the most important thing in the above code is the `throw()` specification next to `Commit`. It documents the reality that `Commit` always succeeds, because you already did the work — `Commit` just tells `VectorInserter`: "Everything's fine, don't **roll back** anything."

You use the whole machinery like this:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
void User::AddFriend(User& newFriend)
{
    VectorInserter ins(friends_, &newFriend);
    pDB_->AddFriend(GetName(), newFriend.GetName());
    // Everything went fine, commit the vector insertion
    ins.Commit();
}
```

`AddFriend` now has two distinct parts: the *activity* phase, in which the operations occur, and the *commitment* phase, which doesn't throw — it only stops the undo from happening.

> NOTE: 非常类似于two-phase commit protocol

The way `AddFriend` works is simple: if any operation fails, the point of commitment is not reached and the whole operation is called off. The inserter `pop_back`s the data entered, so the program remains in the **state** it was before calling `AddFriend`.

The idiom works nicely in all cases. If, for example, the vector insertion fails, the destructor of `ins` is not called, because `ins` isn't constructed. (If you designed C++, would you have called the destructor for an object whose very construction failed?)

This approach works just fine, but in the real world, it turns out not to be that neat. You must write a bunch of little classes to support this idiom. Extra classes mean extra code to write, intellectual(脑力的) overhead, and additional entries to your class browser. Moreover, it turns out there are lots of places where you must deal with exception safety. Let's face it, adding a new class every so often just for undoing an arbitrary operation in its destructor is not the most productive.

Also, `VectorInserter` has a bug. Did you notice it? `VectorInserter`'s **copy constructor** does very bad things. Defining classes is hard; that's another reason for avoiding writing lots of them.

### Solution 3: The Real Approach

> NOTE: "Solution 3: The Real Approach"其实是忽视exception，作者在后面驳斥了这种做法。从行文来看，它是承上启下。

It's one or the other: either you have reviewed all the options above, or you didn't have time or care for them. At the end of the day, do you know what the real approach is? Of course you do. Here it is:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758#)

```C++
void User::AddFriend(User& newFriend)
{
    friends_.push_back(&newFriend);
    pDB_->AddFriend(GetName(), newFriend.GetName());
}
```

It's a solution based upon not so scientific arguments.

"Who said memory's going to exhaust? There's half a gig in this box!"

"Even if memory does exhaust, the paging system will slow the program down to a crawl way before the program crashes."

"The database folks said `AddFriend` cannot possibly fail. They're using XYZ and TZN!"

"It's not worth the trouble. We'll think of it at a later review."

Solutions that require a lot of discipline and grunt work are not very attractive. Under schedule pressure, a good but clumsy solution loses its utility. Everybody knows how things must be done by the book, but will consistently take the shortcut. The one true way is to provide reusable solutions that are correct *and* easy to use.

> NOTE: 这段的作者描述了大多数programmer的现状，以及programmer选择忽视exception的原因，下面是它的大致意思:
>
> 需要大量专业知识和繁重工作的解决方案不是很有吸引力，尤其是在时间压力下，a good but clumsy solution loses its utility。

You check in the code, having an unpleasant feeling of imperfection(瑕疵), which gradually peters out as all tests run just fine. As time goes on and schedule pressure builds up, the spots that can "in theory" cause problems crop up.

You know you have a big problem: you have given up controlling the correctness of your application. Now when the server crashes, you don't have a clue about where to start: is it a hardware failure, a genuine bug, or an amok(狂乱的) state due to an exception? Not only are you exposed to involuntary bugs, you deliberately introduced them!

> NOTE: 一旦exception爆发，则排查起来是非常困难的。

Life is change. The number of users can grow, stressing memory to its limits. Your network administrator might disable paging for the sake of performance. Your database might not be so infallible(绝对可靠的、永远正确的). And you are unprepared for any of these.

## Part 2



### Solution 4: Petru's Approach

Using the `ScopeGuard` tool (which we'll explain in a minute), you can easily write code that's simple, correct, and efficient:

```C++
void User::AddFriend(User& newFriend)
{
    friends_.push_back(&newFriend);
    ScopeGuard guard = MakeObjGuard(
        friends_, &UserCont::pop_back);
    pDB_->AddFriend(GetName(), newFriend.GetName());
    guard.Dismiss();
}
```

`guard`'s only job is to call `friends_.pop_back` when it exits its scope. That is, unless you `Dismiss` it. If you do that, `guard` no longer does anything. `ScopeGuard` implements automatic calls to functions or member functions in its destructor. It can be helpful when you want to implement automatic undoing of atomic operations in the presence of exceptions.

You use `ScopeGuard` like so: if you need to do several operations in an "all-or-none" fashion, you put a `ScopeGuard` after each operation. The execution of that `ScopeGuard` nullifies the effect of the operation above it:

```C++
friends_.push_back(&newFriend);
ScopeGuard guard = MakeObjGuard(
    friends_, &UserCont::pop_back);
```

`ScopeGuard` works with regular functions, too:

```C++
void* buffer = std::malloc(1024);
ScopeGuard freeIt = MakeGuard(std::free, buffer);
FILE* topSecret = std::fopen("cia.txt");
ScopeGuard closeIt = MakeGuard(std::fclose, topSecret);
```

If all atomic operations succeed, you `Dismiss` all guards. Otherwise, each constructed `ScopeGuard` will diligently(勤勤恳恳的) call the function with which you initialized it.

With `ScopeGuard` you can easily arrange to undo various operations without having to write special classes for removing the last element of a vector, freeing some memory, and closing a file. This makes `ScopeGuard` a very useful reusable solution for writing exception-safe code, *easily*.

> NOTE: 上面这一段说明了scope guard VS RAII

### Implementing ScopeGuard

`ScopeGuard` is a generalization of a typical implementation of the "initialization is resource acquisition" C++ idiom. The difference is that `ScopeGuard` focuses only on the cleanup part — you do the resource acquisition, and `ScopeGuard` takes care of relinquishing(释放) the resource. (In fact, cleaning up is arguably the most important part of the idiom.)

> NOTE: 上面这一段说明了scope guard VS RAII

There are different ways of cleaning up resources, such as calling a function, calling a functor, and calling a member function of an object. Each of these can require zero, one, or more arguments.

Naturally, we model these variations by building a class hierarchy. The destructors of the objects in the hierarchies do the actual work. The base of the hierarchy is the `ScopeGuardImplBase` class, shown below:

```C++
class ScopeGuardImplBase
{
public:
    void Dismiss() const throw()
    {    dismissed_ = true;    }
protected:
    ScopeGuardImplBase() : dismissed_(false)
    {}
    ScopeGuardImplBase(const ScopeGuardImplBase& other)
    : dismissed_(other.dismissed_)
    {    other.Dismiss();    }
    ~ScopeGuardImplBase() {} // nonvirtual (see below why)
    mutable bool dismissed_;

private:
    // Disable assignment
    ScopeGuardImplBase& operator=(
        const ScopeGuardImplBase&);
};
```

`ScopeGuardImplBase` manages the `dismissed_` flag, which controls whether derived classes perform cleanup or not. If `dismissed_` is `true`, then derived classes will not do anything during their destruction.

This brings us to the missing `virtual` in the definition of `ScopeGuardImplBase`'s destructor. What polymorphic behavior of the destructor would you expect if it's not virtual? Hold your curiosity for a second; we have an ace up our sleeves that allows us to obtain polymorphic behavior without the overhead of `virtual` functions

For now, let's see how to implement an object that calls a function or functor taking one argument in its destructor. However, if you call `Dismiss`, the function/functor is no longer invoked.

```C++
template <typename Fun, typename Parm>
class ScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
    ScopeGuardImpl1(const Fun& fun, const Parm& parm)
    : fun_(fun), parm_(parm) 
    {}
    ~ScopeGuardImpl1()
    {
        if (!dismissed_) fun_(parm_);
    }
private:
    Fun fun_;
    const Parm parm_;
};
```

To make it easy to use `ScopeGuardImpl1`, let's write a helper function.

```C++
template <typename Fun, typename Parm>
ScopeGuardImpl1<Fun, Parm>
MakeGuard(const Fun& fun, const Parm& parm)
{
    return ScopeGuardImpl1<Fun, Parm>(fun, parm);
}
```

`MakeGuard` relies on the compiler's ability to deduce template arguments for template functions. This way you don't need to specify the template arguments to `ScopeGuardImpl1` — actually, you don't need to explicitly create `ScopeGuardImpl1` objects. This trick is used by standard library functions, such as `make_pair` and `bind1st`.



Still curious about how to achieve polymorphic behavior of the destructor without a `virtual` destructor? It's time to write the definition of `ScopeGuard`, which, surprisingly, is a mere `typedef`:

```C++
typedef const ScopeGuardImplBase& ScopeGuard;
```

Now we'll disclose the whole mechanism. According to the C++ Standard, a reference initialized with a temporary value makes that temporary value live for the lifetime of the reference itself.

Let's explain this with an example. If you write:

```C++
FILE* topSecret = std::fopen("cia.txt");
ScopeGuard closeIt = MakeGuard(std::fclose, topSecret);
```

then `MakeGuard` creates a temporary variable of type (deep breath here):

```C++
ScopeGuardImpl1<int (&)(FILE*), FILE*>
```

This is because the type of `std::fclose` is a function taking a `FILE*` and returning an `int`. The temporary variable of the type above is assigned to the `const` reference `closeIt`. As stated in the language rule above, the temporary variable lives as long as the reference — and when it is destroyed, the correct destructor is called. In turn, the destructor closes the file. `ScopeGuardImpl1` supports functions (or functors) taking one parameter. It is very simple to build classes that accept zero, two, or more parameters (`ScopeGuardImpl0`, `ScopeGuardImpl2`...). Once you have these, you overload `MakeGuard` to achieve a nice, unified syntax:

```C++
template <typename Fun>
ScopeGuardImpl0<Fun>
MakeGuard(const Fun& fun)
{
    return ScopeGuardImpl0<Fun >(fun);
}
...
```

We already have a powerful means of expressing automatic calls to functions. `MakeGuard` is an excellent tool especially when it comes to interfacing with C APIs without having to write lots of wrapper classes.

What's even better is the preservation of efficiency, as there's no virtual call involved.

### ScopeGuard for Objects and Member Functions

So far, so good, but what about invoking member functions for objects? It's not hard at all. Let's implement `ObjScopeGuardImpl0`, a class template that can invoke a parameterless member function for an object.

```C++
template <class Obj, typename MemFun>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
    ObjScopeGuardImpl0(Obj& obj, MemFun memFun)
    : obj_(obj), memFun_(memFun) 
    {}
    ~ObjScopeGuardImpl0()
    {
        if (!dismissed_) (obj_.*fun_)();
    }
private:
    Obj& obj_;
    MemFun memFun_;
};
```

`ObjScopeGuardImpl0` is a bit more exotic because it uses the lesser-known pointers to member functions and `operator.*`. To understand how it works, let's take a look at `MakeObjGuard`'s implementation. (We availed ourselves of `MakeObjGuard` in the opening section.)

```C++
template <class Obj, typename MemFun>
ObjScopeGuardImpl0<Obj, MemFun, Parm>
MakeObjGuard(Obj& obj, Fun fun)
{
    return ObjScopeGuardImpl0<Obj, MemFun>(obj, fun);
}
```

Now if you call:

```C++
ScopeGuard guard = MakeObjGuard(
    friends_, &UserCont::pop_back);
```

then an object of the following type is created:

```C++
ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>
```

Fortunately, `MakeObjGuard` saves you from having to write types that look like uninspired emoticons. The mechanism is the same — when `guard` leaves its scope, the destructor of the temporary object is called. The destructor invokes the member function via a pointer to a member. To achieve that, we use `operator.*`.

## Part 3

### Error Handling

If you have read Herb Sutter's work on exceptions [[2\]](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#2), you know that it is essential that destructors must *not* throw an exception. A throwing destructor makes it impossible to write correct code and can shut down your application without any warning.

The destructors of `ScopeGuardImplX` and `ObjScopeGuardImplX` call an unknown function or member function respectively. These functions might throw. In theory, you should never pass functions that throw to `MakeGuard` or `MakeObjGuard`. In practice (as you can see in the downloadable code), the destructor is shielded from any exceptions:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
template` `<``class` `Obj, ``typename` `MemFun>``class` `ObjScopeGuardImpl0 : ``public` `ScopeGuardImplBase``{``  ``...``public``:``  ``~ScopeGuardImpl1()``  ``{``    ``if` `(!dismissed_)``      ``try` `{ (obj_.*fun_)(); }``      ``catch``(...) {}``  ``}``}
```

The `catch(...)` block does nothing. This is not a hack. In the realm of exceptions, it is fundamental that you can do nothing if your "undo/recover" action fails. You attempt an undo operation, and you move on regardless whether the undo operation succeeds or not.

A possible sequence of actions in our instant messaging example is: you insert a friend in the database, you try to insert it in the `friends_` vector and fail, and consequently you try to delete the user from the database. There is a small chance that somehow the deletion from the database fails, too, which leads to a very unpleasant state of affairs.

In general, you should put guards on operations that you are the most sure you can undo successfully.

### Supporting Parameters by Reference

We were happily using `ScopeGuard` for a while, until we stumbled upon a problem. Consider the code below:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
void` `Decrement(``int``& x) { --x; }``void` `UseResource(``int` `refCount)``{``  ``++refCount;``  ``ScopeGuard guard = MakeGuard(Decrement, refCount);``  ``...``}
```

The `guard` object above ensures that the value of `refCount` is preserved upon exiting `UseResource`. (This idiom is useful in some resource sharing cases.)

In spite of its usefulness, the code above does not work. The problem is, `ScopeGuard` stores a *copy* of `refCount` (see the definition of `ScopeGuardImpl1`, member variable `parm_`) and not a reference to it. In this case, we need to store a *reference* to `refCount` so that `Decrement` can operate on it.

One solution would be to implement additional classes, such as `ScopeGuardImplRef` and `MakeGuardRef`. This is a lot of duplication, and it gets nasty as you implement classes for multiple parameters.

The solution we settled on consists of a little helper class that transforms a reference into a value:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
template` `<``class` `T>``class` `RefHolder``{``  ``T& ref_;``public``:``  ``RefHolder(T& ref) : ref_(ref) {}``  ``operator T& () ``const``  ``{``    ``return` `ref_;``  ``}``};``template` `<``class` `T>``inline` `RefHolder<T> ByRef(T& t)``{``  ``return` `RefHolder<T>(t);``}
```

`RefHolder` and its companion helper function `ByRef` are ingenious; they seamlessly adapt a reference to a value and allow `ScopeGuardImpl1` to work with references without any modification. All you have to do is to wrap your references in calls to `ByRef`, like so:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
void` `Decrement(``int``& x) { --x; }``void` `UseResource(``int` `refCount)``{``  ``++refCount;``  ``ScopeGuard guard = MakeGuard(Decrement, ByRef(refCount));``  ``...``}
```

We find this solution to be pretty expressive and suggestive.



The nicest part of reference support is the `const` modifier used in `ScopeGuardImpl1`. Here's the relevant excerpt:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
template` `<``typename` `Fun, ``typename` `Parm>``class` `ScopeGuardImpl1 : ``public` `ScopeGuardImplBase``{``  ``...``private``:``  ``Fun fun_;``  ``const` `Parm parm_;``};
```

This little `const` is very important. It prevents code that uses non-`const` references from compiling and running incorrectly. In other words, if you forget to use `ByRef` with a function, the compiler will not allow incorrect code to compile.

### But Wait, There's More

You now have everything you need to write exception-safe code without having to agonize over it. Sometimes, however, you want the `ScopeGuard` to always execute when you exit the block. In this case, creating a dummy variable of type `ScopeGuard` is awkward — you only need a temporary, you don't need a *named* temporary.

The macro `ON_BLOCK_EXIT` does exactly that and lets you write expressive code like below:

[?](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=3#)

```
{``  ``FILE``* topSecret = ``fopen``(``"cia.txt"``);``  ``ON_BLOCK_EXIT(std::``fclose``, topSecret);``  ``... use topSecret ...``} ``// topSecret automagically closed
```

`ON_BLOCK_EXIT` says: "I want this action to be performed when the current block exists." Similarly, `ON_BLOCK_EXIT_OBJ` implements the same feature for a member function call.

These macros use non-orthodox (albeit legal) macro wizardry, which shall go undisclosed. The curious can look them up in the code.

### ScopeGuard in the Real World

Maybe the coolest thing about `ScopeGuard` is its ease of use and conceptual simplicity. This article has detailed the entire implementation, but explaining `ScopeGuard`'s usage only takes a couple of minutes. Amongst our colleagues, `ScopeGuard` has spread like wildfire. Everybody takes `ScopeGuard` for granted as a valuable tool that helps in various situations, from premature returns to exceptions. With `ScopeGuard`, you can finally write exception-safe code with reasonable ease and understand and maintain it just as easily.

Every tool comes with a use recommendation, and `ScopeGuard` is no exception. You should use `ScopeGuard` as it was intended — as an automatic variable in functions. You should not hold `ScopeGuard` objects as member variables, try to put them in vectors, or allocate them on the heap. For these purposes, the downloadable code contains a `Janitor` class, which does exactly what `ScopeGuard` does, but in a more general way — at the expense of some efficiency.

### Conclusion

We have presented the issues that arise in writing exception-safe code. After discussing a couple of ways of achieving exception safety, we have introduced a generic solution. `ScopeGuard` uses several generic programming techniques to let you prescribe function and member function calls when a `ScopeGuard` variable exits a scope. Optionally, you can dismiss the `ScopeGuard` object. `ScopeGuard` is useful when you need to perform automatic cleanup of resources. This idiom is important when you want to assemble an operation out of several atomic operations, each of which could fail.

### Acknowledgements

The authors would like to thank to Mihai Antonescu for reviewing this paper and for making useful corrections and suggestions.

### Bibliography

[1] Bjarne Stroustrup. *The C++ Programming Language*, 3rd Edition (Addison Wesley, 1997), page 366.

[2] Herb Sutter. *Exceptional C++: 47 Engineering Puzzles, Programming Problems, and Solutions* (Addison-Wesley, 2000).

------

*Andrei Alexandrescu is a development manager at RealNetworks Inc. (www.realnetworks.com), based in Seattle, WA. He may be contacted at [www.moderncppdesign.com](http://www.moderncppdesign.com/).*

*Petru Marginean is senior C++ developer for Plural, New York. He can be reached at petrum@hotmail.com.*


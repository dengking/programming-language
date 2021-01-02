# isocpp [N4152 uncaught_exceptions](https://isocpp.org/files/papers/N4152.pdf)

This paper is a revision of N3614 to implement EWG direction in Bristol.

> NOTE: EWG 是指 Evolution Working Group，参见 [cplusplus](https://github.com/cplusplus)/[EWG](https://github.com/cplusplus/EWG)。

## Motivation

`std::uncaught_exception` is known to be “nearly useful” in many situations, such as when implementing an Alexandrescu-style ScopeGuard. [1]

In particular, when called in a destructor, what C++ programmers often expect and what is basically true is: “`uncaught_exception` returns true iff this destructor is being called during **stack unwinding**.”

However, as documented at least since 1998 in Guru of the Week `#47` [2], it means **code that is transitively(传递地) called from a destructor that could itself be invoked during stack unwinding** cannot correctly detect whether it itself is actually being called as part of unwinding. Once you’re in unwinding of any exception, to `uncaught_exception` everything looks like unwinding, even if there is more than one active exception.

> NOTE: 上面这段话解释得是否非常不好懂的，在`../GOTW-issue-47-Uncaught-Exceptions`中给出了非常详细的解释。

### Example 1: Transaction (`GotW #47`)

Consider this code taken from [2], which shows an early special case of `ScopeGuard` (`ScopeGuard` is described further in the following section):

```C++
#include<exception>
using namespace std;
class Transaction
{
public:
	~Transaction();
};
Transaction::~Transaction()
{
	if (uncaught_exception()) // unreliable, ONLY if Transaction could be
		Rollback(); // used from within a dtor (transitively!)
}
void LogStuff()
{
	Transaction t( /*...*/);
	// :::
	// do work
	// :::
} // oops, if U::~U() is called as part of unwinding another exception
// so uncaught_exception will return true and t will not commit
U::~U()
{
	/* deep call tree that eventually calls LogStuff() */
}
// for example:
int main()
{
	try
	{
		U u;
		throw 1;
	} // U::~U() invoked here
	catch (...)
	{
	}
}

```

> NOTE: 需要结合注释来理解上述example:
>
> 在`main()`中，`u`由于exception stack unwinding， `U::~U()` 会被执行，其中会调用 `LogStuff()` ，其中`Transaction t`会被析构，`Transaction::~Transaction()`会被执行。

The problem is that, inside `~Transaction`, there is no way to tell whether `~Transaction` is being called as part of **stack unwinding**. Asking `uncaught_exception()` will only say whether some **unwinding** is in progress, which might already have been true, rather than answering whether `~Transaction` itself is being called to perform **unwinding**.

### Example 2: ScopeGuard

Alexandrescu’s ScopeGuard [1, 3] is a major motivating example, where the point is to execute code
upon a scope’s:

a) termination in all cases == cleanup à la `finally`;

> NOTE: 其实非常类似于RAII，即在所有的可能的termination情况下，都执行cleanup

b) successful termination == celebration; or

c) failure termination == rollback-style compensating “undo” code.

> NOTE: 类似于transaction

However, currently there is no way to automatically distinguish between (b) and (c) in standard C++ without requiring the user to explicitly signal successful scope completion by calling a `Dismiss`(解除) function on the guard object, which makes the technique useful but somewhere between tedious and fragile. Annoyingly(令人厌烦的是), that `Dismiss` call is also usually right near where the failure recovery code would have been written without `ScopeGuard`, thus not relieving the programmer of having to think about the placement of success/failure determination and compensating actions shouldn’t/should occur.

For example, adapted from [1]:

```C++
void User::AddFriend(User &newFriend)
{
	friends_.push_back(&newFriend);
	ScopeGuard guard([&]{friends_.pop_back();});
	//:::
	pDB_->AddFriend(GetName(), newFriend.GetName());
	//:::
	guard.Dismiss();
}

```

Nevertheless, despite that current drawback, as demonstrated for example in [4], `ScopeGuard` is known to be useful in practice in C++ programs. See slides 35-44 in the Appendix for additional examples from production code.

`ScopeGuard` is desirable because it simplifies control flow by allowing “ad-hoc destructors” without having to write a custom type for each recovery action. For example, in the D programming language, which has language support for `ScopeGuard` in the form of the `scope` statement, the D standard library uses `scope(…)` almost as frequently as `while`.

We would like to enable `ScopeGuard` and similar uses to automatically and reliably distinguish between **success** and **failure** in standard C++ without requiring the user to explicitly signal success or failure by calling a `Dismiss` function on the guard object. This makes the technique even more useful and less tedious to write code that is clear and exception-safe. The adapted example from [1] would be:

```C++
void User::AddFriend(User& newFriend)
{
    friends_.push_back(&newFriend);
    ScopeGuard guard([&]{ friends_.pop_back(); });
    :::
    pDB_->AddFriend(GetName(), newFriend.GetName());
    :::
    // no need to call guard.Dismiss();
}
```



## Proposal

This paper proposes a simple function that enables the above uses. This paper does not propose adding language support for D-style scope statements, or more general approaches such as suggested in [5].

### `Option 1: bool unwinding_exception()`

The previous version of this paper suggested a function that returned true iff called during stack unwinding. EWG pointed out that this could involve overheads on programs that did not use the feature,
because the implementation would have to be ready to answer the query at any time; it might also be an ABI-breaking change in compilers.

Instead, EWG pointed out that having an integer denoting the number of uncaught exceptions was just as useful to implement cases like `Transaction` and `ScopeGuard`. Furthermore, Alexandrescu [6] and others point out that this just uses information already present in the major implementations. 

Therefore, we do not propose Option 1, favoring instead Option 2 below.

### `Option 2: int uncaught_exceptions()`

This paper proposes a new function int `std::uncaught_exceptions()` that returns the number of exceptions currently active, meaning thrown or rethrown but not yet handled.

A type that wants to know whether its destructor is being run to unwind this object can query `uncaught_exceptions` in its constructor and store the result, then query `uncaught_exceptions` again in its destructor; if the result is different, then this destructor is being invoked as part of **stack unwinding due to a new exception** that was thrown later than the object’s construction.

As demonstrated in slides 28-31 of the Appendix [6], this uses information already present in major implementations, where current implementations of `ScopeGuard` resort to nonportable code that relies on undocumented compiler features to make `ScopeGuard` “portable in practice” today. This option proposes adding a single new function to expose the information that already present in compilers, so that these uses can be truly portable.

## Acknowledgments

Thanks to Andrei Alexandrescu for prompting this paper and providing examples.

## References

[1] A. Alexandrescu. “Change the Way You Write Exception-Safe Code – Forever” (Dr. Dobb’s, December
2000).
[2] H. Sutter. “Guru of the Week #47: Uncaught Exceptions” (November 1998).
[3] A. Alexandrescu. “Three Unlikely Successful Features of D” (video) (Lang.NEXT, April 2012).
[4] K. Rudolph et al. “Does ScopeGuard use really lead to better code?” (StackOverflow, September 2008).
[5] V. Voutilainen. “Accessing current exception during unwinding” (N2952, September 2009).
[6] A. Alexandrescu, “Declarative Control Flow” (C++ and Beyond, Stuttgart, Germany, September 2014).
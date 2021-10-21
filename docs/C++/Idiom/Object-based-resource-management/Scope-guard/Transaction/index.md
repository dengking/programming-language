# Transaction

本文讨论如何实现transaction的all-or-nothing特性？

## 如何通知transaction不能commit？

最好是通过抛出exception的方式，如果使用return error number的话，则需要进行大量的if-else校验；并且结合scope-guard的实现，通过exception的方式是最最好的。

## stackoverflow [C++ Transaction-like Pattern for All or Nothing Work](https://stackoverflow.com/questions/10983791/c-transaction-like-pattern-for-all-or-nothing-work)

[A](https://stackoverflow.com/a/10984171)

A little RAII commit/rollback scope guard might look like this:

```cpp
#include <utility>
#include <functional>

class CommitOrRollback
{
    bool committed;
    std::function<void()> rollback;

public:
    CommitOrRollback(std::function<void()> &&fail_handler)
        : committed(false),
          rollback(std::move(fail_handler))
    {
    }

    void commit() noexcept { committed = true; }

    ~CommitOrRollback()
    {
        if (!committed)
            rollback();
    }
};
```

So, we're assuming we'll always create the guard object after the transaction succeeds, and call `commit` only after *all* the transactions have succeeded.

```cpp
void complicated_task_a();
void complicated_task_b();

void rollback_a();
void rollback_b();

int main()
{
    try {
        complicated_task_a();
        // if this ^ throws, assume there is nothing to roll back
        // ie, complicated_task_a is internally exception safe
        CommitOrRollback taskA(rollback_a);

        complicated_task_b();
        // if this ^ throws however, taskA will be destroyed and the
        // destructor will invoke rollback_a
        CommitOrRollback taskB(rollback_b);


        // now we're done with everything that could throw, commit all
        taskA.commit();
        taskB.commit();

        // when taskA and taskB go out of scope now, they won't roll back
        return 0;
    } catch(...) {
        return 1;
    }
}
```

------

PS. As *Anon Mail* says, it's better to push all those *taskX* objects into a container if you have many of them, giving the container the same semantics (call commit on the container to have it commit each owned guard object).

------

PPS. In principle, you can use `std::uncaught_exception` in the RAII dtor instead of explicitly committing. I prefer to explicitly commit here because I think it's clearer, and also works correctly if you exit scope early with a `return FAILURE_CODE` instead of an exception.

[A](https://stackoverflow.com/a/10983994)

It is hard to achieve Transaction consistency in C++. There is a nice method described using the [ScopeGuard](http://www.drdobbs.com/cpp/184403758?pgno=1) pattern in the Dr Dobb's journal. The beauty of the approach is that this takes cleanup in both the normal situations and in the exception scenarios. It utilizes the fact that object destructors are ensured to call on any scope exits and exception case is just another scope exit.
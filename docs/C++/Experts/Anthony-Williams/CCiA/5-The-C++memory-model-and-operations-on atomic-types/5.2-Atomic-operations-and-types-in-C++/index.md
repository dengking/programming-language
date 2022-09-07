

# 5.2 Atomic operations and types in C++

An atomic operation is an indivisible operation. You can’t observe such an operation half-done from any thread in the system; it’s either done or not done. If the load operation that reads the value of an object is atomic, and all modifications to that object are also atomic, that load will retrieve either the initial value of the object or the value stored by one of the modifications.

The flip side of this is that a nonatomic operation might be seen as half-done by another thread. If that operation is a store, the value observed by another thread might be neither the value before the store nor the value stored but something else. If the nonatomic operation is a load, it might retrieve part of the object, have another thread modify the value, and then retrieve the remainder of the object, thus retrieving neither the first value nor the second but some combination of the two. This is a simple problematic race condition, as described in chapter 3, but at this level it may constitute
a data race (see section 5.1) and thus cause undefined behavior.

In C++, you need to use an atomic type to get an atomic operation in most cases, so let’s look at those.

## 5.2.1 The standard atomic types

### `std::atomic_flag`

The only type that doesn’t provide an `is_lock_free()` member function is `std::atomic_flag`. This type is a really simple Boolean flag, and operations on this type are required to be lock-free; once you have a simple lock-free Boolean flag, you can use that to implement a simple lock and thus implement all the other atomic types using that as a basis. When I said really simple, I meant it: objects of type `std::atomic_flag` are initialized to clear, and they can then either be queried and set (with the `test_and_set()` member function) or cleared (with the `clear()` member function). That’s it: no assignment, no copy construction, no test and clear, no other operations at all.

The remaining atomic types are all accessed through specializations of the `std::atomic<>` class template and are a bit more full-featured but may not be lockfree(as explained previously). On most popular platforms it’s expected that the atomic variants of all the built-in types (such as `std::atomic<int>` and `std::atomic<void*>`) are indeed lock-free, but it isn’t required. As you’ll see shortly, the interface of each specialization reflects the properties of the type; bitwise operations such as `&=` aren’t defined for plain pointers, so they aren’t defined for atomic pointers either, for example.



### not copyable or assignable

The standard atomic types are not copyable or assignable in the conventional sense, in that they have no **copy constructors** or **copy assignment operators**. They do,however, support: 

1、assignment from the corresponding built-in types 

2、implicit conversion to the corresponding built-in types 

3、direct `load()` and `store()` member functions,  `exchange()`, `compare_exchange_weak()`, and `compare_exchange_strong()`.

They also support the compound assignment operators where appropriate: `+=`, `-=`, `*=`, `|=`, and so on, and the integral types and `std::atomic<>` specializations for pointers support `++` and `--`. These operators also have corresponding named member functions with the same functionality: `fetch_add()`, `fetch_or()`, and so on. 

The return value from the **assignment operators** and **member functions** is either the value stored (in the case of the **assignment operators**) or the value prior to the operation (in the case of the **named functions**). This avoids the potential problems that could stem from the usual habit of such **assignment operators** returning a reference to the object being assigned to. In order to get the stored value from such a reference, the code would have to perform a separate read, thus allowing another thread to modify the value between the assignment and the read and opening the door for a race condition.

> NOTE:
>
> 一、"opening the door for a race condition" 如何理解？



## 5.2.2 Operations on `std::atomic_flag`

`std::atomic_flag` is the simplest standard atomic type, which represents a Boolean flag. Objects of this type can be in one of two states: 

1、set 

2、clear

It’s deliberately basic and is intended as a building block only. As such, I’d never expect to see it in use, except under very special circumstances. Even so, it will serve as a starting point for discussing the other atomic types, because it shows some of the general policies that apply to the atomic types.

Objects of type std::atomic_flag must be initialized with `ATOMIC_FLAG_INIT`. This initializes the flag to a **clear state**. There’s no choice in the matter; the flag always starts clear:

```c++
std::atomic_flag f=ATOMIC_FLAG_INIT;
```

This applies wherever the object is declared and whatever scope it has. It’s the only atomic type to require such special treatment for initialization, but it’s also the only type guaranteed to be **lock-free**.

Once you have your flag object initialized, there are only three things you can do with it: 

1、destroy it ( **destructor** )

2、clear it ( `clear()` member function )

3、set it and query the previous value ( `test_and_set()` member function )

`clear()` is a store operation and so can’t have `memory_order_acquire` or `memory_order_acq_rel` semantics.

`test_and_set()` is a **read-modify-write** operation and so can have any of the memory-ordering tags applied.

As with every atomic operation, the default for both is `memory_order_seq_cst`.

For example:

```c++
f.clear(std::memory_order_release);
bool x=f.test_and_set();
```

Here, the call to `clear()` explicitly requests that the flag is cleared with release semantics, while the call to `test_and_set()` uses the default memory ordering for setting the flag and retrieving the old value.

You can’t copy-construct another `std::atomic_flag` object from the first, and you can’t assign one `std::atomic_flag` to another. This isn’t something peculiar to `std::atomic_flag` but something common with all the atomic types. All operations on an atomic type are defined as atomic, and assignment and copy-construction involve two objects. A single operation on two distinct objects can’t be atomic. In the case of **copy-construction** or **copy-assignment**, the value must first be read from one object and then written to the other. These are two separate operations on two separate objects, and the combination can’t be atomic. Therefore, these operations aren’t permitted.

The limited feature set makes `std::atomic_flag` ideally suited to use as a **spinlock mutex**. Initially the flag is clear and the mutex is unlocked. To lock the mutex, loop on `test_and_set()` until the old value is `false`, indicating that this thread set the value to true. Unlocking the mutex is simply a matter of clearing the flag. Such an implementation is shown in the following listing.



```c++
#include <atomic> 

class spinlock_mutex
{
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    spinlock_mutex()
    {
    }
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};

int main()
{
}
```


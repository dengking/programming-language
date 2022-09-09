

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



When we look at the memory-ordering semantics, you’ll see how this guarantees the necessary **enforced ordering** that goes with a mutex lock. This example is covered in section 5.3.6.



## 5.2.3 Operations on `std::atomic<bool>`

The most basic of the atomic integral types is `std::atomic<bool>`. This is a more fullfeatured Boolean flag than `std::atomic_flag`, as you might expect. Although it’s still not copy-constructible or copy-assignable, you can construct it from a nonatomic bool, so it can be initially `true` or `false`, and you can also assign to instances of `std::atomic<bool>` from a nonatomic bool:

```c++
std::atomic<bool> b(true);
b=false;
```

### Assignment operator

One other thing to note about the **assignment operator** from a nonatomic bool is that it differs from the general convention of returning a reference to the object it’s assigned to: it returns a bool with the value assigned instead. This is another common pattern with the atomic types: the **assignment operators** they support return values (of the corresponding nonatomic type) rather than references. If a reference to the atomic variable was returned, any code that depended on the result of the assignment would then have to explicitly load the value, potentially getting the result of a modification
by another thread. By returning the result of the assignment as a nonatomic value, you can avoid this additional load, and you know that the value obtained is the actual value stored.

### Member function comparison

Rather than using the restrictive `clear()` function of `std::atomic_flag`, writes (of either `true` or `false`) are done by calling `store()`, although the memory-order semantics can still be specified. 

Similarly, `test_and_set()` has been replaced with the more general `exchange()` member function that allows you to replace the stored value with a new one of your choosing and atomically retrieve the original value. `std::atomic<bool>` also supports a plain nonmodifying query of the value with an implicit conversion to plain bool or with an explicit call to `load()`. 

As you might expect, `store()` is a **store** operation, whereas `load()` is a **load** operation. `exchange()` is a **read-modify-write** operation:

```c++
std::atomic<bool> b;
bool x=b.load(std::memory_order_acquire);
b.store(true);
x=b.exchange(false,std::memory_order_acq_rel);
```

`exchange()` isn’t the only read-modify-write operation supported by `std::atomic<bool>`; it also introduces an operation to store a new value if the current value is equal to an expected value.

### STORING A NEW VALUE (OR NOT) DEPENDING ON THE CURRENT VALUE

This new operation is called compare/exchange, and it comes in the form of the `compare_exchange_weak()` and `compare_exchange_strong()` member functions. The compare/exchange operation is the cornerstone of programming with atomic types; it compares the value of the atomic variable with a supplied `expected` value and stores the supplied desired value if they’re equal. If the values aren’t equal, the `expected` value is updated with the actual value of the atomic variable. The return type of the compare/exchange functions is a bool, which is `true` if the store was performed and `false` otherwise.

> NOTE:
>
> 零、一般放在CAS loop中，因此它需要:考虑 
>
> 1、stop condition
>
> 2、在两次loop之间，其他thread更新了atomic variable value
>
> 一、values
>
> 0、actual value of the atomic variable
>
> 其它所有的value都是对它进行操作
>
> 1、return value
>
> 2、 `expected` value
>
> 3、`desired` value
>
> 4、
>
> 二、
>
> 1、if value ==  `expected` value
>
> 2、if value !=  `expected` value
>
> "the `expected` value is updated with the actual value of the atomic variable" 有如下价值:
>
> 1、stop condition

#### `compare_exchange_weak()`

For `compare_exchange_weak()`, the store might not be successful even if the original value was equal to the expected value, in which case the value of the variable is unchanged and the return value of `compare_exchange_weak()` is `false`. This is most likely to happen on machines that lack a **single compare-and-exchange instruction**, if the processor can’t guarantee that the operation has been done atomically—possibly because the thread performing the operation was switched out in the middle of the necessary sequence of instructions and another thread scheduled in its place by
the operating system where there are more threads than processors. This is called a **spurious failure**, because the reason for the failure is a function of timing rather than the values of the variables. Because `compare_exchange_weak()` can fail spuriously, it must typically be used in a loop:

```c++
bool expected=false;
extern atomic<bool> b; // set somewhere else
while(!b.compare_exchange_weak(expected,true) && !expected);
```

> NOTE:
>
> 一、如果设置成功，则 `compare_exchange_weak` 会返回true，则此时上述loop会退出。
>
> 二、stackoverflow [Understanding std::atomic::compare_exchange_weak() in C++11](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11) # [Tony D.'s comment](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11#comment39245734_25200031) 
>
> > "*Why is !expected in the example?* It is not needed for correctness. Omitting it would yield the same semantics." - not so... if say the first exchange fails because it finds `b` is already `true`, then - with `expected` now `true` - without `&& !expected` it loops and tries another (silly) exchange of `true` and `true` which may well "succeed" trivially breaking from the `while` loop, ***but*** could exhibit meaningfully different behaviour if `b` had meanwhile changed back to `false`, in which case the loop would continue and *may* ultimately set `b` `true` *yet again* before breaking. – [Tony Delroy](https://stackoverflow.com/users/410767/tony-delroy)
>
> 

In this case, you keep looping as long as expected is still false, indicating that the `compare_exchange_weak()` call failed spuriously.

#### `compare_exchange_strong()`

On the other hand, `compare_exchange_strong()` is guaranteed to return `false` only if the actual value wasn’t equal to the expected value. This can eliminate the need for loops like the one shown where you just want to know whether you successfully changed a variable or whether another thread got there first.

> NOTE:
>
> 一、上面这段话的中文翻译如下:
>
> "仅当actual value不等于 expected value的时候，`compare_exchange_strong()` 才保证返回`false`"。
>
> 上面使用loop的原因: "whether you successfully changed a variable"、"whether another thread got there first"

#### change the variable whatever the initial value is

If you want to change the variable whatever the initial value is (perhaps with an updated value that depends on the current value), the update of `expected` becomes useful; each time through the loop, `expected` is reloaded, so if no other thread modifies the value in the meantime, the `compare_exchange_weak()` or `compare_exchange_strong()` call should be successful the next time around the loop. If the calculation of the value to be stored is simple, it may be beneficial to use `compare_exchange_weak()` in order to avoid a **double loop** on platforms where `compare_exchange_weak()` can fail spuriously (and so `compare_exchange_strong()` contains a loop).

## 5.2.4 Operations on `std::atomic<T*>`: pointer arithmetic

> NOTE:
>
> 一、
>
> 1、load
>
> 2、store
>
> 3、read-modify-write

### Pointer operation

|                             | `std::atomic<bool>` | `std::atomic<T*>` |
| --------------------------- | ------------------- | ----------------- |
| `load()`                    |                     |                   |
| `store()`                   |                     |                   |
| `exchange()`                |                     |                   |
| `compare_exchange_weak()`   |                     |                   |
| `compare_exchange_strong()` |                     |                   |
|                             |                     |                   |



### Pointer arithmetic operations

```c++
class Foo{};
Foo some_array[5];
std::atomic<Foo*> p(some_array);
Foo* x=p.fetch_add(2);
assert(x==some_array);
assert(p.load()==&some_array[2]);
x=(p-=1);
assert(x==&some_array[1]);
assert(p.load()==&some_array[1]);
```



## 5.2.5 Operations on standard atomic integral types



## 5.2.6 The `std::atomic<>` primary class template



## 5.2.7 Free functions for atomic operations





The C++ Standard Library also provides free functions for accessing instances of `std::shared_ptr<>` in an atomic fashion. This is a break from the principle that only the atomic types support atomic operations, because `std::shared_ptr<>` is quite definitely not an atomic type. However, the C++ Standards Committee felt it was sufficiently important to provide these extra functions. The atomic operations available are `load`, `store`, `exchange`, and compare/exchange, which are provided as overloads of the same operations on the standard atomic types, taking a `std::shared_ptr<>*` as
the first argument:

```c++
#include <atomic>
#include <memory>
#include <iostream>

class my_data
{
public:
    my_data()
    {
        std::cout << "my_data()" << std::endl;
    }
    ~my_data()
    {
        std::cout << "~my_data()" << std::endl;
    }
    void foo()
    {
    }
};

void process_data(std::shared_ptr<my_data> &p)
{
    if (p)
    {
        p->foo();
    }
}
std::shared_ptr<my_data> p;

void process_global_data()
{
    std::shared_ptr<my_data> local = std::atomic_load(&p);
    process_data(local);
}

void update_global_data()
{
    std::shared_ptr<my_data> local(new my_data);
    std::atomic_store(&p, local);
}

int main()
{
    process_global_data();
    update_global_data();
    process_global_data();
    update_global_data();
}
```


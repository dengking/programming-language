# 5 The C++ memory model and operations on atomic types

## 5.1 Memory model basics

There are two aspects to the memory model: the basic **structural** aspects, which relate to how things are laid out in memory, and then the **concurrency** aspects.

> NOTE:
>
> 一、
>
> "5.1.1 Objects and memory locations"章节描述: "the basic **structural** aspects"。
>
> "5.1.2 Objects, memory locations, and concurrency"章节描述: "the **concurrency** aspects"



### 5.1.2 Objects, memory locations, and concurrency

Before we look at atomic operations, there’s one more concept that’s important to understand about objects and memory locations: modification orders.

### 5.1.3 Modification orders

> NOTE:
>
> 一、modification order是从shared data的角度来建立的

Every object in a C++ program has a defined **modification order** composed of all the writes to that object from all threads in the program, starting with the object’s initialization. In most cases this order will vary between runs, but in any given execution of the program all threads in the system must agree on the order. If the object in question isn’t one of the atomic types described in section 5.2, you’re responsible for making certain that there’s sufficient synchronization to ensure that threads agree on the **modification order** of each variable. If different threads see distinct sequences of values for a single variable, you have a data race and undefined behavior (see section 5.1.2). If you do use atomic operations, the compiler is responsible for ensuring that the necessary synchronization is in place.

## 5.2 Atomic operations and types in C++

An atomic operation is an indivisible operation. You can’t observe such an operation half-done from any thread in the system; it’s either done or not done. If the load operation that reads the value of an object is atomic, and all modifications to that object are also atomic, that load will retrieve either the initial value of the object or the value stored by one of the modifications.

The flip side of this is that a nonatomic operation might be seen as half-done by another thread. If that operation is a store, the value observed by another thread might be neither the value before the store nor the value stored but something else. If the nonatomic operation is a load, it might retrieve part of the object, have another thread modify the value, and then retrieve the remainder of the object, thus retrieving neither the first value nor the second but some combination of the two. This is a simple problematic race condition, as described in chapter 3, but at this level it may constitute
a data race (see section 5.1) and thus cause undefined behavior.

In C++, you need to use an atomic type to get an atomic operation in most cases, so let’s look at those.

### 5.2.1 The standard atomic types

#### `std::atomic_flag`

The only type that doesn’t provide an `is_lock_free()` member function is `std::atomic_flag`. This type is a really simple Boolean flag, and operations on this type are required to be lock-free; once you have a simple lock-free Boolean flag, you can use that to implement a simple lock and thus implement all the other atomic types using that as a basis. When I said really simple, I meant it: objects of type `std::atomic_flag` are initialized to clear, and they can then either be queried and set (with the `test_and_set()` member function) or cleared (with the `clear()` member function). That’s it: no assignment, no copy construction, no test and clear, no other operations at all.

The remaining atomic types are all accessed through specializations of the `std::atomic<>` class template and are a bit more full-featured but may not be lockfree(as explained previously). On most popular platforms it’s expected that the atomic variants of all the built-in types (such as `std::atomic<int>` and `std::atomic<void*>`) are indeed lock-free, but it isn’t required. As you’ll see shortly, the interface of each specialization reflects the properties of the type; bitwise operations such as `&=` aren’t defined for plain pointers, so they aren’t defined for atomic pointers either, for example.



#### not copyable or assignable

The standard atomic types are not copyable or assignable in the conventional sense, in that they have no **copy constructors** or **copy assignment operators**. They do,however, support: 

1、assignment from the corresponding built-in types 

2、implicit conversion to the corresponding built-in types 

3、direct `load()` and `store()` member functions,  `exchange()`, `compare_exchange_weak()`, and `compare_exchange_strong()`.

They also support the compound assignment operators where appropriate: `+=`, `-=`, `*=`, `|=`, and so on, and the integral types and `std::atomic<>` specializations for pointers support `++` and `--`. These operators also have corresponding named member functions with the same functionality: `fetch_add()`, `fetch_or()`, and so on. 

The return value from the **assignment operators** and **member functions** is either the value stored (in the case of the **assignment operators**) or the value prior to the operation (in the case of the **named functions**). This avoids the potential problems that could stem from the usual habit of such **assignment operators** returning a reference to the object being assigned to. In order to get the stored value from such a reference, the code would have to perform a separate read, thus allowing another thread to modify the value between the assignment and the read and opening the door for a race condition.

> NOTE:
>
> 一、
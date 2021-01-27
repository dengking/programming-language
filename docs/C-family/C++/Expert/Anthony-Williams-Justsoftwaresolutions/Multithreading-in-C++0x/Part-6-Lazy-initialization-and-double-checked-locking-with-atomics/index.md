# [Multithreading in C++0x part 6: Lazy initialization and double-checked locking with atomics](https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-6-double-checked-locking.html)

### Lazy Initialization

The classic lazy-initialization case is where you have an object that is expensive to construct but isn't always needed. In this case you can choose to only initialize it on first use:

```C++
class lazy_init
{
    mutable std::unique_ptr<expensive_data> data;
public:
    expensive_data const& get_data() const
    {
        if(!data)
        {
            data.reset(new expensive_data);
        }
        return *data;
    }
};
```

However, we can't use this idiom in multi-threaded code, since there would be a data race on the accesses to `data`. Enter `std::call_once()` — by using an instance of `std::once_flag` to protect the initialization we can make the data race go away:

> NOTE: 为什么"there would be a data race on the accesses to `data`"?

```c++
class lazy_init
{
    mutable std::once_flag flag;
    mutable std::unique_ptr<expensive_data> data;

    void do_init() const
    {
        data.reset(new expensive_data);
    }
public:
    expensive_data const& get_data() const
    {
        std::call_once(flag,&lazy_init::do_init,this);
        return *data;
    }
};
```

Concurrent calls to `get_data()` are now safe: if the data has already been initialized they can just proceed concurrently. If not, then all threads calling concurrently except one will wait until the remaining thread has completed the initialization.

### Reinitialization

This is all very well if you only want to initialize the data *once*. However, what if you need to update the data — perhaps it's a *cache* of some rarely-changing data that's expensive to come by. `std::call_once()` doesn't support multiple calls (hence the name). You could of course protect the data with a mutex, as shown below:

```C++
class lazy_init_with_cache
{
    mutable std::mutex m;
    mutable std::shared_ptr<const expensive_data> data;

public:
    std::shared_ptr<const expensive_data> get_data() const
    {
        std::lock_guard<std::mutex> lk(m);
        if(!data)
        {
            data.reset(new expensive_data);
        }
        return data;
    }
    void invalidate_cache()
    {
        std::lock_guard<std::mutex> lk(m);
        data.reset();
    }
};
```

Note that in this case we return a `std::shared_ptr<const expensive_data>` rather than a reference to avoid a race condition on the data itself — this ensures that the copy held by the calling code will be valid (if out of date) even if another thread calls `invalidate_cache()` before the data can be used.

This "works" in the sense that it avoids data races, but if the updates are rare and the reads are frequent then this may cause unnecessary serialization when multiple threads call `get_data()` concurrently. What other options do we have?

### Double-checked locking returns

Much has been written about how [double-checked locking](http://en.wikipedia.org/wiki/Double-checked_locking) is broken when using multiple threads. However, the chief cause of the problem is that the sample code uses plain non-atomic operations to check the flag outside the mutex, so is subject to a data race. You can overcome this by careful use of the C++0x atomics, as shown in the example below:

```c++
class lazy_init_with_cache
{
    mutable std::mutex m;
    mutable std::shared_ptr<const expensive_data> data;

public:
    std::shared_ptr<const expensive_data> get_data() const
    {
        std::shared_ptr<const expensive_data> result=
            std::atomic_load_explicit(&data,std::memory_order_acquire);
        if(!result)
        {
            std::lock_guard<std::mutex> lk(m);
            result=data;
            if(!result)
            {
                result.reset(new expensive_data);
                std::atomic_store_explicit(&data,result,std::memory_order_release);
            }
        }
        return result;
    }
    void invalidate_cache()
    {
        std::lock_guard<std::mutex> lk(m);
        std::shared_ptr<const expensive_data> dummy;
        std::atomic_store_explicit(&data,dummy,std::memory_order_relaxed);
    }
};
```

Note that in this case, *all writes to `data` use atomic operations, even those within the mutex lock*. This is necessary in order to ensure that the atomic load operation at the start of `get_data()` actually has a coherent value to read — there's no point doing an atomic load if the stores are not atomic, otherwise you might atomically load some half-written data. Also, the atomic load and store operations ensure that the reference count on the `std::shared_ptr` object is correctly updated, so that the `expensive_data` object is correctly destroyed when the last `std::shared_ptr` object referencing it is destroyed.

If our atomic load actually returned a non-`NULL` value then we can use that, just as we did before. However, if it returned `NULL` then we need to lock the mutex and try again. This time we can use a plain read of `data`, since the mutex is locked. If we still get `NULL` then we need to do the initialization. However, we can't just call `data.reset()` like before, since that is not atomic. Instead we must create a local `std::shared_ptr` instance with the value we want, and store the value with an atomic store operation. We can use `result` for the local value, since we want the value in that variable anyway.

In `invalidate_cache()` we must also store the value using `std::atomic_store_explicit()`, in order to ensure that the `NULL` value is correctly read in `get_data()`. Note also that we must also lock the mutex here, in order to avoid a data race with the initialization code inside the mutex lock in `get_data()`.

### Memory ordering

By using `std::atomic_load_explicit()` and `std::atomic_store_explicit()` we can specify the [memory ordering](http://www.stdthread.co.uk/doc/headers/cstdatomic/memory_order.html) requirements of the operations. We could have just used `std::atomic_load()` and `std::atomic_store()`, but those would have implied `std::memory_order_seq_cst`, which is overkill in this scenario. What we need is to ensure that if a non-`NULL` value is read in `get_data()` then the actual creation of the associated object *happens-before* the read. The store in `get_data()` must therefore use `std::memory_order_release`, whilst the load uses `std::memory_order_acquire`.

On the other hand, the store in `invalidate_cache()` can merrily use `std::memory_order_relaxed`, since there is no data associated with the store: if the load in `get_data()` reads `NULL` then the mutex will be locked, which will handle any necessary synchronization.

Whenever you use atomic operations, you have to make sure that the memory ordering is correct, and that there are no races. Even in such a simple case such as this it is not trivial, and I would not recommend it unless profiling has shown that this is really a problem.
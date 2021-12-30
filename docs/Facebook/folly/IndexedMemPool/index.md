# `IndexedMemPool`

## stackoverflow [How does Facebook folly::AccessSpreader work?](https://stackoverflow.com/questions/47006451/how-does-facebook-follyaccessspreader-work)



## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/**[IndexedMemPool.h](https://github.com/facebook/folly/blob/main/folly/IndexedMemPool.h)**



> NOTE: 
>
> https://gitee.com/mirrors/folly/blob/main/folly/IndexedMemPool.h

"""
Instances of `IndexedMemPool` dynamically allocate and then pool their element type (T), returning 4-byte integer indices that can be passed to the pool's `operator[]` method to access or obtain pointers to the actual elements.  The memory backing items returned from the pool will always be readable, even if items have been returned to the pool. These two features are useful for **lock-free algorithms**.  The indexing behavior makes it easy to build **tagged pointer-like-things**, since a large number of elements can be managed using fewer bits than a full pointer.  The **access-after-free** behavior makes it safe to read from T-s even after they have been recycled, since it is guaranteed that the memory won't have been returned to the OS and unmapped (the algorithm must still use a mechanism to validate that the read was correct, but it doesn't have to worry about page faults), and if the elements use internal sequence numbers it can be guaranteed that there won't be an **ABA match** due to the element being overwritten with a different type that has the same bit pattern.

> NOTE: 一、**access-after-free** 指的就是 "The memory backing items returned from the pool will always be readable, even if items have been returned to the pool"
> NOTE: 二、"The indexing behavior makes it easy to build tagged pointer-like-things, since a large number of elements can be managed using fewer bits than a full pointer."

The object lifecycle strategy is controlled by the `Traits` parameter. One strategy, implemented by `IndexedMemPoolTraitsEagerRecycle`, is to construct objects when they are allocated from the pool and destroy them when they are recycled.  In this mode `allocIndex` and `allocElem` have emplace-like semantics.  In another strategy, implemented by `IndexedMemPoolTraitsLazyRecycle`, objects are default-constructed the first time they are removed from the pool, and deleted when the pool itself is deleted.  By default the first mode is used for **non-trivial T**, and the second is used for **trivial T**.  Clients can customize the object lifecycle by providing their own Traits implementation. See `IndexedMemPoolTraits` for a Traits example.

> NOTE: 一、典型的strategy pattern；二、eager 和 lazy: eager 指的是 eager to "destroy when they are recycled"，lazy 指的是 lazy to "delete when the pool itself is deleted"
> NOTE: 

IMPORTANT: Space for extra elements is allocated to account for those that are inaccessible because they are in other local lists, so the actual number of items that can be allocated ranges from `capacity` to `capacity + (NumLocalLists_-1)*LocalListLimit_`.  This is important if you are trying to maximize the capacity of the pool while constraining the bit size of the resulting pointers, because the pointers will actually range up to the boosted capacity.  See `maxIndexForCapacity` and `capacityForMaxIndex`.

To avoid contention, `NumLocalLists_` free lists of limited (less than or equal to `LocalListLimit_`) size are maintained, and each thread
retrieves and returns entries from its associated **local list**.  If the **local list** becomes too large then elements are placed in bulk in a **global free list**.  This allows items to be efficiently recirculated from consumers to producers.  `AccessSpreader` is used to access the **local lists**, so there is no performance advantage to having more **local lists** than **L1 caches**.

The pool mmap-s the entire necessary address space when the pool is constructed, but delays element construction.  This means that only elements that are actually returned to the caller get paged into the process's resident set (RSS).



## 成员变量 `slots_`


## `struct Slot` 

```c++
  struct Slot {
    T elem;
    Atom<uint32_t> localNext;
    Atom<uint32_t> globalNext;

    Slot() : localNext{}, globalNext{} {}
  };
```

## 成员变量 `slots_`

```
  /// raw storage, only 1..min(size_,actualCapacity_) (inclusive) are
  /// actually constructed.  Note that slots_[0] is not constructed or used
  alignas(hardware_destructive_interference_size) Slot* slots_;
```

## `hardware_destructive_interference_size`

### cppreference [std::hardware_destructive_interference_size, std::hardware_constructive_interference_size](https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size)


### stackoverflow [Understanding std::hardware_destructive_interference_size and std::hardware_constructive_interference_size](https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons)
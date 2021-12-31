# `IndexedMemPool`

## stackoverflow [How does Facebook folly::AccessSpreader work?](https://stackoverflow.com/questions/47006451/how-does-facebook-follyaccessspreader-work)



## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/**[IndexedMemPool.h](https://github.com/facebook/folly/blob/main/folly/IndexedMemPool.h)**



> NOTE: 
>
> https://gitee.com/mirrors/folly/blob/main/folly/IndexedMemPool.h



## doc

Instances of `IndexedMemPool` dynamically allocate and then pool their element type (`T`), returning 4-byte integer indices that can be passed to the pool's `operator[]` method to access or obtain pointers to the actual elements.  

The memory backing items returned from the pool will always be readable, even if items have been returned to the pool. These two features are useful for **lock-free algorithms**.  

> NOTE: 
>
> 一、"The memory backing items returned from the pool will always be readable, even if items have been returned to the pool. "
>
> 关于这一点，在后面有着解释: 
>
> "The **access-after-free** behavior makes it safe to read from T-s even after they have been recycled, since it is guaranteed that the memory won't have been returned to the OS and unmapped (the algorithm must still use a mechanism to validate that the read was correct, but it doesn't have to worry about page faults)"
>
> 简而言之，因为是memory pool，它的已经分配过的memory是不会被unmap到OS的

The indexing behavior makes it easy to build **tagged pointer-like-things**, since a large number of elements can be managed using fewer bits than a full pointer.  The **access-after-free** behavior makes it safe to read from T-s even after they have been recycled, since it is guaranteed that the memory won't have been returned to the OS and unmapped (the algorithm must still use a mechanism to validate that the read was correct, but it doesn't have to worry about page faults), and if the elements use **internal sequence numbers** it can be guaranteed that there won't be an **ABA match** due to the element being overwritten with a different type that has the same bit pattern.

> NOTE: 
>
> 一、**access-after-free** 指的就是 "The memory backing items returned from the pool will always be readable, even if items have been returned to the pool"
>
> 二、"The indexing behavior makes it easy to build **tagged pointer-like-things**, since a large number of elements can be managed using fewer bits than a full pointer."
>
> 

The object lifecycle strategy is controlled by the `Traits` parameter. One strategy, implemented by `IndexedMemPoolTraitsEagerRecycle`, is to construct objects when they are allocated from the pool and destroy them when they are recycled.  In this mode `allocIndex` and `allocElem` have emplace-like semantics.  In another strategy, implemented by `IndexedMemPoolTraitsLazyRecycle`, objects are default-constructed the first time they are removed from the pool, and deleted when the pool itself is deleted.  By default the first mode is used for **non-trivial T**, and the second is used for **trivial T**.  Clients can customize the object lifecycle by providing their own Traits implementation. See `IndexedMemPoolTraits` for a Traits example.

> NOTE: 
>
> 一、典型的strategy pattern，它提供了default strategy，同时也支持"customize" 
>
> 二、上面这段话中涉及到了object lifetime，object lifetime: 
>
> 1、allocation
>
> 2、construction/initialization
>
> 3、destruction/deinitialization
>
> 4、dealloction
>
> 三、eager 和 lazy: eager 指的是 eager to "destroy when they are recycled"，lazy 指的是 lazy to "delete when the pool itself is deleted"
> 四、
>
> `IndexedMemPoolTraitsEagerRecycle`
>
> `IndexedMemPoolTraitsLazyRecycle`

IMPORTANT: Space for extra(额外的) elements is allocated to account for those that are inaccessible because they are in other local lists, so the actual number of items that can be allocated ranges from `capacity` to `capacity + (NumLocalLists_-1)*LocalListLimit_`.  This is important if you are trying to maximize the capacity of the pool while constraining the bit size of the resulting pointers, because the pointers will actually range up to the boosted capacity.  See `maxIndexForCapacity` and `capacityForMaxIndex`.

> NOTE: 
>
> 一、"Space for extra elements is allocated to account for those that are inaccessible because they are in other local lists" 的意思是:
>
> "额外元素的空间被分配给那些由于在其他本地列表中而无法访问的元素"
>
> 一、实际会分配的内存是比用户指定的 `capacity` 要多的，因为需要一些"space for extra elements"

To avoid contention, `NumLocalLists_` free lists of limited (less than or equal to `LocalListLimit_`) size are maintained, and each thread retrieves and returns entries from its associated **local list**.  If the **local list** becomes too large then elements are placed in bulk in a **global free list**.  This allows items to be efficiently recirculated from consumers to producers.  `AccessSpreader` is used to access the **local lists**, so there is no performance advantage to having more **local lists** than **L1 caches**.

The pool mmap-s the entire necessary **address space** when the pool is constructed, but delays element construction.  This means that only elements that are actually returned to the caller get paged into the **process's resident set** (RSS).

> NOTE: 
>
> 这段话要如何理解？

## object lifecycle strategy 



```C++
/// IndexedMemPool traits that implements the lazy lifecycle strategy. In this
/// strategy elements are default-constructed the first time they are allocated,
/// and destroyed when the pool itself is destroyed.
template <typename T>
using IndexedMemPoolTraitsLazyRecycle = IndexedMemPoolTraits<T, false, false>;

/// IndexedMemPool traits that implements the eager lifecycle strategy. In this
/// strategy elements are constructed when they are allocated from the pool and
/// destroyed when recycled.
template <typename T>
using IndexedMemPoolTraitsEagerRecycle = IndexedMemPoolTraits<T, true, true>;
```



## 数据结构

总的来说，它的数据由两部分组成:

1、array of slot

成员变量 `slots_`

2、locallist

成员变量 `local_`


### `struct Slot` 

```C++
  struct Slot {
    T elem;
    Atom<uint32_t> localNext;
    Atom<uint32_t> globalNext;

    Slot() : localNext{}, globalNext{} {}
  };
```

这是单位

### 成员变量 `slots_`

```C++
  /// raw storage, only 1..min(size_,actualCapacity_) (inclusive) are
  /// actually constructed.  Note that slots_[0] is not constructed or used
  alignas(hardware_destructive_interference_size) Slot* slots_;
```



`LocalList` 




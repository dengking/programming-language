# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/**[IndexedMemPool.h](https://github.com/facebook/folly/blob/main/folly/IndexedMemPool.h)**

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

The indexing behavior makes it easy to build **tagged pointer-like-things**, since a large number of elements can be managed using fewer bits than a full pointer.  The **access-after-free** behavior makes it safe to read from T-s even after they have been recycled, since it is guaranteed that the memory won't have been returned to the OS and unmapped (the algorithm must still use a mechanism to validate that the read was correct, but it doesn't have to worry about **page faults**), and if the elements use **internal sequence numbers** it can be guaranteed that there won't be an **ABA match** due to the element being overwritten with a different type that has the same bit pattern.

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

### global list 和 local list

IMPORTANT: Space for extra(额外的) elements is allocated to account for those that are inaccessible because they are in other local lists, so the actual number of items that can be allocated ranges from `capacity` to `capacity + (NumLocalLists_-1)*LocalListLimit_`.  This is important if you are trying to maximize the capacity of the pool while constraining the bit size of the resulting pointers, because the pointers will actually range up to the boosted capacity.  See `maxIndexForCapacity` and `capacityForMaxIndex`.

> NOTE: 
>
> 一、"Space for extra elements is allocated to account for those that are inaccessible because they are in other local lists" 的意思是:
>
> "额外元素的空间被分配给那些由于在其他本地列表中而无法访问的元素"
>
> 一、实际会分配的内存是比用户指定的 `capacity` 要多的，因为需要一些"space for extra elements"

### avoid contention

To avoid contention, `NumLocalLists_` free lists of limited (less than or equal to `LocalListLimit_`) size are maintained, and each thread retrieves and returns entries from its associated **local list**.  If the **local list** becomes too large then elements are placed in bulk in a **global free list**.  This allows items to be efficiently recirculated from consumers to producers.  `AccessSpreader` is used to access the **local lists**, so there is no performance advantage to having more **local lists** than **L1 caches**.

> NOTE: 
>
> 一、local list 和 global list其实都是logical的，因为raw storage是dynamical array，它们都是建立在这个dynamic array之上的，为了支持这个特性，raw storage dynamical array的成员类型为:
>
> ```C++
>   struct Slot {
>     T elem;
>     Atom<uint32_t> localNext;
>     Atom<uint32_t> globalNext;
> 
>     Slot() : localNext{}, globalNext{} {}
>   };
> ```
>
> 可以看到，它有两个属性:
>
> 1、`localNext`
>
> 
>
> 2、`globalNext`

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



## construction



```C++
mmapLength_ = ((needed - 1) & ~(pagesize - 1)) + pagesize;
```

这种写法是非常值得借鉴的: 简洁又高效



### `capacity`、`actualCapacity_`

实际是会分配更多的内存的



## `isAllocated`、`markAllocated`

在 `maxIndexForCapacity` 中，有这样的描述:

> index of `std::numeric_limits<uint32_t>::max()` is reserved for `isAllocated` tracking

### `isAllocated`

```C++
  /// Returns true iff idx has been alloc()ed and not recycleIndex()ed
  bool isAllocated(uint32_t idx) const {
    return slot(idx).localNext.load(std::memory_order_acquire) == uint32_t(-1);
  }
```



### `markAllocated`

```C++
  void markAllocated(Slot& slot) {
    slot.localNext.store(uint32_t(-1), std::memory_order_release);
  }
```



## 数据结构

总的来说，它的数据由两部分组成:

1、raw storage: 它是dynamic array of `Slot`

成员变量 `slots_`

2、locallist

成员变量 `local_`

### index as pointer

使用的是dynamic array作为backing storage，因此它可以使用index作为pointer，并且在实现中，也是这样用的:

1、`struct Slot` 的 `next` 成员:

```C++
Atom<uint32_t> localNext;
Atom<uint32_t> globalNext;
```

2、`struct TaggedPtr` 的 `idx` 成员:

```C++
uint32_t idx;
```

`idx` 作为指向 slot 的pointer。

### raw storage

raw storage只有一个:  `slots_`




#### `struct Slot` 

```C++
  struct Slot {
    T elem;
    Atom<uint32_t> localNext;
    Atom<uint32_t> globalNext;

    Slot() : localNext{}, globalNext{} {}
  };
```

它在`T` object的基础上增加了 `localNext`、`globalNext`。

##### `localNext`

记录的是它是local list中的next元素的index。

##### `globalNext`

记录的是它是global list中的next元素的index。

#### 成员变量 `slots_`

```C++
  /// raw storage, only 1..min(size_,actualCapacity_) (inclusive) are
  /// actually constructed.  Note that slots_[0] is not constructed or used
  alignas(hardware_destructive_interference_size) Slot* slots_;
```

一、`slots_[0]` 没有被使用的是原因是: index的类型是 `uint32_t`，因此它没有负数，因此需要使用 0 来表示失败的分配。

二、它仅仅让成员变量 `slots_` `alignas(hardware_destructive_interference_size)`，这是因为它会被多个thread同时access，需要避免它会被false sharing而带来性能损耗。

三、为什么`struct Slot` 没有像 `LocalList` 一样加上 `alignas(hardware_destructive_interference_size)` 呢？一个`struct Slot` 是否会同时被多个thread access呢？`LocalList` object是会的。

#### 概述: 对raw storage的使用

正如文档中所介绍的，它是通过 `LocalList` 的方式来使用raw storage的，具体来说: 通过 `localHead()`、`globalHead_` 等，这样做的目的是: "To avoid contention"。

下面是  `LocalList`  的声明和使用方式:

```C++
  /// use AccessSpreader to find your list.  We use stripes instead of
  /// thread-local to avoid the need to grow or shrink on thread start
  /// or join.   These are heads of lists chained with localNext
  LocalList local_[NumLocalLists];
```



```C++
  AtomicStruct<TaggedPtr, Atom>& localHead() {
    auto stripe = AccessSpreader<Atom>::current(NumLocalLists);
    return local_[stripe].head;
  }
```

一个令我感到好奇的问题是: 为什么在 `maxIndexForCapacity` 只使用 `(NumLocalLists - 1)`?

```C++
  static constexpr uint32_t maxIndexForCapacity(uint32_t capacity) {
    // index of std::numeric_limits<uint32_t>::max() is reserved for isAllocated
    // tracking
    return uint32_t(std::min(
        uint64_t(capacity) + (NumLocalLists - 1) * LocalListLimit,
        uint64_t(std::numeric_limits<uint32_t>::max() - 1)));
  }
```

这是否和 global 和 local 有关？即: 1个global + (`NumLocalLists` - 1)个local。貌似不是这样的，因为 `LocalList local_[NumLocalLists]` 即 它有 `NumLocalLists` 个 `LocalList` 、`AtomicStruct<TaggedPtr, Atom> globalHead_`。



### local list

每个thread，会获得自己的local list，通过`localHead()`获得local list的head，然后按照linked list的方式，可以找到当前local list的末尾，然后往其中插入元素。这说明是由`AccessSpreader`来决定它到底使用哪个local list。

因为local list是limited size的，因此allocate就相当于是pop，release就相当于是push。

#### size limit: `LocalListLimit_` 

```C++
static_assert(LocalListLimit_ <= 255, "LocalListLimit must fit in 8 bits");
```





#### `struct TaggedPtr` 

```C++
  struct TaggedPtr {
    uint32_t idx;

    // size is bottom 8 bits, tag in top 24.  g++'s code generation for
    // bitfields seems to depend on the phase of the moon, plus we can
    // do better because we can rely on other checks to avoid masking
    uint32_t tagAndSize;
  };      
```

一、它的size是 8 byte，因此它是可以使用 `AtomicStruct` 的

二、`size()` 

```C++
enum : uint32_t {
    SizeBits = 8,
    SizeMask = (1U << SizeBits) - 1, // OX00FF，通过 & 可以获得 size
    TagIncr = 1U << SizeBits,
};
uint32_t size() const { return tagAndSize & SizeMask; }
```

三、它构成的是一个单向链表



#### 成员变量 `local_`

```C++
LocalList local_[NumLocalLists];
```

它仅仅在函数 `localHead()` 中被使用，并且它没有被初始化就直接使用了，这引起了我的好奇。查了一下，下面是回答:

1、cppreference [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)

显然，作者是依赖于C++ 的default initialization behavior。

更多关于array member initialization，参见:

1、stackoverflow [populating int array that is a member variable](https://stackoverflow.com/questions/1811447/populating-int-array-that-is-a-member-variable)

2、stackoverflow [C++ Initializing Non-Static Member Array](https://stackoverflow.com/questions/5643923/c-initializing-non-static-member-array)



#### 成员变量 `globalHead_`

```C++
  /// this is the head of a list of node chained by globalNext, that are
  /// themselves each the head of a list chained by localNext
  alignas(hardware_destructive_interference_size)
      AtomicStruct<TaggedPtr, Atom> globalHead_;
```

在构造函数中，会对它进行初始化:

```C++
globalHead_(TaggedPtr{}) 
```



## `allocIndex`、`recycleIndex`

### `allocIndex`



```C++
auto idx = localPop(localHead());
```

find a slot、allocate a slot

### `recycleIndex`



```C++
localPush(localHead(), idx);
```

release a slot



## `localHead()`、`localPush()`、`localPop()`

### `localHead()`



```C++
  AtomicStruct<TaggedPtr, Atom>& localHead() {
    auto stripe = AccessSpreader<Atom>::current(NumLocalLists);
    return local_[stripe].head;
  }
```

它返回的是 `LocalList` 的 `head` 属性。

### `localPush()`

```C++
  // returns 0 if allocation failed
  uint32_t localPop(AtomicStruct<TaggedPtr, Atom>& head) {
    while (true) {
      TaggedPtr h = head.load(std::memory_order_acquire);
      if (h.idx != 0) {
        // local list is non-empty, try to pop
        Slot& s = slot(h.idx);
        auto next = s.localNext.load(std::memory_order_relaxed);
        if (head.compare_exchange_strong(h, h.withIdx(next).withSizeDecr())) {
          // success
          return h.idx;
        }
        continue;
      }
      // local list is empty  
      uint32_t idx = globalPop();
      if (idx == 0) {
        // global list is empty, allocate and construct new slot
        if (size_.load(std::memory_order_relaxed) >= actualCapacity_ ||
            (idx = ++size_) > actualCapacity_) {
          // allocation failed
          return 0;
        }
        Slot& s = slot(idx);
        // Atom is enforced above to be nothrow-default-constructible
        // As an optimization, use default-initialization (no parens) rather
        // than direct-initialization (with parens): these locations are
        // stored-to before they are loaded-from
        new (&s.localNext) Atom<uint32_t>;
        new (&s.globalNext) Atom<uint32_t>;
        Traits::initialize(&s.elem);
        return idx;
      }

      Slot& s = slot(idx);
      auto next = s.localNext.load(std::memory_order_relaxed);
      if (head.compare_exchange_strong(
              h, h.withIdx(next).withSize(LocalListLimit))) {
        // global list moved to local list, keep head for us
        return idx;
      }
      // local bulk push failed, return idx to the global list and try again
      globalPush(s, idx);
    }
  }
```



## `globalHead_`、`globalPush()`、`globalPop()`



### `globalPop()`

```C++
  // returns 0 if empty
  uint32_t globalPop() {
    while (true) {
      TaggedPtr gh = globalHead_.load(std::memory_order_acquire);
      if (gh.idx == 0 // global list 是空的
          ||
          globalHead_.compare_exchange_strong(
              gh,
              gh.withIdx(
                  slot(gh.idx).globalNext.load(std::memory_order_relaxed)))) {
        // global list is empty, or pop was successful
        return gh.idx;
      }
    }
  }
```



## member accessory



```C++
  /// Provides access to the pooled element referenced by idx
  T& operator[](uint32_t idx) { return slot(idx).elem; }  

  uint32_t slotIndex(uint32_t idx) const {
    assert(
        0 < idx && idx <= actualCapacity_ &&
        idx <= size_.load(std::memory_order_acquire));
    return idx;
  }

  Slot& slot(uint32_t idx) { return slots_[slotIndex(idx)]; }
```



## stackoverflow [How does Facebook folly::AccessSpreader work?](https://stackoverflow.com/questions/47006451/how-does-facebook-follyaccessspreader-work)

> NOTE: 
>
> 其中也描述了 `IndexedMemPool` 是如何使用 `folly::AccessSpreader` 的
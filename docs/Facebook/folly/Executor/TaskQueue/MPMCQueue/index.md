# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[**MPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/MPMCQueue.h)

## doc

`MPMCQueue<T>` is a high-performance bounded concurrent queue that supports multiple producers, multiple consumers, and optional blocking. The queue has a fixed capacity, for which all memory will be allocated up front.  The bulk of the work of enqueuing and dequeuing can be performed in parallel.

### linearizable

`MPMCQueue` is **linearizable**.  That means that if a call to `write(A)` returns before a call to `write(B)` begins, then `A` will definitely end up in the queue before `B`, and if a call to `read(X)` returns before a call to `read(Y)` is started, that `X` will be something from earlier in the queue than `Y`.  This also means that if a read call returns a value, you can be sure that all previous elements of the queue have been assigned a reader (that reader might not yet have returned, but it exists).

> NOTE:
>
> 这是使用 `TurnSequencer` 实现的

### implementation: ticket dispenser(售票机) 

The underlying implementation uses a **ticket dispenser** for the head and the tail, spreading accesses across N single-element queues to produce
a queue with capacity N.  The **ticket dispensers** use atomic increment, which is more robust to contention than a CAS loop.  Each of the single-element queues uses its own CAS to serialize access, with an adaptive spin cutoff(截断).  When spinning fails on a single-element queue it uses `futex()`'s `_BITSET` operations to reduce unnecessary wakeups even if multiple waiters are present on an individual queue (such as when the `MPMCQueue`'s capacity is smaller than the number of enqueuers or dequeuers).

> NOTE: 
>
> 一、
>
> 1、"spreading accesses across N single-element queues to produce a queue with capacity N"、"Each of the single-element queues uses its own CAS to serialize access, with an adaptive spin cutoff" 
>
> 这段话的意思是: 它将"a queue with capacity N"划分为 "N single-element queues"，即N个单元素的queue
>
> 2、"The underlying implementation uses a ticket dispenser for the head and the tail"、"The **ticket dispensers** use atomic increment, which is more robust to contention than a CAS loop."
>
> 这段话的意思是: 使用"ticket dispenser"来处理queue head和queue tail。

### benchmark

In benchmarks (contained in `tao/queues/ConcurrentQueueTests`) it handles 1 to 1, 1 to N, N to 1, and N to M thread counts better than any of the alternatives present in fbcode, for both small (~10) and large capacities.  In these benchmarks it is also faster than `tbb::concurrent_bounded_queue` for all configurations.  When there are many more threads than cores, `MPMCQueue` is _much_ faster than the tbb queue because it uses `futex()` to block and unblock waiting threads, rather than spinning with `sched_yield`.

### NOEXCEPT INTERACTION

NOEXCEPT INTERACTION: If it compiles you're fine.  Ticket-based queues separate the assignment of queue positions from the actual construction of the in-queue elements, which means that the `T` constructor used during enqueue must not throw an exception.  This is enforced at compile time using type traits, which requires that T be adorned(被装饰) with accurate `noexcept` information.  If your type does not use `noexcept`, you will have to wrap it in something that provides the guarantee.  We provide an alternate safe implementation for types that don't use `noexcept` but that are marked `folly::IsRelocatable` and `std::is_nothrow_constructible`, which is common for folly types. In particular, if you can declare `FOLLY_ASSUME_FBVECTOR_COMPATIBLE` then your type can be put in `MPMCQueue`.

If you have a pool of N queue consumers that you want to shut down after the queue has drained, one way is to enqueue N sentinel values
to the queue.  If the producer doesn't know how many consumers there are you can enqueue one sentinel and then have each consumer requeue
two sentinels after it receives it (by requeuing 2 the shutdown can complete in O(log P) time instead of O(P)).



## `class MPMCQueueBase`



### ticket 

```C++
  /// Enqueuers get tickets from here
  alignas(hardware_destructive_interference_size) Atom<uint64_t> pushTicket_;

  /// Dequeuers get tickets from here
  alignas(hardware_destructive_interference_size) Atom<uint64_t> popTicket_;
```



### data structure

#### raw storage

```C++
using Slot = detail::SingleElementQueue<T, Atom>;
```

```C++
  /// Anonymous union for use when Dynamic = false and true, respectively
  union {
    /// An array of capacity_ SingleElementQueue-s, each of which holds
    /// either 0 or 1 item.  We over-allocate by 2 * kSlotPadding and don't
    /// touch the slots at either end, to avoid false sharing
    Slot* slots_;
    /// Current dynamic slots array of dcapacity_ SingleElementQueue-s
    Atom<Slot*> dslots_;
  };
```



### use stride to avoid false sharing

#### 成员变量

```C++
  /// Anonymous union for use when Dynamic = false and true, respectively
  union {
    /// The number of slots_ indices that we advance for each ticket, to
    /// avoid false sharing.  Ideally slots_[i] and slots_[i + stride_]
    /// aren't on the same cache line
    int stride_;
    /// Current stride
    Atom<int> dstride_;
  };
```

#### 成员函数: `computeStride`

We assign **tickets** in increasing order, but we don't want to access neighboring elements of `slots_` because that will lead to **false sharing** (multiple cores accessing the same **cache line** even though they aren't accessing the same bytes in that **cache line**).  To avoid this we advance by stride slots per ticket.

We need `gcd(capacity, stride)` to be 1 so that we will use all of the slots.  We ensure this by only considering prime strides,  which either have no common divisors with capacity or else have a zero remainder after dividing by capacity.  That is sufficient  to guarantee correctness, but we also want to actually spread the  accesses away from each other to avoid false sharing (consider a stride of 7 with a capacity of 8).  To that end we try a few taking care to observe that advancing by -1 is as bad as advancing by 1  when in comes to **false sharing**.

The simple way to avoid false sharing would be to pad each  `SingleElementQueue`, but since we have capacity_ of them that could waste a lot of space.



## `class SingleElementQueue` 

### doc

`SingleElementQueue` implements a blocking queue that holds at most one item, and that requires its users to assign incrementing identifiers
(turns) to each `enqueue` and `dequeue` operation.  Note that the turns used by `SingleElementQueue` are doubled inside the `TurnSequencer`

> NOTE: 
>
> 为什么要double？

### 成员变量

```C++
  /// Storage for a T constructed with placement new
  aligned_storage_for_t<T> contents_;

  /// Even turns are pushes, odd turns are pops
  TurnSequencer<Atom> sequencer_;
```


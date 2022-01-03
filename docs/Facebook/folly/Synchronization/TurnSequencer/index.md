# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[detail](https://github.com/facebook/folly/tree/main/folly/detail)/[**TurnSequencer.h**](https://github.com/facebook/folly/blob/main/folly/detail/TurnSequencer.h)

https://gitee.com/mirrors/folly/blob/main/folly/detail/TurnSequencer.h

```
C:\Users\Kai\Desktop\share\folly-main\folly-main\folly\detail\TurnSequencer.h
```

它的设计非常绝。
## doc

A `TurnSequencer` allows threads to order their execution according to a monotonically increasing (with wraparound) "**turn**" value.  The two operations provided are to **wait for turn `T`**, and to **move to the next turn**.  Every thread that is waiting for `T` must have arrived before that turn is marked completed (for `MPMCQueue` only one thread waits for any particular turn, so this is trivially true).

> NOTE: 
>
> 一、提供了两个operation: 
>
> 1、wait: `waitForTurn`、`tryWaitForTurn`
>
> 2、move to next turn: `completeTurn`
>
> 二、"Every thread that is waiting for `T` must have arrived before that turn is marked completed (for `MPMCQueue` only one thread waits for any particular turn, so this is trivially true)"
>
> 上面这段话的意思是: 一个thread如果想要"waiting for T"，那么必须要在 "turn `T`" 被 "marked completed" 之前 "arrive"

`TurnSequencer`'s `state_` holds 26 bits of the current turn (shifted left by 6), along with a 6 bit saturating(饱和) value that records the **maximum waiter** minus the **current turn**.  Wraparound of the **turn space** is expected and handled.  This allows us to atomically adjust the number of **outstanding waiters** when we perform a `FUTEX_WAKE` operation. Compare this strategy to `sem_t`'s separate `num_waiters` field, which isn't decremented until after the **waiting thread** gets scheduled, during which time more enqueues might have occurred and made pointless `FUTEX_WAKE` calls.

> NOTE: 
>
> 一、一共32-bit: 
>
> 1、"26 bits of the current turn (shifted left by 6)"
>
> 2、6 bit saturating(饱和) value 
>
> "records the **maximum waiter** minus the **current turn**"
>
> "**maximum waiter**"的值应该是固定的，那它的值如何确定呢？它的值是当前的maximum waiter的turn的值，也就是'maximum'是基于turn的值的
>
> 二、"outstanding waiter"是什么含义？
>
> 三、"Wraparound of the turn space "为什么能够 "allows us to atomically adjust the number of **outstanding waiters** when we perform a `FUTEX_WAKE` operation"

`TurnSequencer` uses `futex()` directly.  It is optimized for the case that the **highest awaited turn** is 32 or less higher than the **current turn**.  We use the `FUTEX_WAIT_BITSET` variant, which lets us embed 32 **separate wakeup channels** in a single futex.  See `http://locklessinc.com/articles/futex_cheat_sheet` for a description.

> NOTE: 
> 一、每一bit对应了一个"separate wakeup channel"
>
> 二、"**highest awaited turn** is 32"的含义什么？

We only need to keep exact track of the delta(增量) between the **current turn** and the **maximum waiter** for the 32 turns that follow the current one, because waiters at turn `t+32` will be awoken at turn `t`.  At that point they can then adjust the delta using the **higher base**.  Since we need to encode waiter deltas of 0 to 32 inclusive, we use 6 bits. We actually store** waiter deltas** up to 63, since that might reduce the number of CAS operations a tiny bit.
> NOTE: 
> 一、"delta"要如何理解？增量，这是在将上面这段话放到百度翻译中，其中给出的翻译: 
> "我们只需要精确跟踪当前转弯和当前转弯后32个转弯的最大服务员之间的增量，因为t+32转弯处的服务员将在t转弯处被叫醒。在这一点上，他们可以使用更高的基数调整增量。因为我们需要对0到32（含0到32）的服务员增量进行编码，所以我们使用6位。实际上，我们最多存储63个服务员增量，因为这可能会稍微减少CAS操作的数量。"
>
> 二、"the delta between the **current turn** and the **maximum waiter** for the 32 turns that follow the current one, because waiters at turn `t+32` will be awoken at turn `t`"
> 上面这段话要如何理解？是不是要使用module运算来进行理解？
>
> 三、"Since we need to encode waiter deltas of 0 to 32 inclusive, we use 6 bits" 这段话的意思是: 需要基于waiter deltas的范围是`[0, 32]`，显然它一共有33个元素，因此需要使用6 bit


To avoid some `futex()` calls entirely, `TurnSequencer` uses an adaptive spin cutoff before waiting.  The overheads (and convergence rate) of separately tracking the spin cutoff for each `TurnSequencer` would be prohibitive, so the actual storage is passed in as a parameter and updated atomically.  This also lets the caller use different adaptive cutoffs for different operations (read versus write, for example). To avoid contention, the spin cutoff is only updated when requested by the caller.

On x86 the latency of a **spin loop** varies dramatically across architectures due to changes in the `PAUSE` instruction. Skylake increases the latency by about a factor of 15 compared to previous architectures. To work around this, on x86 we measure spins using `RDTSC` rather than a loop counter.

## 功能

### `waitForTurn`、`tryWaitForTurn`


#### doc

> Internally we always work with shifted turn values, which makes the truncation and wraparound work correctly. This leaves us bits at the bottom to store the number of waiters.  We call shifted turns  "sturns" inside this class.

> Blocks the current thread until turn has arrived.

其实顾名思义就是

### `futexChannel`
> Returns the bitmask to pass futexWait or futexWake when communicating about the specified turn

bitmask。

```C++
  uint32_t futexChannel(uint32_t turn) const noexcept {
    return 1u << (turn & 31);
  }
```

## 实现思考: 如何匹配turn and thread？

如何匹配等待某个turn的thread，并将它唤醒？如何记录哪个turn正在等待哪个sequence呢？这是核心要素

thread在调用 `tryWaitForTurn` 的时候，会传入它所期望的turn。


## usage

### `MPMCQueue`

https://gitee.com/mirrors/folly/blob/main/folly/MPMCQueue.h

### `Rcu`

https://gitee.com/mirrors/folly/blob/main/folly/synchronization/Rcu.h

### `LockFreeRingBuffer`

https://gitee.com/mirrors/folly/blob/main/folly/experimental/LockFreeRingBuffer.h




### draft
```
.h D:\GitHub-src\folly\ content:TurnSequencer 
```

```
D:\GitHub-src\folly\folly\experimental\LockFreeRingBuffer.h
D:\GitHub-src\folly\folly\MPMCQueue.h
D:\GitHub-src\folly\folly\synchronization\Rcu-inl.h
D:\GitHub-src\folly\folly\synchronization\Rcu.h
D:\GitHub-src\folly\folly\detail\TurnSequencer.h
```


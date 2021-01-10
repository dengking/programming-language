# microsoft [Lockless Programming Considerations for Xbox 360 and Microsoft Windows](https://docs.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming?redirectedfrom=MSDN)

Lockless programming is a way to safely share changing data between multiple threads without the cost of acquiring and releasing locks. This sounds like a panacea(灵丹妙药), but lockless programming is complex and subtle, and sometimes doesn't give the benefits that it promises. Lockless programming is particularly complex on Xbox 360.

Using lockless programming correctly and safely requires significant knowledge of both your hardware and your compiler. This article gives an overview of some of the issues to consider when trying to use lockless programming techniques.

## Programming with Locks

> NOTE: 原文这一段的内容比较普通

## Lockless Programming

Lockless programming, as the name suggests, is a family of techniques for safely manipulating shared data without using locks. There are lockless algorithms available for passing messages, sharing lists and queues of data, and other tasks.

> NOTE: 思考: 在lockless programming中，如何实现notify？

When doing lockless programming, there are two challenges that you must deal with: non-atomic operations and reordering.

## Non-Atomic Operations

An atomic operation is one that is indivisible—one where other threads are guaranteed to never see the operation when it is half done. Atomic operations are important for lockless programming, because without them, other threads might see half-written values, or otherwise inconsistent state.

> NOTE: 这里关于atomic的解释是非常好的: 和race condition、inconsistent state进行了结合
>
> atomic意味着没有: 
>
> 1、half-completed state
>
> 2、half done

On all modern processors, you can assume that **reads and writes of naturally aligned native types are atomic**. As long as the memory bus is at least as wide as the type being read or written, the CPU reads and writes these types in a **single bus transaction**, making it impossible for other threads to see them in a **half-completed state**. On x86 and x64 there is no guarantee that reads and writes larger than eight bytes are atomic. This means that 16-byte reads and writes of streaming SIMD extension (SSE) registers, and string operations, might not be atomic.

> NOTE: 这一段关于atomic的说明其实是存在一定错误的；
>
> bus transaction是一个非常好的描述；

Reads and writes of types that are not naturally aligned—for instance, writing DWORDs that cross four-byte boundaries—are not guaranteed to be atomic. The CPU may have to do these reads and writes as multiple bus transactions, which could allow another thread to modify or see the data in the middle of the read or write.

**Composite operations**, such as the read-modify-write sequence that occurs when you increment a shared variable, are not atomic. On Xbox 360, these operations are implemented as multiple instructions (lwz, addi, and stw), and the thread could be swapped out partway through the sequence. On x86 and x64, there is a single instruction (`inc`) that can be used to increment a variable in memory. If you use this instruction, incrementing a variable is atomic on single-processor systems, but it is still not atomic on multi-processor systems. Making `inc` atomic on x86- and x64-based multi-processor systems requires using the **lock prefix**, which prevents another processor from doing its own read-modify-write sequence between the read and the write of the inc instruction.

> NOTE: 上述composite operation其实就是"assemble as atomic primitive"
>
> 上面这段话将thread safe和atomic一起来进行描述了，它从最最底层、最最根本描述了race condition

The following code shows some examples:

```C++
// This write is not atomic because it is not natively aligned.
DWORD* pData = (DWORD*)(pChar + 1);
*pData = 0;

// This is not atomic because it is three separate operations.
++g_globalCounter;

// This write is atomic.
g_alignedGlobal = 0;

// This read is atomic.
DWORD local = g_alignedGlobal;
```

## Guaranteeing Atomicity

You can be sure you are using atomic operations by a combination of the following:

1、Naturally atomic operations

2、Locks to wrap composite operations

3、Operating system functions that implement atomic versions of popular composite operations

## Reordering

A more subtle problem is **reordering**. Reads and writes do not always happen in the order that you have written them in your code, and this can lead to very confusing problems. In many multi-threaded algorithms, a thread writes some data and then writes to a flag that tells other threads that the data is ready. This is known as a **write-release**. If the writes are reordered, other threads may see that the flag is set before they can see the written data.

Similarly, in many cases, a thread reads from a flag and then reads some shared data if the flag says that the thread has acquired access to the **shared data**. This is known as a **read-acquire**. If reads are reordered, then the data may be read from shared storage before the flag, and the values seen might not be up to date.

Reordering of reads and writes can be done both by the compiler and by the processor. Compilers and processors have done this **reordering** for years, but on single-processor machines it was less of an issue. This is because CPU rearrangement of reads and writes is invisible on single-processor machines (for non-device driver code that is not part of a device driver), and compiler rearrangement of reads and writes is less likely to cause problems on single-processor machines.

If the compiler or the CPU rearranges the writes shown in the following code, another thread may see that the alive flag is set while still seeing the old values for `x` or `y`. Similar rearrangement can happen when reading.


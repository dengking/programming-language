# `std::atomic::compare_exchange_weak()`



## stackoverflow [Understanding std::atomic::compare_exchange_weak() in C++11](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11)

```cpp
bool compare_exchange_weak (T& expected, T val, ..);
```

`compare_exchange_weak()` is one of compare-exchange primitives provided in C++11. It's ***weak*** in the sense that it returns false even if the value of the object is equal to `expected`. This is due to ***spurious failure*** on some platforms where a sequence of instructions (instead of one as on x86) are used to implement it. On such platforms, context switch, reloading of the same address (or cache line) by another thread, etc can fail the primitive. It's `spurious` as it's not the value of the object (not equal to `expected`) that fails the operation. Instead, it's kind of timing issues.

But what puzzles me is what's said in C++11 Standard (ISO/IEC 14882),

> 29.6.5 .. A consequence of spurious failure is that nearly all uses of weak compare-and-exchange will be in a loop.

Why does it have to be in a loop in ***nearly all uses*** ? Does that mean we shall loop when it fails because of spurious failures? If that's the case, why do we bother use `compare_exchange_weak()` and write the loop ourselves? We can just use `compare_exchange_strong()` which I think should get rid of spurious failures for us. What are the common use cases of `compare_exchange_weak()`?
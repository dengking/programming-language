# `std::atomic_flag`



## cppreference [`std::atomic_flag`](https://en.cppreference.com/w/cpp/atomic/atomic_flag)

### Example

A spinlock mutex can be implemented in userspace using an `atomic_flag`

```C++
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

std::atomic_flag lock = ATOMIC_FLAG_INIT;

void f(int n)
{
	for (int cnt = 0; cnt < 100; ++cnt)
	{
		while (lock.test_and_set(std::memory_order_acquire))
			// acquire lock
			;// spin
		std::cout << "Output from thread " << n << '\n' << cnt << '\n';
		lock.clear(std::memory_order_release);               // release lock
	}
}

int main()
{
	std::vector<std::thread> v;
	for (int n = 0; n < 10; ++n)
	{
		v.emplace_back(f, n);
	}
	for (auto &t : v)
	{
		t.join();
	}
	getchar();
}
// g++ --std=c++11 test.cpp -lpthread


```

> NOTE: 上面的`std::memory_order_acquire`、`std::memory_order_release`要如何理解呢？参见:
>
> 1、stackoverflow [memory ordering with atomic_flag spin lock](https://stackoverflow.com/questions/14791495/memory-ordering-with-atomic-flag-spin-lock)



## stackoverflow [memory ordering with atomic_flag spin lock](https://stackoverflow.com/questions/14791495/memory-ordering-with-atomic-flag-spin-lock)

I am trying to get familiar with the new memory ordering concepts of c++11 and believed I actully had a quite good grasp on them, until I stumbled upon this implementation of a spin lock:

```cpp
#include <atomic>

namespace JayZ
{
    namespace Tools
    {
        class SpinLock
        {
        private:
            std::atomic_flag spin_lock;
        public:
            inline SpinLock( void ) : atomic_flag( ATOMIC_FLAG_INIT ) {}

            inline void lock( void )
            {
                while( spin_lock.test_and_set( std::memory_order_acquire ) )
                    ;
            }

            inline void unlock( void )
            {
                lock.clear( std::memory_order_release );
            }
        };
    }
}
```

It is e.g. equivalently mentioned at cppreference [std::atomic_flag](http://en.cppreference.com/w/cpp/atomic/atomic_flag ) and also in the book "Concurrency in Action". I also found it someplace here at SO.

But I just don't understand why it would work!

Imagine thread 1 calls `lock()` and `test_and_set()` returns 0 as the old value --> thread 1 has acquired the lock.

But then thread 2 comes along and tries the same. Now since there has occurred no "store synchronization" (`release` ,`seq_cst_acq_rel`) thread 1's store to `spin_lock` should be of type relaxed.

But from this follows that it cannot imao be synchronized with thread 2's read of spin_lock. This should make it possible for thread 2 to read the value 0 from `spin_lock` and thus acquire the lock as well.

Where is my mistake?

[A](https://stackoverflow.com/a/14791524)

Your mistake is in forgetting that `spin_lock` is an `atomic_flag` and thus `test_and_set` is an atomic operation. The `memory_order_acquire` and `memory_order_release` is needed to prevent reads from migrating to before the lock operation or writes from migrating to after the unlock. The lock itself is protected by atomicity which always includes visibility.

> NOTE: 
>
> 1、lock acquire release
>
> 2、这一段给出的介绍是比较好的，它让我理解了: lock、acquire semantic、release semantic之间的关联
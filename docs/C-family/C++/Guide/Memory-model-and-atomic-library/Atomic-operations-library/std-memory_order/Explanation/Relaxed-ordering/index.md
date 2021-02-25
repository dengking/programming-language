# Relaxed ordering

## cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Relaxed ordering

Atomic operations tagged `memory_order_relaxed` are not synchronization operations; they do not impose an order among concurrent memory accesses. They only guarantee atomicity and modification order consistency.

> NOTE: 
>
> 1、上面这段话中的: synchronization operation如何来理解？
>
> 2、上面这段话中的: "modification order consistency"如何理解？

For example, with `x` and `y` initially zero,

```C++
// Thread 1:
r1 = y.load(std::memory_order_relaxed); // A
x.store(r1, std::memory_order_relaxed); // B
// Thread 2:
r2 = x.load(std::memory_order_relaxed); // C 
y.store(42, std::memory_order_relaxed); // D
```

> NOTE: 完整的测试程序如下:
>
> ```C++
> #include <atomic>
> #include <thread>
> #include <iostream>
> std::atomic<int> x { 0 };
> std::atomic<int> y { 0 };
> int main()
> {
> 	// Thread 1:
> 	std::thread t1 {
> 		[&]{
> 			int r1 = y.load(std::memory_order_relaxed); // A
> 			x.store(r1, std::memory_order_relaxed);// B
> 			std::cout << r1 << std::endl;
> 		}
> 	};
> 
> 	// Thread 2:
> 	std::thread t2{
> 		[&]{
> 
> 			int r2 = x.load(std::memory_order_relaxed);// C
> 			y.store(42, std::memory_order_relaxed);// D
> 			std::cout << r2 << std::endl;
> 		}
> 		};
> 	t1.join();
> 	t2.join();
> }
> // g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out
> 
> ```

is allowed to produce `r1 == r2 == 42` because, although A is *sequenced-before* B within thread 1 and C is *sequenced before* D within thread 2, nothing prevents D from appearing before A in the **modification order** of `y`, and B from appearing before C in the **modification order** of x. The side-effect of D on y could be visible to the load A in thread 1 while the side effect of B on x could be visible to the load C in thread 2. In particular, this may occur if D is completed before C in thread 2, either due to compiler reordering or at runtime.

> NOTE: 
>
> 运行结果 `r1 == r2 == 42` 出现的情况是: "this may occur if D is completed before C in thread 2, either due to compiler reordering or at runtime"，需要注意，这是允许的，原因有:
>
> 1、C、D之间没有data dependency，因此，允许 reordering 。
>
> 2、`std::memory_order_relaxed`
>
> 那么产生 运行结果 `r1 == r2 == 42` 的执行序列如下: 
>
> Thread 1: A、B
>
> Thread 2: D，C
>
> Modification order: 
>
> D、A，则`r1`的值为42
>
> B、C，则`r2`的值为42

### (since C++14)

> NOTE: 未读

### Use case

Typical use for relaxed memory ordering is incrementing counters, such as the reference counters of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr), since this only requires atomicity, but not ordering or synchronization (note that decrementing the **shared_ptr** counters requires **acquire-release synchronization** with the destructor)

> NOTE: 
>
> 1、关于具体的实现，参见 `Implement-reference-count-use-std-atomic` 章节
>
> 2、这段话中的"acquire-release synchronization with destructor"如何理解？
>
> 防止memory reordering发生在decrementing the **shared_ptr** counters 和 destructor 之间(这让我想起来很多  **acquire-release synchronization** 的例子)，显然只有当counter为0的时候，才能够调用destructor，这是典型的可以使用 **acquire-release synchronization** 的场景；

```C++
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
 
std::atomic<int> cnt = {0};
 
void f()
{
    for (int n = 0; n < 1000; ++n) {
        cnt.fetch_add(1, std::memory_order_relaxed);
    }
}
 
int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f);
    }
    for (auto& t : v) {
        t.join();
    }
    std::cout << "Final counter value is " << cnt << '\n';
}
// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 
>
> 1、上述例子是典型的只需要atomic，而不需要对memory ordering进行控制的
>
> 2、thread pool
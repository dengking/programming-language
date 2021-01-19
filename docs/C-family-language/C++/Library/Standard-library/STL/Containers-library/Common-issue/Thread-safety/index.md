# Thread safety

## stackoverflow [C++11 STL containers and thread safety](https://stackoverflow.com/questions/12931787/c11-stl-containers-and-thread-safety)

[A](https://stackoverflow.com/a/12931932)

> NOTE: 非常好的总结，从read、write的角度来进行分析的，但是这个回答是存在反例的，下面的回答就是一个反例

I think STL containers offer the following basic thread-safety guarantee:

- *simultaneous reads* of the *same* object are OK
- *simultaneous read/writes* of *different* objects are OK

But you have to use some form of custom synchronization (e.g. critical section) if you want to do something different, like e.g. simultaneous writes on the same object.

[A](https://stackoverflow.com/a/12938588)

Since the existing answers don't cover it (only a comment does), I'll just mention 23.2.2 [container.requirements.dataraces] of the current [C++ standard specification](http://isocpp.org/std/the-standard) which says:

> implementations are required to avoid data races when the contents of the contained object in different elements in the same sequence, excepting `vector<bool>`, are modified concurrently.

i.e. it's safe to access distinct elements of the same container, so for example you can have a global `std::vector<std::future<int>>` of ten elements and have ten threads which each write to a different element of the vector.

Apart from that, the same rules apply to containers as for the rest of the standard library (see 17.6.5.9 [res.on.data.races]), as [Mr.C64's answer](https://stackoverflow.com/a/12931932/981959) says, and additionally [container.requirements.dataraces] lists some non-const member functions of containers that can be called safely because they only return non-const references to elements, they don't actually modify anything (in general any non-const member function must be considered a modification.)



## stackoverflow [Safe parallel read-only access to a STL container](https://stackoverflow.com/questions/10833512/safe-parallel-read-only-access-to-a-stl-container)



[A](https://stackoverflow.com/a/10833580)

A data-race, from the C++11 specification in sections 1.10/4 and 1.10/21, requires at least two threads with non-atomic access to the same set of memory locations, the two threads are not synchronized with regards to accessing the set of memory locations, and *at least one thread **writes to** or modifies an element in the set of memory locations*. So in your case, if the threads are only reading, you are fine ... by definition since none of the threads write to the same set of memory locations, there are no data-races even though there is no explicit synchronization mechanism between the threads.

> NOTE: 从data race的本质来进行分析的；

## cppreference [Containers library # Thread safety](https://en.cppreference.com/w/cpp/container#Thread_safety)



### 3

Different elements in the same container can be modified concurrently by different threads, except for the elements of `std::vector<bool>` (for example, a vector of [std::future](https://en.cppreference.com/w/cpp/thread/future) objects can be receiving values from multiple threads).

> NOTE: 在 stackoverflow [C++11 STL containers and thread safety](https://stackoverflow.com/questions/12931787/c11-stl-containers-and-thread-safety) # [A](https://stackoverflow.com/a/12938588) 中，对此进行了解释；
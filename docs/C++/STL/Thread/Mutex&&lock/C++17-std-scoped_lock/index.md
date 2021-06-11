# cppreference [std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock) C++17

If several mutexes are given, deadlock avoidance algorithm is used as if by [std::lock](https://en.cppreference.com/w/cpp/thread/lock).

> NOTE: 
>
> 最好使用 [std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock) ，而不是  [std::lock](https://en.cppreference.com/w/cpp/thread/lock)，这在  cppreference [std::lock](https://en.cppreference.com/w/cpp/thread/lock) 中有说明


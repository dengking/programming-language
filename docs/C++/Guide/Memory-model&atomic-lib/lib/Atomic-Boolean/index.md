# Atomic Boolean



## `std::atomic<bool>` VS `std::atomic_flag`



### stackoverflow [difference between standard's atomic bool and atomic flag](https://stackoverflow.com/questions/39329311/difference-between-standards-atomic-bool-and-atomic-flag)

1. [`std::atomic`](http://en.cppreference.com/w/cpp/atomic/atomic)
2. [`std::atomic_flag`](http://en.cppreference.com/w/cpp/atomic/atomic_flag)

The `std::atomic_flag` contains the following explanation:

> `std::atomic_flag` is an atomic boolean type. Unlike all specializations of `std::atomic`, it is guaranteed to be lock-free. Unlike `std::atomic<bool>`, `std::atomic_flag` does not provide load or store operations.

which I fail to understand. Is `std::atomic<bool>` not guaranteed to be lock-free? Then it's not atomic or what?



[A](https://stackoverflow.com/a/39329519/10173843)

> std::atomic bool type not guranteed to be **lock-free**?

Correct. `std::atomic` may be implemented using locks.

> then it's not atomic or what?

`std::atomic` is atomic whether it has been implemented using locks, or without. `std::atomic_flag` is guaranteed to be implemented without using locks.

> So what's the difference b/w two

The primary difference besides the lock-free guarantee is:

> `std::atomic_flag` does not provide load or store operations.

------

> and when should I use which?

Usually, you will want to use `std::atomic<bool>` when you need an atomic boolean variable. `std::atomic_flag` is a low level structure that can be used to implement custom atomic structures.
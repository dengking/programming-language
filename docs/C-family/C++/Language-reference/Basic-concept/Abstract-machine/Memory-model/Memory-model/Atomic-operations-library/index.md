# Atomic operations library



## cppreference [Atomic operations library](https://en.cppreference.com/w/cpp/atomic)



## `std::atomic` is neither copyable nor movable



### `std::atomic<bool> as static member`

1、stackoverflow [Initialize static atomic member variable](https://stackoverflow.com/questions/20453054/initialize-static-atomic-member-variable)

[A](https://stackoverflow.com/a/20453089)

```C++
std::atomic<int> order::c(0);   // or {0} for a more C++11 experience
```


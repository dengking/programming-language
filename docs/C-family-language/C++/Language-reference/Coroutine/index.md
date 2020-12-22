# C++ Coroutine 

## Primer

lewissbaker [Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)

lewissbaker [Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)

lewissbaker [Understanding operator co_await](https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await)

## cppreference [Coroutines (C++20)](https://en.cppreference.com/w/cpp/language/coroutines) 

A function is a coroutine if its definition does any of the following:

1) uses the `co_await` operator to suspend execution until resumed

```C++
task<> tcp_echo_server() {
  char data[1024];
  for (;;) {
    size_t n = co_await socket.async_read_some(buffer(data));
    co_await async_write(socket, buffer(data, n));
  }
}
```

2) uses the keyword `co_yield` to suspend execution returning a value

```C++
generator<int> iota(int n = 0) {
  while(true)
    co_yield n++;
}
```

3) uses the keyword `co_return` to complete execution returning a value

```C++
lazy<int> f() {
  co_return 7;
}
```



## Implementation

### Before C++20

#### [lewissbaker](https://github.com/lewissbaker)/[cppcoro](https://github.com/lewissbaker/cppcoro)

The 'cppcoro' library provides a large set of general-purpose primitives for making use of the coroutines TS proposal described in [N4680](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4680.pdf).



#### boost [Coroutine2](https://www.boost.org/doc/libs/1_75_0/libs/coroutine2/doc/html/index.html)

C++11



## TODO

paoloseverini [Async-Await in C++](https://paoloseverini.wordpress.com/2014/04/22/async-await-in-c/)

stackoverflow [What are the mechanics of coroutines in C++20?](https://stackoverflow.com/questions/57621168/what-are-the-mechanics-of-coroutines-in-c20)
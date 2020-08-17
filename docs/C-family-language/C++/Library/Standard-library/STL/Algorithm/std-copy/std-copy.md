# `std::copy`



## cppreference [`std::copy`](https://en.cppreference.com/w/cpp/algorithm/copy) 





## `std::copy` VS `std::memcpy`

### stackoverflow [Is it better to use std::memcpy() or std::copy() in terms to performance?](https://stackoverflow.com/questions/4707012/is-it-better-to-use-stdmemcpy-or-stdcopy-in-terms-to-performance)

#### [A](https://stackoverflow.com/a/9980859)

I'm going to go against the general wisdom here that `std::copy` will have a slight, almost imperceptible performance loss. I just did a test and found that to be untrue: I did notice a performance difference. However, the winner was `std::copy`.

> NOTE:在这里，我将违背一般的智慧，即`std::copy`将有轻微的、几乎难以察觉的性能损失

I wrote a `C++` SHA-2 implementation. In my test, I hash 5 strings using all four SHA-2 versions (224, 256, 384, 512), and I loop 300 times. I measure times using `Boost.timer`. That 300 loop counter is enough to completely stabilize my results. I ran the test 5 times each, alternating between the `memcpy` version and the `std::copy` version. My code takes advantage of grabbing（抓） data in as large of chunks as possible (many other implementations operate with `char` / `char *`, whereas I operate with `T` / `T *` (where `T` is the largest type in the user's implementation that has correct overflow behavior), so fast memory access on the largest types I can is central to the performance of my algorithm. These are my results:

**Time (in seconds) to complete run of SHA-2 tests**

```cpp
std::copy   memcpy  % increase
6.11        6.29    2.86%
6.09        6.28    3.03%
6.10        6.29    3.02%
6.08        6.27    3.03%
6.08        6.27    3.03%
```

**Total average increase in speed of std::copy over memcpy: 2.99%**

My compiler is gcc 4.6.3 on Fedora 16 x86_64. My optimization flags are `-Ofast -march=native -funsafe-loop-optimizations`.

[Code for my SHA-2 implementations.](https://bitbucket.org/davidstone/sha-2/)

I decided to run a test on my MD5 implementation as well. The results were much less stable, so I decided to do 10 runs. However, after my first few attempts, I got results that varied wildly from one run to the next, so I'm guessing there was some sort of OS activity going on. I decided to start over.

Same compiler settings and flags. There is only one version of MD5, and it's faster than SHA-2, so I did 3000 loops on a similar set of 5 test strings.

These are my final 10 results:

**Time (in seconds) to complete run of MD5 tests**

```cpp
std::copy   memcpy      % difference
5.52        5.56        +0.72%
5.56        5.55        -0.18%
5.57        5.53        -0.72%
5.57        5.52        -0.91%
5.56        5.57        +0.18%
5.56        5.57        +0.18%
5.56        5.53        -0.54%
5.53        5.57        +0.72%
5.59        5.57        -0.36%
5.57        5.56        -0.18%
```

**Total average decrease in speed of std::copy over memcpy: 0.11%**

[Code for my MD5 implementation](https://bitbucket.org/davidstone/md5)

These results suggest that there is some optimization that std::copy used in my SHA-2 tests that `std::copy` could not use in my MD5 tests. In the SHA-2 tests, both arrays were created in the same function that called `std::copy` / `memcpy`. In my MD5 tests, one of the arrays was passed in to the function as a function parameter.

I did a little bit more testing to see what I could do to make `std::copy` faster again. The answer turned out to be simple: turn on link time optimization. These are my results with LTO turned on (option -flto in gcc):

**Time (in seconds) to complete run of MD5 tests with -flto**

```cpp
std::copy   memcpy      % difference
5.54        5.57        +0.54%
5.50        5.53        +0.54%
5.54        5.58        +0.72%
5.50        5.57        +1.26%
5.54        5.58        +0.72%
5.54        5.57        +0.54%
5.54        5.56        +0.36%
5.54        5.58        +0.72%
5.51        5.58        +1.25%
5.54        5.57        +0.54%
```

**Total average increase in speed of std::copy over memcpy: 0.72%**

In summary, there does not appear to be a performance penalty for using `std::copy`. In fact, there appears to be a performance gain.

**Explanation of results**

So why might `std::copy` give a performance boost?

First, I would not expect it to be slower for any implementation, as long as the optimization of inlining is turned on. All compilers inline aggressively; it is possibly the most important optimization because it enables so many other optimizations. `std::copy` can (and I suspect all real world implementations do) detect that the arguments are trivially copyable and that memory is laid out sequentially. This means that in the worst case, when `memcpy` is legal, `std::copy` should perform no worse. The trivial implementation of `std::copy` that defers to `memcpy` should meet your compiler's criteria of "always inline this when optimizing for speed or size".

However, `std::copy` also keeps more of its information. When you call `std::copy`, the function keeps the types intact. `memcpy` operates on `void *`, which discards almost all useful information. For instance, if I pass in an array of `std::uint64_t`, the compiler or library implementer may be able to take advantage of 64-bit alignment with `std::copy`, but it may be more difficult to do so with `memcpy`. Many implementations of algorithms like this work by first working on the unaligned portion at the start of the range, then the aligned portion, then the unaligned portion at the end. If it is all guaranteed to be aligned, then the code becomes simpler and faster, and easier for the branch predictor in your processor to get correct.

**Premature optimization?**

`std::copy` is in an interesting position. I expect it to never be slower than `memcpy` and sometimes faster with any modern optimizing compiler. Moreover, anything that you can `memcpy`, you can `std::copy`. `memcpy` does not allow any overlap in the buffers, whereas `std::copy` supports overlap in one direction (with `std::copy_backward` for the other direction of overlap). `memcpy` only works on pointers, `std::copy` works on any iterators (`std::map`, `std::vector`, `std::deque`, or my own custom type). In other words, you should just use `std::copy` when you need to copy chunks of data around.
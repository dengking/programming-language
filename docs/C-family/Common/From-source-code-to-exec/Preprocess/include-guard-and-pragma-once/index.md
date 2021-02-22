

# include guard and `#pragma once`



## wikipedia [include guard](https://en.wikipedia.org/wiki/Include_guard)

In the [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming languages, an **#include guard**, sometimes called a **macro guard**, **header guard** or **file guard**, is a particular construct used to avoid the problem of *double inclusion* when dealing with the [include directive](https://en.wikipedia.org/wiki/Include_directive).

The addition of `#include` guards to a header file is one way to make that file [idempotent](https://en.wikipedia.org/wiki/Idempotent). Another construct to combat(反对) *double inclusion* is [#pragma once](https://en.wikipedia.org/wiki/Pragma_once), which is non-standard but nearly universally supported among C and C++ [compilers](https://en.wikipedia.org/wiki/List_of_compilers).



## wikipedia [pragma once](https://en.wikipedia.org/wiki/Pragma_once)

In the [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming languages, `**pragma once**` is a non-standard but widely supported [preprocessor directive](https://en.wikipedia.org/wiki/C_preprocessor) designed to cause the current [source file](https://en.wikipedia.org/wiki/Source_file) to be included only once in a single compilation.[[1\]](https://en.wikipedia.org/wiki/Pragma_once#cite_note-1) Thus, `#pragma once` serves the same purpose as [include guards](https://en.wikipedia.org/wiki/Include_guard), but with several advantages, including: less code, avoidance of name clashes, and sometimes improvement in compilation speed.[[2\]](https://en.wikipedia.org/wiki/Pragma_once#cite_note-2) On the other hand, `#pragma once` is not necessarily available in all compilers and its implementation is tricky and might not always be reliable.

### stackoverflow [Is #pragma once a safe include guard?](https://stackoverflow.com/questions/787533/is-pragma-once-a-safe-include-guard)

```C++
#pragma once
#ifndef HEADER_H
#define HEADER_H

...

#endif // HEADER_H
```

[A](https://stackoverflow.com/a/1946730)

`#pragma once` does have one drawback (other than being non-standard) and that is if you have the same file in different locations (we have this because our build system copies files around) then the compiler will think these are different files.

[A](https://stackoverflow.com/a/787539)

Using `#pragma once` should work on any modern compiler, but I don't see any reason not to use a standard `#ifndef` include guard. It works just fine. The one caveat is that GCC didn't support `#pragma once` before [version 3.4](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=11569).

I also found that, at least on GCC, [it recognizes the standard `#ifndef` include guard and optimizes it](http://gcc.gnu.org/onlinedocs/gcc-2.95.3/cpp_1.html#SEC8), so it shouldn't be much slower than `#pragma once`.
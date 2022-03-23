# AddressSanitizer[¶](https://clang.llvm.org/docs/AddressSanitizer.html#addresssanitizer)

> NOTE: 
>
> 一、这篇文章中介绍的 `ASAN_OPTIONS`，在github [AddressSanitizerFlags](https://github.com/google/sanitizers/wiki/AddressSanitizerFlags) 中，有着更好的描述，它属于 Run-time flags 范畴，下面是简单的介绍：
>
> > Most run-time flags are passed to [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer) via `ASAN_OPTIONS` environment variable like this:
> >
> > ```shell
> > ASAN_OPTIONS=verbosity=1:malloc_context_size=20 ./a.out
> > ```
>
> 

## [Introduction](https://clang.llvm.org/docs/AddressSanitizer.html#id1)

AddressSanitizer is a fast memory error detector. It consists of a compiler instrumentation module and a run-time library. The tool can detect the following types of bugs:

1、Out-of-bounds accesses to heap, stack and globals

2、Use-after-free

3、Use-after-return (runtime flag `ASAN_OPTIONS=detect_stack_use_after_return=1`)

4、Use-after-scope (clang flag `-fsanitize-address-use-after-scope`)

4、Double-free, invalid free

5、Memory leaks (experimental)

## [Usage](https://clang.llvm.org/docs/AddressSanitizer.html#id3)

The AddressSanitizer run-time library should be linked to the final executable, so make sure to use `clang` (not `ld`) for the final link step. When linking shared libraries, the AddressSanitizer run-time is not linked, so `-Wl,-z,defs` may cause link errors (don’t use it with AddressSanitizer). 

> NOTE:
>
> 上面这段话的意思是： address sanitizer的运行时依赖于 `libclang_rt.asan_osx_dynamic.dylib` ，在使用clang进行链接的时候，clang会自动地链接上述lib；如果不使用clang的话，则存在遗失这个lib的风险

```c++
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <bitset>
#include <map>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <numeric>
#include <climits>

using namespace std;
int main(int argc, char **argv) {
  int *array = new int[100];
  delete [] array;
  return array[argc];  // BOOM
}
```

> NOTE:
>
> ```C++
> (base) dengkai01@dengkaideMacBook-Pro testCpp % otool -L a.out                                                   
> a.out:
>         /usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 1300.21.0)
>         @rpath/libclang_rt.asan_osx_dynamic.dylib (compatibility version 0.0.0, current version 0.0.0)
>         /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1311.100.2)
> ```
>
> 

AddressSanitizer exits on the first detected error. This is by design:

1、Fixing bugs becomes unavoidable. AddressSanitizer does not produce false alarms. Once a memory corruption occurs, the program is in an inconsistent state, which could lead to confusing results and potentially misleading subsequent reports.

## [Symbolizing the Reports](https://clang.llvm.org/docs/AddressSanitizer.html#id4)

> NOTE:
>
> 目前还这一段还没有读懂

## [Additional Check](https://clang.llvm.org/docs/AddressSanitizer.html#id5)

### [Initialization order checking](https://clang.llvm.org/docs/AddressSanitizer.html#id6)

> NOTE:
>
> 这是典型的static initialization order fiasco

Note that this option is not supported on macOS.

### [Stack Use After Return (UAR)](https://clang.llvm.org/docs/AddressSanitizer.html#id7)



### [Memory leak detection](https://clang.llvm.org/docs/AddressSanitizer.html#id8)


# Chromium C++ style guide

https://chromium.googlesource.com/chromium/src/+/lkgr/styleguide/c++/



## doc [Chromium C++ style guide](https://chromium.googlesource.com/chromium/src/+/lkgr/styleguide/c++/c++.md)



### Tests and Test-only Code

> NOTE: 
>
> 一、简而言之: 对于用于test的code，需要进行特别的标注

1、Functions used only for testing should be restricted to test-only usages with the testing suffixes supported by [PRESUMBIT.py](https://chromium.googlesource.com/chromium/src/+/main/PRESUBMIT.py). `ForTesting` is the conventional suffix although similar patterns, such as `ForTest`, are also accepted. These suffixes are checked at presubmit time to ensure the functions are called only by test files.

> NOTE: 
>
> 一、对仅仅用于test的function，需要进行特别的标注: 加上suffix:
>
> 1、`ForTesting` 
>
> 2、`ForTest`
>
> chromium的 [PRESUMBIT.py](https://chromium.googlesource.com/chromium/src/+/main/PRESUBMIT.py) 会据此对代码进行static analysis，确保 "the functions are called only by test files"

2、Classes used only for testing should be in a GN build target that is marked `testonly=true`. Tests can depend on such targets, but production code can not.

> NOTE: 
>
> 一、chromium会为test编译一个特别的target，因此"Classes used only for testing should be in a GN build target that is marked `testonly=true`"；显然chromium会严格地区分test code 和 production code

3、While test files generally appear alongside the production code they test, support code for `testonly` targets should be placed in a `test/` subdirectory. For example, see `//mojo/core/core_unittest.cc` and `//mojo/core/test/mojo_test_base.cc`. For test classes used across multiple directories, it might make sense to move them into a nested `test` namespace for clarity.

> NOTE: 
>
> 一、这一条其实是接上一条而来的，即要区分test code 和 production code，这一条强调的是:
>
> 1、 "support code for `testonly` targets should be placed in a `test/` subdirectory"
>
> 2、"For test classes used across multiple directories, it might make sense to move them into a nested `test` namespace for clarity"

### Unnamed namespaces

Items local to a `.cc` file should be wrapped in an **unnamed namespace**. While some such items are already **file-scope** by default in C++, not all are; also, shared objects on Linux builds export all symbols, so **unnamed namespaces** (which restrict these symbols to the **compilation unit**) improve **function call cost** and reduce the size of entry point tables.

> NOTE: 
>
> 一、为什么 "**unnamed namespaces** (which restrict these symbols to the **compilation unit**) improve **function call cost** "？
>
> 二、这一点是符合CppCoreGuidelines [SF.22: Use an unnamed (anonymous) namespace for all internal/non-exported entities](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#sf22-use-an-unnamed-anonymous-namespace-for-all-internalnon-exported-entities) 

### Exporting symbols

> NOTE: 
>
> 一、这是常见用法

Symbols can be exported (made visible outside of a shared library/DLL) by annotating with a `<COMPONENT>_EXPORT` macro name (where `<COMPONENT>` is the name of the component being built, e.g. `BASE`, `NET`, `CONTENT`, etc.). Class annotations should precede the class name:

```C++
class FOO_EXPORT Foo {
  void Bar();
  void Baz();
  // ...
};
```

### Platform-specific code

> NOTE: 
>
> 一、这也是常见的用法，原文描述了一堆，其实直接看下面的例子即可

```C++
#include "foo/foo.h"

#include <stdint.h>
#include <algorithm>

#include "base/strings/utf_string_conversions.h"
#include "build/build_config.h"
#include "chrome/common/render_messages.h"

#if BUILDFLAG(IS_WIN)
#include <windows.h>
#include "base/win/com_init_util.h"
#elif BUILDFLAG(IS_POSIX)
#include "base/posix/global_descriptors.h"
#endif
```

### CHECK(), DCHECK(), and NOTREACHED()

> NOTE: 
>
> 一、其实就是assertion

The `CHECK()` macro will cause an immediate crash if its condition is not met. 

`DCHECK()` is like `CHECK()` but is only compiled in when `DCHECK_IS_ON` is true (debug builds and some bot configurations, but not end-user builds). `NOTREACHED()` is equivalent to `DCHECK(false)`. 

## doc [C++ Dos and Don'ts](https://chromium.googlesource.com/chromium/src/+/lkgr/styleguide/c++/c++-dos-and-donts.md)

> NOTE: 
>
> 总结得非常好

## doc [Modern C++ use in Chromium](https://chromium.googlesource.com/chromium/src/+/HEAD/styleguide/c++/c++-features.md)
# Clang-Static-Analyzer&Clang-Tidy

## docs [The Clang Static Analyzer](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_static_analyzer.md)

The Clang C/C++ compiler comes with a static analyzer which can be used to find bugs using path sensitive analysis. Path sensitive analysis is a technique that explores all the possible branches in code and records the codepaths that might lead to bad or undefined behavior, like an uninitialized reads, use after frees, pointer leaks, and so on.

> NOTE: 
>
> 一、上面介绍了它的原理
>
> 二、提供后半段可以发现，clang static analyzer还可以作为一个memory debugger，它可以发现很多memory bug。

See the [official Clang static analyzer page](http://clang-analyzer.llvm.org/) for more background information.

The static analyzer can still be invoked with [clang-tidy](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tidy.md).

### Recommended checks

```shell
--tidy_checks="-*,clang-analyzer-core*,clang-analyzer-cplusplus*,clang-analyzer-unix*,clang-analyzer-deadcode*"
```

### Addressing false positives

> NOTE: 
>
> 一、chromium提供了"suppressions"方式

An example would be that if the analyzer detected the function argument `*my_ptr` might be null and dereferencing it would potentially segfault, you would see the error `warning: Dereference of null pointer (loaded from variable 'my_ptr')`. If you know for a fact that `my_ptr` will not be null in practice, then you can place an assert at the top of the function: `DCHECK(my_ptr)`. The analyzer will no longer generate the warning.

> NOTE: 
>
> 一、对于这种情况，cppcoreguideline的原则:
>
> [I.12: Declare a pointer that must not be null as `not_null`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-nullptr)
>
> [F.23: Use a `not_null` to indicate that “null” is not a valid value](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-nullptr)

## docs [Clang Tidy](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tidy.md)

Its purpose is to provide an extensible framework for diagnosing and fixing typical programming errors, like style violations, interface misuse, or bugs that can be deduced via static analysis.

> NOTE: 
>
> 一、支持代码风格的检查，非常好，需要确定它是否支持命名风格的检查
>
> 二、关于"style violations"，chromium还提供了 

### Where is it?

clang-tidy is available in two places in Chromium:

1、In Chromium checkouts

2、In code review on Gerrit

Clang-tidy automatically runs on any CL that Chromium committers upload to Gerrit, and will leave code review comments there. This is the recommended way of using clang-tidy.

> NOTE: 
>
> 一、Gerrit是chromium的code review工具:
>
> https://chromium-review.googlesource.com/q/status:open+-is:wip
>
> 二、"CL"的含义是"change list"，关于此，参见 stackoverflow [What does "CL" mean in a commit message? What does it stand for?](https://stackoverflow.com/questions/25716920/what-does-cl-mean-in-a-commit-message-what-does-it-stand-for) # [A](https://stackoverflow.com/a/27520705/10173843)
>
> > It means *Change List*.
> >
> > > Create a change list (CL)
> > >
> > > Creating a change in git is really just creating a branch.
> >
> > http://www.chromium.org/developers/contributing-code
> >
> > And the code review system there are using: https://code.google.com/p/rietveld/
>
> 三、上面介绍了使用clang-tidy的方式

### Enabled checks

Chromium globally enables a subset of all of clang-tidy's checks (see `${chromium}/src/.clang-tidy`). We want these checks to cover as much as we reasonably can, but we also strive to strike a reasonable balance between signal and noise on code reviews. Hence, a large number of clang-tidy checks are disabled.

> NOTE: 
>
> 一、chromium的clang-tidy配置文件:
>
> https://github.com/chromium/chromium/blob/main/.clang-tidy



## How chromium use clang- tidy

### [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[clang](https://github.com/chromium/chromium/tree/main/tools/clang)/[scripts](https://github.com/chromium/chromium/tree/main/tools/clang/scripts)/[**clang_tidy_tool.py**](https://github.com/chromium/chromium/blob/main/tools/clang/scripts/clang_tidy_tool.py)

> Automatically fetch, build, and run clang-tidy from source.
>
> This script seeks to automate the steps detailed in docs/clang_tidy.md.
>
> Example: the following command disables clang-tidy's default checks (`-*`) and enables the clang static analyzer checks.
>
> ```shell
> tools/clang/scripts/clang_tidy_tool.py \\
>        --checks='-*,clang-analyzer-*,-clang-analyzer-alpha*' \\
>        --header-filter='.*' \\
>        out/Release chrome
> ```
>
> The same, but checks the changes only.
>
> ```shell
> git diff -U5 | tools/clang/scripts/clang_tidy_tool.py \\
> --diff \\
> --checks='-*,clang-analyzer-*,-clang-analyzer-alpha*' \\
> --header-filter='.*' \\
> out/Release chrome
> ```
>
> 

其中调用的:

一、

1、run-clang-tidy.py

2、clang-tidy-diff.py

[llvm-project](https://github.com/llvm/llvm-project)/[clang-tools-extra](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra)/[clang-tidy](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy)/[**tool**](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy/tool)/

二、`generate_compdb.py`

这是在同级目录定义的文件

三、`build_clang_tools_extra.py`

这是在同级目录定义的文件
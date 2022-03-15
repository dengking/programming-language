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
>
> 四、它使用的是哪个script？是[tricium_clang_tidy.py](https://chromium.googlesource.com/chromium/tools/build/+/refs/heads/main/recipes/recipe_modules/tricium_clang_tidy/resources/tricium_clang_tidy.py) 还是 

### Enabled checks

Chromium globally enables a subset of all of clang-tidy's checks (see `${chromium}/src/.clang-tidy`). We want these checks to cover as much as we reasonably can, but we also strive to strike a reasonable balance between signal and noise on code reviews. Hence, a large number of clang-tidy checks are disabled.

> NOTE: 
>
> 一、chromium的clang-tidy配置文件:
>
> https://github.com/chromium/chromium/blob/main/.clang-tidy
>
> 二、`tricium_clang_tidy.py` 的源代码路径：
>
> https://chromium.googlesource.com/chromium/tools/build/+/refs/heads/main/recipes/recipe_modules/tricium_clang_tidy/resources/tricium_clang_tidy.py

To run clang-tidy across all of Chromium, you‘ll need a checkout of Chromium’s [build/](https://chromium.googlesource.com/chromium/tools/build) repository. Once you have that and a Chromium `out/` dir with an `args.gn`, running clang-tidy across all of Chromium is a single command:

```shell
$ cd ${chromium}/src
$ ${chromium_build}/recipes/recipe_modules/tricium_clang_tidy/resources/tricium_clang_tidy.py \
    --base_path $PWD \
    --out_dir out/Linux \
    --findings_file all_findings.json \
    --clang_tidy_binary $PWD/third_party/llvm-build/Release+Asserts/bin/clang-tidy \
    --all
```

To only run clang-tidy against certain files, replace the `--all` parameter with the individual file paths.

All clang-tidy checks are run on Linux builds of Chromium, so please set up your `args.gn` to build Linux.

`all_findings.json` is where all of clang-tidy's findings will be dumped. The format of this file is detailed in `tricium_clang_tidy.py`.

**Note** that the above command will use Chromium‘s top-level `.clang-tidy` file (or `.clang-tidy` files scattered throughout `third_party/`, depending on the files we lint. In order to test a *new* check, you’ll have to add it to Chromium's top-level `.clang-tidy` file.



### But I want to run it locally

**Note** that the local flows with clang-tidy are experimental, and require an LLVM checkout. Tricium is happy to run on WIP CLs, and we strongly encourage its use.

> NOTE:
>
> 一、"WIP"的含义是："work in progress"，参见：
>
> stackoverflow [GitHub: What is a "wip" branch?](https://stackoverflow.com/questions/15763059/github-what-is-a-wip-branch)

That said, assuming you have the LLVM sources available, you'll need to bring your own `clang-apply-replacements` binary if you want to use the `-fix` option noted below.

4、Run clang-tidy.

```
<PATH_TO_LLVM_SRC>/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py \
    -p . \# Set the root project directory, where compile_commands.json is.
    # Set the clang-tidy binary path, if it's not in your $PATH.
    -clang-tidy-binary <PATH_TO_LLVM_BUILD>/bin/clang-tidy \
    # Set the clang-apply-replacements binary path, if it's not in your $PATH
    # and you are using the `fix` behavior of clang-tidy.
    -clang-apply-replacements-binary \
        <PATH_TO_LLVM_BUILD>/bin/clang-apply-replacements \
    # The checks to employ in the build. Use `-*,...` to omit default checks.
    -checks=<CHECKS> \
    -header-filter=<FILTER> \# Optional, limit results to only certain files.
    -fix \# Optional, used if you want to have clang-tidy auto-fix errors.
    'chrome/browser/.*' # A regex of the files you want to check.

Copy-Paste Friendly (though you'll still need to stub in the variables):
<PATH_TO_LLVM_SRC>/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py \
    -p . \
    -clang-tidy-binary <PATH_TO_LLVM_BUILD>/bin/clang-tidy \
    -clang-apply-replacements-binary \
        <PATH_TO_LLVM_BUILD>/bin/clang-apply-replacements \
    -checks=<CHECKS> \
    -header-filter=<FILTER> \
    -fix \
    'chrome/browser/.*'
```



## github [chromium](https://github.com/chromium/chromium)/[**.clang-tidy**](https://github.com/chromium/chromium/blob/main/.clang-tidy)



## monorail-prod.appspot [Issue 1153919: Tricium clang-tidy for Chromium doesn't get complete coverage](https://monorail-prod.appspot.com/p/chromium/issues/detail?id=1153919)

> NOTE:
>
> 一、这个讨论给我的启发是：对于跨平台项目，在使用Tricium clang-tidy的时候，需要考虑如何保证各个平台的代码都被检查到
>
> 二、原文中提及了 CQ 的概念，它的含义是 continuous quality，参见如下文章：
>
> techtarget [continuous quality](https://www.techtarget.com/searchsoftwarequality/definition/continuous-quality)
>
> 这篇文章非常好，其中提及了CQI的approach，就包括 [Static analysis](https://whatis.techtarget.com/definition/static-analysis)
>
> slideshare [Continous Delivery with CQ](https://www.slideshare.net/olimination/continous-delivery-with-cq)
>
> 三、这篇文章给我们的一个启发是：chromium将clang-tidy放到了CQ中



## example

https://chromium-review.googlesource.com/c/chromium/src/+/2893622

https://groups.google.com/a/chromium.org/g/chromium-dev/c/MC3ubeMHFB8

bugs.chromium [Issue 687243: Clang Static Analyzer for Chrome](https://bugs.chromium.org/p/chromium/issues/detail?id=687243)

bugs.chromium [Issue 409318: Clang should detect argument order of evaluation issues with smart pointers and std::move](https://bugs.chromium.org/p/chromium/issues/detail?id=409318)

groups.google [Proposing bugprone-use-after-move Clang-tidy check](https://groups.google.com/a/chromium.org/g/cxx/c/IDD4f6Pmnxc)



## groups.google [Adding more default clang-tidy lints to Chromium](https://groups.google.com/a/chromium.org/g/cxx/c/iZ6-Y9ZhC3Q/m/g-8HzqmbAAAJ)



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
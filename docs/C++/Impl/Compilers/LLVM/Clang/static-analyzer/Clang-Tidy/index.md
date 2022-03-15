# Clang-Tidy

## docs [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/)

docs [Clang-Tidy Checks](https://clang.llvm.org/extra/clang-tidy/checks/list.html)

### [Using clang-tidy](https://clang.llvm.org/extra/clang-tidy/#id2)

For example,

```shell
$ clang-tidy test.cpp -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
```

will disable all default checks (`-*`) and enable all `clang-analyzer-*` checks except for `clang-analyzer-cplusplus*` ones.



The `-list-checks` option lists all the enabled checks. When used without `-checks=`, it shows checks enabled by default. 

> NOTE:
>
> 一、实际运行 `clang-tidy -list-checks` 会发现，默认情况下是启动了很多check的

The `-fix` flag instructs **clang-tidy** to fix found errors if supported by corresponding checks.

> NOTE:
>
> 一、它是如何进行fix的呢？

#### command-line options

> NOTE:

### [Suppressing Undesired Diagnostics](https://clang.llvm.org/extra/clang-tidy/#id3)

> NOTE:
>
> 可以看到它时通过注解的方式来进行surpassing的



## `.clang-tidy` file

clang-tidy会去source base dir下找`.clang-tidy` file。



### example

1、[flamewing](https://gist.github.com/flamewing)/**[[dot\]clang-tidy](https://gist.github.com/flamewing/a5016645064afa086ac8afd98ea7f627)**

2、[chromium](https://github.com/chromium/chromium)/[**.clang-tidy**](https://github.com/chromium/chromium/blob/main/.clang-tidy)



## source code

[llvm-project](https://github.com/llvm/llvm-project)/[clang-tools-extra](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra)/[**clang-tidy**](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy)/

## tool

https://clang.llvm.org/extra/doxygen/run-clang-tidy_8py_source.html

[llvm-project](https://github.com/llvm/llvm-project)/[clang-tools-extra](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra)/[clang-tidy](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy)/[**tool**](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy/tool)/

1、[run-clang-tidy.py](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py)

2、[clang-tidy-diff.py](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/clang-tidy/tool/clang-tidy-diff.py)



### [Clang-Tidy3.8# Running clang-tidy on LLVM](https://releases.llvm.org/3.8.0/tools/clang/tools/extra/docs/clang-tidy/index.html#running-clang-tidy-on-llvm)

> NOTE:
>
> 其中介绍了 [run-clang-tidy.py](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py) 的使用

## clang-apply-replacements

https://lists.llvm.org/pipermail/cfe-users/2019-April/001505.html
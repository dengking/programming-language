# chromium LLVM

chromium充分运用了LLVM toolchain，为此本章将LLVM的相关内容全部收录整理进来: 

[Testing and infrastructure](https://sites.google.com/a/chromium.org/dev/developers/testing)

## [chromium docs](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/) # [Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/#clang)

[Clang Compiler](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) - General information on the clang compiler, used by default on Mac and Linux

> [clang.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md)
>
> 其中概述了chromium对clang的使用

[Clang Tool Refactoring](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tool_refactoring.md) - Leveraging clang tools to perform refactorings that are AST-aware

> [clang_tool_refactoring.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tool_refactoring.md)

[The Clang Static Analyzer](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_static_analyzer.md) - How to enable static analysis at build time

> [clang_static_analyzer.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_static_analyzer.md)

[Clang Code Coverage Wrapper](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_code_coverage_wrapper.md) - Enable Clang code coverage instrumentation for a subset of source files.

> [clang_code_coverage_wrapper.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_code_coverage_wrapper.md)

[Writing Clang Plugins](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/writing_clang_plugins.md) - Don't write a clang plugin, but if you do, read this

> [writing_clang_plugins.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/writing_clang_plugins.md)

[Updating Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) - Updating the version of Clang used to build

> [updating_clang.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md)

[Using clang-format on Chromium C++ Code](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_format.md) - Various ways to invoke clang-format on C++ code

> [clang_format.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_format.md)

[Clang Tidy](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tidy.md) - Support for the `clang-tidy` tool in Chromium

> [clang_tidy.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tidy.md)

[Updating Clang Format Binaries](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang_format_binaries.md) - How up update the clang-format binaries that come with a checkout of Chromium

> [updating_clang_format_binaries.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang_format_binaries.md)



[clang_sheriffing.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_sheriffing.md) 

[clangd.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clangd.md)

### chromium clang

[Clang Compiler](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) 

[clang_sheriffing.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_sheriffing.md) 

[Updating Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang.md) 



### analyzer、linter

[Clang Tidy](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tidy.md)

[The Clang Static Analyzer](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_static_analyzer.md)



### sanitizer

[AddressSanitizer (ASan)](https://sites.google.com/a/chromium.org/dev/developers/testing/addresssanitizer)

[LeakSanitizer](https://sites.google.com/a/chromium.org/dev/developers/testing/leaksanitizer)

[MemorySanitizer (MSan)](https://sites.google.com/a/chromium.org/dev/developers/testing/memorysanitizer)

[ThreadSanitizer (TSan) v. 2](https://sites.google.com/a/chromium.org/dev/developers/testing/threadsanitizer-tsan-v2)

[UndefinedBehaviorSanitizer (UBSan)](https://sites.google.com/a/chromium.org/dev/developers/testing/undefinedbehaviorsanitizer)



### clang tool

#### clang-format

[Using clang-format on Chromium C++ Code](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_format.md) 

[Updating Clang Format Binaries](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang_format_binaries.md) 



#### refactoring

[Clang Tool Refactoring](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_tool_refactoring.md) 



### code coverage

[Clang Code Coverage Wrapper](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang_code_coverage_wrapper.md) 

[Code Coverage in Chromium](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/testing/code_coverage.md)



### Clangd

[clangd.md](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clangd.md)



## Source code

github [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[**clang**](https://github.com/chromium/chromium/tree/main/tools/clang)/

github [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[clang](https://github.com/chromium/chromium/tree/main/tools/clang)/[**plugins**](https://github.com/chromium/chromium/tree/main/tools/clang/plugins)/